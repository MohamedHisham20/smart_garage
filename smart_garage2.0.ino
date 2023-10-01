#include <Servo.h>
#define tempsensor A0
#define IR_enter A1
#define IR_leave A2
#define light_sens A3
#define a 2
#define b 3
#define c 4
#define d 5
#define e 6
#define f 7
#define g 8
#define d1 10
#define d2 9
#define car_disp 12
#define alarm 11
#define light 13
Servo gate;
/Timers/
const unsigned long eventtime_1_gate = 10000;  //Fail safe for counter
const unsigned long eventtime_2_buzzer = 700;  //For led and buzzer
const unsigned long eventtime_3_servo = 100;  //Delay between angle movement
/Temperature sensor/
const int max_temp = 37;  //buzzer trigger
const int del_time = 5;
int x;  //tenth of temperature
int y;  //unit of temperature
bool buzzer = false;

//Gate

int free_space = 8;
int temp =0; //temporary
bool entering = false; 
bool leaving = false;
bool passing = false;
bool start = true;
int i=0;
void setup() {
  pinMode( a, OUTPUT);
  pinMode( b, OUTPUT);
  pinMode( c, OUTPUT);
  pinMode( d, OUTPUT);
  pinMode( e, OUTPUT);
  pinMode( f, OUTPUT);
  pinMode( g, OUTPUT);
  pinMode( IR_enter, INPUT);
  pinMode( IR_leave, INPUT);
  pinMode( car_disp, OUTPUT);
  pinMode( d1, OUTPUT);
  pinMode( d2, OUTPUT);
  pinMode( alarm, OUTPUT);
  pinMode( light_sens, INPUT);
  pinMode( light, OUTPUT);
  gate.attach(A4);
  Serial.begin(9600);
  gate.write(0);
}

void loop() {
  
  //Reading running time for later use
  
  unsigned long current_time = millis();
  static unsigned long prev_time_1 = 0;
  static unsigned long prev_time_3 = 0;
  
  //IR_sensors
  
  int val1 = digitalRead(IR_enter);
  int val2 = digitalRead(IR_leave);
  if (start == true){
    if (current_time - prev_time_3 >= eventtime_3_servo && i > 0){
      i-=5;
      gate.write(i);
      //Serial.print("close: ");
      //Serial.println(i);
      prev_time_3 = current_time;
    }              
    if (val1 == 0){
      entering = true;
      start = false;
      passing = true;
      temp = 1;
      Serial.println("Entering property");
    }
    else if (val2 == 0){
      leaving = true;
      start = false;
      passing = true;
      temp = 1;
      Serial.println("Leaving property");
    }
    prev_time_1 = current_time;
  }
  //عايزين نغير الكود ان مش هيبقي فيه مسافة بين السينسور ir والبوابة
  else if (val2 == 0 && entering == true)
   temp = 0;
  else if (val1 == 0 && leaving == true)
   temp = 0;
  
  //Resets gate system in case of a weird car behaviour
  
  else {
    if (current_time - prev_time_3 >= eventtime_3_servo && i < 90){
      i+=5;
      gate.write(i);
      //Serial.print("open: ");
      //Serial.println(i);
      prev_time_3 = current_time;
    }
    if (current_time - prev_time_1 >= eventtime_1_gate){
      temp =0;
      entering = false; 
      leaving = false;
      passing = false;
      start = true;
      Serial.println("RESET");
      prev_time_1 = current_time;
    }
  }
   
  if (temp == 0){
    if (entering == true){
      if (val2 == 1 && passing == true){
        free_space--;
        start = true;
        entering = false;
        passing = false;
        Serial.print("Free Space= ");
        Serial.println(free_space);
      }
    }
    else if (leaving == true){
      if (val1 == 1 && passing == true){
        free_space++;
        start = true;
        leaving = false;
        passing = false;
        Serial.print("Free Space= ");
        Serial.println(free_space);
      }
    }
  }
  if (free_space >= 0){
    display_dgt( free_space, car_disp, d1, d2);
    delay(del_time);
  }
  else{
     display_dgt( 0, car_disp, d1, d2);
    delay(del_time);
  }

  /Temperature reading equations/
  int temp_read = analogRead(tempsensor);
  float temp_C = temp_read * 5.0/1023;  //convert to volt
  temp_C = temp_C * 100 - 15;  //Changed to Celcius
  static float old_temp= -100;
   
  /Temperature updates on monitor/
  if ( abs(temp_C - old_temp) > 0.5){
    if( temp_C >= max_temp)
      Serial.println("WARNING");
    old_temp = temp_C;
    Serial.print("Temperature = ");
    Serial.print(temp_C); 
    Serial.println(" Celsius/n");
  }
  
  /Temperature display reading/
  x= temp_C / 10;
  int tenth= x * 10;
  y= temp_C - tenth;
  if (temp_C >=100){
    x=9; y=9;
  }
  display_dgt(x, d1, d2, car_disp);
  delay(del_time);
  display_dgt(y, d2, d1, car_disp);
  delay(del_time);

  /buzzer/
  static unsigned long prev_time_2 = 0;
  if (temp_C > max_temp) {
    if (current_time - prev_time_2 >= eventtime_2_buzzer ){
      buzzer = !buzzer;
      if (buzzer)
        digitalWrite( alarm, HIGH);
      else
        digitalWrite( alarm, LOW);
      prev_time_2 = current_time;
    }  
  }
  else
    digitalWrite( alarm, LOW);

  /LDR/
  int brightness = digitalRead(light_sens);
  int light_val = analogRead(light_sens);
  static int old_light = -150;
  if (abs(light_val - old_light) > 100){
    Serial.print("Current reading of light sensor = ");
    Serial.println(light_val);
    old_light = light_val;
  }
  digitalWrite( light, brightness);
}


