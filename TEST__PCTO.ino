#include "MPU9250.h"
#include "arduinoFFT.h"
#include <Wire.h>

MPU9250 mpu;

#define CHANNEL l
const uint16_t samples =64; 
const float samplingFrequency = 50; 
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

void setup() {
 sampling_period_us = round(1000000*(1.0/samplingFrequency));

  Wire.begin();
  mpu.setup(0x68);
  delay(2000);
   mpu.calibrateAccelGyro();

  
}

void loop() {
      if (mpu.update()) {
        l = mpu.getAccZ();
        delay(10);
    }
  microseconds = micros();
  for(int i=0; i<samples; i++)
  {
      vReal[i] = l;
      vImag[i] = 0;
      while(micros() - microseconds < sampling_period_us){
      }
      microseconds += sampling_period_us;
  }

  FFT.windowing(FFTWindow::Hamming, FFTDirection::Forward);	/* Weigh data */
  PrintVector(vReal, samples, SCL_TIME);
  FFT.compute(FFTDirection::Forward); /* Compute FFT */
  PrintVector(vReal, samples, SCL_INDEX);
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
  Serial.println(l);
}