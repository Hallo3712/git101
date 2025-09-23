// SING A SONGS WITH ARDUINO :: http://fitrox.lnwshop.com
// ARDUINO SING A CHRISTMAS SONGS

/*
  Arduino Christmas Songs
  Based on a project and code by Dipto Pratyaksa, updated on 31/3/13
  Modified for Christmas by Joshi, on Dec 17th, 2017.

  Adapted by FITROX ELECTRONICS, on Nov 21st, 2019
*/


#include "pitches.h"
#define melodyPin 9

// โน๊ตเพลง Jingle Bells
int jingle_melody[] = {
  NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_G5, NOTE_C5, NOTE_D5,
  NOTE_E5,
  NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5,
  NOTE_F5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_D5, NOTE_D5, NOTE_E5,
  NOTE_D5, NOTE_G5
};

// จังหวะเพลง Jingle bells
int jingle_tempo[] = {
  8, 8, 4,
  8, 8, 4,
  8, 8, 8, 8,
  2,
  8, 8, 8, 8,
  8, 8, 8, 16, 16,
  8, 8, 8, 8,
  4, 4
};

// โน๊ตเพลง We wish you a merry Christmas
int wish_melody[] = {
  NOTE_B3, 
  NOTE_F4, NOTE_F4, NOTE_G4, NOTE_F4, NOTE_E4,
  NOTE_D4, NOTE_D4, NOTE_D4,
  NOTE_G4, NOTE_G4, NOTE_A4, NOTE_G4, NOTE_F4,
  NOTE_E4, NOTE_E4, NOTE_E4,
  NOTE_A4, NOTE_A4, NOTE_B4, NOTE_A4, NOTE_G4,
  NOTE_F4, NOTE_D4, NOTE_B3, NOTE_B3,
  NOTE_D4, NOTE_G4, NOTE_E4,
  NOTE_F4
};

// จังหวะเพลง We wish you a merry Christmas
int wish_tempo[] = {
  4,
  4, 8, 8, 8, 8,
  4, 4, 4,
  4, 8, 8, 8, 8,
  4, 4, 4,
  4, 8, 8, 8, 8,
  4, 4, 8, 8,
  4, 4, 4,
  2
};

// โน๊ตเพลง Santa Claus is coming to town
int santa_melody[] = {
  NOTE_G4,
  NOTE_E4, NOTE_F4, NOTE_G4, NOTE_G4, NOTE_G4,
  NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, NOTE_C5,
  NOTE_E4, NOTE_F4, NOTE_G4, NOTE_G4, NOTE_G4,
  NOTE_A4, NOTE_G4, NOTE_F4, NOTE_F4,
  NOTE_E4, NOTE_G4, NOTE_C4, NOTE_E4,
  NOTE_D4, NOTE_F4, NOTE_B3,
  NOTE_C4
};

// จังหวะเพลง Santa Claus is coming to town
int santa_tempo[] = {
  8,
  8, 8, 4, 4, 4,
  8, 8, 4, 4, 4,
  8, 8, 4, 4, 4,
  8, 8, 4, 2,
  4, 4, 4, 4,
  4, 2, 4,
  1
};

// ตัวแปรเก็บสถานะของสวิตช์
int switch1 = 0;
int switch2 = 0;
int switch3 = 0;
// ตัวแปร song สำหรับเลือกเพลงที่จะเล่น
int song = 0;


void setup(void) {
// ตั้งค่า pinMode ของ Output
  pinMode(9, OUTPUT);           // ขา 9 ต่อกับ Buzzer
  pinMode(13, OUTPUT);          // LED on board จะติดสว่างตอน Arduino กำลังร้องเพลง

// ตั้งค่า pinMode ของ Input
  pinMode(2, INPUT);            // ขา 2 ต่อกับสวิตช์ 1
  pinMode(3, INPUT);            // ขา 3 ต่อกับสวิตช์ 2
  pinMode(4, INPUT);            // ขา 4 ต่อกับสวิตช์ 3
}


void loop() {
// ตรวจสอบการกดสวิตช์
  switch1 = digitalRead(2);
  switch2 = digitalRead(3);
  switch3 = digitalRead(4);


  if (switch1 == HIGH) { sing(1); }         // ถ้าสวิตช์ 1 ถูกกด ให้ทำงานฟังก์ชั่น sing(1)
  else if (switch2 == HIGH) { sing(2); }    // ถ้าสวิตช์ 2 ถูกกด ให้ทำงานฟังก์ชั่น sing(2) 
  else if (switch3 == HIGH) { sing(3); }    // ถ้าสวิตช์ 3 ถูกกด ให้ทำงานฟังก์ชั่น sing(3)
}
// end loop


