#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Servo.h>

// WiFi credentials
char ssid = "YourWiFiSSID";
char pass = "YourWiFiPassword";

 // Coonect Motor Control Project
char auth_motor[] = "Auth-Token-MotorProject";

// Connect Sensor Stream Project
char auth_sensor[] = "Auth-Token-SensorProject";

// Connect Pan-tilt Servo Project
char auth_sensor[] = "Auth-Token-ServoProject";

// Pins connected to the motors
#define IN_1 2 // Connect to Motor Control Module's pin, Input 1
#define IN_2 4 // Connect to Motor Control Module's pin, Input 2
#define IN_3 5 // Connect to Motor Control Module's pin, Input 3
#define IN_4 18 // Connect to Motor Control Module's pin, Input 4
#define ENA 22 // Enable Motor A Pin
#define ENB 13 // Enable Motor B Pin

// Pins connected to the ultrasonic sensors
#define TRIG_PIN_1 19 // Change to your trigger pin for sensor 1
#define ECHO_PIN_1 23 // Change to your echo pin for sensor 1
#define TRIG_PIN_2 22 // Change to your trigger pin for sensor 2
#define ECHO_PIN_2 21 // Change to your echo pin for sensor 2
#define TRIG_PIN_3 17 // Change to your trigger pin for sensor 3
#define ECHO_PIN_3 16 // Change to your echo pin for sensor 3
#define TRIG_PIN_4 34 // Change to your trigger pin for sensor 4
#define ECHO_PIN_4 14 // Change to your echo pin for sensor 4

const int photoresistorPin = 15; // Analog pin connected to the photoresistor
const int ledPin = 33; // Digital pin connected to the LED

// Create servo objects
Servo servo1; //D25
Servo servo2; //D26

// Initialize the Blynk objects for Motor Control Project and Sensor Stream Project and Servo Project
BlynkTimer timer_motor;
BlynkTimer timer_sensor;
BlynkTimer timer_servo;

//Linear Motion Functions

// Function to control motor speed for function 1
void function1(int speed) {
  // Set speed of alternate motors
  analogWrite(IN_1, speed);
  analogWrite(IN_3, speed);
  analogWrite(IN_2, 0);
  analogWrite(IN_4, 0);
}

// Function to control motor speed for function 2
void function2(int speed) {
  // Set speed of alternate motors
  analogWrite(IN_1, 0);
  analogWrite(IN_3, 0);
  analogWrite(IN_2, speed);
  analogWrite(IN_4, speed);
}

//Turn Functions

// Function to control motor speed for function 3
void function3(int speed) {
  // Set speed of motors 2 and 3
  analogWrite(IN_1, 0);
  analogWrite(IN_4, 0);
  analogWrite(IN_2, speed);
  analogWrite(IN_3, speed);
}

// Function to control motor speed for function 4
void function4(int speed) {
  // Set speed of motors 1 and 4
  analogWrite(IN_2, 0);
  analogWrite(IN_3, 0);
  analogWrite(IN_1, speed);
  analogWrite(IN_4, speed);
}

// Function to control single motor speed for function 4
void function5(int speed) {
  // Set speed of motors 1 and 4
  analogWrite(IN_2, 0);
  analogWrite(IN_3, 0);
  analogWrite(IN_1, 0);
  analogWrite(IN_4, speed);
}

// Function to control single motor speed for function 3
void function6(int speed) {
  // Set speed of motors 1 and 4
  analogWrite(IN_2, 0);
  analogWrite(IN_4, 0);
  analogWrite(IN_1, 0);
  analogWrite(IN_3, speed);
}

// Function to read distance from ultrasonic sensor 1
float readDistance1() {
  digitalWrite(TRIG_PIN_1, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN_1, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN_1, LOW);
  unsigned long duration = pulseIn(ECHO_PIN_1, HIGH);
  float distance = duration * 0.034 / 2;
  return distance;
}

// Function to read distance from ultrasonic sensor 2
float readDistance2() {
  digitalWrite(TRIG_PIN_2, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN_2, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN_2, LOW);
  unsigned long duration = pulseIn(ECHO_PIN_2, HIGH);
  float distance = duration * 0.034 / 2;
  return distance;
}

// Function to read distance from ultrasonic sensor 3
float readDistance3() {
  digitalWrite(TRIG_PIN_3, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN_3, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN_3, LOW);
  unsigned long duration = pulseIn(ECHO_PIN_3, HIGH);
  float distance = duration * 0.034 / 2;
  return distance;
}

// Function to read distance from ultrasonic sensor 4
float readDistance4() {
  digitalWrite(TRIG_PIN_4, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN_4, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN_4, LOW);
  unsigned long duration = pulseIn(ECHO_PIN_4, HIGH);
  float distance = duration * 0.034 / 2;
  return distance;
}


