//s=child-sensor-id,
//c=message-type
//t=sub-type
//pt=payload-type (see MyMessage.h)
//l=length
//200;4;1;0;3;77
//#include <SimpleTimer.h>
#include <RCSwitch.h>
#include <RHDatagram.h>
#include <RH_ASK.h>
#include <SPI.h>
#include <Wire.h>
#include <MySensor.h>  

#include <dht.h>
dht DHT;
#define DHT11_PIN 10

// FOR RADIOHEAD 
#define CLIENT_ADDRESS 3
#define SERVER_ADDRESS 2

// FOR MYSENSORS
#define CID_SOIL0    0    
#define CID_SOIL1    1   
#define CID_SOIL2    2  
#define CID_GET      3
#define CID_LDR	     4
#define CID_LEDR     5
#define CID_LEDG     6
#define CID_LEDB     7
#define CID_TEMP     8
#define CID_BARO     9
#define CID_DHUM    10
#define CID_DTEMP   11
#define CID_EtekA   12
#define CID_EtekB   13
#define CID_EtekC   14
#define CID_EtekD   15
#define CID_EtekE   16
#define CID_RGB     17
#define CID_LIGHT   18

//SimpleTimer timer;

#define EPROM_LIGHT_STATE 1
#define EPROM_DIMMER_LEVEL 2

#define LIGHT_OFF 0
#define LIGHT_ON 1

#define SN "Mega Node"
#define SV "1.0"

#define ALTITUDEmeter 100.0  //meters



// *****************************************
// ETEK CODES
// *****************************************

const int NUMBER_OF_etekELEMENTS = 20;
typedef struct {
   char etekCodeArr[16];
   byte etekStateArr;
   byte etekIDArr;
} etekCODES;

const etekCODES etekArr [NUMBER_OF_etekELEMENTS] PROGMEM = {
  {"F0FF0FFF0101",1,0}, {"F0FF0FFF1001",1,1}, {"F0FF0FF10001",1,2}, {"F0FFFF1F0001",1,3}, {"F0FFF1FF0001",1,4},
  {"F0FFFFFF0101",1,0}, {"F0FFFFFF1001",1,1}, {"F0FFFFF10001",1,2}, {"F0FF0F1F0001",1,3}, {"F0FF01FF0001",1,4},
  {"F0FF0FFF0110",0,0}, {"F0FF0FFF1010",0,1}, {"F0FF0FF10010",0,2}, {"F0FFFF1F0010",0,3}, {"F0FFF1FF0010",0,4},
  {"F0FFFFFF0110",0,0}, {"F0FFFFFF1010",0,1}, {"F0FFFFF10010",0,2}, {"F0FFFF1F0010",0,3}, {"F0FF01FF0010",0,4},
};

// MEGA 2560 PINS 

const byte  DHT_PIN = 10;
const byte IRRX_PIN = 5;      // IMPLICITLY DEFINED IN IRLIB LIBRARY
const byte RCRX_PIN = 2; 
const byte RHRX_PIN = 2; 
const byte RCTX_PIN = 12;  
const byte RHTX_PIN = 12; 

const byte PWR1_PIN = 13;  // RFTX POWER

const byte BUZZ_PIN = 4;   // BUZZER PIN
const byte DIG1_PIN = 7;   // PIR
const byte DIG2_PIN = 11;  

const byte LED1_PIN = A0;
const byte ANA1_PIN = A1;  // 
const byte ANA2_PIN = A2;  // 
const byte GND1_PIN = A3; 

// MICROPHONE
const byte mANA_PIN = A4;
const byte mGND_PIN = A5;
const byte mPWR_PIN = A6;
const byte mDIG_PIN = A7;

const int LDR_PIN=A8;
const int SOIL0_PIN=A9;
const int SOIL1_PIN=A10;
const int SOIL2_PIN=A11;


const byte RGBgnd_PIN = A13;
const int RED   = A12;  // PWM pin for Red
const int GREEN = A14;  // PWM pin for Green
const int BLUE  = A15;  // PWM pin for Blue
const int RGB_PINS[3] = { RED, GREEN, BLUE };
long RGB_VALUES[3] = {0,0,0};


// 433 MODULE VARS
//RH_ASK RHdriver(2000,RHRX_PIN,RHTX_PIN);  //(speed, RXpin, TXpin, ptt;IN, inverted)
//RHDatagram RHmanager(RHdriver, SERVER_ADDRESS);


unsigned long SLEEP_TIME = 10; 
MySensor gw(8,53);

//float lastDTemp;
//float lastDHum;

