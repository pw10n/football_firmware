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
}

void onNetworkDisconnect() {
  Serial.println(">>>> DISCONNECTED from network");
}

void onNetworkError() {
  Serial.println(">>>> ERROR");
}
