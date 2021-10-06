#include <FastLED.h>
#include <Keypad.h>

// --------- BEGIN CONFIGURABLE SETTINGS ---------
// PIN MAPPINGS                             
#define LED_PIN 11                // PIN for the LED Strip
#define LED_PIN_INDICATOR 10      // PIN for the LED indicator (small preview)

#define BRIGHTNESS_PIN A5         // PIN used to control the brightness
#define SPEED_PIN A4              // PIN used to control the speed

#define EFFECT_PIN_01 A0
#define EFFECT_PIN_02 A1
#define EFFECT_PIN_03 A2
#define EFFECT_PIN_04 A3

#define TRIG_PIN 13               // PIN used for the trig side sonar sensor
#define ECHO_PIN 12               // PIN used for the echo side of the sonar sensor

// CONSTANTS
#define DISPLAY_INDICATOR true    // States if the indicator is connected and if we want to use it

#define PING_MIN_RANGE 0                         
#define PING_MAX_RANGE 7                         

#define LED_MIN_SPEED 5                          
#define LED_MAX_SPEED 45                        
                                     
#define NUM_LED 60                 // Number of LED that the strip has                                               
#define NUM_LED_INDICATOR 6        // Number of LED that the indicator has
#define LED_TYPE WS2811                       
#define COLOR_ORDER GRB                         

const byte PAD_ROWS = 4;           // Number of rows that are in the control pad
const byte PAD_COLUMNS = 4;        // Number of columns that are in the control pad

// Define keypad keys
char hexKeys[PAD_ROWS][PAD_COLUMNS] =
{
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'0', 'F', 'E', 'D'}
};

// Assignment of pin for the keypad
byte rows_pin[PAD_ROWS] = {2, 3, 4, 5};
byte columns_pin[PAD_COLUMNS] = {6, 7, 8, 9};

Keypad clavier = Keypad( makeKeymap(hexKeys), rows_pin, columns_pin, PAD_ROWS, PAD_COLUMNS); // creation of object keypad

// ---------- END CONFIGURABLE SETTINGS ----------

// Define color offsets
#define COLOR_01 0
#define COLOR_02 16
#define COLOR_03 32
#define COLOR_04 48

// GLOBAL VARIABLES
CRGB ledArray[NUM_LED];
CRGB ledIndicatorArray[NUM_LED_INDICATOR];
CRGBPalette16 currentPalette;
uint8_t ledBrightness = 100; // Default only, will be overwritten
float ledSpeed = 10;       // Default only, will be overwritten
uint8_t visual = 1;
uint8_t palette = 0;
uint8_t effect = 0;
long update_time = millis();
uint8_t currentLEDPosition = 0;

// BEGIN CODE

void setup() {
  //Initialize LED Strip
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(ledArray, NUM_LED).setCorrection( TypicalLEDStrip );

  if(DISPLAY_INDICATOR){
   FastLED.addLeds<LED_TYPE, LED_PIN_INDICATOR, COLOR_ORDER>(ledIndicatorArray, NUM_LED_INDICATOR).setCorrection( TypicalLEDStrip );
  }

  FastLED.setBrightness(ledBrightness);
 
  //Set pin modes
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(EFFECT_PIN_01, INPUT);
  pinMode(EFFECT_PIN_02, INPUT);
  pinMode(EFFECT_PIN_03, INPUT);
  pinMode(EFFECT_PIN_04, INPUT);

  // Initialize palette to gray
  currentPalette = CRGBPalette16(
                     CRGB(100, 100, 100), CRGB(100, 100, 100), CRGB(100, 100, 100), CRGB(100, 100, 100),
                     CRGB(100, 100, 100), CRGB(100, 100, 100), CRGB(100, 100, 100), CRGB(100, 100, 100),
                     CRGB(100, 100, 100), CRGB(100, 100, 100), CRGB(100, 100, 100), CRGB(100, 100, 100),
                     CRGB(100, 100, 100), CRGB(100, 100, 100), CRGB(100, 100, 100), CRGB(100, 100, 100) );

  Serial.begin(9600);
}

void loop() {
  char key = clavier.getKey();
  // if there is not any key, function getKey send NULL character (0x00)
  switch(key) {
    // Visualizers go from 0 to 9
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      visual = (int)key - '0';
      break;
    // Palettes go from A to F
    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
      palette = (int)key - 'A';
      break;
    default:
      break;
  }

  if (digitalRead(EFFECT_PIN_01) == HIGH) {
    effect = 1;
  }
  if (digitalRead(EFFECT_PIN_02) == HIGH) {
    effect = 2;
  }
  if (digitalRead(EFFECT_PIN_03) == HIGH) {
    effect = 0;
  }
  if (digitalRead(EFFECT_PIN_04) == HIGH) {
    effect = effect == 4 ? 0 : 4;
  }

  // Update variables from potentiometers
  ledBrightness = map(analogRead(BRIGHTNESS_PIN), 0, 1023, 0, 255);
  ledSpeed = map(log(analogRead(SPEED_PIN)) * 147, 0, 1023, 1000 / LED_MIN_SPEED, 1000 / LED_MAX_SPEED);

  // Run visual based on visual id number
  // ---------- CHANGE THESE TO CHANGE SELECTED VISUALIZATION ----------
  if (update_time + ledSpeed < millis() && effect == 0) {
    update_time = millis();
    switch (visual) {
      case 1:
        runVisualFlow(ledArray, NUM_LED, currentLEDPosition);
        break;
      case 2:
        runVisualReverseFlow();
        break;
      case 3:
        runVisualDoubleBounce();
        break;
      case 4:
        runVisualPong();
        break;
      case 5:
        runVisualPing();
        break;
      case 6:
        runVisualPingCenter();
        break;
      case 7:
        runVisualPingBlob();
        break;
      case 8:
        runVisualDoubleSpot();
        break;
      case 9:
        runVisualSegments();
        break;
      case 0:
        runVisualPulse();
        break;
      default:
        runVisualFlow();
        break;
    }
  }
  if (update_time + ledSpeed < millis() && effect != 0) {
    update_time = millis();
    switch (effect) {
      case 1:
        runEffectStrobe();
        break;
      case 2:
        runEffectSplit();
        break;
      default:
        break;
    }
  }

  // ---------- CHANGE THESE TO CHANGE SELECTED PALETTES ----------
  switch (palette) {
    case 1:
      setPaletteSynth();
      break;
    case 2:
      setPatriotPalette();
      break;
    case 3:
      setPaletteBlackWhite();
      break;
    case 4:
      setPaletteFire();
      break;
    case 5:
      setCustom();
      break;
    case 6:
      setReggaePalette();
      break;
    default:
      break;
  }

  FastLED.show();
  FastLED.delay(1);
}

// ------------ BEGIN VISUALS ------------

