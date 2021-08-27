
#include "secrets.h"
#include <SPI.h>
#ifdef ESP32
#include <WiFi.h>
#define LOCK_PIN LED_BUILTIN
#else
#include <WiFi101.h>
#define LOCK_PIN 5
#endif

#define UNLOCK_DELAY 30000

int status = WL_IDLE_STATUS;

WiFiServer server(80);

void setup() {
  // set output pin
  pinMode(LOCK_PIN, OUTPUT);
  
  // initialize serial:
  Serial.begin(115200);
  Serial.println("Attempting to connect...");

  status = WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Connecting to wifi");
  while ( status != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
    status = WiFi.status();
  }

  Serial.println("OK");
  Serial.print("Connected to wifi. My address: ");
  IPAddress myAddress = WiFi.localIP();
  Serial.println(myAddress);

  server.begin();
  Serial.println(">>>> starting server...");
}

void loop() {
  
  WiFiClient client = server.available();
  bool currentLineIsBlank = true;
  
  if (client.connected()){
    String* requestBuffer = new String();
    Serial.println(">>>> client connected...");
    do {

      if (client.available()) {

        char c = client.read();

        (*requestBuffer) += c;

        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply

        if (c == '\n' && currentLineIsBlank) {

          Serial.println(*requestBuffer);
          // send a standard http response header

          if (requestBuffer->startsWith("GET /foo")){
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("Connection: close");  // the connection will be closed after completion of the response
            client.println();
            client.println("<!DOCTYPE HTML>");
            client.println("<html>");
            client.println("bar");
            client.println("</html>");
          } else if (requestBuffer->startsWith("POST /unlock")) {
            Serial.println(" >>> GOT UNLOCK COMMAND <<< ");
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("Connection: close");  // the connection will be closed after completion of the response
            client.println();
            client.println("<!DOCTYPE HTML>");
            client.println("<html>");
            client.println("ok");
            client.println("</html>");
            digitalWrite(LOCK_PIN, HIGH);
            delay(UNLOCK_DELAY);
            digitalWrite(LOCK_PIN, LOW);
          } else {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("Connection: close");  // the connection will be closed after completion of the response
            client.println();
            client.println("<!DOCTYPE HTML>");
            client.println("<html>");
            client.println("hello");
            client.println("</html>");
          }
          break;
        }

        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }

      }

    }while (client.connected());
  }
}
