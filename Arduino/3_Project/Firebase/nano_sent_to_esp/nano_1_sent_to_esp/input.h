int sw_st;
int count;

#define a0 A0

void setup_input(){
  pinMode(a0,INPUT);
}


void read_a0(){
  val_a0 = analogRead(a0);
}

void read_print_a0(){
  read_a0();
  Serial.println("read from a0 is"+String(val_a0));
}




