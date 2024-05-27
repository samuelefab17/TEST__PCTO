#include <Scheduler.h>
#include "arduinoFFT.h"
#include "MPU9250.h"

MPU9250 mpu;

#define CHANNEL l
const uint16_t samples = 64; 
const float samplingFrequency = 100; 
unsigned int sampling_period_us;
unsigned long microseconds;

float vReal[samples];
float vImag[samples];
float l;

ArduinoFFT<float> FFT = ArduinoFFT<float>(vReal, vImag, samples, samplingFrequency, true);

#define SCL_INDEX 0x00
#define SCL_TIME 0x01
#define SCL_FREQUENCY 0x02
#define SCL_PLOT 0x03

int pin1 = 4u; //GP 4
int pin2 = 5u; //GP 5
int pin3 = 6u; //GP 6 
int pin4 = 7u; //GP 7

#define SERIAL_BAUD 9600
//#define SERIAL_BAUD 115200

#define LOOP2 1
#define LOOP3 0
#define LOOP4 0
#define LOOP5 0
#define LOOP6 1
#define LOOP7 0

#define LOOP1_TIME 100 
#define LOOP2_TIME 100  
#define LOOP3_TIME 100  
#define LOOP4_TIME 100       
#define LOOP5_TIME 1000 
#define LOOP6_TIME 2000

void function_pin(int pin, int LOOP_TIME) {

  digitalWrite(pin, HIGH);
  delay(LOOP_TIME);
  digitalWrite(pin, LOW);
  delay(LOOP_TIME);

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

  #if LOOP7 == 1
    Scheduler.startLoop(loop7);
  #endif

  sampling_period_us = round(1000000*(1.0/samplingFrequency));

  Wire.begin();
  mpu.setup(0x68);

}

// Task n.1
void loop() {

       if (mpu.update()) {
        l = mpu.getAccZ();
    }

}

// Task n.2
void loop2() {

   function_pin(pin1, LOOP1_TIME);

}

// Task n.3
void loop3() {

   function_pin(pin3, LOOP3_TIME);

}

// Task n.4
void loop4(){

   function_pin(pin4, LOOP4_TIME);

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

  microseconds = micros();
  for(int i=0; i<samples; i++)
  {
      vReal[i] = l;
      vImag[i] = 0;
      while(micros() - microseconds < sampling_period_us){
      }
      microseconds += sampling_period_us;
  }

  PrintVector(vReal, samples, SCL_TIME);
  FFT.windowing(FFTWindow::Hamming, FFTDirection::Forward);	/* Weigh data */
  PrintVector(vReal, samples, SCL_TIME);
  FFT.compute(FFTDirection::Forward); /* Compute FFT */
  PrintVector(vReal, samples, SCL_INDEX);
  PrintVector(vImag, samples, SCL_INDEX);
  FFT.complexToMagnitude(); /* Compute magnitudes */
  PrintVector(vReal, (samples >> 1), SCL_FREQUENCY);
  float x = FFT.majorPeak();
}

void PrintVector(float *vData, uint16_t bufferSize, uint8_t scaleType)
{
  for (uint16_t i = 0; i < bufferSize; i++)
  {
    float abscissa;
    /* Print abscissa value */
    switch (scaleType)
    {
      case SCL_INDEX:
        abscissa = (i * 1.0);
	break;
      case SCL_TIME:
        abscissa = ((i * 1.0) / samplingFrequency);
	break;
      case SCL_FREQUENCY:
        abscissa = ((i * 1.0 * samplingFrequency) / samples);
	break;
    }
    Serial.print(abscissa, 6);
    if(scaleType==SCL_FREQUENCY)
      Serial.print("Hz");
    Serial.print(" ");
    Serial.println(vData[i], 4);
  }
  Serial.println();
 
   delay(LOOP6_TIME);

   yield();
}

// Task n.7
void loop7(){

   digitalWrite(LED_BUILTIN, HIGH);
   delay(10000);
   digitalWrite(LED_BUILTIN, LOW);
   delay(100);

   yield();

}