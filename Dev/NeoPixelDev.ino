#include <Servo.h>
#include <Adafruit_NeoPixel.h>

// Define servo objects
Servo verticalServo;
Servo horizontalServo;

// Define pin numbers
const int pinVertical = 9;
const int pinHorizontal = 10;
const int pinPir = 2;
const int pinRedLed = 3;
const int pinNeopixel = 5;

// Define home positions
const int verticalHomePosition = 90;
const int horizontalHomePosition = 90;

// Define NeoPixel constants
const int numLeds = 24;

// Create NeoPixel object
Adafruit_NeoPixel strip(numLeds, pinNeopixel, NEO_GRB + NEO_KHZ800);

void setup() {
  // Configure pin modes
  pinMode(pinPir, INPUT);
  pinMode(pinRedLed, OUTPUT);

  // Turn off the red LED initially
  digitalWrite(pinRedLed, LOW);

  // Attach servos to pins
  verticalServo.attach(pinVertical);
  horizontalServo.attach(pinHorizontal);

  // Initialize servos to home position
  verticalServo.write(verticalHomePosition);
  horizontalServo.write(horizontalHomePosition);

  // Initialize NeoPixel strip
  strip.begin();
  strip.show();  // Initialize all LEDs to off

  // Initialize random seed
  randomSeed(analogRead(0));

  // Initialize serial communication
  Serial.begin(9600);
}

void loop() {
  // Read motion sensor pin value
  if (digitalRead(pinPir) == HIGH) {
    Serial.println("Motion detected!");

    // Turn off the red LED when motion is detected
    digitalWrite(pinRedLed, LOW);

    // Clear NeoPixel strip
    for (int i = 0; i < numLeds; i++) {
      strip.setPixelColor(i, strip.Color(0, 0, 0));  // Set color of each LED to off (black)
    }
    strip.show();  // Update LED strip with new color

    // Sequential activation of LEDs
    for (int i = 0; i < numLeds; i++) {
      strip.setPixelColor(i, strip.Color(0, 255, 0));  // Set color to green (RGB)
      strip.show();  // Update LED strip with new color
  
      // Check if servos are still activated
      if (!isServoActive()) {
        break;  // Exit loop if servos are not activated
      }
  
      delay(100);  // Wait for 100 milliseconds

      strip.setPixelColor(i, strip.Color(0, 0, 0));  // Set color of current LED back to off
      strip.show();  // Update LED strip with new color
    }

    // Wait for 2 seconds before restarting the sequence
    delay(2000);

  } else {
    // No motion detected
    
    // Set all the LEDs to red
    for (int i = 0; i < numLeds; i++) {
      strip.setPixelColor(i, strip.Color(255, 0, 0));  // Set color to red (RGB)
    }
    strip.show();  // Update LED strip with new color

    // Turn on the red LED when no motion is detected and no servos are active
    if (!isServoActive()) {
      digitalWrite(pinRedLed, HIGH);
    }
  }
}

// Function to check if servos are active
bool isServoActive() {
  int verticalPos = verticalServo.read();
  int horizontalPos = horizontalServo.read();

  return (verticalPos != verticalHomePosition || horizontalPos != horizontalHomePosition);
}
