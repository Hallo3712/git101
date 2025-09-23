#include "Arduino.h"
/*
void if_serial_available() {
  if (Serial.available()) {
    msg = Serial.readStringUntil("\n");
    Serial.println("get message " + String(msg));
    delay(200);

    if (msg == "all high\n") {
      sr.setAllHigh();
      Serial.println("set all high");
      delay(100);
      led_high();
    } else if (msg == "all low\n") {
      sr.setAllLow();
      Serial.println("set all low");
      delay(100);
      led_low();
    } else if (msg == "change map\n") {
      Serial.println("change map");
      Serial.println("wait for data");
      while (1) {
        if (Serial.available()) {
          msg = Serial.readStringUntil("\n");
          var_map = msg.toInt();
          Serial.println("var_map is " + String(var_map));
          delay(200);
          break;
        }
        Serial.print(".");
        delay(100);
      }
      Serial.println("end change map");
    }
    
    //                              1
    else if (msg == "1 high\n") {
      sr.set(1, HIGH);
      Serial.println("set 1 high");
      delay(100);
    } else if (msg == "1 low\n") {
      sr.set(1, LOW);
      Serial.println("set 1 low");
      delay(100);
    }
    //                              2
    else if (msg == "2 high\n") {
      sr.set(2, HIGH);
      Serial.println("set 2 high");
      delay(100);
    } else if (msg == "2 low\n") {
      sr.set(2, LOW);
      Serial.println("set 2 low");
      delay(100);
    }
  }
}*/

void map_value() {
  val = map(val_a0, 0, var_map, 0, 180);

  Serial.println("a0 after map is" + String(val));
  espSerial.println(val + String(",") + val);
}

void read_and_get_value_from_channal(int ch) {
  sr.set(ch, HIGH);
  read_a0();

  switch (ch) {
    case 1:
      p1 = map(val_a0, 0, var_map, 0, 180);
      break;
    case 2:
      p2 = map(val_a0, 0, var_map, 0, 180);
      break;
    case 3:
      break;
    case 4:
      break;
  }
}

void sent_to_esp(){
  Serial.println(p1 + String(",") + p2);
  espSerial.println(p1 + String(",") + p2);
}




