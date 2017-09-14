#include <Wire.h>
#include <SPI.h>
#include <Keyboard.h>
#include <Adafruit_CAP1188.h>

#define DEBUG true

// Reset Pin is used for I2C or SPI
#define CAP1188_RESET  9

// CS pin is used for software or hardware SPI
#define CAP1188_CS  10

// Define our output characters
#define WORKBENCH 'W'
#define CODE_WINDOW 'C'
#define BROWSER 'B'
#define PIP_WB_ON 'V'
#define PIP_WB_OFF 'T'
#define PIP_CAM_ON 'L'
#define PIP_CAM_OFF 'X'

// Keep note of whether a key has been pressed to avoid double presses
bool keyPress = false;

// These are defined for software SPI, for hardware SPI, check your
// board's SPI pins in the Arduino documentation
//#define CAP1188_MOSI  11
//#define CAP1188_MISO  12
//#define CAP1188_CLK  13

// For I2C, connect SDA to your Arduino's SDA pin, SCL to SCL pin
// On UNO/Duemilanove/etc, SDA == Analog 4, SCL == Analog 5
// On Leonardo/Micro, SDA == Digital 2, SCL == Digital 3
// On Mega/ADK/Due, SDA == Digital 20, SCL == Digital 21

// Use I2C, no reset pin!
//Adafruit_CAP1188 cap = Adafruit_CAP1188();

// Or...Use I2C, with reset pin
Adafruit_CAP1188 cap = Adafruit_CAP1188(CAP1188_RESET);

// Or... Hardware SPI, CS pin & reset pin
// Adafruit_CAP1188 cap = Adafruit_CAP1188(CAP1188_CS, CAP1188_RESET);

// Or.. Software SPI: clock, miso, mosi, cs, reset
//Adafruit_CAP1188 cap = Adafruit_CAP1188(CAP1188_CLK, CAP1188_MISO, CAP1188_MOSI, CAP1188_CS, CAP1188_RESET);

void setup() {
  delay(3000);
  Serial.begin(9600);
  delay(3000);
  debugMsg("CAP1188 test", true);

  // Initialize the sensor, if using i2c you can pass in the i2c address
  // if (!cap.begin(0x28)) {
  if (!cap.begin(0x29)) {
    debugMsg("CAP1188 not found", true);
    exit(0);
  }
  debugMsg("CAP1188 found!", true);

  // Multi-touch OFF 0x80 (2 inputs=0x84, 3 = 0x88) see Table 5.24 Multiple Touch Configuration
  cap.writeRegister(0x2A, 0x80);

  // Sensitivity (default 0x2F see Table 5.8 DELTA_SENSE) 000 most sensitive, 111 least sensitive
  cap.writeRegister(0x1F, B01001111);

  // Disable touch key repeat function see Table 5.23
  cap.writeRegister(0x28, 0x00);

  // Start keyboard
  Keyboard.begin();
}

void loop() {
  uint8_t touched = cap.touched();

  if (touched != 0 && !keyPress) {

    // flag the key press
    keyPress = true;

    debugMsg("-----> ", false);

    for (auto /*uint8_t*/ i = 0; i < 8; i++) {
      if (touched & (1 << i)) {
        debugMsg((String)(i + 1), true);

        /*
          #define WORKBENCH W
          #define CODE_WINDOW C
          #define BROWSER B
          #define PIP_WB_ON V
          #define PIP_WB_OFF T
          #define PIP_CAM_ON L
          #define PIP_CAM_OFF X
        */
        Keyboard.press(KEY_LEFT_CTRL);
        Keyboard.press(KEY_LEFT_ALT);
        switch (i + 1) {
          case 1:
            Keyboard.press(PIP_CAM_OFF);
            break;
          case 2:
            Keyboard.press(PIP_WB_ON);
            break;
          case 3:
            Keyboard.press(WORKBENCH);
            break;
          case 4:
            Keyboard.press(PIP_WB_OFF);
            break;
          case 5:
            Keyboard.press(PIP_CAM_ON);
            break;
          case 6:
            Keyboard.press(CODE_WINDOW);
            break;
          case 7:
            Keyboard.press(BROWSER);
            break;
          default:
            debugMsg("Key not recognised: ", false);
            debugMsg((String)(i + 1), true);
            break;
        }

        // Release modifier keys and give things a chance to catch up
        delay(100);
        Keyboard.releaseAll();
        delay(500);
        break;
      }
    }

    delay(100);
  }
  else {
    // Clear the keypress flag so we can detect another
    keyPress = false;
  }
}

// Prevent serial port being used for debugging
void debugMsg(String msg, bool NL) {
#if DEBUG
  if (NL)
    Serial.println(msg);
  else
    Serial.print(msg);
#endif
}

