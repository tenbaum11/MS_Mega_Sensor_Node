//// DHT FUNCTIONS
//void DHT_READ_FUNC()
//{
//  
//  int temp, hum;
//  RCrecv.disableReceive();
//  RCsend.disableTransmit();
//  RHdriver.setModeIdle();
//  delay(200);
//  int chk = DHT.read11(DHT11_PIN);
//  switch (chk)
//  {
//    case DHTLIB_OK:  
//		Serial.print("OK,\t"); 
//		break;
//    case DHTLIB_ERROR_CHECKSUM: 
//		Serial.print("Checksum error,\t"); 
//		break;
//    case DHTLIB_ERROR_TIMEOUT: 
//		Serial.print("Time out error,\t"); 
//		break;
//    case DHTLIB_ERROR_CONNECT:
//        Serial.print("Connect error,\t");
//        break;
//    case DHTLIB_ERROR_ACK_L:
//        Serial.print("Ack Low error,\t");
//        break;
//    case DHTLIB_ERROR_ACK_H:
//        Serial.print("Ack High error,\t");
//        break;
//    default: 
//		Serial.print("Unknown error,\t"); 
//		break;
//  }
//  // DISPLAY DATA
//  temp = (int)DHT.temperature;
//  hum = (int)DHT.humidity;
//  Serial.print(hum);
//  Serial.print(",\t");
//  Serial.println(temp);
//
//  delay(5000);
//  
//  //irRecv.enableIRIn();
//  RCrecv.enableReceive(0);
//  RCsend.enableTransmit(RCTX_PIN);
//  RCsend.setProtocol(1);
//  RCsend.setPulseLength(pulsetime);
//  RHdriver.setModeRx();
//}

