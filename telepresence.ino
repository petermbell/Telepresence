#include <AccelStepper.h>
//download from http://www.airspayce.com/mikem/arduino/AccelStepper/

// Define a stepper and the pins it will use; (Assuming pinModes are set by the AccelStepper library function.)
AccelStepper stepper1(AccelStepper::DRIVER, 8, 9);
AccelStepper stepper2(AccelStepper::DRIVER, 11, 12);
int powerpin = 7;
int spd = 200;    // The current speed in steps/second
int accel = 150;
boolean toggle1 = 0;
volatile int inc = 0;
void setup()
{
  Serial.begin(9600);
  stepper1.setMinPulseWidth(50);
  stepper2.setMinPulseWidth(50);
  stepper1.setMaxSpeed(spd);
  stepper1.setAcceleration(accel);
  stepper2.setMaxSpeed(spd);
  stepper2.setAcceleration(accel);
  pinMode(powerpin, OUTPUT);
  digitalWrite(powerpin, HIGH);
  //set timer1 interrupt at 1Hz (1 sec)
  cli();//stop interrupts
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 15624;//15624;// = (16*10^6) / (1*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS12 and CS10 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);

  sei();//allow interrupts
}

void loop()
{
  char c;
  if (Serial.available()) {
    c = Serial.read();

    if (c == 'f')
    { // forward
      stepper1.move(100000);
      stepper2.move(100000);
      digitalWrite(powerpin, HIGH);
      inc = 0;
      TCNT1 = 0;
      Serial.write("f\n");
    }

    if (c == 'b')
    { // reverse
      stepper1.move(-100000);
      stepper2.move(-100000);
      digitalWrite(powerpin, HIGH);
      inc = 0;
      TCNT1 = 0;
      Serial.write("b\n");
    }

    if (c == 'r')
    { // right
      stepper1.move(100000);
      stepper2.move(-100000);
      digitalWrite(powerpin, HIGH);
      inc = 0;
      TCNT1 = 0;
      Serial.write("r\n");
    }
    if (c == 'l')
    { // left
      stepper1.move(-100000);
      stepper2.move(100000);
      digitalWrite(powerpin, HIGH);
      inc = 0;
      TCNT1 = 0;
      Serial.write("l\n");
    }

    if (c == 's')
    { // stop
      stepper1.stop();
      stepper2.stop();
      Serial.write("s\n");
    }


  }//serial
  stepper1.run();
  stepper2.run();
}//loop



ISR(TIMER1_COMPA_vect)
{ //timer1 interrupt 1Hz toggles pin 13
  inc++;
  if (inc > 30)
  {
    inc = 0;
    digitalWrite(powerpin, LOW);
  }
}



