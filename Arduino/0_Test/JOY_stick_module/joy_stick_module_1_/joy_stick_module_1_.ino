int X = A0;
int XValue = 0;
int Y = A1;
int YValue = 0;
int SW = A2;
int SWValue = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  XValue = analogRead(X);
  YValue = analogRead(Y);
  SWValue = analogRead(SW);
  Serial.print("XValue: ");
  Serial.print(XValue);
  Serial.print("\t");
  Serial.print("YValue: ");
  Serial.print(YValue);
  Serial.print("\t");
  Serial.print("SWValue: ");
  Serial.println(SWValue);
  delay(200);
}