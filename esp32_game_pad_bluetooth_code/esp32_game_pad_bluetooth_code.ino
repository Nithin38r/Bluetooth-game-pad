#include <Arduino.h>
#include <BleGamepad.h>

#define X_BUTTON           D2
#define CIRCLE_BUTTON      D4
#define TRIANGLE_BUTTON    D5
#define SQUARE_BUTTON      D15

#define L1_BUTTON          D12
#define R1_BUTTON          D14
#define L2_BUTTON          D27
#define R2_BUTTON          D26

#define START_BUTTON       D13
#define SELECT_BUTTON      D25
#define PS_BUTTON          D23
#define R3_BUTTON          D22  // Right joystick SW
#define L3_BUTTON          D21  // Left joystick SW

#define RIGHT_VRX_JOYSTICK D32
#define RIGHT_VRY_JOYSTICK D33
#define LEFT_VRX_JOYSTICK  D18
#define LEFT_VRY_JOYSTICK  D19

#define NUM_BUTTONS        13
#define LED_PIN            D0

int buttonsPins[NUM_BUTTONS] = {
  X_BUTTON, CIRCLE_BUTTON, TRIANGLE_BUTTON, SQUARE_BUTTON,
  R1_BUTTON, R2_BUTTON, L1_BUTTON, L2_BUTTON,
  START_BUTTON, SELECT_BUTTON, PS_BUTTON,
  R3_BUTTON, L3_BUTTON
};

int androidGamepadButtons[NUM_BUTTONS] = {1, 2, 3, 4, 8, 10, 7, 9, 12, 11, 13, 15, 14};
int PS1GamepadButtons[NUM_BUTTONS]     = {2, 3, 4, 1, 6, 8, 5, 7, 10, 9, 13, 12, 11};
int PCGamepadButtons[NUM_BUTTONS]      = {1, 2, 4, 3, 6, 8, 5, 7, 10, 9, 0, 12, 11};

typedef enum {ANDROID, PS1, PC} GamepadModes;
GamepadModes gamepadMode = PC;

BleGamepad bleGamepad("ESP32_BLE Gamepad", "ESP");
BleGamepadConfiguration bleGamepadConfig;

bool wasConnected = false;

void setup() {
  delay(1000);
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  for (int i = 0; i < NUM_BUTTONS; i++) {
    pinMode(buttonsPins[i], INPUT_PULLUP);
  }

  bleGamepadConfig.setAutoReport(false);
  bleGamepadConfig.setControllerType(CONTROLLER_TYPE_GAMEPAD);
  bleGamepadConfig.setVid(0xe502);
  bleGamepadConfig.setPid(0xabcd);
  bleGamepadConfig.setHatSwitchCount(1);
  bleGamepad.begin(&bleGamepadConfig);
}

void loop() {
  bool isConnected = bleGamepad.isConnected();

  if (isConnected && !wasConnected) {
    for (int i = 0; i < 4; i++) {
      digitalWrite(LED_PIN, LOW); delay(200);
      digitalWrite(LED_PIN, HIGH); delay(200);
    }
    wasConnected = true;
  } else if (!isConnected && wasConnected) {
    wasConnected = false;
  }

  if (isConnected) {
    uint16_t rightVrx = map(analogRead(LEFT_VRX_JOYSTICK), 0, 4095, 0, 32767);
    uint16_t rightVry = 32767 - map(analogRead(LEFT_VRY_JOYSTICK), 0, 4095, 0, 32767);
    uint16_t leftVrx  = map(analogRead(RIGHT_VRX_JOYSTICK), 0, 4095, 0, 32767);
    uint16_t leftVry  = map(analogRead(RIGHT_VRY_JOYSTICK), 0, 4095, 0, 32767);

    for (int i = 0; i < NUM_BUTTONS; i++) {
      bool pressed = !digitalRead(buttonsPins[i]);
      int buttonID = 0;
      switch (gamepadMode) {
        case ANDROID: buttonID = androidGamepadButtons[i]; break;
        case PS1:     buttonID = PS1GamepadButtons[i]; break;
        case PC:      buttonID = PCGamepadButtons[i]; break;
      }

      if (i == 9) { // SELECT button → mouse click
        if (pressed) bleGamepad.pressMouseLeft();
        else bleGamepad.releaseMouseLeft();
      } else {
        pressed ? bleGamepad.press(buttonID) : bleGamepad.release(buttonID);
      }
    }

    if (gamepadMode == PC) {
      bleGamepad.setX(leftVrx);
      bleGamepad.setY(leftVry);
      bleGamepad.setZ(rightVrx);
      bleGamepad.setRX(rightVry);
    } else {
      bleGamepad.setLeftThumb(leftVrx, leftVry);
      bleGamepad.setRightThumb(rightVrx, rightVry);
    }

    bleGamepad.sendReport();
  }
}
