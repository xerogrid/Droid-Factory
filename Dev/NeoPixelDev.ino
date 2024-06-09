#include <Arduino.h>
#include <Servo.h>
#include <Adafruit_NeoPixel.h>

// Define servo objects
Servo verticalServo;
Servo horizontalServo;

// Define pin numbers
const int verticalPin = 9;
const int horizontalPin = 10;
const int pirPin = 2;
const int redLedPin = 3;
const int blueLedPin = 4;
const int NeopixelPin = 5;

// Define home positions
const int verticalHomePosition = 90;
const int horizontalHomePosition = 90;

// Create NeoPixel object
Adafruit_NeoPixel ring = Adafruit_NeoPixel(24, NeopixelPin, NEO_GRB + NEO_KHZ800);

// Function prototypes
void blinkRing();
void moveServoIfNeeded(Servo &servo, int position);
void rotateRedRing();
void detachServosIfHome();
void initialVerticalServoSequence();

void setup() {
  // Configure pin modes
  pinMode(pirPin, INPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(blueLedPin, OUTPUT);

  digitalWrite(redLedPin, LOW);
  digitalWrite(blueLedPin, LOW);

  // Initialize serial communication
  Serial.begin(9600);

  // Power up sequence
  // Step 1: LED ring sequence
  blinkRing();

  // Step 2: Attach servos to pins
  verticalServo.attach(verticalPin);
  horizontalServo.attach(horizontalPin);

  // Step 3: Initial vertical servo sequence
  initialVerticalServoSequence();

  // Step 4: Home both servos
  moveServoIfNeeded(verticalServo, verticalHomePosition);
  moveServoIfNeeded(horizontalServo, horizontalHomePosition);

  // Detach servos if they are at home position
  detachServosIfHome();

  // Initialize random seed
  randomSeed(analogRead(0));

  // Initialize NeoPixel ring
  ring.begin();
  ring.setBrightness(100);
  ring.show(); // Update the ring with the current LED colors
}

void loop() {
  // Read motion sensor pin value
  int motion = digitalRead(pirPin);

  if (motion == HIGH) {
    Serial.println("Motion detected!");

    // Turn on the blue LED when motion is detected
    digitalWrite(blueLedPin, HIGH);
    // Turn off the red LED when motion is detected
    digitalWrite(redLedPin, LOW);

    // Set all NeoPixel LEDs to green
    for (int i = 0; i < ring.numPixels(); i++) {
      ring.setPixelColor(i, ring.Color(0, 255, 0)); // Set the color of each LED to green
    }
    ring.show(); // Update the ring with the new LED colors

    // Attach servos before moving
    verticalServo.attach(verticalPin);
    horizontalServo.attach(horizontalPin);

    // Randomly move the vertical servo
    int randomInterval = random(1000, 5000);  // Get a random interval between 1 and 5 seconds
    int randomDirection = random(0, 2);       // Get a random direction (0 or 1)

    if (randomDirection == 0) {
      moveServoIfNeeded(verticalServo, 30);   // Move the vertical servo to the down position
    } else {
      moveServoIfNeeded(verticalServo, 150); // Move the vertical servo to the up position
    }

    delay(randomInterval);  // Wait for the random interval

    // Hold the position for 2 seconds
    moveServoIfNeeded(verticalServo, verticalHomePosition);  // Move the vertical servo to the middle position
    delay(2000);                                 // Hold the position for 2 seconds

    // Return to home position
    moveServoIfNeeded(verticalServo, verticalHomePosition);  // Move the vertical servo to the home position

    // Randomly sweep the horizontal servo
    randomInterval = random(2000, 5000);  // Get a random interval between 2 and 5 seconds
    int randomAngle = random(0, 161);     // Get a random angle between 0 and 180 degrees

    moveServoIfNeeded(horizontalServo, randomAngle);
    delay(randomInterval);

    // Detach servos if they are at home position
    detachServosIfHome();

  } else {
    // Turn on the red LED when no motion is detected and no servo is running
    digitalWrite(redLedPin, HIGH);
    
    // Reset blue LED to off
    digitalWrite(blueLedPin, LOW);

    // Run the 24 LED rotating red pattern
    rotateRedRing();
  }
}

// Function to move servos only if the position is different
void moveServoIfNeeded(Servo &servo, int position) {
  if (servo.read() != position) {
    servo.write(position);
  }
}

// Function to blink the LED ring in orange, purple, and green
void blinkRing() {
  ring.begin();
  ring.setBrightness(100);

  // Orange
  for (int i = 0; i < ring.numPixels(); i++) {
    ring.setPixelColor(i, ring.Color(255, 165, 0));
  }
  ring.show();
  delay(1500);

  // Purple
  for (int i = 0; i < ring.numPixels(); i++) {
    ring.setPixelColor(i, ring.Color(128, 0, 128));
  }
  ring.show();
  delay(1500);

  // Green
  for (int i = 0; i < ring.numPixels(); i++) {
    ring.setPixelColor(i, ring.Color(0, 255, 0));
  }
  ring.show();
  delay(1500);

  // Turn off
  for (int i = 0; i < ring.numPixels(); i++) {
    ring.setPixelColor(i, ring.Color(0, 0, 0));
  }
  ring.show();
}

// Function to run the rotating red pattern on the LED ring
void rotateRedRing() {
  for (int i = 0; i < ring.numPixels(); i++) {
    ring.clear();
    ring.setPixelColor(i, ring.Color(255, 0, 0));
    ring.show();
    delay(50);
  }
}

// Function to detach servos if they are at home position
void detachServosIfHome() {
  if (verticalServo.read() == verticalHomePosition) {
    verticalServo.detach();
  }
  if (horizontalServo.read() == horizontalHomePosition) {
    horizontalServo.detach();
  }
}

// Function for the initial vertical servo sequence
void initialVerticalServoSequence() {
  // Move the vertical servo up
  moveServoIfNeeded(verticalServo, 150);
  delay(2000); // Wait for 2 seconds

  // Move the vertical servo down
  moveServoIfNeeded(verticalServo, 30);
  delay(2000); // Wait for 2 seconds

  // Return the vertical servo to home position
  moveServoIfNeeded(verticalServo, verticalHomePosition);
}