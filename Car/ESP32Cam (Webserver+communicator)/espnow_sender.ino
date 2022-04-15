#include <esp_now.h>
#include <WiFi.h>
#include <queue>

#define CHANNEL 1

typedef struct struct_message {
  int wheel;
  int func;
} struct_message_t;

extern int wheels[];

void initESPNow() {
//  WiFi.mode(WIFI_AP);
//  const char *SSID = "Slave_1";
//  bool result = WiFi.softAP(SSID, "Slave_1_Password", CHANNEL, 0);
//  if (!result) {
//    Serial.println("AP Config failed.");
//  } else {
//    Serial.println("AP Config Success. Broadcasting with AP: " + String(SSID));
//  }
  WiFi.disconnect();
  if (esp_now_init() == ESP_OK) {
    // Serial.println("ESPNow Init Success");
  } else {
    // Serial.println("ESPNow Init Failed");
    ESP.restart();
  }
  esp_now_register_recv_cb(onDataRecv);
  _rtos_callback_service_start();
  // Serial.print("AP MAC: "); Serial.println(WiFi.softAPmacAddress());
}

// callback when data is recv from Master
String mac_addrs[4];
void onDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
   char macStr[18];
   snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
            mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  // Serial.print("Last Packet Recv from: "); Serial.println(macStr);
  struct_message_t myData;
  memcpy(&myData, data, sizeof(myData));
  // Serial.print("Last Packet Recv Data: "); Serial.println(myData.func);
  // Serial.println("");

  bool isAlreadyIn = false;
  int i;
  for (i = 0; i < 4; i++) {
    if (mac_addrs[i] == macStr) {
      isAlreadyIn = true;
      break;
    }
  }

  if (!isAlreadyIn) {
    for (i = 0; i < 4; i++) {
      if (mac_addrs[i] == "") {
        mac_addrs[i] = macStr;
        break;
      }
    }
  }

  int func = myData.func;
  int wheel = myData.wheel;
  
  wheels[wheel] = func;
  _add_service_queue(wheel, func);
  // if (func == 1) {
  //   moveForward(wheel);
  // } else if (func == 2) {
  //   moveBackward(wheel);
  // } else {
  //   stopMoving(wheel);
  // }
}

int getCntClient() {
  int cnt = 0;
  for (int i = 0; i < 4; i++) {
    if (mac_addrs[i] != "") {
      cnt++;
    }
  }
  return cnt;
}

/* =========================================
    > RTOS CALLBACK SERVICE
   ========================================= */
std::queue<std::pair<int, int>> espnow_queue;
void _add_service_queue(int wheel, int func) {
  espnow_queue.push(std::make_pair(wheel, func));
}

void _rtos_callback_service_start() {
  xTaskCreatePinnedToCore(
    _rtos_callback_task,
    "Callback",
    10240,
    NULL,
    2,
    NULL,
    1);
}

void _rtos_callback_task(void* ptr) {
  while (1) {
    if (!espnow_queue.empty()) {
      std::pair<int, int> carry = espnow_queue.front();
      int wheel = carry.first;
      int func = carry.second;
//      Serial.printf("%d %d\n", wheel, func);
      if (func == 1) {
        moveForward(wheel);
      } else if (func == 2) {
        moveBackward(wheel);
      } else {
        stopMoving(wheel);
      }
      espnow_queue.pop();
    }
    delay(50);
  }
}
