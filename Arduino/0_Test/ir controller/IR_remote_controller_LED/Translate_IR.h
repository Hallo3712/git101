void translate_IR() {

  Key21 key = irController.getKey();
  if (key != Key21::NONE) {
    switch (key) {
      case Key21::KEY_CH_MINUS:
        Serial.println("CH-");
        translate_IR_data = 10;
        break;

      case Key21::KEY_CH:
        Serial.println("CH");
        translate_IR_data = 11;
        break;

      case Key21::KEY_CH_PLUS:
        Serial.println("CH+");
        translate_IR_data = 12;
        break;

      case Key21::KEY_PREV:
        Serial.println("<<");
        translate_IR_data = 13;
        break;

      case Key21::KEY_NEXT:
        Serial.println(">>");
        translate_IR_data = 14;
        break;

      case Key21::KEY_PLAY_PAUSE:
        Serial.println(">||");
        translate_IR_data = 15;
        break;

      case Key21::KEY_VOL_MINUS:
        Serial.println("–");
        translate_IR_data = 16;
        break;

      case Key21::KEY_VOL_PLUS:
        Serial.println("+");
        translate_IR_data = 17;
        break;

      case Key21::KEY_EQ:
        Serial.println("EQ");
        translate_IR_data = 18;
        break;

      case Key21::KEY_100_PLUS:
        Serial.println("100+");
        translate_IR_data = 19;
        break;

      case Key21::KEY_200_PLUS:
        Serial.println("200+");
        translate_IR_data = 20;
        break;

      case Key21::KEY_0:
        Serial.println("0");
        translate_IR_data = 0;
        break;

      case Key21::KEY_1:
        Serial.println("1");
        translate_IR_data = 1;
        break;

      case Key21::KEY_2:
        Serial.println("2");
        translate_IR_data = 2;
        break;

      case Key21::KEY_3:
        Serial.println("3");
        translate_IR_data = 3;
        break;

      case Key21::KEY_4:
        Serial.println("4");
        translate_IR_data = 4;
        break;

      case Key21::KEY_5:
        Serial.println("5");
        translate_IR_data = 5;
        break;

      case Key21::KEY_6:
        Serial.println("6");
        translate_IR_data = 6;
        break;

      case Key21::KEY_7:
        Serial.println("7");
        translate_IR_data = 7;
        break;

      case Key21::KEY_8:
        Serial.println("8");
        translate_IR_data = 8;
        break;

      case Key21::KEY_9:
        Serial.println("9");
        translate_IR_data = 9;
        break;

      default:
        Serial.println("WARNING: undefined key:");
        break;
    }
  }
}