// Creates sets of 7 LEDs colored in order from the palette and shifts them 1 each cycle
void runVisualFlow(CRGB lightsArray, int numLights, uint8_t currentPosition) {
  if(currentPosition >= numLights) {
    currentPosition = getSafeIndexFromArray(currentPosition, numLights);
  } else {
    currentPosition++;
  }
  for(int i = 0; i < 7; i++) {
    lightsArray[getSafeIndexFromArray(currentPosition) + i] = getColorPaletteVisualFlow(i);
  }
}

CRGB getColorPaletteVisualFlow(int index) {
  switch(index) {
    case 1:
    case 2:
      return COLOR_02;
    case 3:
    case 4:
      return COLOR_03;
    case 5:
    case 6:
      return COLOR_04;
    case 0:
    default:
      return COLOR_01;
  }
}


// ------------ BEGIN EFFECTS ------------
void runEffectStrobe() {
  static uint8_t current_step = 0;

  if (current_step++ < 16 ) {
    // The effect logic goes here
    for ( int i = 0; i < NUM_LED; i += 1) {
      ledArray[i] = ColorFromPalette( currentPalette, (current_step * 16) % 64, constrain(ledBrightness * (5.0 / 3.0), 0, 255), LINEARBLEND);
    }
  } else {
    effect = 0;
    current_step = 0;
  }
}

// Sets a back drop of the "base" color 4 and moves an LED of "primary" color 1 up and down the string
void runEffectSplit() {
  // Create static var to hand current position
  static uint8_t start_position_1 = NUM_LED / 2;
  static uint8_t start_position_2 = NUM_LED / 2;

  // Update start position to "move" LEDs, changing direction when we hit the ends
  if (--start_position_1 == 0) {
    start_position_1 = NUM_LED / 2;
    start_position_2 = NUM_LED / 2;
    effect = 0;
  }
  if (++start_position_2 == NUM_LED - 1) {
    start_position_1 = NUM_LED / 2;
    start_position_2 = NUM_LED / 2;
    effect = 0;
  }

  uint8_t current_color = COLOR_01;

  // Set all LEDs to "base" color 4
  for ( int i = 0; i < NUM_LED; i += 1) {
    // Added a bit of randomness to make it less static looking
    // Mostly makes it flash a bit
    ledArray[i] = ColorFromPalette( currentPalette, COLOR_04 + random(0, 5), constrain(ledBrightness * (5.0 / 3.0), 0, 255), LINEARBLEND);
  }

  // Set lead LED to primary color
  ledArray[start_position_1] = ColorFromPalette( currentPalette, COLOR_01, constrain(ledBrightness * (5.0 / 3.0), 0, 255), LINEARBLEND);
  ledArray[start_position_2] = ColorFromPalette( currentPalette, COLOR_01, constrain(ledBrightness * (5.0 / 3.0), 0, 255), LINEARBLEND);

  // Set first trailer LED
  if (start_position_1 - 1 > 0) {
    ledArray[start_position_1 - 1] = ColorFromPalette( currentPalette, COLOR_02, constrain(ledBrightness * (5.0 / 3.0), 0, 255), LINEARBLEND);
  }
  if (start_position_2 - 1 > 0) {
    ledArray[start_position_2 - 1] = ColorFromPalette( currentPalette, COLOR_02, constrain(ledBrightness * (5.0 / 3.0), 0, 255), LINEARBLEND);
  }

  // Set the second trailer LED

  if (start_position_1 - 2 > 0) {
    ledArray[start_position_1 - 2] = ColorFromPalette( currentPalette, COLOR_03, constrain(ledBrightness * (5.0 / 3.0), 0, 255), LINEARBLEND);
  }
  if (start_position_2 - 2 > 0) {
    ledArray[start_position_2 - 2] = ColorFromPalette( currentPalette, COLOR_03, constrain(ledBrightness * (5.0 / 3.0), 0, 255), LINEARBLEND);
  }
}



// ---------- BEGIN VISUALIZERS ----------

void runVisualPulse() {

  // Create static var to hand current position
  static uint8_t start_position_1 = 0;
  static bool move_direction = true;
  static uint8_t start_position_2 = NUM_LED;
  static uint8_t start_position_indicator_1 = 0;
  static bool move_direction_indicator = true;
  static uint8_t start_position_indicator_2 = NUM_LED_INDICATOR;
  

  uint8_t m_speed = abs(start_position_1 - (NUM_LED / 2));
  m_speed = constrain(m_speed, 1, 4);

  uint8_t m_speed_indicator = abs(start_position_indicator_1 - (NUM_LED_INDICATOR / 2));
  m_speed_indicator = constrain(m_speed, 1, 4);


  // Update start position to "move" LEDs, changing direction when we hit the ends
  if (move_direction) {
    start_position_1 = constrain(start_position_1 + m_speed, 0, NUM_LED - 1);
    start_position_2 = constrain(NUM_LED - start_position_1, 0, NUM_LED - 1);
    if (start_position_1 == NUM_LED - 1) {
      move_direction = false;
    }
  } else {
    start_position_1 = constrain(start_position_1 - m_speed, 0, NUM_LED - 1);
    start_position_2 = constrain(NUM_LED - start_position_1, 0, NUM_LED - 1);
    if (start_position_1 == 0) {
      move_direction = true;
    }
  }

  if (move_direction_indicator) {
    start_position_indicator_1 = constrain(start_position_1 + m_speed_indicator, 0, NUM_LED_INDICATOR - 1);
    start_position_indicator_2 = constrain(NUM_LED_INDICATOR - start_position_1, 0, NUM_LED_INDICATOR - 1);
    if (start_position_indicator_1 == NUM_LED_INDICATOR - 1) {
      move_direction_indicator = false;
    }
  } else {
    start_position_indicator_1 = constrain(start_position_indicator_1 - m_speed_indicator, 0, NUM_LED_INDICATOR - 1);
    start_position_indicator_2 = constrain(NUM_LED_INDICATOR - start_position_indicator_1, 0, NUM_LED_INDICATOR - 1);
    if (start_position_indicator_1 == 0) {
      move_direction_indicator = true;
    }
  }

  uint8_t current_color = COLOR_01;


  for ( int i = 0; i < NUM_LED; i += 1) {

    if (start_position_1 < i && i < start_position_2) {
      ledArray[i] = ColorFromPalette( currentPalette, COLOR_01 + random(-8, 8), ledBrightness, LINEARBLEND);
    } else if (start_position_1 > i && i > start_position_2) {
      ledArray[i] = ColorFromPalette( currentPalette, COLOR_02 + random(-8, 8), ledBrightness, LINEARBLEND);
    } else {
      ledArray[i] = ColorFromPalette( currentPalette, COLOR_04 + random(0, 2), ledBrightness - (ledBrightness / 4), LINEARBLEND);
    }

  }

  
  for ( int i = 0; i < NUM_LED_INDICATOR; i += 1) {

    if (start_position_indicator_1 < i && i < start_position_indicator_2) {
      ledIndicatorArray[i] = ColorFromPalette( currentPalette, COLOR_01 + random(-8, 8), ledBrightness, LINEARBLEND);
    } else if (start_position_indicator_1 > i && i > start_position_indicator_2) {
      ledIndicatorArray[i] = ColorFromPalette( currentPalette, COLOR_02 + random(-8, 8), ledBrightness, LINEARBLEND);
    } else {
      ledIndicatorArray[i] = ColorFromPalette( currentPalette, COLOR_04 + random(0, 2), ledBrightness - (ledBrightness / 4), LINEARBLEND);
    }

  }
}