void readServoAngles() {
  // Read the current angles of the servos
  int currentAngle1 = servo1.read();
  int currentAngle2 = servo2.read();
  // Update the value of virtual pins V12 and V13 with the servo angles
  Blynk.virtualWrite(V12, currentAngle1);
  Blynk.virtualWrite(V13, currentAngle2);
}

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Initialize the GPIO pins to Enable motors A and B
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  
  digitalWrite(ENA, HIGH); // Turn on the first LED
  digitalWrite(ENB, HIGH); // Turn on the second LED

  // Set motor pins as outputs
  pinMode(IN_1, OUTPUT);
  pinMode(IN_2, OUTPUT);
  pinMode(IN_3, OUTPUT);
  pinMode(IN_4, OUTPUT);

  // Set ultrasonic sensor pins as inputs/outputs
  pinMode(TRIG_PIN_1, OUTPUT);
  pinMode(ECHO_PIN_1, INPUT);
  pinMode(TRIG_PIN_2, OUTPUT);
  pinMode(ECHO_PIN_2, INPUT);
  pinMode(TRIG_PIN_3, OUTPUT);
  pinMode(ECHO_PIN_3, INPUT);
  pinMode(TRIG_PIN_4, OUTPUT);
  pinMode(ECHO_PIN_4, INPUT);

  // Initialize Blynk for the Motor Control Project
  Blynk.config(auth_motor);

  // Initialize Blynk for Sensor Stream Project
 Blynk.config(auth_sensor);

  // Initialize Blynk for the Pan-Tilt Servo Project
  Blynk.config(auth_servo);
  
  Blynk.begin(ssid, pass);

  // Attach servo motors to GPIO pins
  servo1.attach(25); // Pin 19 for servo 1
  servo2.attach(26); // Pin 21 for servo 2

  pinMode(photoresistorPin, INPUT);
  pinMode(ledPin, OUTPUT);

  // Call functions when virtual pins change for Motor Control Project
  timer_motor.setInterval(100L, []() {
    int speed = Blynk.virtualRead(V5).asInt(); // Read speed from V5 slider
    function1(speed); // Call function 1 with speed
    function2(speed); // Call function 2 with speed
    function3(speed); // Call function 3 with speed
    function4(speed); // Call function 4 with speed
    function5(speed); // Call function 5 with speed
    function6(speed); // Call function 6 with speed
  });

  timer.setInterval(100L, servoControl);
  timer.setInterval(100L, readServoAngles);
  
  // Stream distances from ultrasonic sensors to Sensor Stream Project
  timer_sensor.setInterval(1000L, []() {
    float distance1 = readDistance1(); // Read distance from sensor 1
    float distance2 = readDistance2(); // Read distance from sensor 2
    float distance3 = readDistance3(); // Read distance from sensor 3
    float distance4 = readDistance4(); // Read distance from sensor 4
    Blynk.virtualWrite(V6, distance1); // Stream distance to V6
    Blynk.virtualWrite(V7, distance2); // Stream distance to V7
    Blynk.virtualWrite(V8, distance3); // Stream distance to V8
    Blynk.virtualWrite(V9, distance4); // Stream distance to V9
  });
}

void loop() {
  // Initiate Blynk
  Blynk.run();

  // Run BlynkTimer tasks for Motor Control Project
  timer_motor.run();
  // Run BlynkTimer tasks for Sensor Stream Project
  timer_sensor.run();
  // Run BlynkTimer tasks for Servo Project
  timer_servo.run();
  
  // Read the analog value from the photoresistor
  int lightIntensity = analogRead(photoresistorPin);

  // If light intensity is less than one-third of maximum intensity
  if (lightIntensity < 341) { // (1024 * 1/3 = 341)
    // Turn on the LED
    digitalWrite(ledPin, HIGH);
  } else {
    // Turn off the LED
    digitalWrite(ledPin, LOW);
  }

}

// Callback for slider widget to control motor speed
BLYNK_WRITE(V5) {
  int speed = param.asInt(); // Read speed from V5 slider
  // No need to call functions here as the speed is updated in the timer interval function
}

// Callbacks for button widgets to control motor state
BLYNK_WRITE(V1) {
  int buttonState = param.asInt();
  if (buttonState == HIGH) {
    function(speed);
}

BLYNK_WRITE(V2) {
  int buttonState = param.asInt();
  if (buttonState == HIGH) {
    function2(speed);
}

BLYNK_WRITE(V3) {
  int buttonState = param.asInt();
  
  if (buttonState == 1) {
    function3(speed); //Two wheel Turn
  } 
  else if (buttonState == 2) {
    function6(speed); //One wheel Turn
  } 

}

BLYNK_WRITE(V4) {
  int buttonState = param.asInt();
  
  if (buttonState == 1) {
    function4(speed); //Two wheel Turn
  } 
  else if (buttonState == 2) {
    function5(speed); //One wheel Turn
  } 
  
}

// Blynk Write callbacks for Sensor Stream Project to receive data from ultrasonic sensors
BLYNK_WRITE(V6) {
  float distance = param.asFloat(); // Get the distance value from the Blynk app
  // Update the Blynk widget with the received distance value
  Blynk.virtualWrite(V6, distance); 
}

// Similar BLYNK_WRITE() callbacks for V7, V8, and V9
BLYNK_WRITE(V7) {
  float distance = param.asFloat();
  Blynk.virtualWrite(V7, distance);
}

BLYNK_WRITE(V8) {
  float distance = param.asFloat();
  Blynk.virtualWrite(V8, distance);
}

BLYNK_WRITE(V9) {
  float distance = param.asFloat();
  Blynk.virtualWrite(V9, distance);
}


// Blynk Slider Widget callback for servo 1
BLYNK_WRITE(V10) {
  int pos = param.asInt();
  servo1.write(pos);
}

// Blynk Slider Widget callback for servo 2
BLYNK_WRITE(V11) {
  int pos = param.asInt();
  servo2.write(pos);
}

// Blynk Write callback to read and display first servo angle on the first value display widget
BLYNK_WRITE(V12) {
  // Read the current angle of the first servo
  int currentAngle1 = servo1.read();
  // Update the first value display widget with the current angle
  Blynk.virtualWrite(V12, currentAngle1);
}

// Blynk Write callback to read and display second servo angle on the second value display widget
BLYNK_WRITE(V13) {
  // Read the current angle of the second servo
  int currentAngle2 = servo2.read();
  // Update the second value display widget with the current angle
  Blynk.virtualWrite(V13, currentAngle2);
}
