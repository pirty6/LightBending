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

//Declaration of the key of the keypad
char hexKeys[PAD_ROWS][PAD_COLUMNS] =
{
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'0', 'F', 'E', 'D'}
};
//Assignement of pin for the keypad
byte rows_pin[PAD_ROWS] = {2, 3, 4, 5};
byte columns_pin[PAD_COLUMNS] = {6, 7, 8, 9};

Keypad clavier = Keypad( makeKeymap(hexKeys), rows_pin, columns_pin, PAD_ROWS, PAD_COLUMNS); // creation of object keypad
//                                               //
// ---------- END CONFIGURABLE SETTINGS ----------

// Define color offsets
// Basically we are taking the 16 color pallete and reducing it to 4
#define COLOR_01 0
#define COLOR_02 16
#define COLOR_03 32
#define COLOR_04 48

// GLOBAL VARIABLES

CRGB leds[NUM_LED];
CRGB leds_indicator[NUM_LED_INDICATOR];
CRGBPalette16 currentPalette;
uint8_t led_brightness = 100; // Default only, will be overwritten
float led_speed = 10;       // Default only, will be overwritten
uint8_t visual = 1;
uint8_t palette = 0;
uint8_t effect = 0;
long update_time = millis();

bool btn_01_status = false;
bool btn_02_status = false;
bool btn_03_status = false;
bool btn_04_status = false;
bool btn_05_status = false;
bool btn_06_status = false;
bool btn_07_status = false;
bool btn_08_status = false;
bool btn_09_status = false;
bool btn_10_status = false;
bool btn_11_status = false;
bool btn_12_status = false;
bool btn_13_status = false;

bool btn_20_status = false;
bool btn_21_status = false;
bool btn_22_status = false;
bool btn_23_status = false;

int sensorReading = 0;

// BEGIN CODE

void setup() {
  //Initialize LED Strip
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LED).setCorrection( TypicalLEDStrip );

  if(DISPLAY_INDICATOR == true){
   FastLED.addLeds<LED_TYPE, LED_PIN_INDICATOR, COLOR_ORDER>(leds_indicator, NUM_LED_INDICATOR).setCorrection( TypicalLEDStrip );
  }

  FastLED.setBrightness(led_brightness);
 
  //Set pin modes
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(EFFECT_PIN_01, INPUT);
  pinMode(EFFECT_PIN_02, INPUT);
  pinMode(EFFECT_PIN_03, INPUT);
  pinMode(EFFECT_PIN_04, INPUT);

  // Initialize pallete to gray
  currentPalette = CRGBPalette16(
                     CRGB(100, 100, 100), CRGB(100, 100, 100), CRGB(100, 100, 100), CRGB(100, 100, 100),
                     CRGB(100, 100, 100), CRGB(100, 100, 100), CRGB(100, 100, 100), CRGB(100, 100, 100),
                     CRGB(100, 100, 100), CRGB(100, 100, 100), CRGB(100, 100, 100), CRGB(100, 100, 100),
                     CRGB(100, 100, 100), CRGB(100, 100, 100), CRGB(100, 100, 100), CRGB(100, 100, 100) );

  Serial.begin(9600);
}

