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


boolean z_mode = true; //move z-axis if true
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
String Z_distance = "1.0 ";

int XY_Feedrate = 2000;
int Z_Feedrate = 300;

int modeToggle=1; // set 0.1mm,1, 10, 100

//String inputString = "";         // a string to hold incoming data
//boolean stringComplete = false;  // whether the string is complete

bool modeChanged=true;

int minTimeToReleaseBtnDelay = 50;

bool firstXMoveLeft=true;
bool firstXMoveRight=true;
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
  Serial.println("M117 Home is set!");
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
    if(c>150-1){ //long press => z-mode switch (up and down btns = Y instead of Z...)
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
  
  int XY_moveTime=0;
  int Z_moveTime=0;

  if(modeChanged){
    modeChanged=false;
    switch(modeToggle) {
      case 0:
        distance="0.10"; //set movement to 0.1mm
        XY_moveTime=0.1/(XY_Feedrate/60)*1000;
        Z_moveTime =0.1/(Z_Feedrate/60)*1000;
        break;
      case 1: 
        distance="1.0"; //set movement to 1mm
        XY_moveTime=1.0/(XY_Feedrate/60)*1000;
        Z_moveTime =1.0/(Z_Feedrate/60)*1000;
        break;  
      case 2: 
        distance="10.0"; //set movement to 10mm
        XY_moveTime=10.0/(XY_Feedrate/60)*1000;
        Z_moveTime =10.0/(Z_Feedrate/60)*1000;
        break;  
      case 3: 
          Z_distance = "20.0";// maximum Z-axis movement is 20mm not 100. We don't want to shoot it out of the MPCNC!
          Z_moveTime = 20.0/(Z_Feedrate/60)*1000;
          distance="100.0"; //set movement to 100
          XY_moveTime=100.0/(XY_Feedrate/60)*1000; //100mm, / F1000[mm/min]/60[s/min]*1000[ms/s]
        break;  
    }
    if(modeToggle<3 && z_mode)
      Z_distance=distance;

    if(z_mode){
      Serial.print("M117 Joystick XY:");
      Serial.print(distance);
      Serial.print("mm Z:");
      Serial.print(distance);
      Serial.println("mm");
    }
    else
    {
      Serial.print("M117 Joystick XY:");
      Serial.println(distance);
      Serial.println("mm");
    }
    while(digitalRead(active_mode_toggle)==LOW){} //wait till released...
  }

  X_command =" X0";
  Y_command =" Y0";
  Z_command =" Z0";

  bool left_btn = digitalRead(PIN_BUTTON_D)==LOW;
  bool right_btn = digitalRead(PIN_BUTTON_B)==LOW;
  bool up_btn = digitalRead(PIN_BUTTON_A)==LOW;
  bool down_btn = digitalRead(PIN_BUTTON_C)==LOW;

  if (left_btn){
    if(firstXMoveLeft){ //check if both left and right is pressed => reset coordinates
      firstXMoveLeft=false;
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
    firstXMoveLeft=true;

  if(right_btn){
    if(firstXMoveRight){//check if both left and right is pressed => reset coordinates
      firstXMoveRight=false;
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
    firstXMoveRight=true;
    
  if(up_btn){
    if(z_mode)
      Z_command = String(" Z" + Z_distance);
    else
      Y_command = String(" Y" + distance);
  }
  if(down_btn){
    if(z_mode)
      Z_command = String(" Z-" + Z_distance);
    else
      Y_command = String(" Y-" + distance);
  }

  // read the analog and digital in values:
  x = analogRead(x_Pin); 
  if (x < zeroState_x-treshold_x)
    X_command = String(" X-" + distance);
  else if (x > zeroState_x+treshold_x)
    X_command = String(" X" + distance);

  y = analogRead(y_Pin);
  if (y < zeroState_y-treshold_y)
    Y_command = String(" Y-" + distance);
  else if (y > zeroState_y+treshold_y)
    Y_command = String(" Y" + distance);

  if(Z_command !=" Z0"){
    Serial.println("G91");
    Serial.print("G1");
    Serial.print(Z_command);
    Serial.print(" F");
    Serial.println(Z_Feedrate);
    Serial.println("G90");
    if(Z_moveTime<minTimeToReleaseBtnDelay)
      delay(minTimeToReleaseBtnDelay);
    else
      delay(Z_moveTime);
  }
  if(X_command !=" X0" || Y_command !=" Y0"){
    Serial.println("G91");
    Serial.print("G1");
    Serial.print(X_command);
    Serial.print(Y_command);
    Serial.print(" F");
    Serial.println(XY_Feedrate);
    Serial.println("G90");
    if(XY_moveTime<minTimeToReleaseBtnDelay)
      delay(minTimeToReleaseBtnDelay);
    else
      delay(XY_moveTime);
  }
  
  if (hxy==LOW) {
    Serial.println("G28 X Y");
    Serial.println("M117 Homing XY...");
    modeChanged=true; //resets label...
    delay(3000);
  }
  
  if (hz==LOW)
  {
    Serial.println("G28 Z");
    Serial.println("M117 Homing Z...");
    modeChanged=true; //resets label...
    delay(3000);
  }
 
  // if (stringComplete) { //if received: debug to software serial USB-adapter
  //   SoftwareSerial.println(inputString);
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