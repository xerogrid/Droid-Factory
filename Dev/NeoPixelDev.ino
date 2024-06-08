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

// Create Neopixel object
Adafruit_NeoPixel ring = Adafruit_NeoPixel(24, NeopixelPin, NEO_GRB + NEO_KHZ800);

// Function prototypes
void activateLEDs();
void disableLEDs();

void setup() {
  // Configure pin modes
  pinMode(pirPin, INPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(blueLedPin, OUTPUT);

  digitalWrite(redLedPin, LOW);
  digitalWrite(blueLedPin, LOW);

  ring.begin(); // Start the Neopixel ring
  ring.setBrightness(50); // Set the brightness level (0-255)
  ring.show(); // Update the ring with the current LED colors

  // Attach servos to pins
  verticalServo.attach(verticalPin);
  horizontalServo.attach(horizontalPin);

  // Move servos to home position
  verticalServo.write(verticalHomePosition);
  horizontalServo.write(horizontalHomePosition);

  // Initialize random seed
  randomSeed(analogRead(0));

  // Initialize serial communication
  Serial.begin(9600);
}

void loop() {
  // Read motion sensor pin value
  int motion = digitalRead(pirPin);

  // Check if motion is detected or servos are running
  if (motion == HIGH || verticalServo.attached() || horizontalServo.attached()) {
    Serial.println("Motion detected or servos running!");

    // Turn on the blue LED when motion is detected or servos are running
    digitalWrite(blueLedPin, HIGH);
    // Turn off the red LED when motion is detected or servos are running
    digitalWrite(redLedPin, LOW);
    
    // Set all Neopixel LEDs to green
    for (int i = 0; i < ring.numPixels(); i++) {
      ring.setPixelColor(i, ring.Color(0, 255, 0)); // Set the color of each LED to green
    }
    ring.show(); // Update the ring with the new LED colors

  } else {
    // Turn on the red LED when no motion is detected and no servo is running
    digitalWrite(redLedPin, HIGH);

    // Reset blue LED to off
    digitalWrite(blueLedPin, LOW);

    // Run the 24 LED spin pattern in red color
    for (int i = 0; i < ring.numPixels(); i++) {
      ring.setPixelColor(i, ring.Color(255, 0, 0)); // Set the color of each LED to red
      ring.show(); // Update the ring with the new LED colors
      delay(50); // Delay between each LED lighting up
    }
  }

  // Randomly move the vertical servo
  if (!verticalServo.attached() && !horizontalServo.attached()) {
    int randomInterval = random(1000, 5000);  // Get a random interval between 1 and 5 seconds
    int randomDirection = random(0, 2);       // Get a random direction (0 or 1)

    if (randomDirection == 0) {
      verticalServo.attach(verticalPin);
      verticalServo.write(30);   // Move the vertical servo to the down position
    } else {
      verticalServo.attach(verticalPin);
      verticalServo.write(150); // Move the vertical servo to the up position
    }

    delay(randomInterval);  // Wait for the random interval

    // Hold the position for 2 seconds
    verticalServo.write(verticalHomePosition);  // Move the vertical servo to the middle position
    delay(2000);                                 // Hold the position for 2 seconds

    // Return to home position
    verticalServo.write(verticalHomePosition);  // Move the vertical servo to the home position

    // Randomly sweep the horizontal servo
    randomInterval = random(2000, 5000);  // Get a random interval between 2 and 5 seconds
    int randomAngle = random(0, 161);     // Get a random angle between 0 and 180 degrees

    horizontalServo.attach(horizontalPin);
    horizontalServo.write(randomAngle);
    delay(randomInterval);
    horizontalServo.detach();
  }
}
