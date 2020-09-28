#include <Arduino.h>
// #include <SoftwareSerial.h>
// These constants won't change. They're used to give names to the pins used:
// user configurable options
const int treshold_x = 50; // increase this value, if x-axis of joystick is not giving zero values when idle. Keep values between 300 and 0. Decrease to get joystick act more sensible.
const int treshold_y = 50; // increase this value, if y-axis of joystick is not giving zero values when idle. Keep values between 300 and 0. Decrease to get joystick act more sensible.
int zeroState_x =510;
int zeroState_y=510;

const long baud_rate=250000; // Baudrate of your MPCNC-Controller

const int x_Pin = A0;  // Analog input pin that the x-potentiometer is attached to
const int y_Pin = A1;  // Analog input pin that the y-potentiometer is attached to
const byte PIN_BUTTON_A = 2; 
const byte PIN_BUTTON_B = 3;
const byte PIN_BUTTON_C = 4;
const byte PIN_BUTTON_D = 5;
const byte PIN_BUTTON_E = 6;
const byte PIN_BUTTON_F = 7;
const byte PIN_BUTTON_J = 8;

const int home_xy=PIN_BUTTON_F; // home_xy button pin (F)
const int home_z=PIN_BUTTON_E; // home_z button pin (E)
//const int reset_coordinates=12; // reset_ccordinates button
const int active_mode_toggle = PIN_BUTTON_J; // joystick switch pin (speen or z)


boolean z_mode = false; //move z-axis if true, default is xy
boolean hxy = false; //define buttons_presed variables
boolean hz = false;
boolean rc = false;


//const int button_Pin = 4; // Analog output pin that the LED is attached to

int x=0; //initialize variables
int y=0;
int z=0;
int t=0;
String X_command = "";
String Y_command = "";
String Z_command = "";
String distance = "1.0 ";
int modeToggle=1; // set 0.1mm,1, 10, 100

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

bool modeChanged=true;


bool firstXMove=true;
int getAverage(int pin){
  int a =0;
  for(int i=0;i<10;i++){
    a=a+analogRead(pin);
    delay(20); 
  }
  return a/10;
}

void resetCoordinates(){
  //Serial.begin(baud_rate); //open serial port
  Serial.println("G92 X0 Y0 Z0");
  Serial.println("M300 S300 P1000");
  Serial.println("M117 Home is here!");
  //Serial.end();//close serial port to avoid problems with Octoprint
  delay(3000);
  modeChanged=true; //resets label...
}

void setup() {

  pinMode(PIN_BUTTON_A, INPUT_PULLUP);  
  pinMode(PIN_BUTTON_B, INPUT_PULLUP);  
  pinMode(PIN_BUTTON_C, INPUT_PULLUP);  
  pinMode(PIN_BUTTON_D, INPUT_PULLUP);  
  pinMode(PIN_BUTTON_E, INPUT_PULLUP);  
  pinMode(PIN_BUTTON_F, INPUT_PULLUP);
  pinMode(PIN_BUTTON_J, INPUT_PULLUP);

  Serial.begin(baud_rate);//open serial port
  
  delay(5000);
  zeroState_x = getAverage(x_Pin);
  zeroState_y = getAverage(y_Pin);

  // Serial.begin(baud_rate); //open serial port
  // Serial.print("zeroState_x: ");
  // Serial.println(zeroState_x);

  // Serial.print("zeroState_y: ");
  // Serial.println(zeroState_y);

  // Serial.flush();
  // Serial.end();//close serial port to avoid problems with Octoprint
}

