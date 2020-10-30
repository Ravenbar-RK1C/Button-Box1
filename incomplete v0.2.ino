#include <HID-Project.h>
#include <Key.h>
#include <Keypad.h>
#include <Bounce2.h>
#include <Encoder.h>        //http://www.pjrc.com/teensy/td_libs_Encoder.html


/**
 * 
 * 
This is a compliation of code from:
FrankLumien's Micro USB Media controller (Arduino Pro Micro): https://www.thingiverse.com/thing:2967216
Alexander Brevig custom Keypad Example: https://codebender.cc/example/Keypad/CustomKeypad#CustomKeypad.ino
Media Controller by Haid

Set Board to: Arduinio Leonardo

               Pro Micro Pinout
                      ___
                  ___|   |___
                 |    ---    |
                 |* TX0  RAW*|
                 |* RX1  GND*| Ground (enc)
    Ground (but) |* GND  RST*|
                 |* GND  VCC*| VCC (enc)
                 |* 2     A3*| Keypad Column 4
                 |* 3     A2*| Keypad Column 3
                 |* 4     A1*| Keypad Column 2
          Red LED|* 5     A0*| Keypad Column 1
        Encoder A|* 6     15*| Keypad Row 4
        Encoder B|* 7     14*| Keypad Row 3
        Green LED|* 8     16*| Keypad Row 2
 Encoder SW/Mute |* 9     10*| Keypad Row 1               
                  -----------

/**
 * 
 */

 // Setup inputs
bool Special = false; // spacial keys check
int rled=5; // red LED pin
int gled=8; // green LED pin
 const byte ROWS = 4; // four rows
 const byte COLS = 4; // four colomns

//define the symbols on the keypad
char hexaKeys[ROWS][COLS] =
{
    {'a','e','j','o'},
    {'b','f','k','p'},
    {'c','g','l','q'},
    {'d','h','m','r'},
};

byte rowPins[ROWS] = {10, 16, 14, 15}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {A0, A1, A2, A3}; //connect to the column pinouts of the keypad

//Setup Rotary Encoder
const uint8_t encA = 6;       // CLK on encoder
const uint8_t encB = 7;       // DT on encoder
const uint8_t mutePin = 9;   // SW on encoder

long oldPosition  = -999;
bool muteState = false;      // Remember what state was the button last time

Encoder myEnc(encB, encA);
 
// Instantiate a Bounce object
Bounce muteButton = Bounce();
Bounce previousButton = Bounce();
Bounce volumeControl = Bounce();
// Debounce time in milliseconds
const unsigned long intervalMs = 20;

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);




void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
            
    // Setup the Bounce instance :
  pinMode(mutePin, INPUT_PULLUP);
 muteButton.attach(mutePin); 
 muteButton.interval(intervalMs);


  // set the digital pin as output:
  pinMode(rled, OUTPUT);
  pinMode(gled, OUTPUT);
}

//----------------------------------------------------------------
void loop() {
  // put your main code here, to run repeatedly:
  checkVolumeControl();
  checkMuteButton();
  checkKeypad(); 
}

//----------------------------------------------------------------
void checkMuteButton() {
    muteButton.update();
  const byte buttonState = muteButton.read();
  if (buttonState == LOW && muteButton.fallingEdge()) {
      Consumer.write(MEDIA_VOLUME_MUTE);
      redBlink();
    if (!muteState) {
      muteState = true;
                    } else {
      muteState = false;
                }
  }
}
//----------------------------------------------------------------
void checkVolumeControl() {
  const long newPosition = myEnc.read();
  if (newPosition != oldPosition) {
    if (newPosition > oldPosition) {
    
     Consumer.write(MEDIA_VOLUME_UP);
     Serial.print("Volume Up");
         } else {
      Serial.print("Volume down");
      Consumer.write(MEDIA_VOLUME_DOWN);
          }
    oldPosition = newPosition;
    muteState = false;
    Serial.println();
  }
}
void greenBlink(){
  digitalWrite(gled, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(250);               // wait for 1/2 second
  digitalWrite(gled, LOW);    // turn the LED off by making the voltage LOW
  delay(50);
  }

void redBlink(){
  digitalWrite(rled, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(250);               // wait for 1/2 second
  digitalWrite(rled, LOW);    // turn the LED off by making the voltage LOW
  delay(50);
  } 
//----------------------------------------------------------------
void checkKeypad() {

char customKey = customKeypad.getKey();
  if (customKey){ 
    if (customKey == 'a'){
          Serial.print("Previous");
          Consumer.write(MEDIA_PREVIOUS);
    }
    else if(customKey == 'e'){
          Serial.print("PlayPause");
          Consumer.write(MEDIA_PLAY_PAUSE);
    }
    else if(customKey == 'j'){
          Serial.print("Next");
          Consumer.write(MEDIA_NEXT);
          
    }
    else{
           Serial.write(customKey);
           Keyboard.press(KEY_LEFT_ALT);
           Keyboard.press(KEY_RIGHT_CTRL);
           Keyboard.write( customKey );
           Keyboard.release(KEY_LEFT_ALT);
           Keyboard.release(KEY_RIGHT_CTRL); 
        greenBlink();
      
  }
}
}
