//Drum sequencer by James Wagner. For use with arduino mega2560


#include <Wire.h>
#include "Adafruit_Trellis.h"

/*************************************************** 
  This example shows reading buttons and setting/clearing buttons in a loop
  "momentary" mode has the LED light up only when a button is pressed
  "latching" mode lets you turn the LED on/off when pressed

  Up to 8 matrices can be used but this example will show 4 or 1
 ****************************************************/

#define MOMENTARY 0
#define LATCHING 1
// set the mode here
#define MODE LATCHING 


Adafruit_Trellis matrix0 = Adafruit_Trellis();
Adafruit_Trellis matrix1 = Adafruit_Trellis();
Adafruit_Trellis matrix2 = Adafruit_Trellis();
Adafruit_Trellis matrix3 = Adafruit_Trellis();
Adafruit_Trellis matrix4 = Adafruit_Trellis();
Adafruit_Trellis matrix5 = Adafruit_Trellis();
Adafruit_Trellis matrix6 = Adafruit_Trellis();
Adafruit_Trellis matrix7 = Adafruit_Trellis();

Adafruit_TrellisSet trellis =  Adafruit_TrellisSet(&matrix0, &matrix1, &matrix2, &matrix3,&matrix4, &matrix5, &matrix6, &matrix7);

// set to however many you're working with here, up to 8
#define NUMTRELLIS 8

#define numKeys (NUMTRELLIS * 16)

// Connect Trellis Vin to 5V and Ground to ground.
// Connect the INT wire to pin #A2 (can change later!)
#define INTPIN A2
// Connect I2C SDA pin to your Arduino SDA line
// Connect I2C SCL pin to your Arduino SCL line
// All Trellises share the SDA, SCL and INT pin! 
// Even 8 tiles use only 3 wires max

//////// NOW THE DRUMS

//sequence specs
#define NUMBER_OF_STEPS_PER_BEAT 4
#define NUMBER_OF_BARS 1
#define NUMBER_OF_BEATS_PER_BAR 4
#define NUMBER_OF_BEATS (NUMBER_OF_BARS*NUMBER_OF_BEATS_PER_BAR)
#define NUMBER_OF_DRUMS 7
#define REST 0
#define NUMBER_OF_STEPS (NUMBER_OF_STEPS_PER_BEAT*NUMBER_OF_BEATS+REST)
#define SEQUENCE_LENGTH (NUMBER_OF_STEPS*NUMBER_OF_DRUMS)

//sequence order
#define SNARE_OFFSET  0
#define KICK_OFFSET   1
#define HAT_OFFSET    2
#define CRASH_OFFSET  3
#define TOM1_OFFSET   4
#define RIDE_OFFSET   5
#define FTOM_OFFSET   6

//hit strengths. Provides ability to accent notes
#define HARD    255
#define MED     230
#define SOFT    200
#define NO_HIT  0

//interupt pins
const byte PULSE_IN = 18; // The beats from the pi
//const byte MUTE_IN = 19; // Will mute/unmute the drums


//output drum pins
#define SNARE 8
#define KICK  7
#define HAT   3
#define CRASH 2
#define TOM1  9
#define RIDE  5
#define FTOM  6

//timer
#define CLOCK_SPEED   16*10^6 //16 MHz
#define PRESCALER     1024
#define TIMER_RES     (1 / ( CLOCK_SPEED / PRESCALER))
#define TIMER_TIME    5 //ms

//#define TIMER_COUNTS  ((TIMER_TIME*10^-3 / TIMER_RES) -1)
#define TIMER_COUNTS 77

//drum times: how long each drum strike is
#define KICK_TIME   150 //ms
#define SNARE_TIME  80  //ms  
#define HAT_TIME    80  //ms
#define CRASH_TIME  150  //ms
#define TOM1_TIME   80
#define RIDE_TIME   80
#define FTOM_TIME   80