void loop() {
  

  hxy = digitalRead(home_xy);
  hz = digitalRead(home_z);
  
  if (digitalRead(active_mode_toggle)==LOW)
  {
    modeChanged=true;
    int c=0;
    unsigned long startedWaiting = millis();
    while(digitalRead(active_mode_toggle)==LOW && millis() - startedWaiting <= 1500){
      delay(10);
      c++;
    }
    if(c>150-1){ //long press => z-mode switch
      z_mode=!z_mode;
      modeToggle=1;
    }
    else{
      modeToggle++;
      if(modeToggle>3){
        modeToggle=0;
      }
    }
  }
  int moveTime=0;
  switch(modeToggle) {
    case 0:
      distance="0.10"; //set movement to 0.1mm
      moveTime=0.1/(1000/60)*1000;
      break;
    case 1: 
      distance="1.0"; //set movement to 1mm
      moveTime=1.0/(1000/60)*1000;
      break;  
    case 2: 
      distance="10.0"; //set movement to 10mm
      moveTime=10.0/(1000/60)*1000;
      break;  
    case 3: 
      if (z_mode) {// maximum Z-axis movement is 20mm not 100. We don't want to shoot it out of the MPCNC!
        distance="20.0"; //set movement to 20mm
        moveTime=20.0/(1000/60)*1000;
      }else{
        distance="100.0"; //set movement to 100
        moveTime=100.0/(1000/60)*1000; //100mm, / F1000[mm/min]/60[s/min]*1000[ms/s]
      }
      break;  
  }
  if(modeChanged){
    modeChanged=false;
    if(z_mode){
      Serial.print("M117 Z-move:");
      Serial.println(distance);
      while(digitalRead(active_mode_toggle)==LOW){} //wait till released...
    }
    else
    {
      Serial.print("M117 XY-move:");
      Serial.println(distance);
    }
  }
 // default
  X_command =" X0";
  Y_command =" Y0";
  Z_command =" Z0";

  bool left_btn = digitalRead(PIN_BUTTON_D)==LOW;
  bool right_btn = digitalRead(PIN_BUTTON_B)==LOW;
  bool up_btn = digitalRead(PIN_BUTTON_A)==LOW;
  bool down_btn = digitalRead(PIN_BUTTON_C)==LOW;

  if (left_btn){
    if(firstXMove){
      firstXMove=false;
      delay(500);
      if(digitalRead(PIN_BUTTON_B)==LOW && digitalRead(PIN_BUTTON_D)==LOW) 
        resetCoordinates();
      else
        X_command = String(" X-" + distance);
    }
    else
      X_command = String(" X-" + distance);
  }
  else
    firstXMove=true;

  if(right_btn){
    if(firstXMove){
      firstXMove=false;
      delay(500);
      if(digitalRead(PIN_BUTTON_D)==LOW && digitalRead(PIN_BUTTON_B)==LOW) 
        resetCoordinates();
      else
      X_command = String(" X" + distance);
    }
    else
      X_command = String(" X" + distance);
  }
  else
    firstXMove=true;
    
  if(up_btn){
    Y_command = String(" Y" + distance);
    Z_command = String(" Z" + distance);
  }
  if(down_btn){
    Y_command = String(" Y-" + distance);
    Z_command = String(" Z-" + distance);
  }

  // read the analog and digital in values:
  x = analogRead(x_Pin); 
  if (x < zeroState_x-treshold_x) {
    X_command = String(" X-" + distance);
  }
  else if (x > zeroState_x+treshold_x){
    X_command = String(" X" + distance);
  }

  y = analogRead(y_Pin);
  if (y < zeroState_y-treshold_y){
    Y_command = String(" Y-" + distance);
    Z_command = String(" Z-" + distance);
  }
  else if (y > zeroState_y+treshold_y)
  {
    Y_command = String(" Y" + distance);
    Z_command = String(" Z" + distance);
  }

  if (z_mode){
    if(Z_command !=" Z0"){
      //Serial.begin(baud_rate); //open serial port
      Serial.println("G91");
      Serial.print("G1");
      Serial.print(Z_command);
      Serial.println(" F200");
      Serial.println("G90");
      delay(moveTime*5+100);
    }
   }else  {
     if(X_command !=" X0" || Y_command !=" Y0"){
      //Serial.begin(baud_rate); //open serial port
      Serial.println("G91");
      Serial.print("G1");
      Serial.print(X_command);
      Serial.print(Y_command);
      Serial.println(" F1000");
      Serial.println("G90");
      delay(moveTime+100);
     }
  }
  
 if (hxy==LOW) {
    //Serial.begin(baud_rate); //open serial port
    Serial.println("G28 X Y");
    Serial.println("M117 Homing XY...");
    modeChanged=true; //resets label...
    delay(3000);
 }; 
 
 if (hz==LOW)
 {
   //Serial.begin(baud_rate);//open serial port
    Serial.println("G28 Z");
    Serial.println("M117 Homing Z...");
    modeChanged=true; //resets label...
    delay(3000);
 };
 
 
 
  // wait 350 milliseconds before the next loop to avoid too many moves cueing up

  // if (stringComplete) {
  //   Serial.println(inputString);
  //   // clear the string:
  //   inputString = "";
  //   stringComplete = false;
  // }
}

//Serial event
// void serialEvent() {
//   while (Serial.available()) {
//     // get the new byte:
//     char inChar = (char)Serial.read();
//     // add it to the inputString:
//     inputString += inChar;
//     // if the incoming character is a newline, set a flag
//     // so the main loop can do something about it:
//     if (inChar == '\n') 
//       stringComplete = true;
//   }
// }