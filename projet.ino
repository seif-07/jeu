#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Set the LCD address, number of columns, and number of rows
const int lcdAddress = 0x27; // Change this to match your LCD address
const int lcdCols = 16;
const int lcdRows = 2; 

// Initialize the LCD
LiquidCrystal_I2C lcd(lcdAddress, lcdCols, lcdRows);



// Analog joystick pins
const int xAxisPin = A0;
const int yAxisPin = A1;

// Push button pins
const int buttonPin1 = 7; // Change this to the pin connected to the first push button
const int buttonPin2 = 8; // Change this to the pin connected to the second push button

// LED pins
const int ledPin1 = 5; // Change this to the pin connected to the first LED
const int ledPin2 = 4; // Change this to the pin connected to the second LED

// Calibration constants (adjust these based on your joystick)
const int xMin = 0;   // Minimum X-axis reading
const int xMax = 1023; // Maximum X-axis reading
const int yMin = 0;   // Minimum Y-axis reading
const int yMax = 1023; // Maximum Y-axis reading

void setup() {
  // Initialize the LCD
  lcd.begin();
  lcd.backlight();

  // Print a message to the LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Analog Joystick");

  // Initialize the analog pins
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);

  // Initialize the push button pins
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);

  // Initialize the LED pins
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
}

void loop() {
  // Read the X and Y axis values of the analog joystick
  int xAxisValue = analogRead(A0);
  int yAxisValue = analogRead(A1);

  int invertedXValue = map(xAxisValue,xMin, xMax,xMax,xMin);
  int invertedYValue = map(yAxisValue,yMin, yMax,yMax,yMin);

  // Map the raw analog readings to a 0-100 range
  /*int xMapped = map(xAxisValue, xMin, xMax, 0, 100);
  int yMapped = map(yAxisValue, yMin, yMax, 0, 100);*/

  // Print the X and Y axis values to the LCD
  lcd.setCursor(0, 1);  // Set cursor to second row
  lcd.print("X: ");
  lcd.print(invertedXValue);
  lcd.print("   Y: ");
  lcd.print(invertedYValue);

  // Check if the first push button is pressed
  if (digitalRead(buttonPin1) == LOW) {
    digitalWrite(ledPin1, HIGH); // Turn on the first LED
  } else {
    digitalWrite(ledPin1, LOW); // Turn off the first LED
  }

  // Check if the second push button is pressed
  if (digitalRead(buttonPin2) == LOW) {
    digitalWrite(ledPin2, HIGH); // Turn on the second LED
  } else {
    digitalWrite(ledPin2, LOW); // Turn off the second LED
  }

  delay(100);  // Delay for stability
}


/*
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Set the LCD address, number of columns, and number of rows
const int lcdAddress = 0x27; // Change this to match your LCD address
const int lcdCols = 16;
const int lcdRows = 2;

// Initialize the LCD
LiquidCrystal_I2C lcd(lcdAddress, lcdCols, lcdRows);

void setup() {
  // Initialize the LCD
  lcd.begin();

  // Turn on the LCD backlight
  lcd.backlight();

  // Print a message to the LCD
  lcd.clear(); // Clear the LCD screen
  lcd.setCursor(0, 0); // Set cursor to the first row
  lcd.print("Analog Joystick");

  // Initialize the analog pins
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
}

void loop() {
  // Read the X and Y axis values of the analog joystick
  int xAxisValue = analogRead(A0);
  int yAxisValue = analogRead(A1);

  // Print the X and Y axis values to the LCD
  lcd.setCursor(0, 1);  // Set cursor to second row
  lcd.print("X: ");
  lcd.print(xAxisValue);
  lcd.print("   Y: ");
  lcd.print(yAxisValue);

  delay(100);  // Delay for stability
}*/