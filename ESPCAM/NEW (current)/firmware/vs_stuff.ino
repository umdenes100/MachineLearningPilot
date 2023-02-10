#define OP_PING     0
#define OP_BEGIN    2
#define OP_LOCATION 4
#define OP_MISSION  6
#define OP_PRINT    8

#define OP_BEGIN_NEW 3
#define OP_LOCATION_NEW 5

#define OP_PRED 7

//This will decode proprietary stuff and turn it into json. At the time send() is called, everything we need is in buff

typedef union
{
  float f;
  uint8_t b[4];
} float_converter_t;

void send() {
  doc.clear();
  byte i;
  switch (buff[0]) {
    case OP_PING:
      arduinoSerial.write(client.ping() ? 0x05 : 0x09);
      break;
    case OP_BEGIN:
      doc["op"] = "begin";
      teamType = buff[1];
      memset(teamName, 0, 50); //Clear the memory of teamName.
      i = 2;
      while (buff[i] != 0xFF) { //Since ASCII cannot be 0xFF (0-128) we only need to check the first of the ending sequence.
        teamName[i - 2] = buff[i]; //Move the teamName
        i++;
      }
      //            putl((char*) &teamName);
      doc["teamName"] = teamName;
      doc["teamType"] = teamType;
      arucoConfirmed = false;
      needToSendMission = true; //Lets send the mission site immediately after reciept.
      break;
    case OP_BEGIN_NEW:
      doc["op"] = "begin";
      teamType = buff[1];
      aruco = ((int) buff[2] << 8) + buff[3];
      memset(teamName, 0, 50); //Clear the memory of teamName.
      i = 4;
      while (buff[i] != 0xFF) { //Since ASCII cannot be 0xFF (0-128) we only need to check the first of the ending sequence.
        teamName[i - 4] = buff[i]; //Move the teamName
        i++;
      }
      doc["teamName"] = teamName;
      doc["aruco"] = aruco;
      doc["teamType"] = teamType;
      break;
    case OP_LOCATION:
      if (arucoConfirmed == false) {
        aruco = ((int) buff[2] << 8) + buff[1];
        doc["op"] = "aruco";
        doc["aruco"] = aruco;
#ifdef DEBUG
        putl(aruco);
#endif
        needToSendAruco = true; //Lets send the ArUco immediately after reciept.
      } else { //Lets send the last value for x,y and theta we recieved.
        sendAruco();
        return;
      }
      break;
    case OP_LOCATION_NEW:
      sendAruco();
      break;
    case OP_MISSION:
      doc["op"] = "mission";
      doc["teamName"] = teamName;
      doc["type"] = buff[1];
      i = 2;
      while (buff[i] != 0xFF and i < 200) { //Since ASCII cannot be 0xFF (0-128) we only need to check the first of the ending sequence.
        buff[i - 2] = buff[i]; //Move the message foward in the buffer.
        i++;
      }
      buff[i - 2] = '\0'; //End it with the null byte.
      doc["message"] = buff;
      break;
    case OP_PRINT:
      doc["op"] = "print";
      doc["teamName"] = teamName;
      // Ok so my genius idea is that we move all the charachters forward.
      i = 1;
      while (buff[i] != 0xFF and i < 200) { //Since ASCII cannot be 0xFF (0-128) we only need to check the first of the ending sequence.
        buff[i - 1] = buff[i]; //Move the message foward in the buffer.
        i++;
      }
      buff[i - 1] = '\0'; //End it with the null byte.
      doc["message"] = buff;
      break;
    case OP_PRED:
      doc["op"] = "pred";
      doc["teamName"] = teamName;
      camera_fb_t *fb = esp_camera_fb_get();
      if (!fb) {
        Serial.println("Camera capture failed");
      } else {
        Serial.println("Camera capture successful!");
      }
      const char *data = (const char *)fb->buf;
      doc["message"] = *data;
      break;
  }
  //        Ok, now we need to send this baddy out on the websocket.
  serializeJson(doc, buff);
  client.send(buff);
}

void sendAruco() {
  if (aruco_visible) {
    float_converter_t f;
#ifdef DEBUG
    psl("Its visible.");
    put(aruco_x); put(aruco_y); putl(aruco_theta);
    f.f = aruco_x;
    for (int i = 0; i < 4; i++) {
      Serial.print(f.b[i], HEX); Serial.print(" ");
    }
    f.f = aruco_y;
    for (int i = 0; i < 4; i++) {
      Serial.print(f.b[i], HEX); Serial.print(" ");
    }
    f.f = aruco_theta;
    for (int i = 0; i < 4; i++) {
      Serial.print(f.b[i], HEX); Serial.print(" ");
    }
    pl();
#endif
    arduinoSerial.write(0x05);
    arduinoSerial.flush();
    f.f = aruco_x;
    arduinoSerial.write(f.b, 4);
    arduinoSerial.flush();
    f.f = aruco_y;
    arduinoSerial.write(f.b, 4);
    arduinoSerial.flush();
    f.f = aruco_theta;
    arduinoSerial.write(f.b, 4);
    arduinoSerial.flush();
  }
  else {
#ifdef DEBUG
    psl("Its not visible...");
#endif
    arduinoSerial.write(0x09);
  }
}

void onMessageCallback(WebsocketsMessage message) {
#ifdef DEBUG
  //    psl("Got message!");
  //    putl(message.data());
#endif
  StaticJsonDocument<300> doc;
  deserializeJson(doc, message.data());
  if (strcmp(doc["op"], "aruco") == 0) {
    aruco_visible = doc["aruco"]["visible"];
    aruco_x = doc["aruco"]["x"];
    aruco_y = doc["aruco"]["y"];
    aruco_theta = doc["aruco"]["theta"];
  }
  else if (strcmp(doc["op"], "info") == 0) {
    if (needToSendMission) {
      const float x = 0.55, theta = 0;
      float y;
      if (strcmp(doc["mission_loc"], "bottom") == 0) //mission location is on bottom
        y = 0.55;
      else
        y = 1.45;
      float_converter_t f;
      arduinoSerial.write(0x05);
      arduinoSerial.flush();
      f.f = x;
      arduinoSerial.write(f.b, 4);
      arduinoSerial.flush();
      f.f = y;
      arduinoSerial.write(f.b, 4);
      arduinoSerial.flush();
      f.f = theta;
      arduinoSerial.write(f.b, 4);
      arduinoSerial.flush();
      needToSendMission = false;
    }
  }
  else if (strcmp(doc["op"], "aruco_confirm") == 0) {
    arucoConfirmed = true;
#ifdef DEBUG
    psl("Aruco Confirmed...");
#endif
  }
  else if (strcmp(doc["op"], "prediction") == 0) {
    prediction = doc["prediction"];
  }
}