void runVisualPingFlow() {

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long inches = pulseIn(ECHO_PIN, HIGH) / 74 / 2;

  // Translate distance to a value in the ping control range
  uint8_t ledRange_new = constrain(inches, PING_MIN_RANGE, PING_MAX_RANGE);
  ledRange_new = map(ledRange_new, PING_MIN_RANGE, PING_MAX_RANGE, 0, NUM_LED);
  uint8_t ledRange_indicator_new = constrain(inches, PING_MIN_RANGE, PING_MAX_RANGE);
  ledRange_indicator_new = map(ledRange_indicator_new, PING_MIN_RANGE, PING_MAX_RANGE, 0, NUM_LED_INDICATOR);
  static uint8_t ledRange = ledRange_new;
  static uint8_t ledRange_indicator = ledRange_indicator_new;
  ledRange = (ledRange_new + ledRange) / 2;
  ledRange_indicator = (ledRange_indicator_new + ledRange_indicator)/ 2;


  Serial.print("ledRange: ");
  Serial.println(ledRange);

  static uint8_t start_color = COLOR_01;
  static uint8_t current_color = start_color;

  // The visualizer logic goes here
  for ( int i = 0; i < ledRange; i += 1) {
    ledArray[constrain(i, 0, ledRange)] = ColorFromPalette( currentPalette, current_color, ledBrightness, LINEARBLEND);
    current_color += 8;
  }
  for ( int i = 0; i < ledRange_indicator; i += 1) {
    ledIndicatorArray[constrain(i, 0, ledRange_indicator)] = ColorFromPalette( currentPalette, current_color, ledBrightness, LINEARBLEND);
    current_color += 8;
  }
  current_color = start_color;
  for ( int i = ledRange; i < NUM_LED; i += 1) {
    ledArray[i] = ColorFromPalette( currentPalette, current_color, ledBrightness, LINEARBLEND);
    current_color -= 8;
  }
  for ( int i = ledRange_indicator; i < NUM_LED_INDICATOR; i += 1) {
    ledIndicatorArray[i] = ColorFromPalette( currentPalette, current_color, ledBrightness, LINEARBLEND);
    current_color -= 8;
  }
  start_color += 16;

}

// Creates sets of 7 LEDs colored in order from the palette and shifts them 1 each cycle
void runVisualReverseFlow() {
  // Create static var to hand current position
  static uint8_t start_position = NUM_LED;

  // Update start position to "move" LEDs. In this case just a simple shift
  if (start_position < 0) {
    start_position = getSafeIndex(start_position);
  } else {
    start_position -= 1;
  }
  uint8_t current_color = COLOR_01;

  int j = 0;
  // The visualizer logic goes here
  for ( int i = 0; i < NUM_LED; i += 7) {
    ledArray[getSafeIndex(i + start_position)] = ColorFromPalette( currentPalette, COLOR_01, ledBrightness, LINEARBLEND);
    ledArray[getSafeIndex(i + start_position + 1)] = ColorFromPalette( currentPalette, COLOR_02, ledBrightness, LINEARBLEND);
    ledArray[getSafeIndex(i + start_position + 2)] = ColorFromPalette( currentPalette, COLOR_02, ledBrightness, LINEARBLEND);
    ledArray[getSafeIndex(i + start_position + 3)] = ColorFromPalette( currentPalette, COLOR_03, ledBrightness, LINEARBLEND);
    ledArray[getSafeIndex(i + start_position + 4)] = ColorFromPalette( currentPalette, COLOR_03, ledBrightness, LINEARBLEND);
    ledArray[getSafeIndex(i + start_position + 5)] = ColorFromPalette( currentPalette, COLOR_04, ledBrightness, LINEARBLEND);
    ledArray[getSafeIndex(i + start_position + 6)] = ColorFromPalette( currentPalette, COLOR_04, ledBrightness, LINEARBLEND);

    if(DISPLAY_INDICATOR == true) {
      ledIndicatorArray[(j + start_position) % NUM_LED_INDICATOR] = ColorFromPalette(currentPalette, COLOR_01 , ledBrightness, LINEARBLEND);
      ledIndicatorArray[(j + 1 + start_position) % NUM_LED_INDICATOR] = ColorFromPalette(currentPalette, COLOR_02 , ledBrightness, LINEARBLEND);
      ledIndicatorArray[(j + 2 + start_position) % NUM_LED_INDICATOR] = ColorFromPalette(currentPalette, COLOR_02 , ledBrightness, LINEARBLEND);
      ledIndicatorArray[(j + 3 + start_position) % NUM_LED_INDICATOR] = ColorFromPalette(currentPalette, COLOR_03 , ledBrightness, LINEARBLEND);
      ledIndicatorArray[(j + 4 + start_position) % NUM_LED_INDICATOR] = ColorFromPalette(currentPalette, COLOR_03 , ledBrightness, LINEARBLEND);
      ledIndicatorArray[(j + 5 + start_position) % NUM_LED_INDICATOR] = ColorFromPalette(currentPalette, COLOR_04 , ledBrightness, LINEARBLEND);
      j += 1;
    }
  }
}
// Creates sets of 7 LEDs colored in order from the palette and shifts them 1 each cycle
void runVisualCenterFlow() {

  // Create static var to hand current position
  static uint8_t start_position_01 = NUM_LED / 2;
  static uint8_t start_position_02 = NUM_LED / 2;

  // Update start position to "move" LEDs. In this case just a simple shift
  if (start_position_01 > NUM_LED) {
    start_position_01 = getSafeIndexFirstHalf(start_position_01);
  } else {
    start_position_01 -= 1;
  }
  if (start_position_02 > NUM_LED) {
    start_position_02 = getSafeIndexSecondHalf(start_position_02);
  } else {
    start_position_02 += 1;
  }
  uint8_t current_color = COLOR_01;

  // The visualizer logic goes here
  for ( int i = 0; i < NUM_LED / 2; i += 7) {
    ledArray[getSafeIndexFirstHalf(i + start_position_01)] = ColorFromPalette( currentPalette, COLOR_01, ledBrightness, LINEARBLEND);
    ledArray[getSafeIndexFirstHalf(i + start_position_01 + 1)] = ColorFromPalette( currentPalette, COLOR_02, ledBrightness, LINEARBLEND);
    ledArray[getSafeIndexFirstHalf(i + start_position_01 + 2)] = ColorFromPalette( currentPalette, COLOR_02, ledBrightness, LINEARBLEND);
    ledArray[getSafeIndexFirstHalf(i + start_position_01 + 3)] = ColorFromPalette( currentPalette, COLOR_03, ledBrightness, LINEARBLEND);
    ledArray[getSafeIndexFirstHalf(i + start_position_01 + 4)] = ColorFromPalette( currentPalette, COLOR_03, ledBrightness, LINEARBLEND);
    ledArray[getSafeIndexFirstHalf(i + start_position_01 + 5)] = ColorFromPalette( currentPalette, COLOR_04, ledBrightness, LINEARBLEND);
    ledArray[getSafeIndexFirstHalf(i + start_position_01 + 6)] = ColorFromPalette( currentPalette, COLOR_04, ledBrightness, LINEARBLEND);
  }
  // The visualizer logic goes here
  for ( int i = NUM_LED / 2; i < NUM_LED; i += 7) {
    ledArray[getSafeIndexSecondHalf(i + start_position_02)] = ColorFromPalette( currentPalette, COLOR_01, ledBrightness, LINEARBLEND);
    ledArray[getSafeIndexSecondHalf(i + start_position_02 + 1)] = ColorFromPalette( currentPalette, COLOR_02, ledBrightness, LINEARBLEND);
    ledArray[getSafeIndexSecondHalf(i + start_position_02 + 2)] = ColorFromPalette( currentPalette, COLOR_02, ledBrightness, LINEARBLEND);
    ledArray[getSafeIndexSecondHalf(i + start_position_02 + 3)] = ColorFromPalette( currentPalette, COLOR_03, ledBrightness, LINEARBLEND);
    ledArray[getSafeIndexSecondHalf(i + start_position_02 + 4)] = ColorFromPalette( currentPalette, COLOR_03, ledBrightness, LINEARBLEND);
    ledArray[getSafeIndexSecondHalf(i + start_position_02 + 5)] = ColorFromPalette( currentPalette, COLOR_04, ledBrightness, LINEARBLEND);
    ledArray[getSafeIndexSecondHalf(i + start_position_02 + 6)] = ColorFromPalette( currentPalette, COLOR_04, ledBrightness, LINEARBLEND);
  }
}

