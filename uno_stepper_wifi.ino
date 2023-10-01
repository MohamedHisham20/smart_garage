#define REMOTEXY_MODE__ESP8266_HARDSERIAL_POINT

#include <RemoteXY.h>

// RemoteXY connection settings 
#define REMOTEXY_SERIAL Serial
#define REMOTEXY_SERIAL_SPEED 115200
#define REMOTEXY_WIFI_SSID "Rob_Wars"
#define REMOTEXY_WIFI_PASSWORD "123456789"
#define REMOTEXY_SERVER_PORT 6377


// RemoteXY configurate  
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =   // 105 bytes
  { 255,2,0,4,0,98,0,16,31,2,1,1,8,23,31,10,6,37,25,12,
  2,31,76,69,86,69,76,32,49,0,1,1,54,22,32,11,33,37,26,12,
  2,31,76,69,86,69,76,32,50,0,129,0,24,5,46,11,10,13,42,10,
  17,119,101,108,99,111,109,101,0,67,5,59,44,29,11,13,70,34,13,2,
  26,4,129,0,4,44,48,8,1,55,48,8,17,84,101,109,112,101,114,97,
  116,117,114,101,0 };
  
// this structure defines all the variables and events of your control interface 
struct {

    // input variables
  uint8_t LVL_1; // =1 if button pressed, else =0 
  uint8_t LVL_2; // =1 if button pressed, else =0 
  
    // output variables
  char text_tempC[4];  // string UTF8 end zero 
  
    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0 

} RemoteXY;
#pragma pack(pop)

/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////
const int stepPin = 10; 
const int dirPin = 11; 
bool moving = false;
bool lvl1 = true;
bool lvl2 = false;
//bool toggle = true;


void setup() 
{
  RemoteXY_Init (); 
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
  
  // TODO you setup code
  
}

void loop() 
{ 
  RemoteXY_Handler ();
  if (RemoteXY.LVL_1 != 0 && moving == false && !lvl1){
    lvl1= true;
    lvl2 = false;
    moving = true;
    digitalWrite(dirPin,HIGH); // Enables the motor to move in a particular direction

    for(int x = 0; x < 2550; x++) {
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(500); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(500); 
  }
      moving = false;

    }
    else if (RemoteXY.LVL_2 != 0 && moving == false && !lvl2){
    lvl2 = true;
    lvl1 = false;
    moving = true;
    digitalWrite(dirPin,LOW); // Enables the motor to move in a particular direction

    for(int x = 0; x < 2550; x++) {
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(500); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(500); 
  }
      moving = false;
  }
  
  // TODO you loop code
  // use the RemoteXY structure for data transfer
  // do not call delay() 


}
