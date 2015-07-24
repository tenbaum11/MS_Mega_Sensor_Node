/*

    switch (inByte) {
        case 'a':    
          digitalWrite(2, HIGH);
          break;
        case 'e':    
          digitalWrite(6, HIGH);
          break;
        default: // turn all the LEDs off:
          for (int thisPin = 2; thisPin < 7; thisPin++) {
            digitalWrite(thisPin, LOW);
          }
    } // end switch
    
    
    
void incomingMessageworking(const MyMessage &message) {
  if (message.type == V_DIMMER)
    testrxval = atoi(message.data);
  Serial.println("incoming message");
   Serial.println(message.sensor);
   Serial.print("type"); Serial.println(message.type);
  Serial.println(testrxval);
  analogWrite(RGB_PINS[0], testrxval);
  gw.send(msgGET.set(testrxval)); //delay(1000);
}


//void incomingMessage(const MyMessage &message) {
//    // expect one type of message from controller. check anyway.
//    if (message.type==V_VAR1) {
//	// starting to process the hex code
//        String hexstring = message.getString(); //here goes the hex color code coming from Pidome through MySensors (like FF9A00)
//        long number = (long) strtol( &hexstring[0], NULL, 16);
//        RGB_values[0] = number >> 16;
//        RGB_values[1] = number >> 8 & 0xFF;
//        RGB_values[2] = number & 0xFF;
//        
//        for (int i = 0; i<3; i++) {
//            analogWrite(RGB_pins[i],RGB_values[i]);
//        }
//        
//        // Write some debug info
//        Serial.print("Red is " );
//        Serial.println(RGB_values[0]);
//        Serial.print("Green is " );
//        Serial.println(RGB_values[1]);
//        Serial.print("Blue is " );
//        Serial.println(RGB_values[2]);
//
//        //if you want to convert it again from rgb to hex
//        long rgb = 0;
//        rgb = ((long)RGB_values[0] << 16) | ((long)RGB_values[1] << 8 ) | (long)RGB_values[2];
//	Serial.print("This is the reconverted HEX value: #" );
//        Serial.println(String(rgb, HEX));
//    }
//}



//void incomingMessage0(const MyMessage &message)
//{
//  if (message.type == V_LIGHT) {
//    int lstate= atoi( message.data );
//    Serial.print(F("ON/OFF Value: ")); Serial.println(lstate);
//  }
//  else if (message.type == V_DIMMER) {
//    int dimvalue= atoi( message.data );
//    Serial.print(F("Dimmer Value: ")); Serial.println(dimvalue);
//  }
//  else{
//    Serial.println(F("nothing"));
//  }
//  
//}




//void incomingMessage2(const MyMessage &message)
//{
//  if (message.type == V_LIGHT) {
//    Serial.println( "V_LIGHT command received..." );
//    
//    int lstate= atoi( message.data );
//    if ((lstate<0)||(lstate>1)) {
//      Serial.println( "V_LIGHT data invalid (should be 0/1)" );
//      return;
//    }
//    LastLightState=lstate;
//    gw.saveState(EPROM_LIGHT_STATE, LastLightState);
//    
//    if ((LastLightState==LIGHT_ON)&&(LastDimValue==0)) {
//       //In the case that the Light State = On, but the dimmer value is zero,
//       //then something (probably the controller) did something wrong,
//       //for the Dim value to 100%
//      LastDimValue=100;
//      gw.saveState(EPROM_DIMMER_LEVEL, LastDimValue);
//    }
//    
//    //When receiving a V_LIGHT command we switch the light between OFF and the last received dimmer value
//    //This means if you previously set the lights dimmer value to 50%, and turn the light ON
//    //it will do so at 50%
//  }
//  else if (message.type == V_DIMMER) {
//    Serial.println( "V_DIMMER command received..." );  
//    int dimvalue= atoi( message.data );
//    if ((dimvalue<0)||(dimvalue>100)) {
//      Serial.println( "V_DIMMER data invalid (should be 0..100)" );
//      return;
//    }
//    if (dimvalue==0) {
//      LastLightState=LIGHT_OFF;
//    }
//    else {
//      LastLightState=LIGHT_ON;
//      LastDimValue=dimvalue;
//      gw.saveState(EPROM_DIMMER_LEVEL, LastDimValue);
//    }
//  }
//  else {
//    Serial.println( "Invalid command received..." );  
//    return;
//  }
//
//  //Here you set the actual light state/level
//  //SetCurrentState2Hardware();
//}





*/
