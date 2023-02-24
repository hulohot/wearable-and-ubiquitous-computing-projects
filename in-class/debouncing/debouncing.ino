#define BUTTON_PIN 7
#define DEBOUNCE_MS 50

typedef enum {
  MODE_0,
  MODE_1
} MODES;

int read_button_state() {
  // Return 1 if button is pressed, 0 if not
  int buttonState = digitalRead(BUTTON_PIN);
  return buttonState == HIGH;
}

int evalulate_buton_state(int button_value) {
  // Check
  static int button_state = 1;
  static long last_changed = 0;
  if (button_value != button_state) {
    if (millis() - last_changed > DEBOUNCE_MS) {
      button_state = button_value;
      last_changed = millis();
    }
  }
  return button_state;
}

void setup()
{
  // Define Pins
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  // Initialize Serial Monitor
  Serial.begin(115200);
}

void loop()
{
  static MODES mode = MODE_0;
  static int button_value = 0;

  // If the butotn value has changed
  if (button_value != evalulate_buton_state(read_button_state())) {
    if (mode == MODE_0) {
      mode = MODE_1;
    } else {
      mode = MODE_0;
    }
    button_value = evalulate_buton_state(read_button_state());
  }

  if (mode == MODE_0) {
    Serial.println("Mode 0");
  } else {
    Serial.println("Mode 1");
  }
}
