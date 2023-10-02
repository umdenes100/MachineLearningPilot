//Tools -> Manage Libraries -> Search for ArduinoJson
#include <ArduinoJson.h>
//Tools -> Manage Libraries -> Search for ArduinoWebsockets by Gil Maimon
#include <ArduinoWebsockets.h>
#include <Arduino.h>
//#include <ESP8266WiFi.h>
#include <WiFi.h>
#include "helpers.h"
//With DEBUG enabled, it will print out debug messages to the Serial port.
#define DEBUG
//With USE_SWSR_AS_ARD enabled, it will do the Arduino stuff over a software serial part on D3 and D4. Useful to free up the Serial port for debug messages.
#define USE_SWSR_AS_ARD

#define CAMERA_MODEL_AI_THINKER
#include "esp_camera.h"
#include "img_converters.h"
#include "camera_index.h"
#include "fb_gfx.h"
#include "fd_forward.h"
#include "fr_forward.h"
#include "camera_pins.h"


#ifdef USE_SWSR_AS_ARD
#define arduinoSerial Serial2
#else
#define arduinoSerial Serial
#endif


// WiFi network name
#define ROOM 1215
#if ROOM == 1116  //big lab
#define WIFI_NETWORK "VisionSystem1116-2.4" 
#elif ROOM == 1215 //small Lab
#define WIFI_NETWORK "VisionSystem1215-2.4"
#endif

using namespace websockets;

#ifdef DEBUG
void onEventsCallback(WebsocketsEvent event, String data) {
  if (event == WebsocketsEvent::ConnectionOpened) {
    Serial.println("Connnection Opened");
  } else if (event == WebsocketsEvent::ConnectionClosed) {
    Serial.println("Connnection Closed");
    delay(1000);
    ESP.restart();
  } else if (event == WebsocketsEvent::GotPing) {
    Serial.println("Got a Ping!");
  } else if (event == WebsocketsEvent::GotPong) {
    Serial.println("Got a Pong!");
  }
}
#else
void onEventsCallback(WebsocketsEvent event, String data) {
  if (event == WebsocketsEvent::ConnectionClosed) {
    delay(1000);
    ESP.restart();
  }
}
#endif

char buff[2500];
uint16_t buff_index = 0;
byte teamName[50]; //Stores the team name.
byte teamType;
int aruco;

// Camera stuff perhaps : )
const char* prediction;

//Ok basically the ESP will hold the last recieved result.
double aruco_x;
double aruco_y;
double aruco_theta;
bool aruco_visible;

bool newData = false;

//We need to keep sending the aruco information until the aruco is confirmed by the server.
bool arucoConfirmed = false;

//Because of the asynchronous nature of the code and the synchronous nature of the original codebase
bool needToSendAruco = false, needToSendMission = false;

StaticJsonDocument<2500> doc;
const byte FLUSH_SEQUENCE[] = {0xFF, 0xFE, 0xFD, 0xFC};

WebsocketsClient client;

void ESPCAMinit() {
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  if (psramFound()) {
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    ESP.restart();
  }
}

void setup() {
  // Begin serial communication with Arduino
#ifdef DEBUG
  Serial.begin(115200);
  Serial.println("Hellow");
  delay(1000);
#endif
  //Set up the serial port.
#ifdef USE_SWSR_AS_ARD
  arduinoSerial.begin(115200, SERIAL_8N1, 2, 4);
#ifdef DEBUG
  if (!arduinoSerial) { // If the object did not initialize, then its configuration is invalid
    psl("Invalid SoftwareSerial pin configuration, check config");
  }
#endif
#else
  Serial.begin(9600);
#endif

#ifdef DEBUG
  psl("\n\nStarting");
#endif
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_NETWORK, NULL);

  while (WiFi.status() != WL_CONNECTED)
  {
    if (millis() > 10 * 1000) {
#ifdef DEBUG
      psl("Failed to connect...");
      Serial.flush();
#endif
      ESP.restart();
    }
    yield();
  }
  client.onMessage(onMessageCallback);

  client.onEvent(onEventsCallback);
#ifdef DEBUG
  psl("Connected to WiFi");
#endif
  client.connect("ws://192.168.1.2:7755");
  if (!client.available()) {
#ifdef DEBUG
    psl("Failed to connect (websocket)...");
    Serial.flush();
#endif
    delay(1000);
    ESP.restart();
  }
#ifdef DEBUG
  psl("Connected to websocket");
#endif
  while (arduinoSerial.available()) {
    arduinoSerial.read();
  }

  ESPCAMinit();


}

void loop() {
  //Read in data from Arduino
  if (arduinoSerial.available()) {
//    Serial.print((int)arduinoSerial.peek());
    buff[buff_index++] = arduinoSerial.read();
    if (buff_index == 500) { //Buffer overflow. It is very unlikely this will occur. It could only occur with a print so we will just cut it off.
      buff[496] = FLUSH_SEQUENCE[0];
      buff[497] = FLUSH_SEQUENCE[1];
      buff[498] = FLUSH_SEQUENCE[2];
      buff[499] = FLUSH_SEQUENCE[3];
    }
    if (
      buff[buff_index - 4] == FLUSH_SEQUENCE[0] and
      buff[buff_index - 3] == FLUSH_SEQUENCE[1] and
      buff[buff_index - 2] == FLUSH_SEQUENCE[2] and
      buff[buff_index - 1] == FLUSH_SEQUENCE[3]) { //This is the end of the sequence.
#ifdef DEBUG
      ps("sending "); p(buff_index); psl(" bytes.");
#endif
      send();
      buff_index = 0;
    }

#define OP_CHECK    51
    if (buff[0] == OP_CHECK) {
      buff_index = 0;
      //Quick Version. If we have new data, send it back.
      if (newData) {
        newData = false;
        if (aruco_visible) {
          arduinoSerial.write(0x02);
          uint16_t x = max(aruco_x * 100, 0.0);
          arduinoSerial.write((byte *) &x, 2);
          arduinoSerial.write(uint8_t(max(aruco_y * 100, 0.0)));
          int16_t t = aruco_theta * 100;
          arduinoSerial.write((byte *) &t, 2);
          arduinoSerial.flush();
        } else {
          arduinoSerial.write(0x01);
          arduinoSerial.flush();
        }
      } else {
        arduinoSerial.write(0x00); arduinoSerial.flush();
      }
    }
    if (buff[0] == 0x99) { //Ping
      arduinoSerial.write(0x99);
      psl("Started");
      buff_index = 0;
    }
  }

  client.poll();
  yield();
}
