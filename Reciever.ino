#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

Servo Servo1;
Servo Servo2;
int Servo1Val; //Values mapped to 0-180 from intVals
int Servo2Val;
int intXVal; //Values directly from RF24 0-1024
int intYVal;

struct RECEIVE_DATA_STRUCTURE {
  uint16_t XVal;
  uint16_t YVal;
};

RECEIVE_DATA_STRUCTURE mydata_remote;

RF24 radio(7, 8); // CSN, CE
const byte address[6] = "00001";

unsigned long currentMillis;
long previousMillis = 0;
long interval = 10;
unsigned long previousSafetyMillis;

void setup() {
  Serial.begin(115200);

  radio.begin();
  radio.openReadingPipe(1, address[0]);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  Servo1.attach(10);
  Servo2.attach(9);

  Servo1.write(90);
  Servo2.write(90);
}

void loop() {
  currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    if (radio.available()) {
      radio.read(&mydata_remote, sizeof(RECEIVE_DATA_STRUCTURE));
    } else {
      Serial.println("no data");
      Servo1Val = 90;
      Servo2Val = 90;
    }
    intXVal = mydata_remote.XVal;
    intYVal = mydata_remote.YVal;

    Servo1Val = map(intXVal, 0, 1024, 0, 180);
    Servo2Val = map(intYVal, 0, 1024, 180, 0);
    Servo1.write(Servo1Val);
    Servo2.write(Servo2Val);
  }

  Serial.print("Servo 1: ");
  Serial.println(Servo1Val);
  Serial.print(" Servo 2: ");
  Serial.println(Servo2Val);
}
