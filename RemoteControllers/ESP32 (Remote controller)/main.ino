#include <esp_now.h>

#define REMOTE_REAR_LEFT //to define which remote control which wheel
#define LED_BUILTIN 2
#define BTN_FORWARD 32
#define BTN_BACKWARD 33
#define red 19
#define green  18
#define blue  17

//AON remote : 21 16 13 27 26
//CHAW remote : 32 33 19 18 17

String lastColor = "red";

bool prevForwardBtn = 1,
     prevBackwardBtn = 1,
     ForwardBtn = 0,
     BackwardBtn = 0,
     isConnected = 0;

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BTN_FORWARD, INPUT_PULLUP);
  pinMode(BTN_BACKWARD, INPUT_PULLUP);

  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
  
  Serial.println("Connecting...");
  initESPNow();
}

void loop() {
  ForwardBtn = digitalRead(BTN_FORWARD);
  BackwardBtn = digitalRead(BTN_BACKWARD);
  delay(20);
  
  if (prevForwardBtn != ForwardBtn) {
    if (ForwardBtn == 1) {
      // Up
      lightLED("blue",LOW);
      remoteStopMoving();
      Serial.println("Release forward");
    } else {
      // Down
      lightLED("blue",HIGH);
      remoteGoForward();
      Serial.println("Press forward");
    }
    prevForwardBtn = ForwardBtn;
  }

  if (prevBackwardBtn != BackwardBtn) {
    if (BackwardBtn == 1) {
      // Up
      lightLED("green",LOW);
      remoteStopMoving();
      Serial.println("Release backward");
    } else {
      // Down
      remoteGoBackward();
      lightLED("green",HIGH);
      Serial.println("Press backward");
    }
    prevBackwardBtn = BackwardBtn;
  }
}


void blinkLED(int times,String cmd_color) {
  String cmd = lastColor;
  if(cmd_color != ""){
    cmd=cmd_color;
  }
  for (int i = 0; i < times; i++) {
    lightLED(cmd,HIGH);
    delay(150);
    lightLED(cmd,LOW);
    delay(150);
  }
}

void lightLED(String color,int cmd) {
  lastColor = color;
  if(color == "red"){
    digitalWrite(red,cmd);
  }else if(color == "blue"){
    digitalWrite(blue,cmd);
  }else if(color == "green"){
    digitalWrite(green,cmd);
  }
}
