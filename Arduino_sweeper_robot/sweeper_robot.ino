// sweeper and obstacle avoiding robot

#include <NewPing.h>


#define TRIGGER_PIN 7     // pin tied for ultrasonic sensor
#define ECHO_PIN 6        // pin tied for ultrasonic sensor
#define MAX_DISTANCE 100  // max distance we want to ping for (centimeters)
                          // max sensor distance is rated at 400-1000cm

#define CW 0
#define CCW 1

// motor definitions
#define MOTOR_A 0
#define MOTOR_B 1

// pin assignments
const byte PWMA = 3;    // PWM control (speed) for motor A
const byte PWMB = 11;   // PWM control (speed) for motor B
const byte DIRA = 2;   // dirtection control for motor A
const byte DIRB = 4;   // dirtection control for motor B

const int redPin = 10; // R petal on RGB LED module connected to digital pin 10
const int greenPin = 9; // G petal on RGB LED module connected to digital pin 9 
const int bluePin = 5; // B petal on RGB LED module connected to digital pin 10 

const int redPin2 = 8; // R petal on RGB LED module connected to digital pin 8
const int greenPin2 = 13; // G petal on RGB LED module connected to digital pin 13
int val = 250;

int buzzer = 12;//the pin of the active buzzer

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); //NewPing setup of pins and max distance

void setup() 
{
  setupArdumoto(); //set all pins as outputs
  
  pinMode(redPin, OUTPUT); // sets the redPin to be an output 
  pinMode(greenPin, OUTPUT); // sets the greenPin to be an output 
  pinMode(bluePin, OUTPUT); // sets the bluePin to be an output 

  pinMode(buzzer,OUTPUT);//initialize the buzzer pin as an output

  pinMode(redPin2, OUTPUT);
  pinMode(greenPin2, OUTPUT);
  Serial.begin(9600);
}

// driveArdumoto drives 'motor' in direction 'dir' at speed 'spd'
void driveArdumoto(byte motor, byte dir, byte spd)
{
  if (motor == MOTOR_A)
  {
    digitalWrite(DIRA, dir);
    analogWrite(PWMA, spd);
  }
  else if (motor == MOTOR_B)
  {
    digitalWrite(DIRB, dir);
    analogWrite(PWMB, spd);
  }
}

void forward(byte spd)    // runs both motors at speed 'spd'
{
  driveArdumoto(MOTOR_A, CW, spd);  // motor A at speed 'spd'
  driveArdumoto(MOTOR_B, CW, (spd+10));  // motor B at speed 'spd'
}

void reverse(byte spd)
{
  driveArdumoto(MOTOR_A, CCW, (spd-10));
  driveArdumoto(MOTOR_B, CCW, (spd+20));
}

void turnRight(byte spd)
{
  stopArdumoto(MOTOR_B);          // motor B stop
  driveArdumoto(MOTOR_A, CW, spd);// motor A run
}

// stopArdumoto makes a motor stop
void stopArdumoto(byte motor)
{
  driveArdumoto(motor, 0, 0);
}

void park()
{
  stopArdumoto(MOTOR_A);
  stopArdumoto(MOTOR_B);
}

// setupArdumoto initializes all pins
void setupArdumoto()
{
  // all pins should be set up as outputs:
  pinMode(PWMA, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(DIRA, OUTPUT);
  pinMode(DIRB, OUTPUT);

  // initializes all pins as low:
  digitalWrite(PWMA, LOW);
  digitalWrite(PWMB, LOW);
  digitalWrite(DIRA, LOW);
  digitalWrite(DIRB, LOW);
}

void loop() 
{  
  analogWrite(redPin2, val);
  analogWrite(greenPin2, val);
 
  delay(100);
  unsigned int uS = sonar.ping(); // send ping, get ping time in microseconds (uS)

  unsigned char i; //define a variable
  unsigned char j; //define a variable
  
  if (uS / US_ROUNDTRIP_CM >= 30 || uS / US_ROUNDTRIP_CM == 0)
  {
    //move forward
    forward(180);
    color(0,255, 0); // turn the RGB LED green
   
    delay(300);
  }
  else if (uS / US_ROUNDTRIP_CM < 30 && uS / US_ROUNDTRIP_CM >= 10)
  {
    turnRight(180);
    color(0, 0, 255); // turn the RGB LED blue 
    delay(300);
  }
  else if (uS / US_ROUNDTRIP_CM < 10 && uS / US_ROUNDTRIP_CM > 0)
  {
    reverse(180);
    color(255, 0, 0); // turn the RGB LED red 
    for(i=0;i<80;i++)
    {
      digitalWrite(buzzer,HIGH);
      delay(1);//wait for 1ms
      digitalWrite(buzzer,LOW);
      delay(1);//wait for 1ms
    }
    delay(300);
  }
}

void color (unsigned char red, unsigned char green, unsigned char blue) // the color generating function 
{ 
analogWrite(redPin, red); 
analogWrite(greenPin, green); 
analogWrite(bluePin, blue); 
}