//#define KICK_TIME   0 //ms
//#define SNARE_TIME  10  //ms  
//#define HAT_TIME    0  //ms
//#define CRASH_TIME  0  //ms
//#define TOM1_TIME   0
//#define RIDE_TIME   0
//#define FTOM_TIME   0

//interupt stuff
volatile bool kick_active;
volatile bool snare_active;
volatile bool hat_active;
volatile bool crash_active;
volatile bool tom1_active;
volatile bool ride_active;
volatile bool ftom_active;

volatile int s_multiple_of_5;
volatile int k_multiple_of_5;
volatile int h_multiple_of_5;
volatile int c_multiple_of_5;
volatile int t1_multiple_of_5;
volatile int r_multiple_of_5;
volatile int ft_multiple_of_5;

unsigned int sequence[16*7];

//index to sequence array
volatile int seq_count;

volatile bool mute_flag;

volatile bool usr_ctrl = true;

//pwm stuff
int eraser = 7;

int loop_count = 0;


void mute(){
  mute_flag = true;
}

void unmute(){
  seq_count = 0;
  
  mute_flag = false;
}

void phase_rst(){
  seq_count = 0;   
}


void write_drums_high()
{

  if(mute_flag == false)
  { 
    bool is_hit = false;
    
    if(snare_active==false&&sequence[seq_count*NUMBER_OF_DRUMS + SNARE_OFFSET] != NO_HIT)
    {
      analogWrite(SNARE,sequence[seq_count*NUMBER_OF_DRUMS + SNARE_OFFSET]);   //snare
      snare_active = true;
      is_hit = true;
    }
    
    if(kick_active==false&&sequence[seq_count*NUMBER_OF_DRUMS + KICK_OFFSET] != NO_HIT)
    {
      analogWrite(KICK,sequence[seq_count*NUMBER_OF_DRUMS + KICK_OFFSET]);  //kick
      kick_active = true;
      is_hit = true;
    }
    
    if(hat_active==false&&sequence[seq_count*NUMBER_OF_DRUMS + HAT_OFFSET] != NO_HIT) 
    {
      analogWrite(HAT,sequence[seq_count*NUMBER_OF_DRUMS + HAT_OFFSET]);  //hat
      hat_active = true;
      is_hit = true;
    }
    
    if(crash_active==false&&sequence[seq_count*NUMBER_OF_DRUMS + CRASH_OFFSET] != NO_HIT) 
    {
      analogWrite(CRASH,sequence[seq_count*NUMBER_OF_DRUMS + CRASH_OFFSET]);  //crash
      crash_active = true;
      is_hit = true;
    }
    
    if(tom1_active==false&&sequence[seq_count*NUMBER_OF_DRUMS + TOM1_OFFSET] != NO_HIT) 
    {
      analogWrite(TOM1,sequence[seq_count*NUMBER_OF_DRUMS + TOM1_OFFSET]);  //tom1
      tom1_active = true;
      is_hit = true;
    }
    
    if(ride_active==false&&sequence[seq_count*NUMBER_OF_DRUMS + RIDE_OFFSET] != NO_HIT) 
    {
      analogWrite(RIDE,sequence[seq_count*NUMBER_OF_DRUMS + RIDE_OFFSET]);  //ride
      ride_active = true;
      is_hit = true;
    }
    
    if(ftom_active==false&&sequence[seq_count*NUMBER_OF_DRUMS + FTOM_OFFSET] != NO_HIT) 
    {
      analogWrite(FTOM,sequence[seq_count*NUMBER_OF_DRUMS + FTOM_OFFSET]);  //ftom
      ftom_active = true;
      is_hit = true;
    }
//    if (is_hit) {
//      begin_5_timer();
//    }
         
    seq_count++;
    seq_count = seq_count % NUMBER_OF_STEPS;
  }

}

