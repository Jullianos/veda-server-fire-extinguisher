const int fanPin = 2;
const int alarmPin = 4;
const int extPin = 3;
const int buzzerPin = 11;

const int manualPin = 10;
const int sensor1Pin = 7;
const int sensor2Pin = 8;
const int doorSwithPin = 9;
int manualSensorValue = 0;
int preSensorValue = 0;
int sensorValue = 0;
bool extSet = false;
bool fanSet = false;
bool manualSet = false;
bool beep = false;

int beepDelay = 500;
int extDelay = 15000; // Delay before the fire extinguisher and the fan start (ms)
bool extraDelay = false;
int manualDelay = 15000; // Delay before the fire extinguisher and the fan start trough the manual override (ms)
int initialManualDelay = manualDelay;
int fastBeep = beepDelay / 2;

int doorValue = 0;
bool alarmSet = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(fanPin, OUTPUT);
  pinMode(alarmPin, OUTPUT);
  pinMode(extPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  
  pinMode(manualPin, INPUT);
  pinMode(sensor1Pin, INPUT);
  pinMode(sensor2Pin, INPUT);
  pinMode(doorSwithPin, INPUT);
}

void loop() {
  manualSensorValue = digitalRead(manualPin);
  preSensorValue = digitalRead(sensor1Pin);
  sensorValue = digitalRead(sensor2Pin);
  doorValue = digitalRead(doorSwithPin);


  if (sensorValue == 1) {
    extSet = true;
    fanSet = true;
    extraDelay = true;
  }

  
  if (manualSensorValue == 1) {
    manualSet = true;
  }

  if (extSet == true && fanSet == true) {
    if (extraDelay == true) {
      delay(extDelay);
    }            
    digitalWrite(extPin, HIGH);
    digitalWrite(fanPin, HIGH);
  } else {
    digitalWrite(extPin, LOW);
    digitalWrite(fanPin, LOW);
  }

  if (manualSet == true) {
    if (beep == true && manualDelay > 0) {
      beep = false;
      digitalWrite(buzzerPin, HIGH);
    } else {
      beep = true;
      digitalWrite(buzzerPin, LOW);
    }
    if (manualDelay >= 0) {
      if (manualDelay <= (initialManualDelay / 4)) {
        delay(fastBeep);
        manualDelay = manualDelay - beepDelay;
      } else {
        delay(beepDelay);
        manualDelay = manualDelay - beepDelay;
      }
    }
    if (manualDelay < 0) {
      extSet = true;
      fanSet = true;
      alarmSet = true;
    }
  }

  if (doorValue == 1 || preSensorValue == 1) {
    alarmSet = true;
  }

  if (alarmSet == true) {
    digitalWrite(alarmPin, HIGH);
    digitalWrite(buzzerPin, HIGH);
  } else {
    digitalWrite(buzzerPin, LOW);
    digitalWrite(alarmPin, LOW);
  }

  // DEBUG PURPOSES
  Serial.print(preSensorValue);
  Serial.print(" ");
  Serial.print(sensorValue);
  Serial.print(" ");
  Serial.print(doorValue);
  Serial.print(" ext: ");
  Serial.print(extSet);
  Serial.print(" fan: ");
  Serial.print(fanSet);
  Serial.print(" alarm: ");
  Serial.print(alarmSet);
  Serial.print(" - ");
  Serial.println(manualDelay);

  delay(100);
}
