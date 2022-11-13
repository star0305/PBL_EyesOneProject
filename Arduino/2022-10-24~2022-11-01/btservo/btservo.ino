#include <SoftwareSerial.h>
#include <Servo.h>

#define RX 2
#define TX 3
SoftwareSerial hc06(RX, TX);

//String myString="";
//
//int motor = 11;
Servo servo;

void setup() {
  servo.attach(11);   //서보 시그널 핀설정
  servo.write(90);     //서보 초기각도 0도 설정
  Serial.begin(9600);
  hc06.begin(9600);

}

void loop() {
  if(hc06.available()){
    Serial.write(hc06.read());
    String data;
    data = hc06.read();

    if(data == 'a'){
      servo.write(0);
    }
    delay(100);
    servo.write(100);
  }
  if(Serial.available()){
    hc06.write(Serial.read());
  }
}
