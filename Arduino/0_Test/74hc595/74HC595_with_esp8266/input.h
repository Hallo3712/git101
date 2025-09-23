#define sw 2
int sw_st;
int count;

void setup_sw(){
  pinMode(sw,INPUT);
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