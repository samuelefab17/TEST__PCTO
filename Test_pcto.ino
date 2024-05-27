#include <KickFFT.h>
#include <Scheduler.h>

const uint16_t samples = 128;

const int16_t input[] = {773, 702, 515, 389, 370, 447, 554, 601, 561, 492, 455, 458, 480, 497, 545, 724, 801, 651, 460, 370, 396, 494, 579, 570, 507, 450, 433, 449, 480, 499, 500, 591, 756, 736, 548, 396, 356, 416, 522, 577, 549, 479, 436, 430, 455, 485, 502, 504, 496, 570, 737, 731, 540, 391, 362, 436, 539, 580, 543, 471, 428, 427, 450, 473, 486, 492, 485, 471, 486, 635, 748, 639, 455, 354, 363, 461, 560, 577, 518, 463, 443, 458, 482, 503, 521, 515, 499, 585, 735, 710, 536, 402, 368, 422, 520, 584, 551, 487, 444, 433, 448, 478, 496, 502, 505, 595, 753, 695, 514, 383, 366, 433, 529, 584, 562, 498, 447, 437, 454, 479, 494, 500, 530, 681, 752, 622, 450, 366, 384, 468, 557, 584, 534, 469, 435, 438, 464, 495, 517, 526, 634, 777, 730, 544, 402, 366, 424, 528, 602, 574, 499, 450, 440, 457, 481, 500, 520, 658, 781, 678, 485, 374, 374, 447, 547, 599, 563, 491, 442, 439, 459, 480, 495, 567, 742, 768, 599, 421, 350, 377, 481, 574, 590, 528, 458, 430, 438, 464, 488, 510, 640, 776, 689, 502, 379, 363, 437, 546, 605, 581, 507, 452, 438, 455, 478, 504, 633, 790, 740, 541, 390, 356, 418, 520, 588, 573, 506, 450, 430, 444, 472, 581, 770, 764, 558, 383, 327, 377, 477, 564, 573, 514, 452, 425, 428, 449, 556, 752, 762, 575, 398, 337, 385, 496, 595, 590, 522, 460, 440, 451, 484, 650, 810, 723, 521, 389};
const float Fs = 23.8;

int pin1 = 4u; //GP 4
int pin2 = 5u; //GP 5
int pin3 = 6u; //GP 6 
int pin4 = 7u; //GP 7

#define SERIAL_BAUD 9600
//#define SERIAL_BAUD 115200

#define LOOP2 1
#define LOOP3 1
#define LOOP4 1
#define LOOP5 1
#define LOOP6 1
#define LOOP7 1

#define LOOP1_TIME 100 
#define LOOP2_TIME 100  
#define LOOP3_TIME 100  
#define LOOP4_TIME 100       
#define LOOP5_TIME 1000 
#define LOOP6_TIME 1000

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

}

// Task n.1
void loop() {

   function_pin(pin1, LOOP1_TIME);

}

// Task n.2
void loop2() {

   function_pin(pin2, LOOP2_TIME);

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
   Serial.print("Loop 5 :");
   Serial.println(result2_LOOP5);

   delay(LOOP5_TIME);

   yield();

}

// Task n.6
void loop6(){

  Serial.begin(9600); //Use SerialUSB for SparkFun SAMD21 boards
  while(!Serial); //will not run until Serial Monitor is open

  uint32_t mag[samples] = {0};
  uint16_t startIndex = 0;
  uint16_t endIndex = 0;
  KickFFT<int16_t>::fft(Fs, 0, Fs/2, samples, input, mag, startIndex, endIndex);


  //Print to Serial Monitor and copy and paste
  //into a .csv file to display in Excel
  Serial.println("Freq(Hz),Magnitude"); //Use SerialUSB for SparkFun SAMD21 boards
  for(uint16_t i = startIndex; i < endIndex; i++)
  {
    //Peak should be around 1.3 Hz or so
    
    Serial.print(i*Fs/samples); //Use SerialUSB for SparkFun SAMD21 boards
    Serial.print(","); //Use SerialUSB for SparkFun SAMD21 boards
    Serial.print(mag[i]); //Use SerialUSB for SparkFun SAMD21 boards
    Serial.println(); //Use SerialUSB for SparkFun SAMD21 boards
  }

  //Print the Raw Signal
  Serial.println("Time(ms),PPG"); //Use SerialUSB for SparkFun SAMD21 boards
  for(uint16_t i = 0; i < samples; i++)
  {
    Serial.print(i*1/Fs); //Use SerialUSB for SparkFun SAMD21 boards
    Serial.print(","); //Use SerialUSB for SparkFun SAMD21 boards
    Serial.print(input[i]); //Use SerialUSB for SparkFun SAMD21 boards
    Serial.println(); //Use SerialUSB for SparkFun SAMD21 boards
  }

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