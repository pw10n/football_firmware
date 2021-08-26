
#define BOARD_HAS_WIFI

#ifndef ESP32
#include <WiFi101.h>
WiFi.setPins(8,7,4,2);
#endif

#include <Arduino_ConnectionHandler.h>
#include "secrets.h"


// set WIFI_SSID and WIFI_PASS in secrets.h
WiFiConnectionHandler conMan(WIFI_SSID, WIFI_PASS);

void setup(){
  Serial.begin(9600);
  while(!Serial) { }

  setDebugMessageLevel(DBG_INFO);

  conMan.addCallback(NetworkConnectionEvent::CONNECTED, onNetworkConnect);
  conMan.addCallback(NetworkConnectionEvent::DISCONNECTED, onNetworkDisconnect);
  conMan.addCallback(NetworkConnectionEvent::ERROR, onNetworkError);
}

void loop() {
  /* The following code keeps on running connection workflows on our
   * ConnectionHandler object, hence allowing reconnection in case of failure
   * and notification of connect/disconnect event if enabled (see
   * addConnectCallback/addDisconnectCallback) NOTE: any use of delay() within
   * the loop or methods called from it will delay the execution of .check(),
   * which might not guarantee the correct functioning of the ConnectionHandler
   * object.
   */
  conMan.check();
}
/* ... */
void onNetworkConnect() {
  Serial.println(">>>> CONNECTED to network");
  Client& client = conMan.getClient();
  bool currentLineIsBlank = true;
  while (client.connected()) {

      if (client.available()) {

        char c = client.read();

        Serial.write(c);

        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply

        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header

          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("hello world");
          client.println("</html>");

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

    }
}

void onNetworkDisconnect() {
  Serial.println(">>>> DISCONNECTED from network");
}

void onNetworkError() {
  Serial.println(">>>> ERROR");
}
