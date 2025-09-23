#define S0_PIN 8
#define S1_PIN 9
#define S2_PIN 10
#define S3_PIN 11

#define Z_PIN A0  //หากเป็น4067 ให้ต่อขา SIGกับA0

int val1=0;
int val2=0;
int val3=0;
int analog=0;
float volt=0;

void setup() {
  Serial.begin(115200);
  pinMode(S0_PIN, OUTPUT);
  pinMode(S1_PIN, OUTPUT);
  pinMode(S2_PIN, OUTPUT);
  pinMode(Z_PIN,INPUT);
  Serial.println("end setup");
}

void loop() {
  
  int dl1 = 10;
  val1 = analog_Read(0);  //อ่านค่าจาก Y0
  //delay(dl1);
  val2 = analog_Read(1);
  //delay(dl1);
  val3 = analog_Read(2);
  //delay(dl1);

  val1 = map(val1, 0, 1023, 0, 180);
  val2 = map(val2, 0, 1023, 0, 180);
  val3 = map(val3, 0, 1023, 0, 180);

  Serial.print("val1 = " + String(val1));
  Serial.print(" val2 = " + String(val2));
  Serial.println(" val3 = "+String(val3));

  delay(1000);
  
/*
  analog = analog_Read(0);      //อ่านค่าจาก Y0
  volt = (analog*5.0)/1023;
  Serial.print("Y0=");
  Serial.println(volt);
  analog = analog_Read(1);      //อ่านค่าจาก Y1
  volt = (analog*5.0)/1023;
  Serial.print("Y1=");
  Serial.println(volt);
  analog = analog_Read(2);      //อ่านค่าจาก Y2
  volt = (analog*5.0)/1023;
  Serial.print("Y2=");
  Serial.println(volt);
  Serial.println("");
  delay(1000);
*/





}

int analog_Read(byte ch) {  //ฟังชั่นอ่านค่าanalog
  boolean s0[16] = { 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1 };
  boolean s1[16] = { 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1 };
  boolean s2[16] = { 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1 };
  boolean s3[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1 };
  digitalWrite(S0_PIN, s0[ch]);
  digitalWrite(S1_PIN, s1[ch]);
  digitalWrite(S2_PIN, s2[ch]);
  digitalWrite(S3_PIN, s3[ch]);
  delay(1);
  return analogRead(Z_PIN);
}