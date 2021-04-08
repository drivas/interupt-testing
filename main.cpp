#include <Arduino.h>
#include <AccelStepper.h>

#define LEFT_LIMIT_PIN 5
#define RIGHT_LIMIT_PIN 32

AccelStepper stepper (1, 2, 3);

int count{0};
int fieldSize{0};
bool homingPerformed = false; 
volatile bool limitLeftFlag = false;
volatile bool limitRightFlag = false;

void setLimitLFlag_isr();
void setLimitRFlag_isr();
void homing();

void setup() {
  
  Serial.begin(9600);
  //pinMode(5, INPUT_PULLUP);
  //pinMode(6, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(LEFT_LIMIT_PIN), setLimitLFlag_isr,  FALLING);
  attachInterrupt(digitalPinToInterrupt(RIGHT_LIMIT_PIN), setLimitRFlag_isr,  FALLING);
  
}

void loop()
{
  
  delay(5000);
  
  if (homingPerformed  == false)
  {
  homing();
  }
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
}


void setLimitLFlag_isr()
{
  unsigned long interrupt_time = millis();
  static unsigned long last_interrupt_time = 0;
  
	if (interrupt_time - last_interrupt_time > 300)
  {
  limitLeftFlag = true;
  Serial.println("LEFT");
  }
  last_interrupt_time = interrupt_time;
}

void setLimitRFlag_isr()
{
  unsigned long interrupt_time = millis();
  static unsigned long last_interrupt_time = 0;
  
	if (interrupt_time - last_interrupt_time > 300)
  {
  limitRightFlag = true;
  Serial.println("RIGHT");
  }
  last_interrupt_time = interrupt_time;
}

void homing()
{
  
  while(limitLeftFlag == false && homingPerformed == false)
  {
    stepper.setMaxSpeed(100);
    stepper.setAcceleration(200);
    stepper.move(-100000);
    stepper.run();
  } 
  stepper.setCurrentPosition(0);
  while(limitRightFlag == false && limitLeftFlag == true)
  {
    stepper.setSpeed(100);
    stepper.setAcceleration(200);
    stepper.move(100000);
    stepper.run();
  }  
  fieldSize = stepper.currentPosition();
  stepper.setCurrentPosition(0); // needs to be called to get motor to stop imdietly
  while(limitRightFlag == true && limitLeftFlag == true)
  {
  stepper.setMaxSpeed(800);
  stepper.setAcceleration(2000);
  stepper.runToNewPosition((fieldSize/2)*-1);
  limitRightFlag =false;
  limitLeftFlag = false;
  homingPerformed = true;
  Serial.println(fieldSize);
  }
return ;
  }