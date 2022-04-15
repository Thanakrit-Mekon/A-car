#define RXD2          3
#define TXD2          1
#define RECV_PIN1     2
#define RECV_PIN2     4

int status_bit1 = 0;
int status_bit2 = 0;
int game_status = 0;

unsigned long game_st = millis();

// Idle(0) => 0 0 
// Ongoing(1) => 0 1
// Fucked up(2) => 1 0
// Win(3) => 1 1

void updateGameStatus(int bit1, int bit2){
  if (bit1 == 0 && bit2 == 0){
    // Idle
//    game_status = 0;
  }
  else if (bit1 == 0 && bit2 == 1){
    // Playing
//    game_status = 1;
  }
  else if (bit1 == 1 && bit2 == 0){
    // Lose
    game_status = 2;
  }
  else if (bit1 == 1 && bit2 == 1){
    // Win
    game_status = 3;
  }
}

void startGame() {
  game_status = 1;
  delay(50);
  Serial.write("40");
  game_st = millis();
}

void restartGame() {
  game_status = 0;
  delay(50);
  Serial.write("50");
}

void moveForward(int wheel) {
  String code2send;
  //Serial.printf("moveForward(%d)\n", wheel);
  //code2send = String(wheel) + " " + "1";
  //Serial.write(code2send.c_str());
  if (wheel == 0){
//    code2send = "01";
    Serial.write("01");
  }
  else if (wheel == 1){
//    code2send = "11";
    Serial.write("11");
  }
  else if (wheel == 2){
//    code2send = "21";
    Serial.write("21");
  }
  else if (wheel == 3){
//    code2send = "31";
    Serial.write("31");
  }
  if(game_status == 0){
    startGame();
  }
//  Serial.write(code2send.c_str());
}

void moveBackward(int wheel) {
  String code2send;
  //Serial.printf("moveBackward(%d)\n", wheel);
  //code2send = String(wheel) + " " + "2";
  //Serial.write(code2send.c_str());
  if (wheel == 0){
//    code2send = "02";
    Serial.write("02");
  }
  else if (wheel == 1){
//    code2send = "12";
    Serial.write("12");
  }
  else if (wheel == 2){
//    code2send = "22";
    Serial.write("22");
  }
  else if (wheel == 3){
//    code2send = "32";
    Serial.write("32");
  }
  if(game_status == 0){
    startGame();
  }
//  Serial.write(code2send.c_str());
}

void stopMoving(int wheel) {
  String code2send;
  //Serial.printf("stopMoving(%d)\n", wheel);
  //code2send = String(wheel) + " " + "0";
  //Serial.write(code2send.c_str());
  if (wheel == 0){
//    code2send = "00";
    Serial.write("00");
  }
  else if (wheel == 1){
//    code2send = "10";
    Serial.write("10");
  }
  else if (wheel == 2){
//    code2send = "20";
    Serial.write("20");
  }
  else if (wheel == 3){
//    code2send = "30";
      Serial.write("30");
  }
//  Serial.write(code2send.c_str());
}

void nucleoSetup() {
  pinMode(RECV_PIN1, INPUT); 
  pinMode(RECV_PIN2, INPUT); 
  Serial.begin(9600, SERIAL_8N1, RXD2, TXD2);
}

void nucleoLoop() {
  status_bit1 = digitalRead(RECV_PIN1);
  status_bit2 = digitalRead(RECV_PIN2);
  updateGameStatus(status_bit1, status_bit2);
}
