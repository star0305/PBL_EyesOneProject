#include <Keypad.h>
#include <Servo.h>
#include <SoftwareSerial.h>

#define RX 12
#define TX 13
SoftwareSerial Blutooth(RX, TX);

Servo servo;
const int motor = 11;

int input1=10; //gpio input 핀번호 10로 지정

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

void turn_left(){
  servo.write(0);
}

void turn_right(){
  servo.write(180);
}

void succsess(){ //보안 요구 만족시 5초간 문 개방
  turn_right();
  Serial.println("open the door"); 
  countface=0; //얼굴인식 성공 횟수 초기화
  countbt=0; //블루투스 성공횟수 초기화
  countkey=0; //키패드 성공횟수 초기화
  delay(5000);
  turn_left();
}

void fail(){
  turn_left();
  Serial.println("Please try again");
  Serial.println("Close the door");
}

void reset(){//#입력시 초기화
  tru=0;
  count=0;
  Serial.println("Passward reset");
}

void bt(){ //블루투스 입력 함수
  String data; //블루투스 데이터 입력 변수 지정
  if(Blutooth.available()){
    Serial.write(Blutooth.read());
    data = Blutooth.read();
    if(data){
      Serial.println("BT ok");
      countbt++;
    }
    else{
      Serial.println("BT not ok");
      Serial.println("Please try again");
    }
  }
  if(Serial.available()){
    Serial.println("");
    Blutooth.write(Serial.read());
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

void face(){ //얼굴인식이 되서 젯슨나노에서 gpio신호(전압==HIGH)보내주면 아두이노에서 인식하여 카운트 올림
//  int inputstate = digitalRead(input1);
  if(digitalRead(input1) == HIGH){
    countface++;
    Serial.println("Face recognition success");
  }
  else if(digitalRead(input1) == LOW){
    Serial.println("0");
  }
}

void Standby(){
  
}

void setup(){
  Serial.begin(9600);
  Blutooth.begin(9600);
  servo.attach(motor);   //서보 핀설정
  turn_left();     //서보 초기각도 0도 설정
  pinMode(input1, INPUT); //input1 핀번호를 INPUT신호로 설정
}

void loop(){
  keypad1();
  bt();
  face();
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
    Standby();
  }
}