void loop()
{
  char key = clavier.getKey();
  // Handle button presses
  if (key != 0x00) // if there is not any key, function getKey send NULL character (0x00)
  {
    Serial.print("Key: ");
    Serial.println(key);
    // Visualizers
    if (key == '1') {
      if (btn_01_status == false) {
        btn_01_status = true;
        visual = 1;
      }
    } else {
      btn_01_status = false;
    }
    if (key == '2') {
      if (btn_02_status == false) {
        btn_02_status = true;
        visual = 2;
      }
    } else {
      btn_02_status = false;
    }
    if (key == '3') {
      if (btn_03_status == false) {
        btn_03_status = true;
        visual = 3;
      }
    } else {
      btn_03_status = false;
    }
    if (key == '4') {
      if (btn_04_status == false) {
        btn_04_status = true;
        visual = 4;
      }
    } else {
      btn_04_status = false;
    }
    if (key == '5') {
      if (btn_05_status == false) {
        btn_05_status = true;
        visual = 5;
      }
    } else {
      btn_05_status = false;
    }
    if (key == '6') {
      if (btn_06_status == false) {
        btn_06_status = true;
        visual = 6;
      }
    } else {
      btn_06_status = false;
    }
    if (key == '7') {
      if (btn_07_status == false) {
        btn_07_status = true;
        visual = 7;
      }
    } else {
      btn_07_status = false;
    }
    if (key == '8') {
      if (btn_08_status == false) {
        btn_08_status = true;
        visual = 8;
      }
    } else {
      btn_08_status = false;
    }
    if (key == '9') {
      if (btn_09_status == false) {
        btn_09_status = true;
        visual = 9;
      }
    } else {
      btn_09_status = false;
    }
    // Palettes
    if (key == '0') {
      if (btn_10_status == false) {
        btn_10_status = true;
        visual = 10;
      }
    } else {
      btn_10_status = false;
    }

    if (key == 'F') {
      if (btn_11_status == false) {
        btn_11_status = true;
        palette = 1;
      }
    } else {
      btn_11_status = false;
    }

    if (key == 'E') {
      if (btn_12_status == false) {
        btn_12_status = true;
        palette = 2;
      }
    } else {
      btn_12_status = false;
    }

    if (key == 'D') {
      if (btn_12_status == false) {
        btn_12_status = true;
        palette = 3;
      }
    } else {
      btn_12_status = false;
    }

    if (key == 'A') {
      if (btn_12_status == false) {
        btn_12_status = true;
        palette = 4;
      }
    } else {
      btn_12_status = false;
    }

    if (key == 'B') {
      if (btn_12_status == false) {
        btn_12_status = true;
        palette = 5;
      }
    } else {
      btn_12_status = false;
    }
    if (key == 'C') {
      if (btn_13_status == false) {
        btn_13_status = true;
        palette = 6;
      }
    } else {
      btn_13_status = false;
    }
  }



  if (digitalRead(EFFECT_PIN_01) == HIGH) {
    if (btn_20_status == false) {
      btn_20_status = true;
      effect = 1;
    }
  } else {
    btn_20_status = false;
  }
  if (digitalRead(EFFECT_PIN_02) == HIGH) {
    if (btn_21_status == false) {
      btn_21_status = true;
      effect = 2;
    }
  } else {
    btn_21_status = false;
  }
  if (digitalRead(EFFECT_PIN_03) == HIGH) {
    if (btn_22_status == false) {
      btn_22_status = true;
      effect = 0;
    }
  } else {
    btn_22_status = false;
  }
  if (digitalRead(EFFECT_PIN_04) == HIGH) {
    if (btn_23_status == false) {
      btn_23_status = true;
      if (effect == 4) {
        effect = 0;
      } else {
        effect = 4;
      }

    }
  } else {
    btn_23_status = false;
  }
  //Serial.print("Effect: ");
  //Serial.println(effect);
  /*
    Serial.print("Visual: ");
    Serial.print(visual);
    Serial.print(" Palette: ");
    Serial.println(palette);
  */

  // Update variables from potentiometers
  led_brightness = map(analogRead(BRIGHTNESS_PIN), 0, 1023, 0, 255);
  led_speed = map(log(analogRead(SPEED_PIN)) * 147, 0, 1023, 1000 / LED_MIN_SPEED, 1000 / LED_MAX_SPEED);

  // Visualizers:
  // runVisualFlow();
  // runVisualCenterFlow();
  // runVisualPing();
  // runVisualPingBlob();
  // runVisualBounce();
  // runVisualDoubleBounce();
  // runVisualSegments();
  // runVisualSound();
  // runVisualDoubleSpot();

  // Run visual based on visual id number
  // ---------- CHANGE THESE TO CHANGE SELECTED VISUALIZATION ----------
  if (update_time + led_speed < millis() && effect == 0) {
    update_time = millis();
    switch (visual) {
      case 1:
        runVisualFlow();
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
      case 10:
        runVisualPulse();
        break;
      default:
        runVisualFlow();
        break;
    }
  }
  if (update_time + led_speed < millis() && effect != 0) {
    update_time = millis();
    switch (effect) {
      case 1:
        runEffectStrobe();
        break;
      case 2:
        runEffectSplit();
        break;
      case 3:

        break;
      case 4:
        // Do not use. Effect 4 is a pause configured in the button management section
        break;
    }

  }

  // Palettes:
  // setPaletteFire(),
  // setPaletteSynth(),
  // setPaletteParty(),
  // setPaletteBlackWhite()

  // Map Palettes to visual id number
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
  /*  case 7:
      setCustom();
      break;
      */
    default:
      break;
  }

  FastLED.show();
  FastLED.delay(1);
}

