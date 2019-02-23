#include <elapsedMillis.h>
elapsedMillis timeElapsed;
#define EnA 10  //motor enable
#define EnB 5
#define In1 9   // motor input signals
#define In2 8
#define In3 7
#define In4 6
#define ENC1 3  //encoder inputs
#define ENC2 2 
int temp_enc1;  //temporary variables for encoder calculation
int temp_enc2;
int enc1_count;  //encoder counters
int enc2_count;
int speed1;      //actual speed storing variables
int speed2;
int mot1_speed;  //PWM speed for motors
int mot2_speed;
unsigned int interval;  //for setting interval of speed calculation

void setup()
{
  Serial.begin(9600);
  pinMode(EnA, OUTPUT);
  pinMode(EnB, OUTPUT);
  pinMode(In1, OUTPUT);
  pinMode(In2, OUTPUT);
  pinMode(In3, OUTPUT);
  pinMode(In4, OUTPUT);
  pinMode(ENC1, INPUT);
  pinMode(ENC2, INPUT);
  temp_enc1 = 0; enc1_count = 0;    //intitialize variables
  temp_enc2 = 0; enc2_count = 0;
  speed1 = 0; speed2 = 0;
  mot1_speed = 100; mot2_speed = 100;  //set initial speed of motor
  interval = 100;    //speed is measured every 200ms
}
void goStraight(int speed1, int speed2)   //run both motors in the same direction
{ 
  digitalWrite(In1, HIGH); // turn on motor A
  digitalWrite(In2, LOW);
  analogWrite(EnA, speed1); // set speed out of 255
  
  digitalWrite(In3, HIGH); // turn on motor B
  digitalWrite(In4, LOW);
  analogWrite(EnB, speed2); // set speed out of 255
  
}

int speedMeasure()       //function to measure speed
{
  int enc1_status = digitalRead(ENC1); //read pulses from encoder
  int enc2_status = digitalRead(ENC2);
  
  if(temp_enc1!= enc1_status)   //to detect change in pulse which corresponds to movement
  { enc1_count++;
    temp_enc1= enc1_status; 
  }
  
  if(temp_enc2!= enc2_status)  //to detect change in pulse which corresponds to movement
  { enc2_count++;
    temp_enc2= enc2_status;
  }
  
  if (timeElapsed > interval)    //update speed
  { speed1 = enc1_count;
    speed2 = enc2_count;
    enc1_count = 0;
    enc2_count = 0;
    timeElapsed = 0;
  }
  
}

void loop()
{ if (timeElapsed > interval)
 {
  Serial.print(mot1_speed);
  Serial.print("    ");
  Serial.print(mot2_speed);
  Serial.println();
  
   
  if(speed1 > speed2)
  {
//    if(mot1_speed>100)
    mot1_speed--;
//    else
    mot2_speed++; 
  }

  if(speed2 > speed1)
  {
//   if(mot2_speed>100)
   mot2_speed--;
//   else
   mot1_speed++; 
  }
 }

  speedMeasure();    //calling function to update speed
  goStraight(mot1_speed,mot2_speed);  //setting motors to run at updaed speed
}
