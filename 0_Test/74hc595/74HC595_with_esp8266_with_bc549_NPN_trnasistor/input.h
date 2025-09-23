#define sw 2
int sw_st;
int count;

#define a0 A0

void setup_sw(){
  pinMode(sw,INPUT);
  pinMode(a0,INPUT);
}

void get_value(){
  sw_st = digitalRead(sw);
  delay(200);

  switch (sw_st){
    case 0:
      break;
    case 1:
      count+=1;
      break;
  }
  if(count >= 5){
    count = 0;
  }
}

void read_a0(){
  val_a0 = analogRead(a0);
}

void map_a0(){
  val_a0 = map(val_a0,0,1023,0,180);
}

void read_print_a0(){
  read_a0();
  map_a0();
  Serial.println(val_a0);
}




