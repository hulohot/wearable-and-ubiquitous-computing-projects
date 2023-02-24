#include <Servo.h>
#include <AbleButtons.h>

// Define variables
const int photoresistorPin = A0; // Photoresistor pin
const int servoPin = 7;          // Servomotor signal pin
const int buttonPin = 4;         // Pushbutton pin
const int redLedPin = 3;         // Red LED pin
const int greenLedPin = 2;       // Green LED pin

int mode = 0;             // 0 for light meter mode, 1 for timer mode
int lightThreshold = 100; // Light threshold for timer mode
int count = 0;            // Counter for timer mode

using Button = AblePulldownClickerButton;

// Define objects
Servo servoMotor; // Servomotor object
Button button(buttonPin);

// Define button state variables
unsigned long lastDebounceTime = 0;

void setup()
{
  // Initialize serial monitor
  Serial.begin(115200);

  // Set pin modes
  pinMode(buttonPin, INPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);

  // Attach servomotor to pin
  servoMotor.attach(servoPin);
  servoMotor.write(180);
  delay(500);
  servoMotor.write(0);

  // Initialize button state
  button.begin();

  // Initialize LED states
  digitalWrite(redLedPin, LOW);
  digitalWrite(greenLedPin, LOW);
  delay(200);
  digitalWrite(redLedPin, HIGH);
  digitalWrite(greenLedPin, HIGH);
  delay(200);
  digitalWrite(redLedPin, LOW);
  digitalWrite(greenLedPin, LOW);

  Serial.println("Setup complete");
}

void loop()
{
  // Read button state
  button.handle();

  // Read photoresistor value
  int lightValue = analogRead(photoresistorPin);

  // Light meter mode
  if (mode == 0)
  {
    // Map photoresistor value to servo angle
    lightValue = min(lightValue, 300);
    int servoAngle = map(lightValue, 10, 300, 0, 180);

    // Move servo to corresponding angle
    servoMotor.write(servoAngle);
  }
  // Timer mode
  else
  {
    // Check if count is greater than 0
    // If so, check if a 1 second interval has passed
    if (count > 0 && millis() - lastDebounceTime >= 1000 && lightValue > lightThreshold)
    {
      // Decrement count
      count--;
      // Reset lastDebounceTime
      lastDebounceTime = millis();

      Serial.print("Count ");
      Serial.println(count);
    }

    // Handle LED state
    if (count > 0)
    {
      digitalWrite(redLedPin, LOW);
      digitalWrite(greenLedPin, HIGH);
    }
    else
    {
      digitalWrite(redLedPin, HIGH);
      digitalWrite(greenLedPin, LOW);
    }

    // Move servo to corresponding angle
    int servoAngle = map(count, 30, 0, 0, 180);
    servoMotor.write(servoAngle);
  }

  // Check if button has been pressed
  if (button.resetClicked())
  {
    Serial.println("Button pressed");
    if (mode == 0)
    {
      mode = 1;
      // Set the counter to 30
      count = 30;
    }
    else
    {
      mode = 0;
      // Reset counter
      count = 0;

      // Reset LEDs   
      digitalWrite(redLedPin, LOW);
      digitalWrite(greenLedPin, LOW);   
    }
  }
}