// Creates sets of 7 LEDs colored in order from the palette and shifts them 1 each cycle
void runVisualDoubleSpot() {
  // Create static var to hand current status
  static uint8_t current_color = 0;
  static uint8_t glow_pos_01 = NUM_LED / 2 - 1;
  static uint8_t glow_step_01 = 0;
  static uint8_t glow_pos_02 = NUM_LED / 2 + 1;
  static uint8_t glow_step_02 = 0;

  // Shift the background between the palette colors smoothly
  for ( int i = 0; i < NUM_LED; i += 1) {
    ledArray[i] = ColorFromPalette( currentPalette, current_color, ledBrightness / 2, LINEARBLEND);
    if(DISPLAY_INDICATOR == true) {
      ledIndicatorArray[i % NUM_LED_INDICATOR] = ColorFromPalette(currentPalette, current_color, ledBrightness / 2, LINEARBLEND);
    }
  }
  current_color += 1;
  uint8_t complement_color = current_color + 16;
  /*
    Serial.print("Current Color: ");
    Serial.print(current_color);
    Serial.print(" Glow Pos: ");
    Serial.print(glow_pos_01);
    Serial.print(" Glow Step: ");
    Serial.print(glow_step_01);
    Serial.print(" Complement Color: ");
    Serial.println(complement_color);
  */

  // Color a section with the "glow" spot
  switch (glow_step_01++) {
    case 0:
      glow_pos_01 = random(0, NUM_LED / 2 - 1); // Random location
      ledArray[glow_pos_01] = ColorFromPalette( currentPalette, complement_color, ledBrightness / 2, LINEARBLEND);
      if(DISPLAY_INDICATOR) {
       ledIndicatorArray[glow_pos_01 % NUM_LED_INDICATOR] = ColorFromPalette(currentPalette, complement_color, ledBrightness / 2, LINEARBLEND); 
      }
      break;
    case 1:
      ledArray[glow_pos_01] = ColorFromPalette( currentPalette, complement_color, ledBrightness, LINEARBLEND);
      ledArray[constrain(glow_pos_01 + 1, 0, NUM_LED - 1)] = ColorFromPalette( currentPalette, complement_color, ledBrightness / 2, LINEARBLEND);
      ledArray[constrain(glow_pos_01 + 1, 0, NUM_LED - 1)] = ColorFromPalette( currentPalette, complement_color, ledBrightness / 2, LINEARBLEND);
      if(DISPLAY_INDICATOR) {
        ledIndicatorArray[glow_pos_01 % NUM_LED_INDICATOR] = ColorFromPalette( currentPalette, complement_color, ledBrightness, LINEARBLEND);
        ledIndicatorArray[constrain((glow_pos_01 + 1) % NUM_LED_INDICATOR, 0, NUM_LED - 1)] = ColorFromPalette( currentPalette, complement_color, ledBrightness / 2, LINEARBLEND);
        ledIndicatorArray[constrain((glow_pos_01 + 1) % NUM_LED_INDICATOR, 0, NUM_LED - 1)] = ColorFromPalette( currentPalette, complement_color, ledBrightness / 2, LINEARBLEND);
      }
      break;
    case 2:
      ledArray[glow_pos_01] = ColorFromPalette( currentPalette, complement_color, ledBrightness, LINEARBLEND);
      ledArray[constrain(glow_pos_01 + 1, 0, NUM_LED - 1)] = ColorFromPalette( currentPalette, complement_color, ledBrightness, LINEARBLEND);
      ledArray[constrain(glow_pos_01 + 2, 0, NUM_LED - 1)] = ColorFromPalette( currentPalette, complement_color, ledBrightness / 2, LINEARBLEND);
      ledArray[constrain(glow_pos_01 - 1, 0, NUM_LED - 1)] = ColorFromPalette( currentPalette, complement_color, ledBrightness, LINEARBLEND);
      ledArray[constrain(glow_pos_01 - 2, 0, NUM_LED - 1)] = ColorFromPalette( currentPalette, complement_color, ledBrightness / 2, LINEARBLEND);
      if(DISPLAY_INDICATOR) {
        ledIndicatorArray[glow_pos_01 % NUM_LED_INDICATOR] = ColorFromPalette( currentPalette, complement_color, ledBrightness, LINEARBLEND);
        ledIndicatorArray[constrain((glow_pos_01 + 1) % NUM_LED_INDICATOR, 0, NUM_LED - 1)] = ColorFromPalette( currentPalette, complement_color, ledBrightness, LINEARBLEND);
        ledIndicatorArray[constrain((glow_pos_01 + 2) % NUM_LED_INDICATOR, 0, NUM_LED - 1)] = ColorFromPalette( currentPalette, complement_color, ledBrightness / 2, LINEARBLEND);
        ledIndicatorArray[constrain((glow_pos_01 - 1) % NUM_LED_INDICATOR, 0, NUM_LED - 1)] = ColorFromPalette( currentPalette, complement_color, ledBrightness, LINEARBLEND);
        ledIndicatorArray[constrain((glow_pos_01 - 2) % NUM_LED_INDICATOR, 0, NUM_LED - 1)] = ColorFromPalette( currentPalette, complement_color, ledBrightness / 2, LINEARBLEND);  
      }
      break;
    case 3:
      ledArray[glow_pos_01] = ColorFromPalette( currentPalette, complement_color, ledBrightness, LINEARBLEND);
      ledArray[constrain(glow_pos_01 + 1, 0, NUM_LED - 1)] = ColorFromPalette( currentPalette, complement_color, ledBrightness / 2, LINEARBLEND);
      ledArray[constrain(glow_pos_01 - 1, 0, NUM_LED - 1)] = ColorFromPalette( currentPalette, complement_color, ledBrightness / 2, LINEARBLEND);
      if(DISPLAY_INDICATOR) {
        ledIndicatorArray[glow_pos_01 % NUM_LED_INDICATOR] = ColorFromPalette( currentPalette, complement_color, ledBrightness, LINEARBLEND);
        ledArray[constrain((glow_pos_01 + 1) % NUM_LED_INDICATOR, 0, NUM_LED - 1)] = ColorFromPalette( currentPalette, complement_color, ledBrightness / 2, LINEARBLEND);
        ledArray[constrain((glow_pos_01 - 1) % NUM_LED_INDICATOR, 0, NUM_LED - 1)] = ColorFromPalette( currentPalette, complement_color, ledBrightness / 2, LINEARBLEND);
      }
      break;
    case 4:
      ledArray[glow_pos_01] = ColorFromPalette( currentPalette, complement_color, ledBrightness / 2, LINEARBLEND);
      if(DISPLAY_INDICATOR) {
        ledIndicatorArray[glow_pos_01 % NUM_LED_INDICATOR] = ColorFromPalette( currentPalette, complement_color, ledBrightness / 2, LINEARBLEND);
      }
      glow_step_01 = 0;
      break;
  }
  switch (glow_step_02++) {
    case 0:
      glow_pos_02 = random(NUM_LED / 2 + 1, NUM_LED); // Random location
      ledArray[glow_pos_02] = ColorFromPalette( currentPalette, complement_color + 8, ledBrightness / 2, LINEARBLEND);
      break;
    case 1:
      ledArray[glow_pos_02] = ColorFromPalette( currentPalette, complement_color + 8, ledBrightness, LINEARBLEND);
      ledArray[constrain(glow_pos_02 + 1, 0, NUM_LED - 1)] = ColorFromPalette( currentPalette, complement_color + 8, ledBrightness / 2, LINEARBLEND);
      ledArray[constrain(glow_pos_02 + 1, 0, NUM_LED - 1)] = ColorFromPalette( currentPalette, complement_color + 8, ledBrightness / 2, LINEARBLEND);
      break;
    case 2:
      ledArray[glow_pos_02] = ColorFromPalette( currentPalette, complement_color + 8, ledBrightness, LINEARBLEND);
      ledArray[constrain(glow_pos_02 + 1, 0, NUM_LED - 1)] = ColorFromPalette( currentPalette, complement_color + 8, ledBrightness, LINEARBLEND);
      ledArray[constrain(glow_pos_02 + 2, 0, NUM_LED - 1)] = ColorFromPalette( currentPalette, complement_color + 8, ledBrightness / 2, LINEARBLEND);
      ledArray[constrain(glow_pos_02 - 1, 0, NUM_LED - 1)] = ColorFromPalette( currentPalette, complement_color + 8, ledBrightness, LINEARBLEND);
      ledArray[constrain(glow_pos_02 - 2, 0, NUM_LED - 1)] = ColorFromPalette( currentPalette, complement_color + 8, ledBrightness / 2, LINEARBLEND);
      break;
    case 3:
      ledArray[glow_pos_02] = ColorFromPalette( currentPalette, complement_color + 8, ledBrightness, LINEARBLEND);
      ledArray[constrain(glow_pos_02 + 1, 0, NUM_LED - 1)] = ColorFromPalette( currentPalette, complement_color + 8, ledBrightness / 2, LINEARBLEND);
      ledArray[constrain(glow_pos_02 - 1, 0, NUM_LED - 1)] = ColorFromPalette( currentPalette, complement_color + 8, ledBrightness / 2, LINEARBLEND);
      break;
    case 4:
      ledArray[glow_pos_02] = ColorFromPalette( currentPalette, complement_color + 8, ledBrightness / 2, LINEARBLEND);
      glow_step_02 = 0;
      break;
  }

}


