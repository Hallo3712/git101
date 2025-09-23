


int d1;
int d2;
int d3;
int d4;
int d5;
String status;

void setup() {
  Serial.begin(9600);
  delay(3000);
  Serial.println("end setup ");
  delay(3000);
}

void loop() {
  if (Serial.available() > 0) {
    Serial.println("something");
    // รับค่า
    String strIN = Serial.readStringUntil('\n');
    Serial.println("strIN is " + String(strIN));  //ปริ้น

    // แปลงเป็น const char
    //const char* cstrIn = strIN.c_str();

    int result_sscanf = sscanf(strIN.c_str(), "%d,%d,%d,%d,%d", &d1, &d2, &d3, &d4, &d5);
    Serial.println("result_sscanf is " + String(result_sscanf));


    status = String(d1) + "," + String(d2) + "," + String(d3) + "," + String(d4) + "," + String(d5);

    Serial.println("status in esp is " + String(status));
    //espserial.println("status in esp is " + String(status));
  } else if (Serial.available() > 0) {
    String val = Serial.readString();
    Serial.println(val);
  } else {
    Serial.println("nothing");
    delay(1000);
  }
}
