#include <PCA9685.h>
#include <Wire.h>

#define SERVOMIN 150           
#define SERVOMAX 600 
#define W_MAX 130
#define W_DEFAULT 110

#define FOWARD_RANGE 30

#define APHASE_s     0
#define  AENBL_s     1
#define BPHASE_s     2
#define  BENBL_s     3

/*横ステピンの配線，秋月との対応は，0⇨4, 1⇨5, 2⇨6, 3⇨7
ドライバとの対応は，0⇨AIN1, 1⇨AIN2, 2⇨BIN1, 3⇨BIN2     */

#define APHASE_f     4
#define  AENBL_f     5
#define BPHASE_f     6
#define  BENBL_f     7

/*縦ステピンの配線
ドライバとの対応は，4⇨AIN1, 5⇨AIN2, 6⇨BIN1, 7⇨BIN2     */

#define ServoButton1 8
#define ServoButton2 9

#define sw_pin       10

//ボタンプッシュでhigh, ServoButton1 か ServoButton2 がhigh なら掴んでいる

struct User{
  char nam[50];
  int face_w;
  int face_d;
};

int sw_val, servo1_val, servo2_val ;//変更したので，スイッチ制御の場合は配線に注意！！！！
int ServoVal1, ServoVal2;
int n = 0;


PCA9685 pwm = PCA9685(0x40);

struct User Ms[5] = {
    {"fuyuki", 150, 6},
    {"daisuke", 140, 6},
    {"seizi", 140, 5},
    {"hiroki", 150, 6},
    {"wataru", 160, 7}
    
  };//構造体で管理．呼び出す場合は，Ms[i].nam　or Ms[i].face_w
void setup() {
  /*Serial.begin(9600);
  Wire.begin();
  
  I2C通信する際に必要*/
  Serial.begin(115200);
  
  setupStepper();
  
  pinMode(sw_pin, INPUT);//タクトスイッチ
  pinMode(ServoButton1, INPUT);
  pinMode(ServoButton2, INPUT);//サーボスイッチ
  
  pwm.begin();                   //初期設定 (アドレス0x40用)
  pwm.setPWMFreq(60);            //PWM周期を50Hzに設定 (アドレス0x40用)
  servo_write(15, 160);
  servo_write(7, 0);

  }
  

void loop(){

  //まずはタクトスイッチの状態を確認する→AI用spresenseからの信号受信にも変更可能
    sw_val = digitalRead(sw_pin); //high なら顔を認識した．

    
    if(sw_val == 1){

  

   /* stepping_motor_s_plus(pulse((Ms[2].face_w - W_DEFAULT + 10) / 4));//mmをpulse関数の引数に入力 (W_MAX - Ms[2].face_w) / 2)とか
    delay(500);
    stepping_motor_f_foward(); //前にFOWARD_RANGE分動く
    delay(500);
    
   stepping_motor_s_minus(pulse(5));//mmをpulse関数の引数に入力 (W_MAX - Ms[2].face_w) / 2) とか
    ServoVal1 = digitalRead(ServoButton1);
    ServoVal2 = digitalRead(ServoButton2);
    Serial.print(ServoVal1);
    Serial.print("  ");
    Serial.println(ServoVal2);
  delay(500);*/
    if(n % 2 == 1){//掴んでいる場合，リリース
      stepping_motor_s_plus(pulse(3));//mmをpulse関数の引数に入力 (W_MAX - Ms[2].face_w) / 2)とか
    delay(500);
    stepping_motor_f_foward(); //前にFOWARD_RANGE分動く
    delay(500);
    
   stepping_motor_s_minus(pulse(3));//mmをpulse関数の引数に入力 (W_MAX - Ms[2].face_w) / 2) とか
      gRelease();
   stepping_motor_s_plus(pulse(5));//mmをpulse関数の引数に入力 (W_MAX - Ms[2].face_w) / 2)とか
       stepping_motor_f_back();

    stepping_motor_s_minus(pulse(3));//mmをpulse関数の引数に入力.定位置になるよう調整すること

    }else if(n % 2 == 0){//離している時，キャッチ
    stepping_motor_s_minus(pulse(10));  //キャリブレーションで原点合わせ
    
    stepping_motor_s_plus(pulse((Ms[2].face_w - W_DEFAULT) / 4));//mmをpulse関数の引数に入力 (W_MAX - Ms[2].face_w) / 2)とか
    delay(500);
    stepping_motor_f_foward(); //前にFOWARD_RANGE分動く
    delay(500);
                                     
   stepping_motor_s_minus(pulse(Ms[2].face_d));//mmをpulse関数の引数に入力 (W_MAX - Ms[2].face_w) / 2) とか
      gCatch();

    stepping_motor_f_back();

    //stepping_motor_s_minus(pulse());//mmをpulse関数の引数に入力.定位置になるよう調整すること

    }

    delay(500); 
    //stepping_motor_s_plus(pulse(5));//mmをpulse関数の引数に入力 (W_MAX - Ms[2].face_w) / 2) とか

       delay(1000);
    n ++;
 }

}