// Sets a back drop of the "base" color 4 and moves an LED of "primary" color 1 up and down the string
void runVisualBounce() {
  // Create static var to hand current position
  static uint8_t start_position = 0;
  static bool move_direction = true;
  static bool move_direction_indicator = true;
  static uint8_t start_position_indicator = 0;

  // Update start position to "move" LEDs, changing direction when we hit the ends
  if (move_direction) {
    if (++start_position == NUM_LED - 1) {
      move_direction = false;
    }
  } else {
    if (--start_position == 0) {
      move_direction = true;
    }
  }

  if (move_direction_indicator) {
    if (++start_position_indicator == NUM_LED_INDICATOR - 1) {
      move_direction_indicator = false;
    }
  } else {
    if (--start_position_indicator == 0) {
      move_direction_indicator = true;
    }
  }

  uint8_t current_color = COLOR_01;

  // Set all LEDs to "base" color 4
  uint8_t base_color = COLOR_04 + random(0, 5);
  for ( int i = 0; i < NUM_LED; i += 1) {
    // Added a bit of randomness to make it less static looking
    // Mostly makes it flash a bit   
    ledArray[i] = ColorFromPalette( currentPalette, current_color, ledBrightness, LINEARBLEND);
  }

  if(DISPLAY_INDICATOR) {
    for(int i = 0; i < NUM_LED_INDICATOR; i++) {
      ledIndicatorArray[i] = ColorFromPalette( currentPalette, base_color, ledBrightness, LINEARBLEND);
    }
  }

  // Set lead LED to primary color
  ledArray[start_position] = ColorFromPalette( currentPalette, COLOR_01, ledBrightness, LINEARBLEND);
  if(false) {
    ledIndicatorArray[start_position_indicator] = ColorFromPalette( currentPalette, COLOR_01, ledBrightness, LINEARBLEND);
  }

  // Set first trailer LED
  if (move_direction) {
    if (start_position - 1 > 0) {
      ledArray[start_position - 1] = ColorFromPalette( currentPalette, COLOR_02, ledBrightness, LINEARBLEND);
    }
  } else {
    if (start_position + 1 < NUM_LED) {
      ledArray[start_position + 1] = ColorFromPalette( currentPalette, COLOR_02, ledBrightness, LINEARBLEND);
    }
  }

  if(false) {
    if(move_direction_indicator) {
       if (start_position_indicator - 1 > 0) {
        ledIndicatorArray[start_position_indicator - 1] = ColorFromPalette( currentPalette, COLOR_02, ledBrightness, LINEARBLEND);
      }
    } else {
      if (start_position_indicator + 1 < NUM_LED_INDICATOR) {
        ledIndicatorArray[start_position_indicator + 1] = ColorFromPalette( currentPalette, COLOR_02, ledBrightness, LINEARBLEND);
      }
    }
   }

  // Set the second trailer LED
  if (move_direction) {
    if (start_position - 2 > 0) {
      ledArray[start_position - 2] = ColorFromPalette( currentPalette, COLOR_03, ledBrightness, LINEARBLEND);
    }
  } else {
    if (start_position + 2 < NUM_LED) {
      ledArray[start_position + 2] = ColorFromPalette( currentPalette, COLOR_03, ledBrightness, LINEARBLEND);
    }
  }

    if(false) {
      if(move_direction_indicator) {
         if (start_position_indicator - 2 > 0) {
          ledIndicatorArray[start_position_indicator - 2] = ColorFromPalette( currentPalette, COLOR_03, ledBrightness, LINEARBLEND);
        }
      } else {
        if (start_position_indicator + 2 < NUM_LED_INDICATOR) {
          ledIndicatorArray[start_position_indicator + 2] = ColorFromPalette( currentPalette, COLOR_03, ledBrightness, LINEARBLEND);
        }
      }
   }
   
}

