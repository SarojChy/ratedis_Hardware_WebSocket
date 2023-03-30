void webSocketEvent(WStype_t eventType, uint8_t * payload, size_t length) {
  switch (eventType) {
    case WStype_DISCONNECTED:
      configTime(20700, 0, "pool.ntp.org");
      Serial.println("WebSocket disconnected");
      break;
    case WStype_CONNECTED:
      Serial.println("WebSocket connected");
      configTime(20700, 0, "pool.ntp.org");
      break;
    case WStype_TEXT:
      Serial.print("Received text data: ");
      //      Serial.write(payload, length);
      char myVariable[length];
      memcpy(myVariable, payload, length);
      Serial.println(myVariable);
      String valueReadings(myVariable);
      Serial.println(valueReadings);

      //    Serial.println(valueReadings);
      JSONVar myObject = JSON.parse(valueReadings);

      // JSON.typeof(jsonVar) can be used to get the type of the var
      if (JSON.typeof(myObject) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }
      Serial.print('\n');
      //     myObject.keys() can be used to get an array of all the keys in the object
      JSONVar keys = myObject.keys();

      for (int i = 0; i < keys.length(); i++) {
        JSONVar value = myObject[keys[i]];
        valueReadingsArr[i] = double(value);
        data_date = valueReadingsArr[0];
        buy_gold_22_per_tola = valueReadingsArr[1];
        buy_gold_24_per_tola = valueReadingsArr[2];
        buy_gold_22_per_10g = valueReadingsArr[3];
        buy_silver = valueReadingsArr[4];
        sell_gold_22_per_tola = valueReadingsArr[5];
        sell_gold_24_per_tola = valueReadingsArr[6];
        sell_gold_22_per_10g = valueReadingsArr[7];
        sell_silver = valueReadingsArr[8];

      }
      // printing Received values
      printData();
      //Parsing Received price datas to the display variables
      assigningValue();

      //Displaying Received Data to the Segmnet Board
      dispalyData();

      //      parseJsonData();
      //      for (size_t i = 0; i < length; i++) {
      //        Serial.print((char)payload[i]);
      //      }
      break;
      //    case WStype_BIN:
      //      Serial.print("Received binary data: ");
      //      Serial.write(payload, length);
      //      break;
      //    case WStype_ERROR:
      //      Serial.println("WebSocket error");
      //      break;
  }
}