MyMessage msgSoil0(CID_SOIL0, V_VAR1);
MyMessage msgSoil1(CID_SOIL1, V_VAR1);
MyMessage msgSoil2(CID_SOIL2, V_VAR1);

MyMessage msgLDR(CID_LDR, V_VAR2);
MyMessage msgGET(CID_GET, V_DIMMER);

MyMessage tempMsg(CID_TEMP, V_TEMP);
MyMessage pressureMsg(CID_BARO, V_PRESSURE);
MyMessage forecastMsg(CID_BARO, V_FORECAST);

MyMessage lightMsg(CID_LIGHT, V_LIGHT);
MyMessage dimmerMsg(CID_LIGHT, V_DIMMER);

MyMessage msgDHum(CID_DHUM, V_HUM);
MyMessage msgDTemp(CID_DTEMP, V_TEMP);

MyMessage etekAMsg(CID_EtekA, V_LIGHT);
MyMessage etekBMsg(CID_EtekB, V_LIGHT);
MyMessage etekCMsg(CID_EtekC, V_LIGHT);
MyMessage etekDMsg(CID_EtekD, V_LIGHT);
MyMessage etekEMsg(CID_EtekE, V_LIGHT);



boolean metric = false;

//int testrxval =66;
//unsigned long previousMillis = 0L; 
//unsigned long interval = 9000L; 
const unsigned long wdt_reset = 30000;
volatile unsigned long wdt_cnt=0;
int cnt=0;
 


// BMP VARS
//int degF, absHg, adjHg, errorFlag=9; //BMP180 vars




// LED VARS
int LastLightState=LIGHT_OFF;
int LastDimValue=100;

// ^^^^ RC VARIABLES^^^^
//RCSwitch RCsend = RCSwitch();
//RCSwitch RCrecv = RCSwitch();
//unsigned long Etek_code;
//unsigned int Etek_length;
//unsigned int pulsetime = 140;  // RCremote pulse out

int etekState = 0;
int etekID = 4;

void sendMSG()
{
  
    // ANALOG VARS
    int valLDR=99, valSOIL0=8, valSOIL1=10, valSOIL2=20;
    int LDR = analogRead(LDR_PIN);
    valLDR = map(LDR, 0, 1023, 0, 100);
    double degInF=44, baroInHg=77; 
    //const char *weather[] = {"stable","sunny","cloudy","unstable","thunderstorm","unknown"};
    if(++cnt>200) cnt=0;
    //gw.sleep(1000);
    Serial.println(F("sending start..."));  delay(5);
    gw.send(msgSoil0.set(valSOIL0+cnt)); delay(5);
    gw.send(msgSoil1.set(valSOIL1+cnt)); delay(5);
    gw.send(msgSoil2.set(valSOIL2+cnt)); delay(5);
    
    gw.send(msgLDR.set(valLDR)); delay(5);
    gw.send(tempMsg.set(degInF,0)); delay(5);
    gw.send(pressureMsg.set(baroInHg, 0)); delay(5);
    //gw.send(forecastMsg.set(weather[1])); delay(1);
    //gw.send(etekAMsg.set(etekState)); delay(1);
    Serial.println(F("...sending done")); 
    //gw.sleep(500);
  
}




