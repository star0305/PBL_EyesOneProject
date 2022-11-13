void setup() {
  // put your setup code here, to run once:

}

//블루투스로 신호 받으면 case문 동작
//젯슨으로 얼굴인식 하면 case문 동작
//키패드 입력 받으면 case문 동작

void loop() {
  switch(data){
    case bt_key:
      if(hc06.available()){
        Serial.write(hc06.read());
        String data;
        data = hc06.read();

       if(data == 'a'){
//      analogWrite(motor, 50);
         servo.write(50);
    }
  }
      break;
    case bt_face:

     break;
    case face_key:

      break;

      
//    case key_bt:
//
//      break;
//    case face_bt:
//
//      break;
//    case key_face:
//
//      break;
  }

}
