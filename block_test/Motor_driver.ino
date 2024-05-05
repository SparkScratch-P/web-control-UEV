#include <WiFi.h>
#include <WiFiClient.h>
#include <Esp32.h>
#include <BlynkSimpleEsp32.h>
#include <Servo.h>

//Connect to Blynk IoT
#define BLYNK_TEMPLATE_ID " " // Edit
#define BLYNK_TEMPLATE_NAME "Web Control UEV"
#define BLYNK_DEVICE_NAME "Web Control UEV"
#define BLYNK_AUTH_TOKEN " " // Edit
#define BLYNK_PRINT Serial

// WiFi credentials
char ssid = "YourWiFiSSID";
char pass = "YourWiFiPassword";

 // Coonect Motor Control Project
char auth_motor[] = "Auth-Token-MotorProject";

// Pins connected to the motors
#define IN_1 2 // Connect to Motor Control Module's pin, Input 1
#define IN_2 4 // Connect to Motor Control Module's pin, Input 2
#define IN_3 5 // Connect to Motor Control Module's pin, Input 3
#define IN_4 18 // Connect to Motor Control Module's pin, Input 4
#define ENA 22 // Enable Motor A Pin
#define ENB 15 // Enable Motor B Pin


// Initialize the Blynk objects for Motor Control Project and Sensor Stream Project and Servo Project
BlynkTimer timer_motor;

//Linear Motion Functions

// Function to control motor speed for function 1
void forward(int speed) {
  // Set speed of alternate motors
  analogWrite(IN_1, speed);
  analogWrite(IN_3, speed);
  analogWrite(IN_2, 0);
  analogWrite(IN_4, 0);
}

// Function to control motor speed for function 2
void reverse(int speed) {
  // Set speed of alternate motors
  analogWrite(IN_1, 0);
  analogWrite(IN_3, 0);
  analogWrite(IN_2, speed);
  analogWrite(IN_4, speed);
}

//Turn Functions

// Function to control motor speed for function 3
void right(int speed) {
  // Set speed of motors 2 and 3
  analogWrite(IN_1, 0);
  analogWrite(IN_4, 0);
  analogWrite(IN_2, speed);
  analogWrite(IN_3, speed);
}

// Function to control motor speed for function 4
void left(int speed) {
  // Set speed of motors 1 and 4
  analogWrite(IN_2, 0);
  analogWrite(IN_3, 0);
  analogWrite(IN_1, speed);
  analogWrite(IN_4, speed);
}

// Function to control single motor speed for function 5
void leftrad(int speed) {
  // Set speed of motors 1 and 4
  analogWrite(IN_2, 0);
  analogWrite(IN_3, 0);
  analogWrite(IN_1, 0);
  analogWrite(IN_4, speed);
}

// Function to control single motor speed for function 6
void rightrad(int speed) {
  // Set speed of motors 1 and 4
  analogWrite(IN_2, 0);
  analogWrite(IN_4, 0);
  analogWrite(IN_1, 0);
  analogWrite(IN_3, speed);


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
  
  // Initialize Blynk for the Motor Control Project
  Blynk.config(auth_motor);
  
  Blynk.begin(ssid, pass);

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
}

void loop() {
  // Initiate Blynk
  Blynk.run();

  // Run BlynkTimer tasks for Motor Control Project
  timer_motor.run();
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
    forward(speed);
}

BLYNK_WRITE(V2) {
  int buttonState = param.asInt();
  if (buttonState == HIGH) {
    reverse(speed);
}

BLYNK_WRITE(V3) {
  int buttonState = param.asInt();
  
  if (buttonState == 1) {
    right(speed); //Two wheel Turn
  } 
  else if (buttonState == 2) {
    rightrad(speed); //One wheel Turn
  } 

}

BLYNK_WRITE(V4) {
  int buttonState = param.asInt();
  
  if (buttonState == 1) {
    left(speed); //Two wheel Turn
  } 
  else if (buttonState == 2) {
    leftrad(speed); //One wheel Turn
  } 
  