// Sets a back drop of the "base" color 4 and moves an LED of "primary" color 1 up and down the string
void runVisualDoubleBounce() {
  // Create static var to hand current position
  static uint8_t start_position_1 = 0;
  static bool move_direction_1 = true;
  static uint8_t start_position_2 = NUM_LED;
  static bool move_direction_2 = false;

  static uint8_t start_position_indicator_1 = 0;
  static bool move_direction_indicator_1 = true;

  // Update start position to "move" LEDs, changing direction when we hit the ends
  if (move_direction_1) {
    if (++start_position_1 == NUM_LED - 1) {
      move_direction_1 = false;
    }
  } else {
    if (--start_position_1 == 0) {
      move_direction_1 = true;
    }
  }
  if (move_direction_indicator_1) {
    if (++start_position_indicator_1 == NUM_LED_INDICATOR - 1) {
      move_direction_indicator_1 = false;
    }
  } else {
    if (--start_position_indicator_1 == 0) {
      move_direction_indicator_1 = true;
    }
  }

  uint8_t current_color = COLOR_01;

  // Set all LEDs to "base" color 4
  for ( int i = 0; i < NUM_LED; i += 1) {
    // Added a bit of randomness to make it less static looking
    // Mostly makes it flash a bit
    uint8_t base_color = random(0,5) + COLOR_04;
    ledArray[i] = ColorFromPalette( currentPalette, base_color, ledBrightness, LINEARBLEND);
    ledIndicatorArray[i % NUM_LED_INDICATOR] = ColorFromPalette( currentPalette, base_color, ledBrightness, LINEARBLEND);
  }

  // Set lead LED to primary color
  ledArray[start_position_1] = ColorFromPalette( currentPalette, COLOR_01, ledBrightness, LINEARBLEND);
  ledArray[start_position_2] = ColorFromPalette( currentPalette, COLOR_01, ledBrightness, LINEARBLEND);

  ledIndicatorArray[start_position_indicator_1] = ColorFromPalette( currentPalette, COLOR_01, ledBrightness, LINEARBLEND);

  // Set first trailer LED
  if (move_direction_1) {
    if (start_position_1 - 1 > 0) {
      ledArray[start_position_1 - 1] = ColorFromPalette( currentPalette, COLOR_02, ledBrightness, LINEARBLEND);
    }
  } else {
    if (start_position_1 + 1 < NUM_LED) {
      ledArray[start_position_1 + 1] = ColorFromPalette( currentPalette, COLOR_02, ledBrightness, LINEARBLEND);
    }
  }
  if (move_direction_indicator_1) {
    if (start_position_indicator_1 - 1 > 0) {
      ledIndicatorArray[start_position_indicator_1 - 1] = ColorFromPalette( currentPalette, COLOR_02, ledBrightness, LINEARBLEND);
    }
  } else {
    if (start_position_indicator_1 + 1 < NUM_LED_INDICATOR) {
      ledIndicatorArray[start_position_indicator_1 + 1] = ColorFromPalette( currentPalette, COLOR_02, ledBrightness, LINEARBLEND);
    }
  }
  
  if (move_direction_2) {
    if (start_position_2 - 1 > 0) {
      ledArray[start_position_2 - 1] = ColorFromPalette( currentPalette, COLOR_02, ledBrightness, LINEARBLEND);
    }
  } else {
    if (start_position_2 + 1 < NUM_LED) {
      ledArray[start_position_2 + 1] = ColorFromPalette( currentPalette, COLOR_02, ledBrightness, LINEARBLEND);
    }
  }

  // Set the second trailer LED
  if (move_direction_1) {
    if (start_position_1 - 2 > 0) {
      ledArray[start_position_1 - 2] = ColorFromPalette( currentPalette, COLOR_03, ledBrightness, LINEARBLEND);
    }
  } else {
    if (start_position_1 + 2 < NUM_LED) {
      ledArray[start_position_1 + 2] = ColorFromPalette( currentPalette, COLOR_03, ledBrightness, LINEARBLEND);
    }
  }
  if (move_direction_indicator_1) {
    if (start_position_indicator_1 - 2 > 0) {
      ledIndicatorArray[start_position_indicator_1 - 2] = ColorFromPalette( currentPalette, COLOR_03, ledBrightness, LINEARBLEND);
    }
  } else {
    if (start_position_indicator_1 + 2 < NUM_LED_INDICATOR) {
      ledIndicatorArray[start_position_indicator_1 + 2] = ColorFromPalette( currentPalette, COLOR_03, ledBrightness, LINEARBLEND);
    }
  }
  if (move_direction_2) {
    if (start_position_2 - 2 > 0) {
      ledArray[start_position_2 - 2] = ColorFromPalette( currentPalette, COLOR_03, ledBrightness, LINEARBLEND);
    }
  } else {
    if (start_position_2 + 2 < NUM_LED) {
      ledArray[start_position_2 + 2] = ColorFromPalette( currentPalette, COLOR_03, ledBrightness, LINEARBLEND);
    }
  }
}

