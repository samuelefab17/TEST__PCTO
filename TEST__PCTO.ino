#include <Scheduler.h>
#include "arduinoFFT.h"
#include "MPU9250.h"
#include "simpleDSP_fft.h"

#define DATA_LEN 2
float L;
float N;
int i;

COMPLEX data[DATA_LEN] = {};

MPU9250 mpu;

int pin1 = 4u; //GP 4
int pin2 = 5u; //GP 5
int pin3 = 6u; //GP 6 
int pin4 = 7u; //GP 7

//#define SERIAL_BAUD 9600
#define SERIAL_BAUD 115200

#define LOOP2 0
#define LOOP3 0
#define LOOP4 0
#define LOOP5 0
#define LOOP6 0
#define KEEP_ALIVE 1

#define LOOP1_TIME 100 //ms
#define LOOP2_TIME 100  
#define LOOP3_TIME 100  
#define LOOP4_TIME 100       
#define LOOP5_TIME 1000 
#define LOOP6_TIME 2000

#define DUTY_CYCLE 50 //%

void function_pin(int, int, int);
void function_pin(int pin, int LOOP_TIME, int CYCLE) {

  int LOOP_TIME_DUTY_CYCLE = (LOOP_TIME * CYCLE) / 100;
  digitalWrite(pin, HIGH);
  delay(LOOP_TIME_DUTY_CYCLE);
  digitalWrite(pin, LOW);
  delay(LOOP_TIME_DUTY_CYCLE);

}

void setup() {
  
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  pinMode(pin3, OUTPUT);
  pinMode(pin4, OUTPUT);
  
  Serial.begin(SERIAL_BAUD);

  #if LOOP2 == 1
    Scheduler.startLoop(loop2);
  #endif

  #if LOOP3 == 1
    Scheduler.startLoop(loop3);
  #endif

  #if LOOP4 == 1
    Scheduler.startLoop(loop4);
  #endif

  #if LOOP5 == 1
    Scheduler.startLoop(loop5);
  #endif

  #if LOOP6 == 1
    Scheduler.startLoop(loop6);
  #endif

  #if KEEP_ALIVE == 1
    Scheduler.startLoop(keep_alive);
  #endif

  Wire.begin();
  mpu.setup(0x68);

  if (!mpu.setup(0x68)) {  
     while (1) {
       Serial.println("MPU connection failed.");
       delay(5000);
     }
   }

    mpu.verbose(true);  
    mpu.calibrateAccelGyro();
    delay(2500);
    mpu.calibrateMag();
    delay(500);
    mpu.verbose(false);

}

// Task n.1
void loop() {
  if (mpu.update()){
static uint32_t prev_ms = millis();
        if (millis() > prev_ms + 25) {
      Serial.print(mpu.getAccZ());
      Serial.print(" ; ");
      Serial.println(mpu.getMagX());
      delay(100);
      prev_ms = millis();
        }
  }
}

// Task n.2
void loop2() {

   function_pin(pin2, LOOP2_TIME, DUTY_CYCLE);

}

// Task n.3
void loop3() {

   function_pin(pin3, LOOP3_TIME, DUTY_CYCLE);

}

// Task n.4
void loop4(){

   function_pin(pin4, LOOP4_TIME, DUTY_CYCLE);

}

// Task n.5
void loop5(){

   float number1_LOOP5 = random(99999) / 100000.0;
   float number2_LOOP5 = random(99999) / 1000.0;
   float number3_LOOP5 = random(99999) / 10000.0;
   float number4_LOOP5 = random(99999) / 1000.0;

   double result1_LOOP5 = (number1_LOOP5 * number4_LOOP5) / (number2_LOOP5 * number3_LOOP5);
   double result2_LOOP5 = sqrt(result1_LOOP5);

   delay(LOOP5_TIME);

   yield();

}

// Task n.6
void loop6(){

    FFT(data, DATA_LEN);

    for (int i = 0; i < DATA_LEN; i++)
    {
        Serial.print(data[i].real / DATA_LEN);
        Serial.print("\t\t");
        Serial.println(data[i].imag / DATA_LEN);
    }
 
   delay(LOOP6_TIME);

   yield();
}

// Task n.7
void keep_alive(){

   digitalWrite(LED_BUILTIN, HIGH);
   delay(10000);
   digitalWrite(LED_BUILTIN, LOW);
   delay(100);

   yield();

}