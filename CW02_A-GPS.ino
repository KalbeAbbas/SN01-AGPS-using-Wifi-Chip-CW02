#include <Arduino.h>

#include <WiFi.h>
#include <WiFiMulti.h>

#include <HTTPClient.h>

#include <Wire.h>

#include <TinyGPS.h>

#define USE_SERIAL Serial

long tick_Print = 0;

TinyGPS gps;

WiFiMulti wifiMulti;

void setup() {

  USE_SERIAL.begin(9600);

    USE_SERIAL.flush();

 wifiMulti.addAP("SSID", "PSK");

  // Start the I2C Communication
  Wire.begin();

  // wait for WiFi connection
  if ((wifiMulti.run() == WL_CONNECTED)) {

    HTTPClient http;

    // Paste below ublox almanac downloading link
    http.begin("http://online-live1.services.u-blox.com/GetOnlineData.ashx?token=TOKEN;gnss=gps;datatype=eph,alm,aux,pos;filteronpos;format=aid;lat=LATITUDE;lon=LONGITUDE;pacc=5000");

    // start connection and send HTTP header
    int httpCode = http.GET();
    if (httpCode > 0) {

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

      }
    } else {
      USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }

}

void loop() {

  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;
  
  Wire.requestFrom(0x42, 1);    // request 1 byte from slave device #2

  while(Wire.available())    // slave may send less than requested
  { 
    char c = Wire.read();
    if (gps.encode(c)) // Did a new valid sentence come in?
    newData = true;// receive a byte as character
   // Serial.print(c);         // print the character
  }

    if (newData)
  {
    float flat, flon;
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);
    Serial.print("LAT=");
    Serial.println(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
    Serial.print(" LON=");
    Serial.println(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
    Serial.print(" SAT=");
    Serial.println(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
    Serial.print(" PREC=");
    Serial.println(gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop());
  }
  
  gps.stats(&chars, &sentences, &failed);
 // Serial.print(" CHARS=");
 //Serial.print(chars);
 // Serial.print(" SENTENCES=");
 // Serial.print(sentences);
 // Serial.print(" CSUM ERR=");
 // Serial.println(failed);
  if (chars == 0)
    Serial.println("** No characters received from GPS: check wiring **");

  delay(1);
}