void runVisualPong() {
  // Create static var to hand current position
  static uint8_t start_position = 0;
  static bool move_direction = true;

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long inches = pulseIn(ECHO_PIN, HIGH) / 74 / 2;
  Serial.print("Distance: ");
  Serial.println(inches);

  // Translate distance to a value in the ping control range
  uint8_t ledRange = constrain(inches, PING_MIN_RANGE, PING_MAX_RANGE);
  ledRange = map(ledRange, PING_MIN_RANGE, PING_MAX_RANGE, 0, NUM_LED);

  // Update start position to "move" LEDs, changing direction when we hit the ends
  if (move_direction) {
    if (++start_position == NUM_LED - 1 || start_position > ledRange) {
      move_direction = false;
    }
  } else {
    if (--start_position == 0) {
      move_direction = true;
    }
  }

  uint8_t current_color = COLOR_01;

  // Set all LEDs to "base" color 4
  for ( int i = 0; i < NUM_LED; i += 1) {
    // Added a bit of randomness to make it less static looking
    // Mostly makes it flash a bit
    ledArray[i] = ColorFromPalette( currentPalette, COLOR_04 + random(0, 5), ledBrightness, LINEARBLEND);
  }

  // Set lead LED to primary color
  ledArray[start_position] = ColorFromPalette( currentPalette, COLOR_01, ledBrightness, LINEARBLEND);

  // Set first trailer LED
  if (move_direction) {
    if (start_position - 1 > 0) {
      ledArray[start_position - 1] = ColorFromPalette( currentPalette, COLOR_02, ledBrightness, LINEARBLEND);
    }
  } else {
    if (start_position + 1 < NUM_LED) {
      ledArray[start_position + 1] = ColorFromPalette( currentPalette, COLOR_02, ledBrightness, LINEARBLEND);
    }
  }

  // Set the second trailer LED
  if (move_direction) {
    if (start_position - 2 > 0) {
      ledArray[start_position - 2] = ColorFromPalette( currentPalette, COLOR_03, ledBrightness, LINEARBLEND);
    }
  } else {
    if (start_position + 2 < NUM_LED) {
      ledArray[start_position + 2] = ColorFromPalette( currentPalette, COLOR_03, ledBrightness, LINEARBLEND);
    }
  }
}

// Creates segments of 6 LEDs color from the palette randomly and updates them (and their position) every cycle
void runVisualSegments() {

  // Create random start position to begin our segments at so they don't always appear in the same place
  uint8_t start_pos = random(0, NUM_LED + 1);

  for ( int i = 0; i < NUM_LED; i += 6) {
    // Pick a random color from our color palette
    // Remember we are effectively limiting it to 4 colors, which have a gap of 16
    uint8_t current_color_offset = random(0, 4) * 16;

    ledArray[getSafeIndex(i + start_pos)] = ColorFromPalette( currentPalette, current_color_offset, ledBrightness, LINEARBLEND);
    ledArray[getSafeIndex(i + start_pos + 1)] = ColorFromPalette( currentPalette, current_color_offset, ledBrightness, LINEARBLEND);
    ledArray[getSafeIndex(i + start_pos + 2)] = ColorFromPalette( currentPalette, current_color_offset, ledBrightness, LINEARBLEND);
    ledArray[getSafeIndex(i + start_pos + 3)] = ColorFromPalette( currentPalette, current_color_offset, ledBrightness, LINEARBLEND);
    ledArray[getSafeIndex(i + start_pos + 4)] = ColorFromPalette( currentPalette, current_color_offset, ledBrightness, LINEARBLEND);
    ledArray[getSafeIndex(i + start_pos + 5)] = ColorFromPalette( currentPalette, current_color_offset, ledBrightness, LINEARBLEND);

  }
}

// Sets the "base" color to palette color 4 and lights up all LEDs lower than the ping distance to the "primary" color 1
void runVisualPing() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long inches = pulseIn(ECHO_PIN, HIGH) / 74 / 2;
  Serial.print("Distance: ");
  Serial.println(inches);

  // Translate distance to a value in the ping control range
  uint8_t ledRange = constrain(inches, PING_MIN_RANGE, PING_MAX_RANGE);
  uint8_t ledRange_indicator = map(ledRange, PING_MIN_RANGE, PING_MAX_RANGE, 0, NUM_LED_INDICATOR);
  ledRange = map(ledRange, PING_MIN_RANGE, PING_MAX_RANGE, 0, NUM_LED);


  for ( int i = 0; i < NUM_LED; i++) {

    if (i < ledRange) {
      ledArray[i] = ColorFromPalette( currentPalette, COLOR_01, ledBrightness, LINEARBLEND);
    } else {
      ledArray[i] = ColorFromPalette( currentPalette, COLOR_04, ledBrightness, LINEARBLEND);
    }
  }

  for ( int i = 0; i < NUM_LED_INDICATOR; i++) {

    if (i < ledRange_indicator) {
      ledIndicatorArray[i] = ColorFromPalette( currentPalette, COLOR_01, ledBrightness, LINEARBLEND);
    } else {
      ledIndicatorArray[i] = ColorFromPalette( currentPalette, COLOR_04, ledBrightness, LINEARBLEND);
    }
  }
}
// Sets the "base" color to palette color 4 and lights up all LEDs lower than the ping distance to the "primary" color 1
void runVisualPingCenter() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long inches = pulseIn(ECHO_PIN, HIGH) / 74 / 2;
  Serial.print("Distance: ");
  Serial.println(inches);

  // Translate distance to a value in the ping control range
  uint8_t ledRange = constrain(inches, PING_MIN_RANGE, PING_MAX_RANGE);
  uint8_t ledRange_indicator = map(ledRange, PING_MIN_RANGE, PING_MAX_RANGE, 0, NUM_LED_INDICATOR / 2);
  ledRange = map(ledRange, PING_MIN_RANGE, PING_MAX_RANGE, 0, NUM_LED / 2);

  ledArray[0] = ColorFromPalette( currentPalette, COLOR_04, ledBrightness, LINEARBLEND);
  ledIndicatorArray[0]= ColorFromPalette( currentPalette, COLOR_04, ledBrightness, LINEARBLEND);

  for ( int i = 0; i < NUM_LED / 2; i++) {

    if (i < ledRange) {
      ledArray[NUM_LED / 2 + i] = ColorFromPalette( currentPalette, COLOR_01, ledBrightness, LINEARBLEND);
      ledArray[NUM_LED / 2 - i] = ColorFromPalette( currentPalette, COLOR_01, ledBrightness, LINEARBLEND);
    } else {
      ledArray[NUM_LED / 2 + i] = ColorFromPalette( currentPalette, COLOR_04, ledBrightness, LINEARBLEND);
      ledArray[NUM_LED / 2 - i] = ColorFromPalette( currentPalette, COLOR_04, ledBrightness, LINEARBLEND);
    }
  }

  for ( int i = 0; i < NUM_LED_INDICATOR / 2; i++) {

    if (i < ledRange_indicator) {
      ledIndicatorArray[NUM_LED_INDICATOR / 2 + i] = ColorFromPalette( currentPalette, COLOR_01, ledBrightness, LINEARBLEND);
      ledIndicatorArray[NUM_LED_INDICATOR / 2 - i] = ColorFromPalette( currentPalette, COLOR_01, ledBrightness, LINEARBLEND);
    } else {
      ledIndicatorArray[NUM_LED_INDICATOR / 2 + i] = ColorFromPalette( currentPalette, COLOR_04, ledBrightness, LINEARBLEND);
      ledIndicatorArray[NUM_LED_INDICATOR / 2 - i] = ColorFromPalette( currentPalette, COLOR_04, ledBrightness, LINEARBLEND);
    }
  }
  
}