//------------------------------------------------------
// SETUP -----------------------------------------------
//------------------------------------------------------
void setup()  
{ 
  //timer.setInterval(10000, sendMSG);
    
  gw.begin(incomingMessage, 205, false);  // for callback
  //dht.setup(DHT_PIN); 
  
  pinMode(RGBgnd_PIN, OUTPUT); analogWrite(RGBgnd_PIN, 0);
  pinMode(mGND_PIN, OUTPUT); analogWrite(mGND_PIN, 0);
  pinMode(GND1_PIN, OUTPUT); analogWrite(GND1_PIN, 0);
  
  pinMode(mPWR_PIN, OUTPUT); analogWrite(mPWR_PIN, 254);
  pinMode(PWR1_PIN, OUTPUT); digitalWrite(PWR1_PIN, HIGH);
 
  
  pinMode(LDR_PIN,INPUT);
  pinMode(SOIL0_PIN,INPUT_PULLUP); 
  pinMode(SOIL1_PIN,INPUT_PULLUP); 
  pinMode(SOIL2_PIN,INPUT_PULLUP); 

 
  for (int thisPin=0; thisPin < 3; thisPin++) { 
    pinMode(RGB_PINS[thisPin], OUTPUT); 
    Serial.print("PIN: "); Serial.println(RGB_PINS[thisPin]); 
    analogWrite(RGB_PINS[thisPin], 254); delay(500);
    analogWrite(RGB_PINS[thisPin], 0);   delay(300);
  }

  //gw.begin();
//  if (!RHmanager.init())  // start RH Driver
//    Serial.println(F("RH init failed"));  /* DEFAULTS 434MHz, 0.05MHz AFC pull-in, modulation FSK_Rb2_4Fd36  */
  //RHdriver.setModeIdle();
  
  gw.sendSketchInfo(SN, SV);
  gw.present(CID_SOIL0, S_CUSTOM);
  gw.present(CID_SOIL1, S_CUSTOM);
  gw.present(CID_SOIL2, S_CUSTOM);
  gw.present(CID_LDR,   S_CUSTOM);
  gw.present(CID_GET, S_DIMMER );
  gw.present(CID_BARO, S_BARO);
  gw.present(CID_TEMP, S_TEMP);
  gw.present(CID_DHUM, S_HUM);
  gw.present(CID_DTEMP, S_TEMP);
  gw.present(CID_RGB, S_CUSTOM);
  gw.present(CID_LIGHT, S_DIMMER );
  
//  RCsend.enableTransmit(RCTX_PIN);
//  RCsend.setProtocol(1);
//  RCsend.setPulseLength(pulsetime);
//  RCsend.setRepeatTransmit(6); 
//  RCrecv.enableReceive(0);   // ARDUINO PIN 2 - interrupt 0 
  //205;18;1;0;3;0

  //Retreive our last light state from the eprom
  int LightState=gw.loadState(EPROM_LIGHT_STATE); 
  if (LightState<=1) {
    LastLightState=LightState;
    int DimValue=gw.loadState(EPROM_DIMMER_LEVEL); 
    if ((DimValue>0)&&(DimValue<=100)) {
      //There should be no Dim value of 0, this would mean LIGHT_OFF
      LastDimValue=DimValue;
    }
  }
  
  
//  if (pressure.begin())
//    Serial.println(F("BMP180 init success"));
//  else{
//    Serial.println(F("BMP180 init fail\n\n"));
//    while(1); // Pause forever.
//  }


  SetCurrentState2Hardware(); // Switch on/off the light with last known dim level  
  Serial.println(F("setup complete"));
}
//--------------------------------------------------------------------
int del = 1000;
uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
//uint8_t RHRXbuf[RH_ASK_MAX_MESSAGE_LEN]; // Dont put this on the stack:
void loop()      
{     
    gw.process();
    sendMSG();
    gw.wait(7000);
    dhtREAD(); 
    gw.wait(700);
    gw.process(); 
    dhtREAD();     
    gw.wait(700);
    
}



//##############################################################
//#######    FUNCTIONS
//##############################################################
void dhtREAD()
{
  //Serial.print("DHT22, \t");

   
    float Dhum, Dtemp;
   // uint32_t start = micros();
    int chk = DHT.read11(10);
    //uint32_t stop = micros();

//    switch (chk)
//    {
//    case DHTLIB_OK:
//        Serial.print("OK,\t");
//        del -= 0;
//        break;
//    case DHTLIB_ERROR_CHECKSUM:
//        Serial.print("Checksum error,\t");
//        break;
//    case DHTLIB_ERROR_TIMEOUT:
//        Serial.print("Time out error,\t");
//        del += 0;
//        break;
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
//        Serial.print("Unknown error,\t");
//        break;
//    }
    // DISPLAY DATA
      Dhum = DHT.humidity;
      Dtemp = DHT.temperature;
      //int celc = (int)Dtemp;
      
      //DtempF = Dtemp*9.0/5.0 + 32.0;
    //Serial.print(DHT.humidity, 1);
   // Serial.print("checksum:"); Serial.println(chk);
   // Serial.print(Dhum, 1);
   // Serial.print(",\t");
    //Serial.print(DHT.temperature, 1);
   // Serial.println(Dtemp);
//    Serial.print(",\t");
//    Serial.print(stop - start);
//    Serial.print(",\t");
//    Serial.print(del);
//    Serial.println();

    if(chk==0 && Dtemp!=0 && Dhum!=0 ){
      //Serial.println(F("sending dht data"));
      gw.send(msgDTemp.set(Dtemp, 1));
      gw.send(msgDHum.set(Dhum, 1));
    }
    
    
    delay(del); 
   
}



