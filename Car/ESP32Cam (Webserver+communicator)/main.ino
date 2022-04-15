#include "esp_camera.h"
#include <WiFi.h>
#define CAMERA_MODEL_AI_THINKER
#include "camera_pins.h"

#define LED_BUILTIN 33

const char *ssid = "yourAP";
const char *password = "yourPassword";

extern int wheels[];
void startCameraServer();

void initCamera() {
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
  //init with high specs to pre-allocate larger buffers
  if(psramFound()){
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  sensor_t * s = esp_camera_sensor_get();
  s->set_framesize(s, FRAMESIZE_CIF);
}

void setup() {
//   Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password, 1, 0);
  IPAddress myIP = WiFi.softAPIP();
  // Serial.print("AP IP address: ");
  // Serial.println(myIP);

  initESPNow();
  initCamera();
  startCameraServer();
  nucleoSetup();

  for (int i = 0; i < 3; i++) {
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(150);                       // wait for a second
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    delay(150);                       // wait for a second
  }
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
  nucleoLoop();
  delay(5);
}

/* =========================================
    > BLUETOOTH CALLBACK SERVICE
   ========================================= */

// void moveForward(int wheel) {
//   Serial.printf("moveForward(%d)\n", wheel);
//   wheels[wheel] = 1;
// }

// void moveBackward(int wheel) {
//   Serial.printf("moveBackward(%d)\n", wheel);
//   wheels[wheel] = 2;
// }

// void stopMoving(int wheel) {
//   Serial.printf("stopMoving(%d)\n", wheel);
//   wheels[wheel] = 0;
// }
