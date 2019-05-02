/* 
|| @Modified by Johannes Regneri 2018
|| @file MultiKey_Midi_Matrix.ino
||
|| @version 1.0
|| @author Mark Stanley
|| @file MultiKey.ino
|| @contact mstanley@technologist.com
|| 
|| @description
|| | The latest version, 3.0, of the keypad library supports up to 10
|| | active keys all being pressed at the same time. This sketch is an
|| | example of how you can get multiple key presses from a keypad or
|| | keyboard.
|| | 2018  Added midi Serial Output
|| #
+ Software for Midi Connection:
  https://projectgus.github.io/hairless-midiserial/
*/

#include <Keypad.h>
//******************************************************USEROPTIONSSTART***********************************
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char keys[ROWS][COLS] = {   //Midi Note Information
{61,62,63,64},
{51,52,53,54},
{41,42,43,44},
{36,37,38,39}
};

byte rowPins[ROWS] = {5, 6, 7, 8}; //connect to the row pinouts of the kpd
byte colPins[COLS] = {12, 11, 10, 9}; //connect to the column pinouts of the kpd
//******************************************************USEROPTIONSEND************************************
Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

unsigned long loopCount;
unsigned long startTime;
String msg;


void setup() {
    Serial.begin(19200);        //Same Bautrate in Midi Serial Bridge
    loopCount = 0;
    startTime = millis();
    msg = "";
}


void loop() {
    loopCount++;
    if ( (millis()-startTime)>5000 ) {
//        Serial.print("Average loops per second = ");    //for Debug
//        Serial.println(loopCount/5);    //for Debug
        startTime = millis();
        loopCount = 0;
    }

    // Fills kpd.key[ ] array with up-to 10 active keys.
    // Returns true if there are ANY active keys.
    if (kpd.getKeys())
    {
        for (int i=0; i<LIST_MAX; i++)   // Scan the whole key list.
        {
            if ( kpd.key[i].stateChanged )   // Only find keys that have changed state.
            {
                switch (kpd.key[i].kstate) {  // Report active key state : IDLE, PRESSED, HOLD, or RELEASED
                    case PRESSED:
                    msg = " PRESSED.";
                break;
                    case HOLD:
                    msg = " HOLD.";
                break;
                    case RELEASED:
                    msg = " RELEASED.";
                break;
                    case IDLE:
                    msg = " IDLE.";
                }
//                Serial.print("Key ");
//                Serial.print(kpd.key[i].kchar);
//                Serial.println(msg);

                 
                 if (msg == " PRESSED."){
                    Serial.write(144);                // 1001 0000 = Note On Kanal 1
                    Serial.write(kpd.key[i].kchar);   //Note defined in Set
                    Serial.write(127);                //Velocity
//                    Serial.println("STATE PRESSET 100");     //for Debug
                 }
                 if (msg == " IDLE."){
                    Serial.write(144);                // 1001 0000 = Note On Kanal 1
                    Serial.write(kpd.key[i].kchar);   //Note defined in Setup
                    Serial.write(0);                  //Velocity
//                    Serial.println("STATE IDLE 0"); //for Debug
                 }
                 
            }
        }
    }
}  // End loop