//***************************************************
// DHT11 FUNCTIONS
//***************************************************
//void dhtREADING(){
//  
//          delay(8000);
//          Serial.print(F("DHT Sample Period: ")); Serial.println(dht.getMinimumSamplingPeriod());
//          float Dtemperature = dht.getTemperature();
//          if (isnan(Dtemperature)) {
//              Serial.println(F("Failed reading temperature from DHT"));
//          } else if (Dtemperature != lastDTemp) {
//            lastDTemp = Dtemperature;
//            if (!metric) {
//              Dtemperature = dht.toFahrenheit(Dtemperature);
//            }
//            gw.send(msgDTemp.set(Dtemperature, 1));
//            Serial.print("T: ");
//            Serial.println(Dtemperature);
//          }
//          
//          float Dhumidity = dht.getHumidity();
//          if (isnan(Dhumidity)) {
//              Serial.println(F("Failed reading humidity from DHT"));
//          } else if (Dhumidity != lastDHum) {
//              lastDHum = Dhumidity;
//              gw.send(msgDHum.set(Dhumidity, 1));
//              Serial.print("H: ");
//              Serial.println(Dhumidity);
//          }  
//}






//***************************************************
// MySensor Incoming messages
//***************************************************

void incomingMessage(const MyMessage &message)
{
  int CID = (int)message.sensor;
  if (CID>=CID_EtekA && CID<=CID_EtekE) {
    
//    etekState = atoi( message.data );
//    //Serial.println( F("Etek msg received") );         
//    if ((etekState<0)||(etekState>1)) {
//      Serial.println( F("Etek data invalid (should be 0/1)") );
//      return;
//    }
//    
//    int etekID = CID - CID_EtekA;
//    Serial.print( F("EtekID: ") );   Serial.println( etekID );  
//    Serial.print( F("EtekState: ") );   Serial.println( etekState );    
//    RC_SEND( etekID, etekState );
   
  }
  else {

      if (message.type == V_LIGHT) {
        
        //Serial.println( F("V_LIGHT command received...") );
        int lstate= atoi( message.data );
        
        if ((lstate<0)||(lstate>1)) {
          Serial.println( F("V_LIGHT data invalid (should be 0/1)") );
          return;
        }
        
        LastLightState=lstate;
        gw.saveState(EPROM_LIGHT_STATE, LastLightState);
        
        if ((LastLightState==LIGHT_ON)&&(LastDimValue==0)) {
           //In the case that the Light State = On, but the dimmer value is zero,
           //then something (probably the controller) did something wrong,
           //for the Dim value to 100%
          LastDimValue=100;
          gw.saveState(EPROM_DIMMER_LEVEL, LastDimValue);
        }
        
        //When receiving a V_LIGHT command we switch the light between OFF and the last received dimmer value
        //This means if you previously set the lights dimmer value to 50%, and turn the light ON
        //it will do so at 50%
      }
      else if (message.type == V_DIMMER) {
        //Serial.println( F("V_DIMMER command received...") );  
        int dimvalue= atoi( message.data );
        if ((dimvalue<0)||(dimvalue>100)) {
          //Serial.println( F("V_DIMMER data invalid (should be 0..100)") );
          return;
        }
        if (dimvalue==0) {
          LastLightState=LIGHT_OFF;
        }
        else {
          LastLightState=LIGHT_ON;
          LastDimValue=dimvalue;
          //Serial.print(F("Dim level: ")); Serial.println(LastDimValue);
          gw.saveState(EPROM_DIMMER_LEVEL, LastDimValue);
        }
      }
      else {
        Serial.println( F("Invalid command received...") );  
        return;
      }
    
      SetCurrentState2Hardware();  //Here you set the actual light state/level
  } // end dimmer switch
  

}







//***************************************************
//
//***************************************************
void SetCurrentState2Hardware()
{
  int lightLEVEL = map(LastDimValue, 0, 100, 0, 254);
  
  if (LastLightState==LIGHT_OFF) {
     //Serial.println( F("Light state: OFF") );
     analogWrite(RED, LIGHT_OFF);
  }
  else {
     Serial.print( F("Light state: ON, Level: "));
     Serial.println( LastDimValue );
     analogWrite(RED, lightLEVEL);
  }

  //Send current state to the controller
  SendCurrentState2Controller();
}


void SendCurrentState2Controller()
{
  if ((LastLightState==LIGHT_OFF)||(LastDimValue==0)) {
    //gw.send(dimmerMsg.set(0));
  } else {
    //gw.send(dimmerMsg.set(LastDimValue));
  }
}



