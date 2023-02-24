#include <Servo.h>
#include <AbleButtons.h>

// Define variables
const int photoresistorPin = A0; // Photoresistor pin
const int servoPin = 7;          // Servomotor signal pin
const int buttonPin = 4;         // Pushbutton pin
const int redLedPin = 3;         // Red LED pin
const int greenLedPin = 2;       // Green LED pin

int mode = 1;             // 1 for light meter mode, 2 for timer mode
int lightThreshold = 155; // Light threshold for timer mode
int lightMinium = 10;     // Minimum light value for light meter mode
int lightMaximum = 300;   // Maximum light value for light meter mode
int count = 0;            // Counter for timer mode

using Button = AblePulldownClickerButton;

// Define objects
Servo servoMotor; // Servomotor object
Button button(buttonPin);

// Define button state variables
unsigned long lastDebounceTime = 0;

/**
 * @brief Function to blink both LEDs.
 */
void blink_leds()
{
  delay(200);
  leds_on();
  delay(200);
  leds_off();
}

/**
 * @brief Function to turn off both LEDs.
 */
void leds_off()
{
  digitalWrite(redLedPin, LOW);
  digitalWrite(greenLedPin, LOW);
}

/**
 * @brief Function to turn on both LEDs.
 */
void leds_on()
{
  digitalWrite(redLedPin, HIGH);
  digitalWrite(greenLedPin, HIGH);
}

/**
 * @brief Function to turn on the green LED and turn off the red LED.
 */
void green_led_on()
{
  digitalWrite(greenLedPin, HIGH);
  digitalWrite(redLedPin, LOW);
}

/**
 * @brief Function to turn on the red LED and turn off the green LED.
 */
void red_led_on()
{
  digitalWrite(redLedPin, HIGH);
  digitalWrite(greenLedPin, LOW);
}

/**
 * @brief Function to handle the behavior of the system in light meter mode.
 * @param lightValue The value read from the photoresistor.
 */
void handle_mode_1(int lightValue)
{
  // Map photoresistor value to servo angle
  lightValue = min(lightValue, 300);
  int servoAngle = map(lightValue, lightMinium, lightMaximum, 0, 180);

  // Move servo to corresponding angle
  servoMotor.write(servoAngle);
}

/**
 * @brief Function to handle the behavior of the system in timer mode.
 * @param lightValue The value read from the photoresistor.
 */
void handle_mode_2(int lightValue)
{
  // Check if we should decrement count
  // If the count is above 0, the last debounce time is more than 1 second ago, and the light value is above the threshold
  if (count > 0 && millis() - lastDebounceTime >= 1000 && lightValue > lightThreshold)
  {
    // Decrement count
    count--;
    // Reset lastDebounceTime
    lastDebounceTime = millis();
  }

  // Handle LED state
  if (count > 0)
  {
    green_led_on();
  }
  else
  {
    red_led_on();
  }

  // Move servo to corresponding angle
  int servoAngle = map(count, 30, 0, 0, 180);
  servoMotor.write(servoAngle);
}

/**
 * @brief Function to handle the behavior of the system when the button is pressed.
 */
void handle_button_pressed()
{
  if (mode == 1)
  {
    mode = 2;
    // Set the counter to 30
    count = 30;
  }
  else
  {
    mode = 1;
    // Reset counter
    count = 0;

    // Reset LEDs
    leds_off();
  }
}

/**
 * @brief Function to set up the system.
 */
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
  servoMotor.write(0);

  // Initialize button state
  button.begin();

  // Initialize LED states
  leds_off();
  blink_leds();

  Serial.println("Setup complete");
}

/**
 * @brief Function to run the main loop of the system.
 */
void loop()
{
  // Read button state
  button.handle();

  // Read photoresistor value
  int lightValue = analogRead(photoresistorPin);

  // Light meter mode
  if (mode == 1)
  {
    handle_mode_1(lightValue);
  }
  // Timer mode
  else
  {
    handle_mode_2(lightValue);
  }

  // Check if button has been pressed
  if (button.resetClicked())
  {
    handle_button_pressed();
  }
}
