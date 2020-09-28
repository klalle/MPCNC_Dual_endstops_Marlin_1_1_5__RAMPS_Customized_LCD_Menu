//  #include <Arduino.h>
// /// Armuino --- Funduino Joystick Shield ---
// // Playlist: https://www.youtube.com/playlist?list=PLRFnGJH1nJiKIpz_ZyaU-uAZOkMH8GAcw
// //
// // Part 1. Introduction - Basic Functions: https://www.youtube.com/watch?v=lZPZuBCFMH4

// // Arduino digital pins associated with buttons
// const byte PIN_BUTTON_A = 2; 
// const byte PIN_BUTTON_B = 3;
// const byte PIN_BUTTON_C = 4;
// const byte PIN_BUTTON_D = 5;
// const byte PIN_BUTTON_E = 6;
// const byte PIN_BUTTON_F = 7;

// // Arduino analog pins associated with joystick
// const byte PIN_ANALOG_X = 0;
// const byte PIN_ANALOG_Y = 1;


// void setup() {
//   Serial.begin(115200);

//   pinMode(PIN_BUTTON_B, INPUT);  
//   digitalWrite(PIN_BUTTON_B, HIGH);
 
//   pinMode(PIN_BUTTON_E, INPUT);  
//   digitalWrite(PIN_BUTTON_E, HIGH);
 
//   pinMode(PIN_BUTTON_C, INPUT);  
//   digitalWrite(PIN_BUTTON_C, HIGH);
 
//   pinMode(PIN_BUTTON_D, INPUT);  
//   digitalWrite(PIN_BUTTON_D, HIGH);
 
//   pinMode(PIN_BUTTON_A, INPUT);  
//   digitalWrite(PIN_BUTTON_A, HIGH);  
// }

// void loop() {
//   Serial.print("Buttons A:");
//   Serial.print(digitalRead(PIN_BUTTON_A));
//   Serial.print(" ");
 
//   Serial.print("B:");
//   Serial.print(digitalRead(PIN_BUTTON_B));
//   Serial.print(" ");
 
//   Serial.print("C:");
//   Serial.print(digitalRead(PIN_BUTTON_C));
//   Serial.print(" ");
 
//   Serial.print("D:");
//   Serial.print(digitalRead(PIN_BUTTON_D));
//   Serial.print(" ");

//   Serial.print("E:");
//   Serial.print(digitalRead(PIN_BUTTON_E));
//   Serial.print(" ");
 
//   Serial.print("F:");
//   Serial.print(digitalRead(PIN_BUTTON_F));
//   Serial.print(" -- ");
 
//   Serial.print("Position X:");
//   Serial.print(analogRead(PIN_ANALOG_X)-504);
//   Serial.print(" ");
 
//   Serial.print("Y:");
//   Serial.print(analogRead(PIN_ANALOG_Y)-514);
//   Serial.print(" ");  
 
//   Serial.println();
//   delay(1000);
// }
