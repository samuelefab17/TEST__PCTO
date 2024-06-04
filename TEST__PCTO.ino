#include <Scheduler.h>
#include "MPU9250.h"

float acce[12];
float magn[12];
volatile int acce_index;
volatile int magn_index;

MPU9250 mpu;

int pin1 = 4u; //GP 4
int pin2 = 5u; //GP 5
int pin3 = 6u; //GP 6 
int pin4 = 7u; //GP 7

//#define SERIAL_BAUD 9600
#define SERIAL_BAUD 115200

#define SERIAL_BAUD_COMMUNICATION 9600 //GP 0
//#define SERIAL_BAUD_COMMUNICATION 115200

#define LOOP2 1
#define LOOP3 1
#define LOOP4 0
#define LOOP5 0
#define LOOP6 0
#define KEEP_ALIVE 1

#define LOOP1_TIME 100 //ms
#define LOOP2_TIME 1000  
#define LOOP3_TIME 100  
#define LOOP4_TIME 100       
#define LOOP5_TIME 1000 
#define LOOP6_TIME 2000

#define DUTY_CYCLE 50 //%

void function_pin(int, int, int);
void function_pin(int pin, int LOOP_TIME, int CYCLE) 
{
  int LOOP_TIME_DUTY_CYCLE = (LOOP_TIME * CYCLE) / 100;
  digitalWrite(pin, HIGH);
  delay(LOOP_TIME_DUTY_CYCLE);
  digitalWrite(pin, LOW);
  delay(LOOP_TIME_DUTY_CYCLE);
}

void setup() 
{
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  pinMode(pin3, OUTPUT);
  pinMode(pin4, OUTPUT);
  
  Serial.begin(SERIAL_BAUD);
  Serial1.begin(SERIAL_BAUD_COMMUNICATION);

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

  if (!mpu.setup(0x68)) 
  {  
    while (1) 
    {
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
void loop() 
{
  if (mpu.update())
  {
    static uint32_t prev_ms = millis();
    if (millis() > prev_ms + LOOP1_TIME) 
    {
      float lett = mpu.getMagX();
      float let = mpu.getAccZ();
      acce[acce_index] = let;
      magn[magn_index] = lett;
      acce_index++;
      magn_index++;
      //Serial.println(acce_index);
      //Serial.print("Mag "); Serial.println(lett);
      //Serial.print("Acc "); Serial.println(let);
      prev_ms = millis();
    }    
  }
}
        
#define ELEM 10
// Task n.2
void loop2() 
{
  float sum;
  float ris;
  if(acce_index == (ELEM - 1))
  {
    for (int i = 0; i < ELEM; i++)	
		 sum = sum + acce[i];
	  ris = sum /ELEM;
    sum = 0;
    //Serial.print("d: "); Serial.println(acce_index);
    Serial.print("r: "); Serial.print(ris);
    acce_index = 0;
  }
  delay(10);
}

// Task n.3
void loop3() 
{
  float sum;
  float ris;
  if(magn_index == (ELEM - 1))
  {
    for (int i = 0; i < ELEM; i++)	
		  sum = sum + magn[i];
	  ris = sum /ELEM;
    sum = 0;
    Serial.print("r: "); Serial.print(ris);
    magn_index = 0;
  }
  delay(10);
}

// Task n.4
void loop4()
{
  char stringa[20] = "Hello World";
  Serial1.write(stringa, 20); 
  delay(1000);
}

// Task n.5
void loop5()
{
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
void loop6()
{

}

// Task n.7
void keep_alive()
{
  digitalWrite(LED_BUILTIN, HIGH);
  delay(10000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);

  yield();
}