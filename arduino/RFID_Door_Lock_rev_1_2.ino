/* Code written by Shane Fonyi
 * Using Arduino UNO, servo HiTEC HS-322HD, & ElectHouse RFID  
 * Shane Fonyi (C) All Rights Reserved
 * 2/23/2016
 * Email: shane@jayhawk.us 
 * Rev 1.1: Method added to toggle lock
 * Rev 1.2 : 5 Sec delay added for auto door lock
 * Rev 1.3 : Removed delay for security reasons. Fixed the serial input for IDs. Added a delay for queue to build.
 */

#include <SoftwareSerial.h> 
SoftwareSerial mySerial(10, 11); //pin10 Tx, pin11 Rx 
#include <Servo.h>
Servo myservo;  //declare the servo attach to pin 3
int unlock=90; //right turn for servo
int lock=7; //up position for servo
int out_flag =0;
String cardNumber=""; //string to store card number
unsigned long actionTime;    //store time when lock is toggled
int buttonPress = 0;  //declare variable for button status
int lastbuttonPress = 0;  //declare variable for last button status
boolean locked = true;
const int buttonPin = 2;     // the number of the pushbutton pin
int buttonState = 0; //current button state
void setup() 
{ 
myservo.attach(3);//servo assigned pin 3
myservo.write(lock);  //set servo to lock
delay(3000);
myservo.detach(); //detach the servo to save battery
// initialize the pushbutton pin as an input:
pinMode(buttonPin, INPUT);
Serial.begin(9600);
mySerial.listen();
Serial.println("We ready\n"); 
// set the data rate for the SoftwareSerial port 
mySerial.begin(9600); 
mySerial.write(0x02); //Send the command to RFID, please refer to RFID manual 
} 
void loop() // run over and over 
{ 
      buttonState = digitalRead(buttonPin); //check button state
       if (buttonState == HIGH) {
        // if button is pressed toggle lock
        //Serial.print("Button Pressed");
        toggleLock();
      }
      while (mySerial.available()) { //check serial
      delay(10);
      byte C = mySerial.read(); //read data from RFID to byte
      //if (C<16) Serial.print("0"); //add ending marker
      cardNumber.concat(C); //concatinate the bytes and typecast string
      //Serial.print(cardNumber);
      //Serial.print(C ,HEX); //Display the Serial Number in HEX
      out_flag =1;  
    }
      //Serial.print(cardNumber);
      if (cardNumber == '123456789'){
      //Serial.print("ALLOWED\n");
      toggleLock();
      cardNumber.remove(0); //clear the string for next number
      }
     if (out_flag >0) {
       Serial.println();
       cardNumber.remove(0);
       out_flag = 0;
     }
     
}


void toggleLock(){  //sequence to cycle the lock (servo)
  myservo.attach(3);
  if(locked == true){      // compare locked status to last time
    //Serial.println("Door Unlocked");
    myservo.write(unlock);
    delay(1500);
    locked = !locked;      //change locked status
    //actionTime = millis();  //record time this locking action happened
    //delay(5000); //delay 5 seconds
    //toggleLock(); //auto lock the door
  } 
  else {
    //Serial.println("Door Locked");
    myservo.write(lock);
    delay(1500);
    locked = !locked;      //change the locked status
    //actionTime = millis();  //record time this locking action happened
  }
  myservo.detach();
  cardNumber.remove(0);
}
