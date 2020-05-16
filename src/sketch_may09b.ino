#include <NewPing.h>

#define TRIGGER_PIN  12
#define ECHO_PIN     11
#define LED_PIN     2
#define MAX_DISTANCE 300

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

void setup() {
 // Serial.begin(115200);
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  delay(50);
  int cm=sonar.ping_cm();
  Serial.print("Ping: ");
  Serial.print(cm);
  Serial.println("cm");

  if(cm !=0 && cm<10) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }

}
