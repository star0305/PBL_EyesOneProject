#include <Keypad.h>
#include <Servo.h>
#include <SoftwareSerial.h>

#define RX 12
#define TX 13
SoftwareSerial hc06(RX, TX);

Servo servo;
const int motor = 11;

int sec=0;

int tru=0; //비밀번호 확인
int count=0;

int countface=0; //얼굴인식 성공 횟수
int countbt=0; //블루투스 성공횟수
int countkey=0; //키패드 성공횟수

char pw[4]={'0', '0', '0', '0'}; //비밀번호

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'},
};

byte rowpins[ROWS] = {9, 8, 7, 6};
byte colpins[ROWS] = {5, 4, 3, 2};

Keypad keypad = Keypad(makeKeymap(keys), rowpins, colpins, ROWS, COLS);

void succsess(){
  servo.write(180);
  Serial.println("open the door"); 
  delay(500);
  servo.write(0);
  countface=0; //얼굴인식 성공 횟수 초기화
  countbt=0; //블루투스 성공횟수 초기화
  countkey=0; //키패드 성공횟수 초기화
}

void fail(){
  servo.write(0);
  Serial.println("close the door");
}

void reset(){//#입력시 초기화
  tru=0;
  count=0;
  Serial.println("Passward reset");
}

void bt(){ //블루투스 입력 함수
  String data;
  if(hc06.available()){
    Serial.write(hc06.read());
    data = hc06.read();
    if(data == 'a'){
      Serial.println("BT ok");
      countbt++;
     }
     else{
      Serial.println("BT not ok");
     }
  }
  if(Serial.available()){
    hc06.write(Serial.read());
  }
}

void keypad1(){ //키패드 입력 함수
  char key = keypad.getKey();
  if(key){
    Serial.println(key); //키패드 입력
    if(key==pw[count]){
      count++;
      tru++;
    }
    else if(key!=pw[count]){
      count++;
    }
    if(key=='#'){
      reset();
    }
    if(count==4){
      if(tru==4){
        Serial.println("keypad ok");
        countkey++;
      }
      else{
        fail();
      }
      tru=0;
      count=0;
    }
  }
}

void face(){
  
}

void setup(){
  Serial.begin(9600);
  hc06.begin(9600);
  servo.attach(motor);   //서보 핀설정
  servo.write(90);     //서보 초기각도 90도 설정
}

void loop(){  
  keypad1();
  bt();
  if((countbt>=1)&&(countkey>=1)){ //키패드, 블루투스 둘 다 성공시 서보작동
    succsess();
  }
  else if((countface>=1)&&(countkey>=1)){ //키패드, 얼굴인식 둘 다 성공시 서보작동
    succsess();
  }
  else if((countbt>=1)&&(countface>=1)){ //얼굴인식, 블루투스 둘 다 성공시 서보작동
    succsess();
  }
  else{
    fail();
  }
}