void display_dgt(int n, int led_on ,int led_off , int led_off1) {
  digitalWrite( led_off, HIGH);
  digitalWrite( led_off1, HIGH);
  digitalWrite( led_on, LOW);
  num_display(n);
}

void num_display(int n) {
  switch (n) {
    case 0:
      digitalWrite( a, HIGH);
      digitalWrite( b, HIGH);
      digitalWrite( c, HIGH);
      digitalWrite( d, HIGH);
      digitalWrite( e, HIGH);
      digitalWrite( f, HIGH);
      digitalWrite( g, LOW);
      break;

    case 1:
      digitalWrite( a, LOW);
      digitalWrite( b, HIGH);
      digitalWrite( c, HIGH);
      digitalWrite( d, LOW);
      digitalWrite( e, LOW);
      digitalWrite( f, LOW);
      digitalWrite( g, LOW);
      break;

    case 2:
      digitalWrite( a, HIGH);
      digitalWrite( b, HIGH);
      digitalWrite( c, LOW);
      digitalWrite( d, HIGH);
      digitalWrite( e, HIGH);
      digitalWrite( f, LOW);
      digitalWrite( g, HIGH);
      break;

    case 3:
      digitalWrite( a, HIGH);
      digitalWrite( b, HIGH);
      digitalWrite( c, HIGH);
      digitalWrite( d, HIGH);
      digitalWrite( e, LOW);
      digitalWrite( f, LOW);
      digitalWrite( g, HIGH);
      break;

    case 4:
      digitalWrite( a, LOW);
      digitalWrite( b, HIGH);
      digitalWrite( c, HIGH);
      digitalWrite( d, LOW);
      digitalWrite( e, LOW);
      digitalWrite( f, HIGH);
      digitalWrite( g, HIGH);
      break;

    case 5:
      digitalWrite( a, HIGH);
      digitalWrite( b, LOW);
      digitalWrite( c, HIGH);
      digitalWrite( d, HIGH);
      digitalWrite( e, LOW);
      digitalWrite( f, HIGH);
      digitalWrite( g, HIGH);
      break;

    case 6:
      digitalWrite( a, HIGH);
      digitalWrite( b, LOW);
      digitalWrite( c, HIGH);
      digitalWrite( d, HIGH);
      digitalWrite( e, HIGH);
      digitalWrite( f, HIGH);
      digitalWrite( g, HIGH);
      break;

    case 7:
      digitalWrite( a, HIGH);
      digitalWrite( b, HIGH);
      digitalWrite( c, HIGH);
      digitalWrite( d, LOW);
      digitalWrite( e, LOW);
      digitalWrite( f, LOW);
      digitalWrite( g, LOW);
      break;

    case 8:
      digitalWrite( a, HIGH);
      digitalWrite( b, HIGH);
      digitalWrite( c, HIGH);
      digitalWrite( d, HIGH);
      digitalWrite( e, HIGH);
      digitalWrite( f, HIGH);
      digitalWrite( g, HIGH);
      break;

     case 9:
      digitalWrite( a, HIGH);
      digitalWrite( b, HIGH);
      digitalWrite( c, HIGH);
      digitalWrite( d, HIGH);
      digitalWrite( e, LOW);
      digitalWrite( f, HIGH);
      digitalWrite( g, HIGH);
  }
}
