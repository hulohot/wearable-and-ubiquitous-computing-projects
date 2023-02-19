#include <Servo.h>

// Define variables
const int photoresistorPin = A0; // Photoresistor pin
const int servoPin = 7; // Servomotor signal pin
const int buttonPin = 0; // Pushbutton pin
const int redLedPin = 1; // Red LED pin
const int greenLedPin = 2; // Green LED pin
const int debounceDelay = 50; // Debounce delay time

int mode = 0; // 0 for light meter mode, 1 for timer mode
int lightThreshold = 500; // Light threshold for timer mode
int count = 0; // Counter for timer mode

// Define objects
Servo servoMotor; // Servomotor object

// Define button state variables
int buttonState = HIGH;
int lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;

void setup() {
  // Set pin modes
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);

  // Attach servomotor to pin
  servoMotor.attach(servoPin);
  servoMotor.write(0);

  // Initialize LED states
  digitalWrite(redLedPin, LOW);
  digitalWrite(greenLedPin, LOW);
  delay(200);
  digitalWrite(redLedPin, HIGH);
  digitalWrite(greenLedPin, HIGH);
  delay(200);
  digitalWrite(redLedPin, LOW);
  digitalWrite(greenLedPin, LOW);
}

void loop() {
  // Read photoresistor value
  int lightValue = analogRead(photoresistorPin);

  // Light meter mode
  if (mode == 0) {
    // Map photoresistor value to servo angle
    int servoAngle = map(lightValue, 0, 1023, 0, 180);

    // Move servo to corresponding angle
    servoMotor.write(servoAngle);

    // Delay for servo to reach position
    delay(15);

    // Check button state
    buttonState = digitalRead(buttonPin);

    // If button is pressed, switch mode and reset timer
    if (buttonState == LOW && lastButtonState == HIGH && millis() - lastDebounceTime > debounceDelay) {
      mode = 1;
      count = 0;
      digitalWrite(redLedPin, LOW);
      digitalWrite(greenLedPin, HIGH);
      lastDebounceTime = millis();
    }
  }
  // Timer mode
  else {
    // If light is below threshold, reset timer
    if (lightValue < lightThreshold) {
      count = 0;
      digitalWrite(redLedPin, LOW);
      digitalWrite(greenLedPin, HIGH);
    }
    // If light is above threshold, increment counter and move servo
    else {
      count++;
      int servoAngle = map(count, 0, 30, 180, 0);
      servoMotor.write(servoAngle);
      delay(15);
    }

    // If counter reaches 30 seconds, turn on red LED and switch back to light meter mode
    if (count >= 30) {
      mode = 0;
      digitalWrite(redLedPin, HIGH);
      digitalWrite(greenLedPin, LOW);
    }
  }

  // Update button state variables
  lastButtonState = buttonState;
  
}