ISR(TIMER1_COMPA_vect)
{

  if(snare_active == true){
    s_multiple_of_5++; //another 5ms passed
  }

  if(kick_active == true)
    k_multiple_of_5++; //another 5ms passed

  if(hat_active == true)
    h_multiple_of_5++; //another 5ms passed

  if(crash_active == true)
    c_multiple_of_5++; //another 5ms passed

  if(tom1_active == true)
    t1_multiple_of_5++; //another 5ms passed

  if(ride_active == true)
     r_multiple_of_5++; //another 5ms passed

  if(ftom_active == true)
     ft_multiple_of_5++; //another 5ms passed

  
  if(snare_active==true&&s_multiple_of_5==(SNARE_TIME/TIMER_TIME)){
    analogWrite(SNARE,0); 
    snare_active=false;
    s_multiple_of_5 = 0;
  }
  if(kick_active==true&&k_multiple_of_5==(KICK_TIME/TIMER_TIME)){
    analogWrite(KICK,0); 
    kick_active=false;
    k_multiple_of_5 = 0;
  }
  if(hat_active==true&&h_multiple_of_5==(HAT_TIME/TIMER_TIME)){
    analogWrite(HAT,0);
    hat_active=false;
    h_multiple_of_5 = 0;
  }
  if(crash_active==true&&c_multiple_of_5==(CRASH_TIME/TIMER_TIME)){
    analogWrite(CRASH,0); 
    crash_active=false;
    c_multiple_of_5 = 0;
  }
  
  if(tom1_active==true&&t1_multiple_of_5==(TOM1_TIME/TIMER_TIME)){
    analogWrite(TOM1,0); 
    tom1_active=false;
    t1_multiple_of_5 = 0;
  }
  if(ride_active==true&&r_multiple_of_5==(RIDE_TIME/TIMER_TIME)){
    analogWrite(RIDE,0); 
    ride_active=false;
    r_multiple_of_5 = 0;
  }
  
  if(ftom_active==true&&ft_multiple_of_5==(FTOM_TIME/TIMER_TIME)){
    analogWrite(FTOM,0); 
    ftom_active=false;
    ft_multiple_of_5 = 0;
  }
  
  begin_5_timer();
  
}

void begin_5_timer()
{
  TCCR1A = 0;
  TCCR1B = 0;//stop timer
  
  OCR1A = TIMER_COUNTS; //will count to 5ms
  TCCR1B |= (1 << WGM12); //compare mode
  TCCR1B |= (1 << CS10);// 1024 prescaler
  TCCR1B |= (1 << CS12);
  TIMSK1 |= (1 << OCIE1A);//enable compare interrupt
}

void set_pwm_2_3_5_6_7_8_9_10()
{
  //timer 3
  int prescaler = 1;
  TCCR3B &= ~eraser; //Clear last 3 bits
  TCCR3B |=prescaler; //Change frequency to 31KHz
  //timer 4
  TCCR4B &= ~eraser; //Clear last 3 bits
  TCCR4B |=prescaler; //Change frequency to 31KHz
  //timer 2
  TCCR2B &= ~eraser; //Clear last 3 bits
  TCCR2B |=prescaler; //Change frequency to 31KHz
}

// This whole thing is to map the buttons nicely to the drums,
// without changing how the drums are addressed.
int get_drum_index(int i) {
  int result = 0;

  int block_num = i/16;
  int button_num = i%16;

  // flip the blocks.
  if (block_num < 4) {
    block_num = block_num * 2;
  } else {
    block_num = block_num * 2 - 7;
  }

  int row = button_num / 4 + 4*(block_num % 2);
  int col = button_num % 4 + 4*(block_num / 2);

  // this is to make the top drum start from zero
  // if the row is -1, then it is one of the control rows. (play, pause, shift phase, etc.)
  row--;
  
  result = col*7+row;
  
  // put it back together
  Serial.print("Row number: "); Serial.println(row);
  Serial.print("Col number: "); Serial.println(col);
  Serial.print("Block number: "); Serial.println(block_num);
  Serial.print("Button number: "); Serial.println(button_num);
  Serial.print("Drum number: "); Serial.println(result);

  
  return result;
}

