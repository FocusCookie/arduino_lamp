#include <ButtonEvents.h>

// ADAFRUIT
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define NEO_PIN    6  // NeoPixel DATA
// What type of NeoPixel strip is attached to the Arduino?
#define NEO_PTYPE  NEO_GRBW  // f.e. SK6812
//#define NEO_PTYPE NEO_GRB   // most NeoPixel products

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS  115

#define BRIGHTNESS  200 // set max brightness

#define IWAIT    2000
#define SWAIT   25
#define LWAIT   50
#define HWAIT   500

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_GRBW    Pixels are wired for GRBW bitstream (f.e. SK6812)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, NEO_PIN, NEO_PTYPE + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

// NOTE: RGBW LEDs draw up to 80mA with all colors + white at full brightness!
// That means that a 60-pixel strip can draw up to 60x 80 = 4800mA, so you
// should use a 5A power supply; for a 144-pixel strip its max 11520mA = 12A!

// COLOR VARS for NEO Pixels
uint32_t warmWhite = strip.Color(0, 0, 0, 255);
uint32_t coldWhite = strip.Color(0, 0, 255, 255);
uint32_t magenta = strip.Color(255, 153, 255);
uint32_t magentaWhite = strip.Color(255, 0, 255, 255);
uint32_t red = strip.Color(255, 0, 0, 0);
uint32_t green = strip.Color(0, 255, 0, 0);
uint32_t blue = strip.Color(0, 0, 255, 0);
uint32_t yellow = strip.Color(0, 255, 255, 0);
uint32_t orange = strip.Color(255, 153, 0, 0);
uint32_t colors[] = {warmWhite, coldWhite, magentaWhite, magenta, red, green, orange, blue, yellow};
int colorsCount = 8;

// Black / OFF

uint32_t black = strip.Color(0, 0, 0, 0);




// Initialization of the needed variables<br />
int Counter = 255; 
boolean direction;

int pinClkLastValue;  
int pinClkCurrentValue;
 
// Definition of the input-pins
int PinCLK = 2;  
int PinDT = 4; 


const byte buttonPin = 3;
#define LED_PIN 13 // Builtin LED 
int ledState = LOW;

// ButtonEvents Instance
ButtonEvents button;


// LED STRIPES
int startStripe_1 = 0;
int startStripe_2 = 24;
int startStripe_3 = 46;
int startStripe_4 = 70;
int startStripe_5 = 92;

int endStripe_1 = 21;
int endStripe_2 = 45;
int endStripe_3 = 67;
int endStripe_4 = 91;
int endStripe_5 = 113;

boolean stripeStates[] = {1,1,1,1,1};
int stripeStartPosition = 0;

int maxStripes = 5;

// Variables for the lamp
int maxHeight = 22;

int lampMode = 0;
int maxLampMode = 3;
boolean lampState = false;
int brightnessFactor = 5;
int maxCounter;
int minCounter;

int currentColor = 0;
int currentBrightness = BRIGHTNESS;
int currentHeight = 1;
int currentStripeAmount = maxStripes;
int currentStripeStartPosition = stripeStartPosition;

  
void setup() 
{ 
  // Internal LED
    pinMode(LED_PIN,OUTPUT); // Setup the LED
    digitalWrite(LED_PIN,ledState);

  // Serial
  Serial.begin (9600);

  // ******************************************************* KY404 SETUP  
  // Initialization of the input-pins...
    pinMode (PinCLK,INPUT);
    pinMode (PinDT,INPUT);
  // ...and activating of their pull up resistors
    digitalWrite(PinCLK, true);
    digitalWrite(PinDT, true);
  // Initial reading of the Pin_CLK
    pinClkLastValue = digitalRead(PinCLK);   
  // Button on KY404
    pinMode(buttonPin, INPUT_PULLUP);
    // Attach buttonEvents with the button pin
    button.attach(buttonPin);

  // ADAFRUIT LEDs
  #ifdef __AVR_ATtiny85__
    // This is for Trinket 5V 16MHz
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
    // End of trinket special code
  #endif
    
  strip.begin(); // This initializes the NeoPixel library.
  strip.setBrightness(BRIGHTNESS); // set brightness
  strip.show(); // Initialize all pixels to 'off'
  
  #ifdef IWAIT
    delay(IWAIT);
  #endif
  
 } 
 