// Sets the "base" color to palette color 4 and light up 5 LEDs around the location of the
void runVisualPingBlob() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long inches = pulseIn(ECHO_PIN, HIGH) / 74 / 2;
  Serial.print("Distance: ");
  Serial.println(inches);

  // Translate distance to a value in the ping control range
  uint8_t ledRange = constrain(inches, PING_MIN_RANGE, PING_MAX_RANGE);
  uint8_t ledRange_indicator = map(ledRange, PING_MIN_RANGE, PING_MAX_RANGE, 0, NUM_LED_INDICATOR);
  ledRange = map(ledRange, PING_MIN_RANGE, PING_MAX_RANGE, 0, NUM_LED);

  for ( int i = 0; i < NUM_LED; i++) {
    if (i <= ledRange + 2 && i >= ledRange - 2) {
      ledArray[i] = ColorFromPalette( currentPalette, COLOR_01, ledBrightness, LINEARBLEND);
    } else {
      ledArray[i] = ColorFromPalette( currentPalette, COLOR_04, ledBrightness, LINEARBLEND);
    }
  }
  for ( int i = 0; i < NUM_LED_INDICATOR; i++) {
    if (i <= ledRange_indicator + 2 && i >= ledRange_indicator - 2) {
      ledIndicatorArray[i] = ColorFromPalette( currentPalette, COLOR_01, ledBrightness, LINEARBLEND);
    } else {
      ledIndicatorArray[i] = ColorFromPalette( currentPalette, COLOR_04, ledBrightness, LINEARBLEND);
    }
  }
}

// Gets an index we can safely assign to the led array, makes overflow "wrap" around
uint8_t getSafeIndex(uint8_t index) {
  return (index % NUM_LED);
}

uint8_t getSafeIndexFromArray(uint8_t index, int numLength) {
  return (index % numLength);
}

// Gets an index we can safely assign to the led array, makes overflow "wrap" around
uint8_t getSafeIndexFirstHalf(uint8_t index) {
  return (index % (NUM_LED / 2));
}
// Gets an index we can safely assign to the led array, makes overflow "wrap" around
uint8_t getSafeIndexSecondHalf(uint8_t index) {
  return ((index - NUM_LED / 2) % (NUM_LED / 2)) + NUM_LED / 2;
}
// ---------- END VISUALIZERS ----------


// ---------- BEGIN PALETTES ----------

void setPaletteFire()
{
  CRGB color4 = CRGB(180, 0, 0);
  CRGB color3 = CRGB(255, 77, 0);
  CRGB color2 = CRGB(255, 116, 0);
  CRGB color1 = CRGB(255, 154, 0);

  currentPalette = CRGBPalette16(
                     color1, color2, color3, color4,
                     color1, color2, color3, color4,
                     color1, color2, color3, color4,
                     color1, color2, color3, color4);
}
void setPaletteSynth()
{
  CRGB color1 = CRGB(212, 0, 120);
  CRGB color2 = CRGB(146, 0, 117);
  CRGB color3 = CRGB(20, 75, 156);
  CRGB color4 = CRGB(0, 0, 0);

  currentPalette = CRGBPalette16(
                     color1, color2, color3, color4,
                     color1, color2, color3, color4,
                     color1, color2, color3, color4,
                     color1, color2, color3, color4);
}
void setPaletteParty()
{
  CRGB color1 = CRGB(255, 0, 102);
  CRGB color2 = CRGB(255, 235, 0);
  CRGB color3 = CRGB(0, 152, 195);
  CRGB color4 = CRGB(0, 220, 110);

  currentPalette = CRGBPalette16(
                     color1, color2, color3, color4,
                     color1, color2, color3, color4,
                     color1, color2, color3, color4,
                     color1, color2, color3, color4);
}
void setPaletteBlackWhite()
{
  CRGB color1 = CRGB(255, 255, 255);
  CRGB color2 = CRGB(200, 200, 200);
  CRGB color3 = CRGB(150, 150, 150);
  CRGB color4 = CRGB(0, 0, 0);

  currentPalette = CRGBPalette16(
                     color1, color2, color3, color4,
                     color1, color2, color3, color4,
                     color1, color2, color3, color4,
                     color1, color2, color3, color4);
}

void setChloePalette()
{
  CRGB color1 = CRGB(148, 0, 211);  //148, 0, 211
  CRGB color2 = CRGB(255, 105, 180);
  CRGB color3 = CRGB(160, 32, 240);
  CRGB color4 = CRGB(255, 69, 0); //255, 69, 0

  currentPalette = CRGBPalette16(
                     color1, color2, color3, color4,
                     color1, color2, color3, color4,
                     color1, color2, color3, color4,
                     color1, color2, color3, color4);
}


void setReggaePalette()
{
  CRGB color1 = CRGB(255, 0, 0);  //148, 0, 211
  CRGB color2 = CRGB(255, 215, 0);
  CRGB color3 = CRGB(255, 255, 0);
  CRGB color4 = CRGB(0, 128, 0); //255, 69, 0

  currentPalette = CRGBPalette16(
                     color1, color2, color3, color4,
                     color1, color2, color3, color4,
                     color1, color2, color3, color4,
                     color1, color2, color3, color4);
}

void setPatriotPalette()
{
  CRGB color1 = CRGB(0, 191, 255);  //148, 0, 211
  CRGB color2 = CRGB(255, 0, 0);
  CRGB color3 = CRGB(0, 0, 255);
  CRGB color4 = CRGB(0, 0, 0); //255, 69, 0

  currentPalette = CRGBPalette16(
                     color1, color2, color3, color4,
                     color1, color2, color3, color4,
                     color1, color2, color3, color4,
                     color1, color2, color3, color4);
}

void setCustom()
{
  CRGB color1 = CRGB(0, 128, 128);  //148, 0, 211
  CRGB color2 = CRGB(222, 93, 131);
  CRGB color3 = CRGB(64, 224, 208);
  CRGB color4 = CRGB(141, 0, 211); //255, 69, 0

  currentPalette = CRGBPalette16(
                     color1, color2, color3, color4,
                     color1, color2, color3, color4,
                     color1, color2, color3, color4,
                     color1, color2, color3, color4);
}// ---------- END PALETTES ----------
