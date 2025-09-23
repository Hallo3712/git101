void save_data(int i) {
  switch (i) {
    case 0:
      val0 = mappedValue;
      break;
    case 1:
      val1 = mappedValue;
      break;
    case 2:
      val2 = mappedValue;
      break;
    case 3:
      val3 = mappedValue;
      break;
    case 4:
      val4 = mappedValue;
      break;
    case 5:
      val5 = mappedValue;
      break;
    case 6:
      val6 = mappedValue;
      break;
    case 7:
      val7 = mappedValue;
      break;
    case 8:
      val8 = mappedValue;
      break;
    case 9:
      val9 = mappedValue;
      break;
    case 10:
      val10 = mappedValue;
      break;
    case 11:
      val11 = mappedValue;
      break;
    case 12:
      val12 = mappedValue;
      break;
    case 13:
      val13 = mappedValue;
      break;
    case 14:
      val14 = mappedValue;
      break;
    case 15:
      val15 = mappedValue;
      break;
  }
  Serial.println("print from save data");
  for (int i = 0; i < 16; i++) {
    Serial.print("val");
    Serial.print(i);
    Serial.print(" = ");
    Serial.print(val[i]);
    Serial.print("  ");
  }
  Serial.println();  // ขึ้นบรรทัดใหม่
}