// The program checks, which of the status pins have changed first, to calculate the rotational direction,<br />// after a change of the pin status was detected.
// You will get this information if you compare the pin status of the current run with the pin status of the run before.<br />// After detecting the rotational direction, the steps from the start position will be counted and outputted.<br />// Pushing the button of the rotary encoder will reset the current position.



 
void loop()
{ 
  

  
  // Reading of the current status  
  pinClkCurrentValue = digitalRead(PinCLK);
  
  // Check for a Change
  if (pinClkCurrentValue != pinClkLastValue)
  { 
        
      if (digitalRead(PinDT) != pinClkCurrentValue) 
      {  
          // Pin_CLK has changed first
          direction = true;
          counterCheck(Counter, direction);
          lampControl();           
      } 
        
      else
      {       // Else PinDT changed first
          direction = false;
          counterCheck(Counter, direction);
          lampControl();
         
      }
      Serial.print ("Rotational direction: ");
       
      if (direction)
      {
         Serial.println ("Clockwise");
      }
      else
      {
         Serial.println("Counterclockwise");
         Serial.print("Adresse zum Ausschalten");
         Serial.println(startStripe_1+currentHeight-1);
      }
       
      Serial.print("Current brightness: ");
      Serial.println(currentBrightness);
      Serial.println("------------------------------");

      Serial.print("Current color: ");
      Serial.println(currentColor);
      Serial.println("------------------------------");

      Serial.print("Current height: ");
      Serial.println(currentHeight);
      Serial.println("------------------------------");

      Serial.print("maxCount: ");
      Serial.println(maxCounter);
      Serial.println("------------------------------");

        
  } 





  // KY404 BUTTON
  button.update();

  if(button.tapped() == true){
    ledState = !ledState; // Toggle LED state
    digitalWrite(LED_PIN,ledState); // Apply new LED state
    lampMode++;
    if(lampMode > maxLampMode){
      lampMode = 1;
    }
    lampControl();
    Serial.print("lampMode: ");
    Serial.println(lampMode);
    Serial.println("------------------------------");

    Serial.print("Current position: ");
    Serial.println(Counter);
    Serial.println("------------------------------");
  }

  if(button.doubleTapped() == true){
    Serial.println("DOUBLE-TAP event detected");
  }

  if(button.held() == true){
    Serial.println("HOLD event detected");
    // switch off the lamp
    lampMode = 0;
    lampControl();
  }
 } 



// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint16_t wait) {
  for(uint16_t i=0; i<currentHeight; i++) {
    // Stripe 1    
      strip.setPixelColor(i+startStripe_1, c);

    // Stripe 2
      strip.setPixelColor(endStripe_2-i, c);

    // Stripe 3    
      strip.setPixelColor(i+startStripe_3, c);

    // Stripe 4
      strip.setPixelColor(endStripe_4-i, c);

    // Stripe 5
      strip.setPixelColor(i+startStripe_5, c);
 
    strip.show();
    delay(wait);
  }
}
// Fade off with reducing brightness
void fadeOff(uint16_t wait) {
  byte brightness;
  while ((brightness = strip.getBrightness()) > 0) {
    strip.setBrightness(--brightness);
    strip.show(); // This sends the updated pixel brightness to the hardware.
    delay(wait); // Delay for a period of time (in milliseconds).
  }
}

// led Stripe Height
void stripeHeight(uint16_t wait) {
  
// direction = clockwise turn add height


  if(direction){
    // SWITCH ON
    // Stripe 1    
      strip.setPixelColor(startStripe_1+currentHeight-1, colors[currentColor]);


    // Stripe 2
      strip.setPixelColor(endStripe_2-currentHeight+1, colors[currentColor]);

    // Stripe 3   
      strip.setPixelColor(startStripe_3+currentHeight-1, colors[currentColor]);
   
    // Stripe 4
      strip.setPixelColor(endStripe_4-currentHeight+1, colors[currentColor]);

    

    // Stripe 5
      strip.setPixelColor(startStripe_5+currentHeight-1, colors[currentColor]);

    
    
    strip.show();
    delay(wait);
  }

    if(!direction){
    // SWITCH OFF
    // Stripe 1    
      strip.setPixelColor(startStripe_1+currentHeight, black);
    

    // Stripe 2
      strip.setPixelColor(endStripe_2-currentHeight, black);
    

    // Stripe 3    
      strip.setPixelColor(startStripe_3+currentHeight, black);
    

    // Stripe 4
      strip.setPixelColor(endStripe_4-currentHeight, black);
    

    // Stripe 5
      strip.setPixelColor(startStripe_5+currentHeight, black);
    
    
    strip.show();
    delay(wait);
  } 

}



