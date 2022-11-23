#include <Keypad.h>
#include <Servo.h>
#include <SoftwareSerial.h>

#define RX 12
#define TX 13
SoftwareSerial Blutooth(RX, TX);

Servo servo;
const int motor = 11;

int tru=0; //올바른 비밀번호 입력시 카운트
int count=0; //비밀번호 입력시 카운트

int countface=0; //얼굴인식 성공 횟수
int countbt=0; //블루투스 성공횟수
int countkey=0; //키패드 성공횟수

char pw[5]={'0', '0', '0', '0', '0'}; //비밀번호설정

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

void turn_left(){ //도어락 잠금
  servo.write(0);
}

void turn_right(){ //도어락 열림
  servo.write(180);
}

void succsess(){ //보안요구 만족시 5초간 문 개방 후 잠금
  turn_right();
  Serial.println("open the door"); 
  countface=0; //얼굴인식 성공 횟수 초기화
  countbt=0; //블루투스 성공횟수 초기화
  countkey=0; //키패드 성공횟수 초기화
  delay(5000);
  turn_left();
}

void fail(){ //키패드 입력 실패시 도어락 잠금 및 아래 문구 출력
  turn_left();
  Serial.println("Please try again");
  Serial.println("Close the door");
}

void reset(){//#입력시 키패드입력 부분 초기화되서 키패드를 다시누르는 상황 만듬
  tru=0;
  count=0;
  Serial.println("Passward reset");
}

void bt(){ //블루투스 입력 함수, 블루투스 신호중 알맞은 데이터가 들어오면 블루투스 요구조건 만족으로 간주 및 카운트 올림
  String data; //블루투스 데이터 입력 변수 지정
  if(Blutooth.available()){
    Serial.write(Blutooth.read());
    data = Blutooth.read();
    if(data=='a'){
      Serial.println("BT ok");
      countbt++;
    }
    else if(data =! 'a'){ //올바른 블루투스 신호가 입력이 되지 않았을 시 아래 문구 출력
      Serial.println("BT not ok");
      Serial.println("Please try again");
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
    Serial.println(key);
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
    if(count==5){ //5자리 번호를 모두입력시
      if(tru==5){//5자리 번호를 입력하여 모두 맞으면 요구조건 만족된 것으로 간주하여 카운트 올림, 입력에 관한 카운트 0으로 초기화
        Serial.println("keypad ok");
        countkey++;
      }
      else{ //5자리 번호 입력 중 한자리 이상 틀렸을 시 도어락 잠금 및 실패 문구 출력
        fail();
      }
      tru=0;
      count=0;
    }
  }
}

void face(){ //정상 사용자 얼굴이 인식되면 젯슨나노에서 아날로그신호0을 보내줌->카운트 올려서 요구조건 만족된 것으로 간주
  if(analogRead(3) == 0){
    countface++;
    Serial.println("Face recognized");
  }
  else if(analogRead(3) =! 0){
    Serial.println("Face not recognized");
  }
}

void Standby(){
  Serial.println("Standby");
}

void setup(){
  Serial.begin(9600);
  Blutooth.begin(9600);
  servo.attach(motor);   //서보 핀설정
  turn_left();     //서보 초기각도 0도 설정->도어락잠금으로 시작
}

void loop(){
  keypad1();
  bt();
  face();
  if((countbt>=1)&&(countkey>=1)){ //키패드, 블루투스의 요구조건 만족시 서보작동
    succsess();
  }
  else if((countface>=1)&&(countkey>=1)){ //키패드, 얼굴인식의 요구조건 만족시 서보작동
    succsess();
  }
  else if((countbt>=1)&&(countface>=1)){ //얼굴인식, 블루투스의 요구조건 만족시 서보작동
    succsess();
  }
  else{
    Standby();
  }
}
