#define led 2
#define pir 3
boolean pir_st;


void setup() {
  Serial.begin(9600);
  pinMode(led,OUTPUT);
  pinMode(pir,INPUT);

}

void loop() {
  pir_st = digitalRead(pir);

  if(pir_st == 1){
    digitalWrite(led,HIGH);
    Serial.println("detect");
  }
  else{
    digitalWrite(led,LOW);
    Serial.println("Off");
  }
  


}