//関数定義

void setupStepper(){
   //横ステピン準備
  pinMode(APHASE_s,OUTPUT);
  pinMode(AENBL_s,OUTPUT);
  pinMode(BPHASE_s,OUTPUT);
  pinMode(BENBL_s,OUTPUT);
  digitalWrite(AENBL_s,HIGH);
  digitalWrite(BENBL_s,HIGH);
  //縦ステピン準備
  pinMode(APHASE_f,OUTPUT);
  pinMode(AENBL_f,OUTPUT);
  pinMode(BPHASE_f,OUTPUT);
  pinMode(BENBL_f,OUTPUT);
  digitalWrite(AENBL_f,HIGH);
  digitalWrite(BENBL_f,HIGH);

}

void stepping_motor_s_plus(int j){//横ステッピングモーター広がる
int i = 0;

for(i = 0; i < j; i++){
      digitalWrite(APHASE_s,HIGH);
      delayMicroseconds(1300);
      digitalWrite(BPHASE_s,HIGH);
      delayMicroseconds(1300);
      digitalWrite(APHASE_s,LOW);
      delayMicroseconds(1300);
      digitalWrite(BPHASE_s,LOW);
      delayMicroseconds(1300);
}

delay(500);
}

void stepping_motor_s_minus(int j){//横ステッピングモーター狭まる
int i = 0;
for(i = 0; i < j; i++){
      digitalWrite(APHASE_s,HIGH);
      delayMicroseconds(1300);
      digitalWrite(BPHASE_s,LOW);
      delayMicroseconds(1300);
      digitalWrite(APHASE_s,LOW);
      delayMicroseconds(1300);
      digitalWrite(BPHASE_s,HIGH);
      delayMicroseconds(1300);
}

delay(500);
}


void stepping_motor_f_foward(){//縦ステッピングモーター前進
int i = 0;

for(i = 0; i < pulse(FOWARD_RANGE); i++){
      digitalWrite(APHASE_f,HIGH);
      delayMicroseconds(1300);
      digitalWrite(BPHASE_f,HIGH);
      delayMicroseconds(1300);
      digitalWrite(APHASE_f,LOW);
      delayMicroseconds(1300);
      digitalWrite(BPHASE_f,LOW);
      delayMicroseconds(1300);
}

delay(500);
}

void stepping_motor_f_back(){//縦ステッピングモーター後退
int i = 0;
for(i = 0; i < pulse(FOWARD_RANGE); i++){
      digitalWrite(APHASE_f,HIGH);
      delayMicroseconds(1300);
      digitalWrite(BPHASE_f,LOW);
      delayMicroseconds(1300);
      digitalWrite(APHASE_f,LOW);
      delayMicroseconds(1300);
      digitalWrite(BPHASE_f,HIGH);
      delayMicroseconds(1300);
}

delay(500);
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
 