// ------------ BEGIN EFFECTS ------------
void runEffectStrobe() {
  static uint8_t current_step = 0;

  if (current_step++ < 16 ) {
    // The effect logic goes here
    for ( int i = 0; i < NUM_LED; i += 1) {
      leds[i] = ColorFromPalette( currentPalette, (current_step * 16) % 64, constrain(led_brightness * (5.0 / 3.0), 0, 255), LINEARBLEND);
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
    leds[i] = ColorFromPalette( currentPalette, COLOR_04 + random(0, 5), constrain(led_brightness * (5.0 / 3.0), 0, 255), LINEARBLEND);
  }

  // Set lead LED to primary color
  leds[start_position_1] = ColorFromPalette( currentPalette, COLOR_01, constrain(led_brightness * (5.0 / 3.0), 0, 255), LINEARBLEND);
  leds[start_position_2] = ColorFromPalette( currentPalette, COLOR_01, constrain(led_brightness * (5.0 / 3.0), 0, 255), LINEARBLEND);

  // Set first trailer LED
  if (start_position_1 - 1 > 0) {
    leds[start_position_1 - 1] = ColorFromPalette( currentPalette, COLOR_02, constrain(led_brightness * (5.0 / 3.0), 0, 255), LINEARBLEND);
  }
  if (start_position_2 - 1 > 0) {
    leds[start_position_2 - 1] = ColorFromPalette( currentPalette, COLOR_02, constrain(led_brightness * (5.0 / 3.0), 0, 255), LINEARBLEND);
  }

  // Set the second trailer LED

  if (start_position_1 - 2 > 0) {
    leds[start_position_1 - 2] = ColorFromPalette( currentPalette, COLOR_03, constrain(led_brightness * (5.0 / 3.0), 0, 255), LINEARBLEND);
  }
  if (start_position_2 - 2 > 0) {
    leds[start_position_2 - 2] = ColorFromPalette( currentPalette, COLOR_03, constrain(led_brightness * (5.0 / 3.0), 0, 255), LINEARBLEND);
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
      leds[i] = ColorFromPalette( currentPalette, COLOR_01 + random(-8, 8), led_brightness, LINEARBLEND);
    } else if (start_position_1 > i && i > start_position_2) {
      leds[i] = ColorFromPalette( currentPalette, COLOR_02 + random(-8, 8), led_brightness, LINEARBLEND);
    } else {
      leds[i] = ColorFromPalette( currentPalette, COLOR_04 + random(0, 2), led_brightness - (led_brightness / 4), LINEARBLEND);
    }

  }

  
  for ( int i = 0; i < NUM_LED_INDICATOR; i += 1) {

    if (start_position_indicator_1 < i && i < start_position_indicator_2) {
      leds_indicator[i] = ColorFromPalette( currentPalette, COLOR_01 + random(-8, 8), led_brightness, LINEARBLEND);
    } else if (start_position_indicator_1 > i && i > start_position_indicator_2) {
      leds_indicator[i] = ColorFromPalette( currentPalette, COLOR_02 + random(-8, 8), led_brightness, LINEARBLEND);
    } else {
      leds_indicator[i] = ColorFromPalette( currentPalette, COLOR_04 + random(0, 2), led_brightness - (led_brightness / 4), LINEARBLEND);
    }

  }
}

