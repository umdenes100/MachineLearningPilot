//Tools -> Manage Libraries -> Search for ArduinoJson
#include <ArduinoJson.h>
//Tools -> Manage Libraries -> Search for ArduinoWebsockets by Gil Maimon
#include <ArduinoWebsockets.h>
#include <Arduino.h>
//#include <ESP8266WiFi.h>
#include <WiFi.h>
#include "helpers.h"
//With DEBUG enabled, it will print out debug messages to the Serial port.
//#define DEBUG
//With USE_SWSR_AS_ARD enabled, it will do the Arduino stuff over a software serial part on D3 and D4. Useful to free up the Serial port for debug messages.
//#define USE_SWSR_AS_ARD

#include "esp_camera.h"
#include "img_converters.h"
#include "camera_index.h"
#include "fb_gfx.h"
#include "fd_forward.h"
#include "fr_forward.h"


#ifdef USE_SWSR_AS_ARD
#include "SoftwareSerial.h"
SoftwareSerial arduinoSerial;
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

char buff[500];
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

//We need to keep sending the aruco information until the aruco is confirmed by the server.
bool arucoConfirmed = false;

//Because of the asynchronous nature of the code and the synchronous nature of the original codebase
bool needToSendAruco = false, needToSendMission = false;

StaticJsonDocument<300> doc;
const byte FLUSH_SEQUENCE[] = {0xFF, 0xFE, 0xFD, 0xFC};

WebsocketsClient client;
void setup() {
    // Begin serial communication with Arduino
#ifdef DEBUG
    Serial.begin(115200);
    Serial.println("Hellow");
    delay(1000);
#endif
    //Set up the serial port.
#ifdef USE_SWSR_AS_ARD
    arduinoSerial.begin(9600, SWSERIAL_8N1, D3, D4, false);
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
    // Connect to Wifi. At an attempt at robustness, we will also catch any incoming serial data. However, we are only going to hold one command.
    bool stop = false; //Once we get a stop sequence (Assuming its from the begin statement) lets stop.
    while (WiFi.status() != WL_CONNECTED)
    {
        if (not stop) {
            if (arduinoSerial.available())
                buff[buff_index++] = arduinoSerial.read();
            if (
                buff[buff_index - 4] == FLUSH_SEQUENCE[0] and
                buff[buff_index - 3] == FLUSH_SEQUENCE[1] and
                buff[buff_index - 2] == FLUSH_SEQUENCE[2] and
                buff[buff_index - 1] == FLUSH_SEQUENCE[3]) {
                stop = true;
#ifdef DEBUG
                ps("we got one. OP: "); Serial.println(buff[0], HEX);
#endif
            }
            if (buff_index == 500) { //Buffer overflow. It is very unlikely this will occur. It could only occur with a print so we will just cut it off.
                buff[496] = FLUSH_SEQUENCE[0];
                buff[497] = FLUSH_SEQUENCE[1];
                buff[498] = FLUSH_SEQUENCE[2];
                buff[499] = FLUSH_SEQUENCE[3];
                stop = true;
            }
        }
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
    if (stop) {
        send();
        buff_index = 0;
    }
}

void loop() {
    //Read in data from Arduino
    if (arduinoSerial.available()) {
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
            //            ps("sending "); p(buff_index); psl(" bytes.");
#endif
            send();
            buff_index = 0;
        }
    }

    client.poll();
    yield();
}
