#include <Keypad.h>
#include <Servo.h>

Servo servo;
const int motor = 11;


int tru=0; //비밀번호 확인
int count=0;
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
  delay(1000);
  servo.write(0);
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

void setup(){
  Serial.begin(9600);
  servo.attach(motor);   //서보 핀설정
  servo.write(90);     //서보 초기각도 0도 설정
}

void loop(){

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
        succsess();
      }
    }
    else{
      fail();
    }
    tru=0;
    count=0;
  }
}
