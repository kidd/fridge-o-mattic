#include <NewPing.h>
#include <time.h>
#include <math.h>
#include <stdio.h>

#define TRIGGER_PIN  12
#define ECHO_PIN     11
#define LED_PIN     2
#define MAX_DISTANCE 300

#define ALARM_SET_PIN 2
#define FALSE 0
#define TRUE -1

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

int base;
unsigned long last_fixed_timestamp;
unsigned long now;
int in_flux;

void set_base(int cm){
  base = cm;
}

void setup() {
 // Serial.begin(115200);
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
}

int sameish(int a, int b){
  int e = 03;
  return (abs(a-b) < e) ;
}


void quick_flash() {
  digitalWrite(ALARM_SET_PIN, HIGH);
  set_base(100000);
  last_fixed_timestamp = 10000000000;
}

unsigned long time_deadline = 0;
unsigned long cm_deadline = 0;

void set_chrono(int cm) {
  time_deadline = millis() + (((unsigned long)cm - 10) * 1000 * 60 * 0.5);
  cm_deadline = cm;
  Serial.print("chrono at: ");
  Serial.println(time_deadline);
  Serial.print("cm: ");
  Serial.println(cm);
  Serial.print("time: ");
  Serial.println(time_deadline);

}

void update_flags(int cm) {
  if ((time_deadline != 0) && (time_deadline <= now) && (time_deadline+5000 >=now)) {
    quick_flash();
  } else if ((time_deadline != 0) && (time_deadline <= now)) {
    digitalWrite(ALARM_SET_PIN, LOW);
    time_deadline = 0;
  } else if ((time_deadline != 0) && (time_deadline > now)) {
    /* alarm set, not there yet */
  } else if (sameish(cm, cm_deadline) || (cm == 0) ) {
    /* not moved since last chrono. don't do shit. */
  } else if (sameish(base, cm) && (now > last_fixed_timestamp + 5000) ) {
    Serial.print("chrono set to ");
    Serial.println(cm);
    set_chrono(cm);
  } else if (sameish(base, cm)) {
    Serial.println(".....steady....");
  } else {
    Serial.println("stay put..");
    digitalWrite(ALARM_SET_PIN, LOW);
    base = cm ;
    last_fixed_timestamp = millis();
  }
}

/* This could be a pointer to have a stm! */
void loop() {
  delay(500);                    /* This could be a var */
  now = millis();
  int cm=sonar.ping_cm();

  Serial.print("Ping: ");
  Serial.print(cm);
  Serial.println("cm");
  Serial.print("ts:");
  Serial.println(now);

  if(cm !=0 && cm<10) {
    set_base(cm);
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
    update_flags(cm);
  }

}