void counterCheck(int32_t count, boolean clockwise){
  switch(lampMode) {
    case 1: // Brightness
      // Max and min counter value
      maxCounter = BRIGHTNESS;
      
      if(clockwise){
        currentBrightness += brightnessFactor;
      } else {
        currentBrightness -= brightnessFactor;
      }
      
      if(currentBrightness > maxCounter){
        currentBrightness = maxCounter;
      }
      if(currentBrightness < brightnessFactor+1){
        currentBrightness = brightnessFactor;
      } 
      break;
    case 2: // color
      // Max and min counter value
      maxCounter = colorsCount;
      minCounter = 0;

      if(clockwise){
        currentColor++;
      } else {
        currentColor--;
      }
      
      if(currentColor > maxCounter){
        currentColor = maxCounter;
      }
      if(currentColor < minCounter){
        currentColor = minCounter;
      } 
      break;
    case 3: // height
      // Max and min counter value
      maxCounter = maxHeight;
      minCounter = 1;


      if(clockwise){
        currentHeight++;
      } else {
        currentHeight--;
      }
      
      if(currentHeight > maxCounter){
        currentHeight = maxCounter;
      }
      if(currentHeight < minCounter){
        currentHeight = minCounter;
      } 
      break;
    case 4: // StripeAmount
      // Max and min counter value
      maxCounter = maxStripes;
      minCounter = 1;


      if(clockwise){
        currentStripeAmount++;
      } else {
        currentStripeAmount--;
      }
      
      if(currentStripeAmount > maxCounter){
        currentStripeAmount = maxCounter;
      }
      if(currentStripeAmount < minCounter){
        currentStripeAmount = minCounter;
      } 
      break;
    case 5: // Stripe Start Address
      // Max and min counter value
      maxCounter = maxStripes-1;
      minCounter = 0;


      if(clockwise){
        currentStripeStartPosition++;
      } else {
        currentStripeStartPosition--;
      }
      
      if(currentStripeStartPosition > maxCounter){
        currentStripeStartPosition = maxCounter;
      }
      if(currentStripeStartPosition < minCounter){
        currentStripeStartPosition = minCounter;
      } 
      break;
  }
}

void lampControl() {
  switch(lampMode){
    case 0:
      fadeOff(SWAIT);
      strip.clear(); // turn all pixels off
      strip.setBrightness(currentBrightness); // restore max brightness value   
      lampState = false;
      break;
    case 1:
      if(!lampState){
        colorWipe(colors[currentColor], LWAIT);
        lampState = true;
      }
      strip.setBrightness(currentBrightness);
      strip.show(); // This sends the updated pixel brightness to the hardware.
      delay(SWAIT); // Delay for a period of time (in milliseconds).
      break;
    case 2:
      colorWipe(colors[currentColor], 10);
      break;
    case 3:
      stripeHeight(SWAIT);
      break;
  }
}

void setStripesAmount(uint16_t count){
  if(direction){
    stripeStates[currentStripeAmount] = true;
  } else {
    stripeStates[currentStripeAmount+1] = false;
  }
}

void setStripesPosition(uint16_t start){
  if(direction){
    boolean save = stripeStates[4];
    for(uint16_t i = maxStripes; i>0; i--){
      if(i == maxStripes-1){
        stripeStates[0] = save;
      } else {
        stripeStates[i] = stripeStates[i-1];
      }
    }
  } else {
    boolean save = stripeStates[0];
    for(uint16_t i = 0; i < maxStripes; i++){
      if(i == 4){
        stripeStates[4] = save;
      } else {
        stripeStates[i] = stripeStates[i+1];
      }
    }
  }
}

 