//***************************************************
// RADIOHEAD SEND CODE 
//***************************************************
//void RH_SEND_MSG()
//{ 
//    Serial.print(F("SENDING RADIOHEAD: HELLO")); 
//    const char *msg = "hello";
//    RHdriver.send((uint8_t *)msg, strlen(msg));
//    RHdriver.waitPacketSent();
//    delay(200);
//} 
//
//
//void RH_SEND_DATA()
//{ 
//  const char *msg = "hello";
//  char RHMsg[20];
//  //A1_VAL = constrain( map(analogRead(A1_PIN), 0, 1023, 0, 1023), 0, 1023);   //415 - 275 415,275
//  //A2_VAL = constrain( map(analogRead(A2_PIN), 0, 1023, 0, 1023), 0, 1023);   //415 - 275 415,275 
//  //A3_VAL = constrain( map(analogRead(A3_PIN), 0, 1023, 0, 1023), 0, 1023);   //415 - 275 415,275  
//  //A4_VAL = constrain( map(analogRead(A4_PIN), 0, 1023, 0, 1023), 0, 1023);   //415 - 275 415,275 
//  
//  int A1_VAL=0, A2_VAL=10, A3_VAL=20, A4_VAL=30;
//  
//  //sprintf(RHMsg, "%d,%d,%d.", TdegF, PinHg, AinFT);
//  sprintf(RHMsg, "%d,%d,%d,%d.", A1_VAL, A2_VAL, A3_VAL, A4_VAL);
//  //sprintf(RHMsg, "%d.", A4_VAL);
// 
//  if (RHmanager.sendto((uint8_t*)RHMsg, strlen(RHMsg), SERVER_ADDRESS))
//  {
//    Serial.print(F("Sending Data: "));
//    Serial.println(RHMsg);
//  }
//   delay(1200);
//
//}


//***************************************************
// RADIOHEAD RECEIVE CODE 
//***************************************************
//void RH_RECEIVE2()
//{
//  int W=0, X=11, Y=12, Z=13;
//  if (RHmanager.available())
//  {
//    uint8_t len = sizeof(buf);
//    uint8_t from;
//    
//    if (RHmanager.recvfrom(buf, &len, &from))
//    {
//      sscanf ((char*)buf, "%d%*c%d%*c%d%*c%d%", &W, &X, &Y, &Z);
//      //sscanf ((char*)buf, "%d%*", &testX);//123,456,7890      
//      Serial.print("got[");
//      Serial.print(from); Serial.print("]: ");
//      Serial.print(W); Serial.print(F("|"));
//      Serial.print(X); Serial.print(F("|"));
//      Serial.print(Y); Serial.print(F("|"));
//      Serial.println(Z);
//      //Serial.print(": ");
//     // Serial.println((char*)buf);      
//    }    
//  }
//}
//
//
//
////------------------------
//// RC REMOTE SEND code 
////-----------------------
//void RC_SEND(int etekID, int etekState)
//{
//  RCrecv.disableReceive();
//  RCsend.enableTransmit(RCTX_PIN);
//  RCsend.setProtocol(1);
//  RCsend.setPulseLength(pulsetime);
//  
//  if(etekState==1){
//    txRemoteOn(etekID);
//  }
//  else if(etekState==0){
//    txRemoteOff(etekID);
//  }
//  
//  RCsend.disableTransmit();
//  RCrecv.enableReceive(0);
//  
//}
//
//
//void txRemoteOn(int p)
//{
//  etekCODES etekItem1;
//  etekCODES etekItem2;
//  memcpy_P (&etekItem1, &etekArr[p], sizeof etekItem1);
//  memcpy_P (&etekItem2, &etekArr[p+5], sizeof etekItem2);
//  for(int k = 0; k<2; k++)
//  {
//    RCsend.sendTriState(etekItem1.etekCodeArr); 
//    RCsend.sendTriState(etekItem2.etekCodeArr); 
//    delay(4);
//  }	
//  //Serial.print(F("EtekON:")); Serial.println(etekItem1.etekIDArr);
//  delay(1);
//}
//
//
//void txRemoteOff(int j)
//{ 
//  etekCODES etekItem1;
//  etekCODES etekItem2;
//  memcpy_P (&etekItem1, &etekArr[j+10], sizeof etekItem1);
//  memcpy_P (&etekItem2, &etekArr[j+15], sizeof etekItem2);
// for(int k = 0; k<2; k++)
// {
//    RCsend.sendTriState(etekItem1.etekCodeArr); 
//    RCsend.sendTriState(etekItem2.etekCodeArr); 
//    delay(4);
//  }
//  //Serial.print(F("EtekOFF:")); Serial.println(etekItem1.etekIDArr);
//  delay(1);
//}