void send_msg(String msg){
  Serial.println(msg);
  Serial.flush();
}

String receive_msg(){
  String msg;
  while(1){
    if(Serial.available()){
        msg = Serial.readString();
        return msg;
    } 
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println("Trellis Demo");
  
  // INT pin requires a pullup
  pinMode(INTPIN, INPUT);
  digitalWrite(INTPIN, HIGH);

  
  // begin() with the addresses of each panel in order
  // I find it easiest if the addresses are in order
//  trellis.begin(0x70);  // only one
   trellis.begin(0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77);  // or four!

  // light up all the LEDs in order
  for (uint8_t i=0; i<numKeys; i++) {
    trellis.setLED(i);
    trellis.writeDisplay();    
    delay(2);
  }
  // then turn them off
  for (uint8_t i=0; i<numKeys; i++) {
    trellis.clrLED(i);
    trellis.writeDisplay();    
    delay(2);
  }

  // now do the drums
  seq_count = 0;
  pinMode(KICK,OUTPUT);
  pinMode(SNARE,OUTPUT);
  pinMode(HAT,OUTPUT);
  pinMode(CRASH,OUTPUT);
  pinMode(TOM1,OUTPUT);
  pinMode(RIDE,OUTPUT);
  pinMode(FTOM,OUTPUT);
  
  TCCR1A = 0; //Timer 1 (used by servo lib)
  TCCR1B = 0;

//  attachInterrupt(digitalPinToInterrupt(MUTE_IN),mute,HIGH); //Mute/unmute drums
//  attachInterrupt(digitalPinToInterrupt(MUTE_IN),unmute,LOW);
  //attachInterrupt(digitalPinToInterrupt(PULSE_IN),write_drums_high,RISING); //Whenever pin 19 goes from low to high write drums
 unmute();
  
  kick_active = false;
  snare_active = false;
  hat_active = false;
  crash_active = false;
  tom1_active = false;
  ride_active = false;
  ftom_active = false;
  
  //default to drums on
  mute_flag = false;
  
  s_multiple_of_5 = 0;
  k_multiple_of_5 = 0;
  h_multiple_of_5 = 0;
  c_multiple_of_5 = 0;
  t1_multiple_of_5 = 0;
  r_multiple_of_5 = 0;
  ft_multiple_of_5 = 0;
  
  //setting pwm frequency to 31KHz on pins 2,3,5,6,7,8,9,10
  set_pwm_2_3_5_6_7_8_9_10(); 
  
  //Serial stuff
  
  //initialising sequence
  
  unsigned int sequence = {0};
  
  Serial.println("Setup finished");
  Serial.println("Setting up timer.");
  begin_5_timer();
  Serial.println("Timer has been set up");
  delay(4500);
  Serial.println("Ready to go");

}


void loop() {
  
  delay(30); // 30ms delay is required, dont remove me!
  int drum_index = 0;
  
  if (usr_ctrl) {
    
    // If a button was just pressed or released...
    if (trellis.readSwitches()) {  
      // go through every button
      for (uint8_t i=0; i<numKeys; i++) {
        // if it was pressed...
        if (trellis.justPressed(i)) {
          Serial.print("v"); Serial.println(i);
          // Alternate the LED
          // need to find the block number
          drum_index = get_drum_index(i);
          if (trellis.isLED(i)) {
            sequence[drum_index] = NO_HIT;
            trellis.clrLED(i);
          } else {
            sequence[drum_index] = HARD;
            trellis.setLED(i);
          }
        } 
      }
      // tell the trellis to set the LEDs we requested
      
    }
    trellis.writeDisplay();
  }
  
  loop_count++;
}

