#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

char auth[] = "YourAuthToken";
char ssid[] = "YourNetworkName";
char pass[] = "YourPassword";

#define LED1_PIN 5
#define LED2_PIN 4
#define LED3_PIN 2
#define LED4_PIN 15

BlynkTimer timer;

void setup()
{
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);

  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  pinMode(LED4_PIN, OUTPUT);

  timer.setInterval(100L, []() {});
}


void loop()
{
  Blynk.run();
  timer.run();
}

BLYNK_WRITE(V0)
{
  int value = param.asInt();
  analogWrite(LED4_PIN, value);
}

BLYNK_WRITE(V1)
{
  digitalWrite(LED1_PIN, HIGH);
}

BLYNK_WRITE(V2)
{
  digitalWrite(LED2_PIN, HIGH);
}

BLYNK_WRITE(V3)
{
  digitalWrite(LED3_PIN, HIGH);
}
