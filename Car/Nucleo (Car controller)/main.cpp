#include "mbed.h"
#include "config.h"
#include <cstdio>
#include <cstring>

#define BUFF_SIZE 2

using namespace std;
using namespace ThisThread;

static BufferedSerial pc(PB_6,PB_7);

// Communication back //
DigitalOut com_pin_1(D11);
DigitalOut com_pin_2(D12);

// Front motot //
DigitalOut right_front1(FRONT_L3);
DigitalOut right_front2(FRONT_L4);
DigitalOut left_front1(FRONT_L1);
DigitalOut left_front2(FRONT_L2);

// Rear motor //
DigitalOut right_rear1(REAR_L1);
DigitalOut right_rear2(REAR_L2);
DigitalOut left_rear1(REAR_L3);
DigitalOut left_rear2(REAR_L4);

// IR //
AnalogIn ir_fr(FR_IR);
AnalogIn ir_fl(FL_IR);
AnalogIn ir_rr(RR_IR);
AnalogIn ir_rl(RL_IR);
AnalogIn ldr(LDR);

int status = 0;
int ref_IR;
int REF_MUL = 2;
int REF_ADD = 0; 

void send_game_stats(){
    // Standby(0) => 0 0 
    // Playing(1) => 0 1
    // Lost(2) => 1 0
    // Win(3) => 1 1
    if (status == 0){
        com_pin_1 = 0;
        com_pin_2 = 0;
    }
    else if(status == 1){
        com_pin_1 = 0;
        com_pin_2 = 1;
    }
    else if(status == 2){
        com_pin_1 = 1;
        com_pin_2 = 0;
    }
    else if(status == 3){
        com_pin_1 = 1;
        com_pin_2 = 1;
    }
}

void setStatus(int newStatus){
    status = newStatus;
    send_game_stats();
}

void FR(char dir) {
    if(dir == '1'){
        right_front1 = 1;
        right_front2 = 0;
    }
    else if(dir == '0'){
        right_front1 = 0;
        right_front2 = 0;
    }
    else if(dir == '2'){
        right_front1 = 0;
        right_front2 = 1;
    }
}

void FL(char dir){
    if(dir == '1'){
        left_front1 = 0;
        left_front2 = 1;
    }
    else if(dir == '0'){
        left_front1 = 0;
        left_front2 = 0;
    }
    else if(dir == '2'){
        left_front1 = 1;
        left_front2 = 0;
    }
}

void RR(char dir){
    if(dir == '1'){
        right_rear1 = 0;
        right_rear2 = 1;
    }
    else if(dir == '0'){
        right_rear1 = 0;
        right_rear2 = 0;
    }
    else if(dir == '2'){
        right_rear1 = 1;
        right_rear2 = 0;
    }
}

void RL(char dir){
    if(dir == '1'){
        left_rear1 = 0;
        left_rear2 = 1;
    }
    else if(dir == '0'){
        left_rear1 = 0;
        left_rear2 = 0;
    }
    else if(dir == '2'){
        left_rear1 = 1;
        left_rear2 = 0;
    }
}

bool isWin(){
	int ref_win = 2000;
    if(ldr.read_u16() < ref_win)
        return true;
    return false;
}


bool isDead(){
    int ref_dead = 65500;//ref_IR * REF_MUL + REF_ADD;
    if(ir_fl.read_u16() > ref_dead){
        return true;
    }
    if(ir_fr.read_u16() > ref_dead){
        return true;
    }
     if(ir_rl.read_u16() > ref_dead){
        return true;
    }
     if(ir_rr.read_u16() > ref_dead){
         return true;
     }

    ref_IR = ir_fl.read_u16();
    return false;
}

void setRefIR(){
    ref_IR = ir_fl.read_u16();
}

void startGame(){
    setStatus(1);
    setRefIR();
}

int main(){  
    printf("Start car engine\n"); 
    pc.set_baud(9600);
    char buffer[BUFF_SIZE] = {};
    
    while(1){
        sleep_for(50);
        if(pc.readable()){
            memset(buffer, 0, BUFF_SIZE);
            pc.read(buffer, sizeof(buffer));
            printf("%s \n",buffer); 
            char cmd = buffer[0];  
            char action = buffer[1];
            

            if (cmd == '0'){
                FL(action);
            }
            if (cmd == '1'){
                FR(action);
            }
            if (cmd == '2'){
                RL(action);
            }
            if (cmd == '3'){
                RR(action);
            }
            if (cmd == '4'){
                startGame();
            }
            if(cmd == '5'){
                setStatus(0);
            }
        }
    // printf("Front right IR: %d \n", ir_fr.read_u16());
    //  printf("Front left IR: %u \n", ir_fl.read_u16());
    //  printf("Rear right IR: %d \n", ir_rr.read_u16());
    //  printf("Rear left IR: %d \n", ir_rl.read_u16());
    //  printf("LDR : %u\n",ldr.read_u16());
    //FL('0');
    //FR('0');
    //RL('0');
    //RR('0');
    //printf("status : %d\n",status);
        if(status == 1){
            // printf("Playing..");
            if(isDead()){
                setStatus(2);
                 printf("LOSE");
            }else
            if(isWin()){
                setStatus(3);
                 printf("WIN");
            }
        }

    }
}