// ฟังก์ชั่น sing
void sing(int s) {
  song = s;

// ถ้า song = 1 เล่นเพลง We wish you a Merry Christmas
  if (song == 1) {
    Serial.println(" 'We wish you a Merry Christmas'");
    // คำนวณหาขนาดของเพลง และเล่นวนซ้ำจนครบโน๊ตทุกตัวในเพลง
    int size = sizeof(wish_melody) / sizeof(int);
    for (int thisNote = 0; thisNote < size; thisNote++) {

      // คำนวณจังหวะ โดยใช้ 1 วินาที หารด้วย ประเภทของตัวโน๊ต
      // เช่น quarter note = 1000 / 4, eighth note = 1000/8, etc.
      int noteDuration = 1000 / wish_tempo[thisNote];

      // เรียกใช้ฟังก์ชั่น buzz เพื่อขับเสียงโน๊ตและจังหวะที่คำนวณได้ออกทางขา 9
      buzz(melodyPin, wish_melody[thisNote], noteDuration);

      // ตั้งค่า delay เพื่อทำการแยกโน๊ตแต่ละตัวออกจากกัน
      // ค่าที่เหมาะสมคือ จังหวะที่คำนวณได้ + 30%
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);

      // เมื่อเล่นจบเพลงให้หยุดโดยการสร้างโน๊ตความถี่ 0 Hz (ไม่มีเสียง)
      buzz(melodyPin, 0, noteDuration);
    }
  }

// ถ้า song = 2 เล่นเพลง Santa Claus is coming to town
// (ทำงานเหมือนเดิมเพียงแค่เปลี่ยนโน๊ตและจังหวะเป็นของ santa_melody)
  else if (song == 2) {
    Serial.println(" 'Santa Claus is coming to town'");
    int size = sizeof(santa_melody) / sizeof(int);
    for (int thisNote = 0; thisNote < size; thisNote++) {

      int noteDuration = 900 / santa_tempo[thisNote];

      buzz(melodyPin, santa_melody[thisNote], noteDuration);

      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);

      buzz(melodyPin, 0, noteDuration);
    }
  } 
  
// ถ้าเป็นค่าอื่นๆ (song = 3) เล่นเพลง Jingle Bells
// (ทำงานเหมือนเดิมเพียงแค่เปลี่ยนโน๊ตและจังหวะเป็น jingle_melody และ jingle_tempo)
  else {
    Serial.println(" 'Jingle Bells'");
    int size = sizeof(jingle_melody) / sizeof(int);
    for (int thisNote = 0; thisNote < size; thisNote++) {

      int noteDuration = 1000 / jingle_tempo[thisNote];

      buzz(melodyPin, jingle_melody[thisNote], noteDuration);

      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);

      buzz(melodyPin, 0, noteDuration);
    }
  }
}   // end sing(s)


// ฟังก์ชั่น buzz
void buzz(int targetPin, long frequency, long length) {
  // เมื่อส่งเสียงให้ LED on board ติดสว่างขึ้น
  digitalWrite(13, HIGH);
  
  // คำนวณการหน่วงเวลาระหว่างเปลี่ยนโน๊ต โดย 1 วินาที(ในรูปไมโครวินาที) หารด้วยความถี่ หารด้วย 2
  // (เนื่องจากสัญญาณมีสองซีก บน-ล่าง)
  long delayValue = 1000000 / frequency / 2;

  // คำนวณ cycle ที่เหมาะสมกับความถี่
  long numCycles = frequency * length / 1000;

  // เมื่อคำนวณได้นำมาใส่คำสั่ง for
  for (long i = 0; i < numCycles; i++) {
    digitalWrite(targetPin, HIGH);        // ส่งสัญญาณ HIGH ออก
    delayMicroseconds(delayValue);        // delay ตามค่าที่คำนวณได้
    digitalWrite(targetPin, LOW);         // ส่งสัญญาณ LOW ออก
    delayMicroseconds(delayValue);        // delay ตามค่าที่คำนวณได้
  }
  
  // เมื่อหยุดส่งเสียงให้ LED on board ดับลง
  digitalWrite(13, LOW);
}   // end buzz