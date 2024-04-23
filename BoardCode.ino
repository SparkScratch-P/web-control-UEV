#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Servo.h>

// WiFi credentials for Motor Control Project
char ssid = "YourWiFiSSID_motor";
char pass = "YourWiFiPassword_motor";
char auth_motor[] = "Auth-Token-MotorProject";

// WiFi credentials for Sensor Stream Project
char auth_sensor[] = "Auth-Token-SensorProject";

// WiFi credentials for Pan-tilt Servo Project
char auth_sensor[] = "Auth-Token-ServoProject";

// Pins connected to the motors
#define IN_1 2 // Change to your PWM pin for motor 1
#define IN_2 4 // Change to your PWM pin for motor 2
#define IN_3 5 // Change to your PWM pin for motor 3
#define IN_4 18 // Change to your PWM pin for motor 4

// Pins connected to the ultrasonic sensors
#define TRIG_PIN_1 19 // Change to your trigger pin for sensor 1
#define ECHO_PIN_1 23 // Change to your echo pin for sensor 1
#define TRIG_PIN_2 22 // Change to your trigger pin for sensor 2
#define ECHO_PIN_2 21 // Change to your echo pin for sensor 2
#define TRIG_PIN_3 17 // Change to your trigger pin for sensor 3
#define ECHO_PIN_3 16 // Change to your echo pin for sensor 3
#define TRIG_PIN_4 15 // Change to your trigger pin for sensor 4
#define ECHO_PIN_4 14 // Change to your echo pin for sensor 4

const int ENA = 22; // Enable Motor A Pin
const int ENB = 23; // Enable Motor B Pin

// Create servo objects
Servo servo1;
Servo servo2;

// Initialize the Blynk objects for Motor Control Project and Sensor Stream Project
BlynkTimer timer_motor;
BlynkTimer timer_sensor;

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
  Blynk.begin(auth_motor, ssid, pass);

  // Initialize Blynk for Sensor Stream Project
  Blynk.begin(auth_sensor, ssid, pass);

  // Initialize Blynk for the Pan-Tilt Servo Project
  Blynk.config(auth_servo);
  Blynk.begin(ssid, pass);

  // Attach servo motors to GPIO pins
  servo1.attach(19); // Pin 19 for servo 1
  servo2.attach(21); // Pin 21 for servo 2

  // Call functions when virtual pins change for Motor Control Project
  timer_motor.setInterval(100L, []() {
    int speed = Blynk.virtualRead(V5).asInt(); // Read speed from V5 slider
    function1(speed); // Call function 1 with speed
    function2(speed); // Call function 2 with speed
    function3(speed); // Call function 3 with speed
    function4(speed); // Call function 4 with speed
  });

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
  // Run Blynk tasks for Motor Control Project
  Blynk.run();
  // Run Blynk tasks for Sensor Stream Project
  Blynk.run();
  // Run BlynkTimer tasks for Motor Control Project
  timer_motor.run();
  // Run BlynkTimer tasks for Sensor Stream Project
  timer_sensor.run();
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
  if (buttonState == HIGH) {
    function3(speed);
}

BLYNK_WRITE(V4) {
  int buttonState = param.asInt();
  if (buttonState == HIGH) {
    function4(speed);
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
