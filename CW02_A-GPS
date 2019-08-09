#include <Arduino.h>

#include <WiFi.h>
#include <WiFiMulti.h>

#include <HTTPClient.h>

#include <Wire.h> 

#define USE_SERIAL Serial

long tick_Print = 0;

WiFiMulti wifiMulti;

void setup() {

  USE_SERIAL.begin(9600);

  /*USE_SERIAL.println();
  USE_SERIAL.println();
  USE_SERIAL.println();*/

  /*for (uint8_t t = 4; t > 0; t--) {
    USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);*/
    USE_SERIAL.flush();
    /*delay(1000);
  }*/
 wifiMulti.addAP("SSID", "PSK");

  // Start the I2C Communication
  Wire.begin();

  // wait for WiFi connection
  if ((wifiMulti.run() == WL_CONNECTED)) {

    HTTPClient http;

    //USE_SERIAL.print("[HTTP] begin...\n");

    // link to download ALMANAC
    http.begin("http://online-live1.services.u-blox.com/GetOnlineData.ashx?token=TOKEN;gnss=gps;datatype=eph,alm,aux,pos;filteronpos;format=aid;lat=;lon=;pacc=5000");
    //http.begin("192.168.1.12", 80, "/test.html");

    //USE_SERIAL.print("[HTTP] GET...\n");
    // start connection and send HTTP header
    int httpCode = http.GET();
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      //USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK) {

        // get lenght of document (is -1 when Server sends no Content-Length header)
        int len = http.getSize();

        // create buffer for read
        uint8_t buff[4000] = { 0 };

        // get tcp stream
        WiFiClient * stream = http.getStreamPtr();

        // read all data from server
        while (http.connected() && (len > 0 || len == -1)) {
          // get available data size
          size_t size = stream->available();

         // Serial.print("Size is: ");
         // Serial.println(size);

          if (size) {
            // read bytes
            int c = stream->readBytes(buff, ((size > sizeof(buff)) ? sizeof(buff) : size));

            // write it to Serial
            USE_SERIAL.write(buff, c);

            if (len > 0) {
              len -= c;
            }
          }
          delay(1);
        }

        //USE_SERIAL.println();
        //USE_SERIAL.print("[HTTP] connection closed or file end.\n");

      }
    } else {
      USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }

}

void loop() {
  
  Wire.requestFrom(0x42, 1);    // request 1 byte from slave device #2

  while(Wire.available())    // slave may send less than requested
  { 
    char c = Wire.read();    // receive a byte as character
    Serial.print(c);         // print the character
  }

  delay(1);
}
