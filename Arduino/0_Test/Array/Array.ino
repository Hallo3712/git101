// Declare an array of a given length without initializing the values:
char myInts[2] = "Hi";       //{0,0,0,0,0,0,};

int val;

void setup() {
  Serial.begin(9600);
/*
  myInts[0] = 1;
  myInts[1] = 2;
  myInts[2] = 3;
  myInts[3] = 1;
  myInts[4] = 2;
  myInts[5] = 3;*/

  Serial.println("myInts[0] is " + String(myInts[0]));
  Serial.println("myInts[1] is " + String(myInts[1]));
  /*Serial.println("myInts[2] is " + String(myInts[2]));
  Serial.println("myInts[3] is " + String(myInts[3]));
  Serial.println("myInts[4] is " + String(myInts[4]));
  Serial.println("myInts[5] is " + String(myInts[5]));
  */
}

void loop() {
  if(myInts[0]  == 'H' && myInts[1] == 'i'){
    Serial.println("A");
  }
}
