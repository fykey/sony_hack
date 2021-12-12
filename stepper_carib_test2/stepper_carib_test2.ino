/*
 * スイッチはD4，gndにつなげて，input_pullupキーワードを使った
 * https://mag.switch-science.com/2013/05/23/input_pullup/
 * キャリブレーション関数のテスト
 * 
 * スイッチを押すと止まるようになってる
 */
#define APHASE_s     0
#define  AENBL_s     1
#define BPHASE_s     2
#define  BENBL_s     3

#define sw_pin 4
int sw_val;
void setupStepper();
void caribrate();

unsigned long cnt = 0;

void count(){
  ++cnt;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  setupStepper();
  pinMode(sw_pin, INPUT_PULLUP);
  sw_val = digitalRead(sw_pin);
  attachInterrupt(digitalPinToInterrupt(sw_pin), count, CHANGE); 
  caribrate();

}

void loop() {
  // put your main code here, to run repeatedly:
  sw_val = digitalRead(sw_pin);
  Serial.println(cnt);

}


void setupStepper(){
   //横ステピン準備
  pinMode(APHASE_s,OUTPUT);
  pinMode(AENBL_s,OUTPUT);
  pinMode(BPHASE_s,OUTPUT);
  pinMode(BENBL_s,OUTPUT);
  digitalWrite(AENBL_s,HIGH);
  digitalWrite(BENBL_s,HIGH);

}

void caribrate(){//横ステッピングモーター広がる
  while(cnt == 0){
      digitalWrite(APHASE_s,HIGH);
      delayMicroseconds(1300);
      digitalWrite(BPHASE_s,LOW);
      delayMicroseconds(1300);
      digitalWrite(APHASE_s,LOW);
      delayMicroseconds(1300);
      digitalWrite(BPHASE_s, HIGH);
      delayMicroseconds(1300);
      
  }
  
      delay(500);
}
