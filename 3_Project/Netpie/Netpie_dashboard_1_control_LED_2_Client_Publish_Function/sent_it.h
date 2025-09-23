void sent_bool_led_st_to_netpie(int delay_1)  // sent_bool_led_st_to_netpie(int_delay_1);
{
  String str_led_status = "{\"data\":{\"led status\":" + String(bool_led_st) + "}}";
  Serial.println(str_led_status);
  str_led_status.toCharArray(msg, (str_led_status.length() + 1));
  client.publish("@shadow/data/update", msg);
  delay(delay_1);
}

void sent_all_data_to_netpie(int delay_1)  // sent_all_data_to_netpie(int_delay_1);
{
  String str = "{\"data\" : {\"incoming\":" + String(incoming) + "}}";
  Serial.println(str);
  str.toCharArray(msg, (str.length() + 1));
  client.publish("@shadow/data/update", msg);
  delay(delay_1);
}