/*
サーボモーターのテストプログラム．
digital pin 8, 9をサーボスイッチにPULL UPで接続
digital pin 10をタクトスイッチにPULL UPで接続
*/

#include <PCA9685.h>
#include <Wire.h>

#define SERVOMIN      150           
#define SERVOMAX      600 

#define ServoButton1 8
#define ServoButton2 9
#define sw_pin       10


int sw_val, servo1_val, servo2_val;
int ServoVal1, ServoVal2;
int n = 0;
PCA9685 pwm = PCA9685(0x40);

void setup() {
  // put your setup code here, to run once:
  
  
 
  
  pinMode(ServoButton1, INPUT);
  pinMode(ServoButton2, INPUT);
  pinMode(sw_pin, INPUT);
  pwm.begin();                   //初期設定 (アドレス0x40用)
  pwm.setPWMFreq(60);            //PWM周期を50Hzに設定 (アドレス0x40用)
  servo_write(15, 160);
  servo_write(7, 0);

}

void loop() {
    ServoVal1 = digitalRead(ServoButton1);
    ServoVal2 = digitalRead(ServoButton2);
    sw_val = digitalRead(sw_pin);

    //離しているとき，1で，押している時，0
    if(sw_val == 0){

    if(ServoVal1 == 1 && ServoVal2 == 1){
      gCatch();
    }
    if(ServoVal1 == 0 || ServoVal2 == 0){
      gRelease();
    }
    }
    else{
      delay(500);
    }
  

}

void servo_write(int ch, int ang){//サーボに使う
     //動かすサーボチャンネルと角度を指定
   ang = map(ang, 0, 180, SERVOMIN, SERVOMAX);
     //角度（0～180）をPWMのパルス幅（102～492）に変換
   pwm.setPWM(ch, 0, ang);
     //  pwm2.setPWM(0, 0, ang);
 }  

 void gCatch(){//サーボリリース
  servo_write(7,130);//80~160
  servo_write(15,80);  //80~0
  delay(1000);

}

void gRelease(){//サーボキャッチ
  servo_write(7,20);//80~160
  servo_write(15,180);  //80~0
  delay(1000);

}

int pulse(int miri){
  return miri * 50;
}
 