// Creates sets of 7 LEDs colored in order from the palette and shifts them 1 each cycle
void runVisualFlow() {
  // Create static var to hand current position
  static uint8_t start_position = 0;

  // Update start position to "move" LEDs. In this case just a simple shift
  if (start_position > NUM_LED) {
    start_position = getSafeIndex(start_position);
  } else {
    start_position += 1;
  }
  uint8_t current_color = COLOR_01;

  // The visualizer logic goes here
  int j = 0;
  for ( int i = 0; i < NUM_LED; i += 7) {
    leds[getSafeIndex(i + start_position)] = ColorFromPalette( currentPalette, COLOR_01, led_brightness, LINEARBLEND);
    leds[getSafeIndex(i + start_position + 1)] = ColorFromPalette( currentPalette, COLOR_02, led_brightness, LINEARBLEND);
    leds[getSafeIndex(i + start_position + 2)] = ColorFromPalette( currentPalette, COLOR_02, led_brightness, LINEARBLEND);
    leds[getSafeIndex(i + start_position + 3)] = ColorFromPalette( currentPalette, COLOR_03, led_brightness, LINEARBLEND);
    leds[getSafeIndex(i + start_position + 4)] = ColorFromPalette( currentPalette, COLOR_03, led_brightness, LINEARBLEND);
    leds[getSafeIndex(i + start_position + 5)] = ColorFromPalette( currentPalette, COLOR_04, led_brightness, LINEARBLEND);
    leds[getSafeIndex(i + start_position + 6)] = ColorFromPalette( currentPalette, COLOR_04, led_brightness, LINEARBLEND);
    if(DISPLAY_INDICATOR == true) {
      leds_indicator[(j + start_position) % NUM_LED_INDICATOR] = ColorFromPalette(currentPalette, COLOR_01 , led_brightness, LINEARBLEND);
      leds_indicator[(j + 1 + start_position) % NUM_LED_INDICATOR] = ColorFromPalette(currentPalette, COLOR_02 , led_brightness, LINEARBLEND);
      leds_indicator[(j + 2 + start_position) % NUM_LED_INDICATOR] = ColorFromPalette(currentPalette, COLOR_02 , led_brightness, LINEARBLEND);
      leds_indicator[(j + 3 + start_position) % NUM_LED_INDICATOR] = ColorFromPalette(currentPalette, COLOR_03 , led_brightness, LINEARBLEND);
      leds_indicator[(j + 4 + start_position) % NUM_LED_INDICATOR] = ColorFromPalette(currentPalette, COLOR_03 , led_brightness, LINEARBLEND);
      leds_indicator[(j + 5 + start_position) % NUM_LED_INDICATOR] = ColorFromPalette(currentPalette, COLOR_04 , led_brightness, LINEARBLEND);
      j += 1;
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
    leds[constrain(i, 0, ledRange)] = ColorFromPalette( currentPalette, current_color, led_brightness, LINEARBLEND);
    current_color += 8;
  }
  for ( int i = 0; i < ledRange_indicator; i += 1) {
    leds_indicator[constrain(i, 0, ledRange_indicator)] = ColorFromPalette( currentPalette, current_color, led_brightness, LINEARBLEND);
    current_color += 8;
  }
  current_color = start_color;
  for ( int i = ledRange; i < NUM_LED; i += 1) {
    leds[i] = ColorFromPalette( currentPalette, current_color, led_brightness, LINEARBLEND);
    current_color -= 8;
  }
  for ( int i = ledRange_indicator; i < NUM_LED_INDICATOR; i += 1) {
    leds_indicator[i] = ColorFromPalette( currentPalette, current_color, led_brightness, LINEARBLEND);
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
    leds[getSafeIndex(i + start_position)] = ColorFromPalette( currentPalette, COLOR_01, led_brightness, LINEARBLEND);
    leds[getSafeIndex(i + start_position + 1)] = ColorFromPalette( currentPalette, COLOR_02, led_brightness, LINEARBLEND);
    leds[getSafeIndex(i + start_position + 2)] = ColorFromPalette( currentPalette, COLOR_02, led_brightness, LINEARBLEND);
    leds[getSafeIndex(i + start_position + 3)] = ColorFromPalette( currentPalette, COLOR_03, led_brightness, LINEARBLEND);
    leds[getSafeIndex(i + start_position + 4)] = ColorFromPalette( currentPalette, COLOR_03, led_brightness, LINEARBLEND);
    leds[getSafeIndex(i + start_position + 5)] = ColorFromPalette( currentPalette, COLOR_04, led_brightness, LINEARBLEND);
    leds[getSafeIndex(i + start_position + 6)] = ColorFromPalette( currentPalette, COLOR_04, led_brightness, LINEARBLEND);

    if(DISPLAY_INDICATOR == true) {
      leds_indicator[(j + start_position) % NUM_LED_INDICATOR] = ColorFromPalette(currentPalette, COLOR_01 , led_brightness, LINEARBLEND);
      leds_indicator[(j + 1 + start_position) % NUM_LED_INDICATOR] = ColorFromPalette(currentPalette, COLOR_02 , led_brightness, LINEARBLEND);
      leds_indicator[(j + 2 + start_position) % NUM_LED_INDICATOR] = ColorFromPalette(currentPalette, COLOR_02 , led_brightness, LINEARBLEND);
      leds_indicator[(j + 3 + start_position) % NUM_LED_INDICATOR] = ColorFromPalette(currentPalette, COLOR_03 , led_brightness, LINEARBLEND);
      leds_indicator[(j + 4 + start_position) % NUM_LED_INDICATOR] = ColorFromPalette(currentPalette, COLOR_03 , led_brightness, LINEARBLEND);
      leds_indicator[(j + 5 + start_position) % NUM_LED_INDICATOR] = ColorFromPalette(currentPalette, COLOR_04 , led_brightness, LINEARBLEND);
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
    leds[getSafeIndexFirstHalf(i + start_position_01)] = ColorFromPalette( currentPalette, COLOR_01, led_brightness, LINEARBLEND);
    leds[getSafeIndexFirstHalf(i + start_position_01 + 1)] = ColorFromPalette( currentPalette, COLOR_02, led_brightness, LINEARBLEND);
    leds[getSafeIndexFirstHalf(i + start_position_01 + 2)] = ColorFromPalette( currentPalette, COLOR_02, led_brightness, LINEARBLEND);
    leds[getSafeIndexFirstHalf(i + start_position_01 + 3)] = ColorFromPalette( currentPalette, COLOR_03, led_brightness, LINEARBLEND);
    leds[getSafeIndexFirstHalf(i + start_position_01 + 4)] = ColorFromPalette( currentPalette, COLOR_03, led_brightness, LINEARBLEND);
    leds[getSafeIndexFirstHalf(i + start_position_01 + 5)] = ColorFromPalette( currentPalette, COLOR_04, led_brightness, LINEARBLEND);
    leds[getSafeIndexFirstHalf(i + start_position_01 + 6)] = ColorFromPalette( currentPalette, COLOR_04, led_brightness, LINEARBLEND);
  }
  // The visualizer logic goes here
  for ( int i = NUM_LED / 2; i < NUM_LED; i += 7) {
    leds[getSafeIndexSecondHalf(i + start_position_02)] = ColorFromPalette( currentPalette, COLOR_01, led_brightness, LINEARBLEND);
    leds[getSafeIndexSecondHalf(i + start_position_02 + 1)] = ColorFromPalette( currentPalette, COLOR_02, led_brightness, LINEARBLEND);
    leds[getSafeIndexSecondHalf(i + start_position_02 + 2)] = ColorFromPalette( currentPalette, COLOR_02, led_brightness, LINEARBLEND);
    leds[getSafeIndexSecondHalf(i + start_position_02 + 3)] = ColorFromPalette( currentPalette, COLOR_03, led_brightness, LINEARBLEND);
    leds[getSafeIndexSecondHalf(i + start_position_02 + 4)] = ColorFromPalette( currentPalette, COLOR_03, led_brightness, LINEARBLEND);
    leds[getSafeIndexSecondHalf(i + start_position_02 + 5)] = ColorFromPalette( currentPalette, COLOR_04, led_brightness, LINEARBLEND);
    leds[getSafeIndexSecondHalf(i + start_position_02 + 6)] = ColorFromPalette( currentPalette, COLOR_04, led_brightness, LINEARBLEND);
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
    leds[i] = ColorFromPalette( currentPalette, current_color, led_brightness / 2, LINEARBLEND);
    if(DISPLAY_INDICATOR == true) {
      leds_indicator[i % NUM_LED_INDICATOR] = ColorFromPalette(currentPalette, current_color, led_brightness / 2, LINEARBLEND);
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
      leds[glow_pos_01] = ColorFromPalette( currentPalette, complement_color, led_brightness / 2, LINEARBLEND);
      if(DISPLAY_INDICATOR) {
       leds_indicator[glow_pos_01 % NUM_LED_INDICATOR] = ColorFromPalette(currentPalette, complement_color, led_brightness / 2, LINEARBLEND); 
      }
      break;
    case 1:
      leds[glow_pos_01] = ColorFromPalette( currentPalette, complement_color, led_brightness, LINEARBLEND);
      leds[constrain(glow_pos_01 + 1, 0, NUM_LED - 1)] = ColorFromPalette( currentPalette, complement_color, led_brightness / 2, LINEARBLEND);
      leds[constrain(glow_pos_01 + 1, 0, NUM_LED - 1)] = ColorFromPalette( currentPalette, complement_color, led_brightness / 2, LINEARBLEND);
      if(DISPLAY_INDICATOR) {
        leds_indicator[glow_pos_01 % NUM_LED_INDICATOR] = ColorFromPalette( currentPalette, complement_color, led_brightness, LINEARBLEND);
        leds_indicator[constrain((glow_pos_01 + 1) % NUM_LED_INDICATOR, 0, NUM_LED - 1)] = ColorFromPalette( currentPalette, complement_color, led_brightness / 2, LINEARBLEND);
        leds_indicator[constrain((glow_pos_01 + 1) % NUM_LED_INDICATOR, 0, NUM_LED - 1)] = ColorFromPalette( currentPalette, complement_color, led_brightness / 2, LINEARBLEND);
      }
      break;
    case 2:
      leds[glow_pos_01] = ColorFromPalette( currentPalette, complement_color, led_brightness, LINEARBLEND);
      leds[constrain(glow_pos_01 + 1, 0, NUM_LED - 1)] = ColorFromPalette( currentPalette, complement_color, led_brightness, LINEARBLEND);
      leds[constrain(glow_pos_01 + 2, 0, NUM_LED - 1)] = ColorFromPalette( currentPalette, complement_color, led_brightness / 2, LINEARBLEND);
      leds[constrain(glow_pos_01 - 1, 0, NUM_LED - 1)] = ColorFromPalette( currentPalette, complement_color, led_brightness, LINEARBLEND);
      leds[constrain(glow_pos_01 - 2, 0, NUM_LED - 1)] = ColorFromPalette( currentPalette, complement_color, led_brightness / 2, LINEARBLEND);
      if(DISPLAY_INDICATOR) {
        leds_indicator[glow_pos_01 % NUM_LED_INDICATOR] = ColorFromPalette( currentPalette, complement_color, led_brightness, LINEARBLEND);
        leds_indicator[constrain((glow_pos_01 + 1) % NUM_LED_INDICATOR, 0, NUM_LED - 1)] = ColorFromPalette( currentPalette, complement_color, led_brightness, LINEARBLEND);
        leds_indicator[constrain((glow_pos_01 + 2) % NUM_LED_INDICATOR, 0, NUM_LED - 1)] = ColorFromPalette( currentPalette, complement_color, led_brightness / 2, LINEARBLEND);
        leds_indicator[constrain((glow_pos_01 - 1) % NUM_LED_INDICATOR, 0, NUM_LED - 1)] = ColorFromPalette( currentPalette, complement_color, led_brightness, LINEARBLEND);
        leds_indicator[constrain((glow_pos_01 - 2) % NUM_LED_INDICATOR, 0, NUM_LED - 1)] = ColorFromPalette( currentPalette, complement_color, led_brightness / 2, LINEARBLEND);  
      }
      break;
    case 3:
      leds[glow_pos_01] = ColorFromPalette( currentPalette, complement_color, led_brightness, LINEARBLEND);
      leds[constrain(glow_pos_01 + 1, 0, NUM_LED - 1)] = ColorFromPalette( currentPalette, complement_color, led_brightness / 2, LINEARBLEND);
      leds[constrain(glow_pos_01 - 1, 0, NUM_LED - 1)] = ColorFromPalette( currentPalette, complement_color, led_brightness / 2, LINEARBLEND);
      if(DISPLAY_INDICATOR) {
        leds_indicator[glow_pos_01 % NUM_LED_INDICATOR] = ColorFromPalette( currentPalette, complement_color, led_brightness, LINEARBLEND);
        leds[constrain((glow_pos_01 + 1) % NUM_LED_INDICATOR, 0, NUM_LED - 1)] = ColorFromPalette( currentPalette, complement_color, led_brightness / 2, LINEARBLEND);
        leds[constrain((glow_pos_01 - 1) % NUM_LED_INDICATOR, 0, NUM_LED - 1)] = ColorFromPalette( currentPalette, complement_color, led_brightness / 2, LINEARBLEND);
      }
      break;
    case 4:
      leds[glow_pos_01] = ColorFromPalette( currentPalette, complement_color, led_brightness / 2, LINEARBLEND);
      if(DISPLAY_INDICATOR) {
        leds_indicator[glow_pos_01 % NUM_LED_INDICATOR] = ColorFromPalette( currentPalette, complement_color, led_brightness / 2, LINEARBLEND);
      }
      glow_step_01 = 0;
      break;
  }
  switch (glow_step_02++) {
    case 0:
      glow_pos_02 = random(NUM_LED / 2 + 1, NUM_LED); // Random location
      leds[glow_pos_02] = ColorFromPalette( currentPalette, complement_color + 8, led_brightness / 2, LINEARBLEND);
      break;
    case 1:
      leds[glow_pos_02] = ColorFromPalette( currentPalette, complement_color + 8, led_brightness, LINEARBLEND);
      leds[constrain(glow_pos_02 + 1, 0, NUM_LED - 1)] = ColorFromPalette( currentPalette, complement_color + 8, led_brightness / 2, LINEARBLEND);
      leds[constrain(glow_pos_02 + 1, 0, NUM_LED - 1)] = ColorFromPalette( currentPalette, complement_color + 8, led_brightness / 2, LINEARBLEND);
      break;
    case 2:
      leds[glow_pos_02] = ColorFromPalette( currentPalette, complement_color + 8, led_brightness, LINEARBLEND);
      leds[constrain(glow_pos_02 + 1, 0, NUM_LED - 1)] = ColorFromPalette( currentPalette, complement_color + 8, led_brightness, LINEARBLEND);
      leds[constrain(glow_pos_02 + 2, 0, NUM_LED - 1)] = ColorFromPalette( currentPalette, complement_color + 8, led_brightness / 2, LINEARBLEND);
      leds[constrain(glow_pos_02 - 1, 0, NUM_LED - 1)] = ColorFromPalette( currentPalette, complement_color + 8, led_brightness, LINEARBLEND);
      leds[constrain(glow_pos_02 - 2, 0, NUM_LED - 1)] = ColorFromPalette( currentPalette, complement_color + 8, led_brightness / 2, LINEARBLEND);
      break;
    case 3:
      leds[glow_pos_02] = ColorFromPalette( currentPalette, complement_color + 8, led_brightness, LINEARBLEND);
      leds[constrain(glow_pos_02 + 1, 0, NUM_LED - 1)] = ColorFromPalette( currentPalette, complement_color + 8, led_brightness / 2, LINEARBLEND);
      leds[constrain(glow_pos_02 - 1, 0, NUM_LED - 1)] = ColorFromPalette( currentPalette, complement_color + 8, led_brightness / 2, LINEARBLEND);
      break;
    case 4:
      leds[glow_pos_02] = ColorFromPalette( currentPalette, complement_color + 8, led_brightness / 2, LINEARBLEND);
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
    leds[i] = ColorFromPalette( currentPalette, current_color, led_brightness, LINEARBLEND);
  }

  if(DISPLAY_INDICATOR) {
    for(int i = 0; i < NUM_LED_INDICATOR; i++) {
      leds_indicator[i] = ColorFromPalette( currentPalette, base_color, led_brightness, LINEARBLEND);
    }
  }

  // Set lead LED to primary color
  leds[start_position] = ColorFromPalette( currentPalette, COLOR_01, led_brightness, LINEARBLEND);
  if(false) {
    leds_indicator[start_position_indicator] = ColorFromPalette( currentPalette, COLOR_01, led_brightness, LINEARBLEND);
  }

  // Set first trailer LED
  if (move_direction) {
    if (start_position - 1 > 0) {
      leds[start_position - 1] = ColorFromPalette( currentPalette, COLOR_02, led_brightness, LINEARBLEND);
    }
  } else {
    if (start_position + 1 < NUM_LED) {
      leds[start_position + 1] = ColorFromPalette( currentPalette, COLOR_02, led_brightness, LINEARBLEND);
    }
  }

  if(false) {
    if(move_direction_indicator) {
       if (start_position_indicator - 1 > 0) {
        leds_indicator[start_position_indicator - 1] = ColorFromPalette( currentPalette, COLOR_02, led_brightness, LINEARBLEND);
      }
    } else {
      if (start_position_indicator + 1 < NUM_LED_INDICATOR) {
        leds_indicator[start_position_indicator + 1] = ColorFromPalette( currentPalette, COLOR_02, led_brightness, LINEARBLEND);
      }
    }
   }

  // Set the second trailer LED
  if (move_direction) {
    if (start_position - 2 > 0) {
      leds[start_position - 2] = ColorFromPalette( currentPalette, COLOR_03, led_brightness, LINEARBLEND);
    }
  } else {
    if (start_position + 2 < NUM_LED) {
      leds[start_position + 2] = ColorFromPalette( currentPalette, COLOR_03, led_brightness, LINEARBLEND);
    }
  }

    if(false) {
      if(move_direction_indicator) {
         if (start_position_indicator - 2 > 0) {
          leds_indicator[start_position_indicator - 2] = ColorFromPalette( currentPalette, COLOR_03, led_brightness, LINEARBLEND);
        }
      } else {
        if (start_position_indicator + 2 < NUM_LED_INDICATOR) {
          leds_indicator[start_position_indicator + 2] = ColorFromPalette( currentPalette, COLOR_03, led_brightness, LINEARBLEND);
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
    leds[i] = ColorFromPalette( currentPalette, base_color, led_brightness, LINEARBLEND);
    leds_indicator[i % NUM_LED_INDICATOR] = ColorFromPalette( currentPalette, base_color, led_brightness, LINEARBLEND);
  }

  // Set lead LED to primary color
  leds[start_position_1] = ColorFromPalette( currentPalette, COLOR_01, led_brightness, LINEARBLEND);
  leds[start_position_2] = ColorFromPalette( currentPalette, COLOR_01, led_brightness, LINEARBLEND);

  leds_indicator[start_position_indicator_1] = ColorFromPalette( currentPalette, COLOR_01, led_brightness, LINEARBLEND);

  // Set first trailer LED
  if (move_direction_1) {
    if (start_position_1 - 1 > 0) {
      leds[start_position_1 - 1] = ColorFromPalette( currentPalette, COLOR_02, led_brightness, LINEARBLEND);
    }
  } else {
    if (start_position_1 + 1 < NUM_LED) {
      leds[start_position_1 + 1] = ColorFromPalette( currentPalette, COLOR_02, led_brightness, LINEARBLEND);
    }
  }
  if (move_direction_indicator_1) {
    if (start_position_indicator_1 - 1 > 0) {
      leds_indicator[start_position_indicator_1 - 1] = ColorFromPalette( currentPalette, COLOR_02, led_brightness, LINEARBLEND);
    }
  } else {
    if (start_position_indicator_1 + 1 < NUM_LED_INDICATOR) {
      leds_indicator[start_position_indicator_1 + 1] = ColorFromPalette( currentPalette, COLOR_02, led_brightness, LINEARBLEND);
    }
  }
  
  if (move_direction_2) {
    if (start_position_2 - 1 > 0) {
      leds[start_position_2 - 1] = ColorFromPalette( currentPalette, COLOR_02, led_brightness, LINEARBLEND);
    }
  } else {
    if (start_position_2 + 1 < NUM_LED) {
      leds[start_position_2 + 1] = ColorFromPalette( currentPalette, COLOR_02, led_brightness, LINEARBLEND);
    }
  }

  // Set the second trailer LED
  if (move_direction_1) {
    if (start_position_1 - 2 > 0) {
      leds[start_position_1 - 2] = ColorFromPalette( currentPalette, COLOR_03, led_brightness, LINEARBLEND);
    }
  } else {
    if (start_position_1 + 2 < NUM_LED) {
      leds[start_position_1 + 2] = ColorFromPalette( currentPalette, COLOR_03, led_brightness, LINEARBLEND);
    }
  }
  if (move_direction_indicator_1) {
    if (start_position_indicator_1 - 2 > 0) {
      leds_indicator[start_position_indicator_1 - 2] = ColorFromPalette( currentPalette, COLOR_03, led_brightness, LINEARBLEND);
    }
  } else {
    if (start_position_indicator_1 + 2 < NUM_LED_INDICATOR) {
      leds_indicator[start_position_indicator_1 + 2] = ColorFromPalette( currentPalette, COLOR_03, led_brightness, LINEARBLEND);
    }
  }
  if (move_direction_2) {
    if (start_position_2 - 2 > 0) {
      leds[start_position_2 - 2] = ColorFromPalette( currentPalette, COLOR_03, led_brightness, LINEARBLEND);
    }
  } else {
    if (start_position_2 + 2 < NUM_LED) {
      leds[start_position_2 + 2] = ColorFromPalette( currentPalette, COLOR_03, led_brightness, LINEARBLEND);
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
    leds[i] = ColorFromPalette( currentPalette, COLOR_04 + random(0, 5), led_brightness, LINEARBLEND);
  }

  // Set lead LED to primary color
  leds[start_position] = ColorFromPalette( currentPalette, COLOR_01, led_brightness, LINEARBLEND);

  // Set first trailer LED
  if (move_direction) {
    if (start_position - 1 > 0) {
      leds[start_position - 1] = ColorFromPalette( currentPalette, COLOR_02, led_brightness, LINEARBLEND);
    }
  } else {
    if (start_position + 1 < NUM_LED) {
      leds[start_position + 1] = ColorFromPalette( currentPalette, COLOR_02, led_brightness, LINEARBLEND);
    }
  }

  // Set the second trailer LED
  if (move_direction) {
    if (start_position - 2 > 0) {
      leds[start_position - 2] = ColorFromPalette( currentPalette, COLOR_03, led_brightness, LINEARBLEND);
    }
  } else {
    if (start_position + 2 < NUM_LED) {
      leds[start_position + 2] = ColorFromPalette( currentPalette, COLOR_03, led_brightness, LINEARBLEND);
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

    leds[getSafeIndex(i + start_pos)] = ColorFromPalette( currentPalette, current_color_offset, led_brightness, LINEARBLEND);
    leds[getSafeIndex(i + start_pos + 1)] = ColorFromPalette( currentPalette, current_color_offset, led_brightness, LINEARBLEND);
    leds[getSafeIndex(i + start_pos + 2)] = ColorFromPalette( currentPalette, current_color_offset, led_brightness, LINEARBLEND);
    leds[getSafeIndex(i + start_pos + 3)] = ColorFromPalette( currentPalette, current_color_offset, led_brightness, LINEARBLEND);
    leds[getSafeIndex(i + start_pos + 4)] = ColorFromPalette( currentPalette, current_color_offset, led_brightness, LINEARBLEND);
    leds[getSafeIndex(i + start_pos + 5)] = ColorFromPalette( currentPalette, current_color_offset, led_brightness, LINEARBLEND);

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
      leds[i] = ColorFromPalette( currentPalette, COLOR_01, led_brightness, LINEARBLEND);
    } else {
      leds[i] = ColorFromPalette( currentPalette, COLOR_04, led_brightness, LINEARBLEND);
    }
  }

  for ( int i = 0; i < NUM_LED_INDICATOR; i++) {

    if (i < ledRange_indicator) {
      leds_indicator[i] = ColorFromPalette( currentPalette, COLOR_01, led_brightness, LINEARBLEND);
    } else {
      leds_indicator[i] = ColorFromPalette( currentPalette, COLOR_04, led_brightness, LINEARBLEND);
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

  leds[0] = ColorFromPalette( currentPalette, COLOR_04, led_brightness, LINEARBLEND);
  leds_indicator[0]= ColorFromPalette( currentPalette, COLOR_04, led_brightness, LINEARBLEND);

  for ( int i = 0; i < NUM_LED / 2; i++) {

    if (i < ledRange) {
      leds[NUM_LED / 2 + i] = ColorFromPalette( currentPalette, COLOR_01, led_brightness, LINEARBLEND);
      leds[NUM_LED / 2 - i] = ColorFromPalette( currentPalette, COLOR_01, led_brightness, LINEARBLEND);
    } else {
      leds[NUM_LED / 2 + i] = ColorFromPalette( currentPalette, COLOR_04, led_brightness, LINEARBLEND);
      leds[NUM_LED / 2 - i] = ColorFromPalette( currentPalette, COLOR_04, led_brightness, LINEARBLEND);
    }
  }

  for ( int i = 0; i < NUM_LED_INDICATOR / 2; i++) {

    if (i < ledRange_indicator) {
      leds_indicator[NUM_LED_INDICATOR / 2 + i] = ColorFromPalette( currentPalette, COLOR_01, led_brightness, LINEARBLEND);
      leds_indicator[NUM_LED_INDICATOR / 2 - i] = ColorFromPalette( currentPalette, COLOR_01, led_brightness, LINEARBLEND);
    } else {
      leds_indicator[NUM_LED_INDICATOR / 2 + i] = ColorFromPalette( currentPalette, COLOR_04, led_brightness, LINEARBLEND);
      leds_indicator[NUM_LED_INDICATOR / 2 - i] = ColorFromPalette( currentPalette, COLOR_04, led_brightness, LINEARBLEND);
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
      leds[i] = ColorFromPalette( currentPalette, COLOR_01, led_brightness, LINEARBLEND);
    } else {
      leds[i] = ColorFromPalette( currentPalette, COLOR_04, led_brightness, LINEARBLEND);
    }
  }
  for ( int i = 0; i < NUM_LED_INDICATOR; i++) {
    if (i <= ledRange_indicator + 2 && i >= ledRange_indicator - 2) {
      leds_indicator[i] = ColorFromPalette( currentPalette, COLOR_01, led_brightness, LINEARBLEND);
    } else {
      leds_indicator[i] = ColorFromPalette( currentPalette, COLOR_04, led_brightness, LINEARBLEND);
    }
  }
}

// Gets an index we can safely assign to the led array, makes overflow "wrap" around
uint8_t getSafeIndex(uint8_t index) {
  return (index % NUM_LED);
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
