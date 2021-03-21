#include <Adafruit_NeoPixel.h>


//setup control panel LEDs
#define controlPanelLED_PIN 4
#define controlPanelLED_COUNT 22
Adafruit_NeoPixel controlPanelLED(controlPanelLED_COUNT, controlPanelLED_PIN, NEO_GRB + NEO_KHZ800);

//setup chessboard LEDs
#define chessboardLED_PIN A5
#define chessboardLED_COUNT 64
Adafruit_NeoPixel chessboardLED(chessboardLED_COUNT, chessboardLED_PIN, NEO_GRB + NEO_KHZ800);


uint32_t ledWHITE = controlPanelLED.Color(255, 255, 255);
uint32_t ledBLACK = controlPanelLED.Color(0, 0, 0);
uint32_t ledBLUE = controlPanelLED.Color(0, 0, 255);
uint32_t ledRED = controlPanelLED.Color(255, 0, 0);
uint32_t ledGREEN = controlPanelLED.Color(0, 255, 0);

const int delayTiming = 1000;
const int inputButtons[] = {3,5,6,7,8,9,10,11,12,A1};
const int buttonDebounceTime  = 300;

void setup() {
  // put your setup code here, to run once:
    pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);

  //setup the buttons for inputting moves
  for (int i=0; i<10; i++)  //the less then needs to represent how many buttons we are using for this to complete enough loops
  {
    pinMode(inputButtons[i], INPUT_PULLUP);
  }

  //setup the hint interupt pin
  attachInterrupt(digitalPinToInterrupt(3), hint, FALLING);

  
  //setup for the control panels neopixels
  controlPanelLED.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  controlPanelLED.show();            // Turn OFF all pixels ASAP
  controlPanelLED.setBrightness(100); // Set BRIGHTNESS (max = 255)

  //setup for the chessboard neopixels
  chessboardLED.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  chessboardLED.show();            // Turn OFF all pixels ASAP
  chessboardLED.setBrightness(20); // Set BRIGHTNESS (max = 255)
}

void loop() {
  // put your main code here, to run repeatedly:

    Serial.println("Testing the LED's.....");
    chessboardLED.fill(ledRED, 0, 8);
    chessboardLED.show();
    delay(delayTiming);
    chessboardLED.fill(ledWHITE, 8, 8);
    chessboardLED.show();
    delay(delayTiming);
    chessboardLED.fill(ledBLUE, 16, 8);
    chessboardLED.show();
    delay(delayTiming);
    chessboardLED.fill(ledGREEN, 24, 8);
    chessboardLED.show();
    delay(delayTiming);
    chessboardLED.fill(ledRED, 32, 8);
    chessboardLED.show();
    delay(delayTiming);
    chessboardLED.fill(ledWHITE, 40, 8);
    chessboardLED.show();
    delay(delayTiming);
    chessboardLED.fill(ledBLUE, 48, 8);
    chessboardLED.show();
    delay(delayTiming);
    chessboardLED.fill(ledGREEN, 56, 8);
    chessboardLED.show();
    delay(delayTiming);

    controlPanelLED.fill(ledWHITE, 0, 2);
    controlPanelLED.show();
    delay(delayTiming);
    controlPanelLED.fill(ledWHITE, 2, 2);
    controlPanelLED.show();
    delay(delayTiming);
    controlPanelLED.fill(ledWHITE, 4, 1);
    controlPanelLED.show();
    delay(delayTiming);
    controlPanelLED.fill(ledWHITE, 5, 1);
    controlPanelLED.show();
    delay(delayTiming);
    controlPanelLED.fill(ledBLUE, 6, 8);
    controlPanelLED.show();
    delay(delayTiming);
    controlPanelLED.fill(ledBLUE, 14, 8);
    controlPanelLED.show();
    delay(delayTiming);
    controlPanelLED.fill(ledGREEN, 0, 22);
    controlPanelLED.show();
    chessboardLED.fill(ledGREEN, 0, 64);
    chessboardLED.show();
    delay(delayTiming);
    Serial.println("LED's should all now be green, you can test the buttons now by pressing them.");

    while (true) {
      int detectButton();
      int val;
      //Serial.println("Waiting for a button to be detected...");
        while (true) {  
        if (digitalRead(5) == LOW){
          Serial.println("Button 1 (for A/1) detected");
          val = 1;
          break;
        } else if (digitalRead(6) == LOW){
          Serial.println("Button 2 (for B/2) detected");
          val = 2;
          break;
        } else if (digitalRead(7) == LOW){
          Serial.println("Button 3 (for C/3) detected");
          val = 3;
          break;
        } else if (digitalRead(8) == LOW){
          Serial.println("Button 4 (for D/4) detected");
          val = 4;
          break;
        } else if (digitalRead(9) == LOW){
          Serial.println("Button 5 (for E/5) detected");
          val = 5;
          break;
        } else if (digitalRead(10) == LOW){
          Serial.println("Button 6 (for F/6) detected");
          val = 6;
          break;
        } else if (digitalRead(11) == LOW){
          Serial.println("Button 7 (for G/7) detected");
          val = 7;
          break;
        } else if (digitalRead(12) == LOW){
          Serial.println("Button 8 (for H/8) detected");
          val = 8;
          break;
        } else if (digitalRead(A1) == LOW){
          Serial.println("Button for 'OK' detected");
          val = 9;
          break;
          }
        }
      }
    }


void hint(){
 static unsigned long last_interrupt_time = 0;
 unsigned long interrupt_time = millis();
 // If interrupts come faster than 200ms, assume it's a bounce and ignore
 if (interrupt_time - last_interrupt_time > 200) 
 {
Serial.println("Button connected to 'HINT' detected");
 }
 last_interrupt_time = interrupt_time;
}
