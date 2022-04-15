#include <esp_now.h>
#include <WiFi.h>

#if defined(REMOTE_FRONT_LEFT)
  #define WHEEL 0
#elif defined(REMOTE_FRONT_RIGHT)
  #define WHEEL 1
#elif defined(REMOTE_REAR_LEFT)
  #define WHEEL 2
#else defined(REMOTE_REAR_RIGHT)
  #define WHEEL 3
#endif

uint8_t broadcastAddress[] = {0x30, 0xC6, 0xF7, 0x22, 0xB3, 0xBD}; // To EspCam
//uint8_t broadcastAddress[] = {0xA4, 0xCF, 0x12, 0x80, 0xB7, 0x21}; // To Test
esp_now_peer_info_t peerInfo;

typedef struct struct_message {
    int wheel;
    int func;
} struct_message_t;

void initESPNow() {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  if (esp_now_init() == ESP_OK) {
    Serial.println("ESPNow Init Success");
  } else {
    Serial.println("ESPNow Init Failed");
    ESP.restart();
  }

  esp_now_register_send_cb(onDataSent);

  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  esp_err_t addStatus = esp_now_add_peer(&peerInfo);
  if (addStatus != ESP_OK) {
    Serial.println("Fail to Pair, Restart!");
    blinkLED(5, "red");
    ESP.restart();
  }
  blinkLED(2, "green");
  remoteStopMoving();
}

void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  // char macStr[18];
  // snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
  //  mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  // Serial.print("Last Packet Sent to: ");
  // Serial.println(macStr);
  Serial.print("Last Packet Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void sentToPeer(int func) {
  struct_message_t myData;
  myData.wheel = WHEEL;
  myData.func = func;
  Serial.println(func);
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  } else {
    Serial.println("Error sending the data");
  }
}

void remoteGoForward() {
  sentToPeer(1);
}

void remoteGoBackward() {
  sentToPeer(2);
}

void remoteStopMoving() {
  sentToPeer(0);
}
