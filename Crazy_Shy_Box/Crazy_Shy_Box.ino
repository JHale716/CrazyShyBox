//
//
// This is code for Crazy Shy Box
//
//
float Version = 1.1;
// by JP Hale jphale716@gmail.com

#include <Servo.h>

Servo servo_ear_left;
Servo servo_ear_right;

#define STEPPER_PIN_1 9
#define STEPPER_PIN_2 10
#define STEPPER_PIN_3 11
#define STEPPER_PIN_4 12
#define PowerRelay 5
#define LEDPin 13
#define STATE_OPEN 0
#define STATE_CLOSED 1

int stepper_eyes_up = 120; // Number of Steps for Eyes up
int ear_left_open = 2200;
int ear_left_middle = 1700;
int ear_left_closed = 1365;
int ear_right_open = 1400;
int ear_right_middle = 1700;
int ear_right_closed = 2110;
int step_number = 0;
int lefttenth;
int righttenth;

int sonar_trig_pin = A1;
int sonar_echo_pin = A2;
int stepCount = 0; // number of steps the motor has take
int switchState = 0;
int lastSwitch = 0;
int eyePosition = 0; // 0 Normal position, 1 up position
int firstrun = 1;
int selectedMove = 0;
int lastdistance = 0;
static bool is_open = true;
int stepsupdown;
static int doorstate = STATE_OPEN;
unsigned long sleeptimerdelay = 300000;
unsigned long sleeptimer;
unsigned long sleepingtimer;
unsigned long breathing;
unsigned long breathingtimerdelay = 5000;
int sleeping;
int action;

void setup() {
  lefttenth = (ear_left_open - ear_left_closed) / 10;
  righttenth = (ear_right_closed - ear_right_open) / 10;
  pinMode(STEPPER_PIN_1, OUTPUT);
  pinMode(STEPPER_PIN_2, OUTPUT);
  pinMode(STEPPER_PIN_3, OUTPUT);
  pinMode(STEPPER_PIN_4, OUTPUT);
  pinMode(PowerRelay, OUTPUT);
  pinMode(LEDPin, OUTPUT);
  delay(1000);
  digitalWrite(PowerRelay, HIGH);
  Serial.begin(9600);
  Serial.println(F("Loaded with JPs Crazy Shy Box"));
  Serial.print(F("Version: "));
  Serial.println(Version);
  servo_ear_left.attach(3);
  servo_ear_right.attach(2);
  pinMode(sonar_trig_pin, OUTPUT);
  pinMode(sonar_echo_pin, INPUT);
  servo_ear_left.writeMicroseconds(ear_left_open);
  servo_ear_right.writeMicroseconds(ear_right_open);
  StepperEyes(true, stepper_eyes_up); // Eyes to normal position
  delay(200);
  servo_ear_right.writeMicroseconds(ear_right_closed);
  servo_ear_left.writeMicroseconds(ear_left_closed);
  doorstate = STATE_CLOSED;
  delay(1000);
  sleeptimer = millis();
}

void loop() {
  if (sleeping) {
    sleepingtask();
  } else {
    shybox();
    checksleep();
  }
}

void StepperEyes(bool eyedirection, int movement) {
  for (int a = 0; a < movement; a++) {
    if (a == 1) {
      digitalWrite(LEDPin, eyedirection);
    }
    OneStep(eyedirection);
    delay(4);
  }
  digitalWrite(STEPPER_PIN_1, LOW);
  digitalWrite(STEPPER_PIN_2, LOW);
  digitalWrite(STEPPER_PIN_3, LOW);
  digitalWrite(STEPPER_PIN_4, LOW);
}

void eyeslift() {
  if (eyePosition != 2) {
    bool dirlift;
    if (!eyePosition) {
      dirlift = false;
      stepsupdown = stepper_eyes_up * 2 / 3;
    } else {
      dirlift = true;
      stepsupdown = stepper_eyes_up * 1 / 3;
    }
    StepperEyes(dirlift, stepsupdown); // Eyes to lift position
    Serial.print("Steps of ");
    Serial.print(stepsupdown);
    Serial.println(" for Eye's Lift");
    eyePosition = 2;
  }
}

void eyesup() {
  if (eyePosition == 2) {
    stepsupdown = stepper_eyes_up * 1 / 3;
  } else {
    stepsupdown = stepper_eyes_up;
  }
  StepperEyes(false, stepsupdown); // Eyes to up position
  Serial.print("Steps of ");
  Serial.print(stepsupdown);
  Serial.println(" for Eye's Up");
  eyePosition = 1;
}

void eyesnormal() {
  if (eyePosition == 2) {
    stepsupdown = stepper_eyes_up * 2 / 3;
  } else {
    stepsupdown = stepper_eyes_up;
  }
  StepperEyes(true, stepsupdown + 25); // Eyes to normal position
  Serial.print("Steps of ");
  Serial.print(stepsupdown);
  Serial.println(" for Eye's Normal");
  eyePosition = 0;
}

void checksleep() {
  if (action) {
    sleeptimer = millis();
    action = 0;
  } else {
    if (millis() - sleeptimer >= sleeptimerdelay) {
      delay(500);
      servo_ear_right.writeMicroseconds(ear_right_open);
      delay(100);
      servo_ear_left.writeMicroseconds(ear_left_open);
      delay(1000);
      servo_ear_right.writeMicroseconds(ear_right_open + (1 * righttenth));
      delay(900);
      servo_ear_left.writeMicroseconds(ear_left_open - (1 * lefttenth));
      delay(1000);
      servo_ear_right.writeMicroseconds(ear_right_open + (2 * righttenth));
      delay(900);
      servo_ear_left.writeMicroseconds(ear_left_open - (2 * lefttenth));
      delay(1000);
      servo_ear_right.writeMicroseconds(ear_right_open + (3 * righttenth));
      delay(800);
      servo_ear_left.writeMicroseconds(ear_left_open - (3 * lefttenth));
      delay(700);
      servo_ear_right.writeMicroseconds(ear_right_open + (4 * righttenth));
      delay(900);
      servo_ear_left.writeMicroseconds(ear_left_open - (4 * lefttenth));
      delay(500);
      servo_ear_right.writeMicroseconds(ear_right_open + (5 * righttenth));
      delay(1000);
      servo_ear_left.writeMicroseconds(ear_left_open - (5 * lefttenth));
      delay(800);
      servo_ear_right.writeMicroseconds(ear_right_open + (6 * righttenth));
      delay(1500);
      servo_ear_left.writeMicroseconds(ear_left_open - (6 * lefttenth));
      delay(900);
      servo_ear_right.writeMicroseconds(ear_right_open + (7 * righttenth));
      delay(500);
      servo_ear_left.writeMicroseconds(ear_left_open - (7 * lefttenth));
      delay(400);
      servo_ear_right.writeMicroseconds(ear_right_open + (8 * righttenth));
      delay(800);
      servo_ear_left.writeMicroseconds(ear_left_open - (8 * lefttenth));
      delay(1000);
      servo_ear_right.writeMicroseconds(ear_right_open + (9 * righttenth));
      delay(1200);
      servo_ear_left.writeMicroseconds(ear_left_open - (9 * lefttenth));
      delay(1000);
      servo_ear_right.writeMicroseconds(ear_right_closed);
      delay(500);
      servo_ear_left.writeMicroseconds(ear_left_closed);
      delay(600);
      eyesnormal();
      sleepingtimer = millis();
      breathing = millis();
      sleeping = 1;
    }
  }
}

void sleepingtask() {
  if (millis() - sleepingtimer >= sleeptimerdelay) {
    eyesup;
    eyesnormal;
    eyeslift;
    eyesup;
    eyesnormal;
    delay(500);
    servo_ear_left.writeMicroseconds(ear_left_closed + (1 * lefttenth));
    delay(100);
    servo_ear_left.writeMicroseconds(ear_left_closed + (2 * lefttenth));
    delay(1000);
    servo_ear_left.writeMicroseconds(ear_left_closed + (3 * lefttenth));
    delay(900);
    servo_ear_left.writeMicroseconds(ear_left_closed + (4 * lefttenth));
    delay(1000);
    servo_ear_left.writeMicroseconds(ear_left_closed + (5 * lefttenth));
    delay(900);
    servo_ear_left.writeMicroseconds(ear_left_closed + (6 * lefttenth));
    delay(1000);
    servo_ear_right.writeMicroseconds(ear_right_closed - (1 * righttenth));
    delay(800);
    servo_ear_left.writeMicroseconds(ear_left_closed + (8 * lefttenth));
    delay(700);
    servo_ear_left.writeMicroseconds(ear_left_closed + (7 * lefttenth));
    delay(900);
    servo_ear_left.writeMicroseconds(ear_left_closed + (8 * lefttenth));
    delay(500);
    servo_ear_right.writeMicroseconds(ear_right_closed - (2 * righttenth));
    delay(900);
    servo_ear_left.writeMicroseconds(ear_left_closed + (9 * lefttenth));
    delay(500);
    servo_ear_left.writeMicroseconds(ear_left_open);
    delay(1000);
    servo_ear_left.writeMicroseconds(ear_left_closed + (9 * lefttenth));
    delay(800);
    servo_ear_right.writeMicroseconds(ear_right_closed - (3 * righttenth));
    delay(1500);
    servo_ear_right.writeMicroseconds(ear_right_closed - (4 * righttenth));
    delay(1200);
    servo_ear_right.writeMicroseconds(ear_right_closed - (5 * righttenth));
    delay(900);
    servo_ear_right.writeMicroseconds(ear_right_closed - (6 * righttenth));
    delay(500);
    servo_ear_right.writeMicroseconds(ear_right_closed - (7 * righttenth));
    delay(400);
    servo_ear_right.writeMicroseconds(ear_right_closed - (8 * righttenth));
    delay(800);
    servo_ear_right.writeMicroseconds(ear_right_closed - (9 * righttenth));
    delay(1000);
    servo_ear_right.writeMicroseconds(ear_right_closed - (8 * righttenth));
    delay(200);
    servo_ear_right.writeMicroseconds(ear_right_closed - (9 * righttenth));
    delay(1000);
    servo_ear_right.writeMicroseconds(ear_right_open);
    delay(500);
    servo_ear_left.writeMicroseconds(ear_left_open);
    delay(600);
    sleeping = 0;
    sleeptimer = millis();
  }
  if (millis() - breathing >= breathingtimerdelay) {
    breathing = millis();
    if (eyePosition) {
      eyesnormal();
    } else {
      eyeslift();
    }
  }
}

void shybox() {
  if (doorstate == STATE_CLOSED) {
    delay(2000);
    servo_ear_left.writeMicroseconds(ear_left_middle);
    servo_ear_right.writeMicroseconds(ear_right_middle);
    delay(200);
    Serial.println("Set Middle");
    delay(500);
    doorstate = STATE_OPEN;
    Serial.println("State Open");
  }
  else if (doorstate == STATE_OPEN) {
    int distance = getDistance();
    if (firstrun) {
      distance = getDistance();
      distance = getDistance();
      distance = getDistance();
      distance = getDistance();
      distance = 80;
      lastdistance = 80;
    }
    if (distance <= 45 && distance >= 15) {
      Serial.println("step One");
      Serial.print("Last Distance : ");
      Serial.println(lastdistance);
      if (lastdistance <= 15) {
        Serial.println("step Two");
        SelectAMove();
      }
    }
    if (distance >= 45) {
      if (firstrun) {
        Serial.println("First Run");
      }
      if (lastdistance < 45) {
        lastdistance = distance; // avoid running the same loop
        action = 1;
        Serial.println(distance);
        if (eyePosition) {
          eyesnormal();
        }
        delay(200);
        servo_ear_left.writeMicroseconds(ear_left_open);
        servo_ear_right.writeMicroseconds(ear_right_open);
        Serial.println("Set Open");
      }
    }
    else if (distance < 45 && distance >= 30) {
      if (firstrun) {
        Serial.println("First Run");
      }
      if (lastdistance < 45 && distance >= 30) {
        lastdistance = distance; // avoid running the same loop
        action = 1;
        Serial.println(distance);
        if (eyePosition != 2) {
          eyeslift();
        }
      }
      delay(200);
      servo_ear_left.writeMicroseconds(ear_left_open);
      servo_ear_right.writeMicroseconds(ear_right_open);
      Serial.println("Set Open");
    }
    else if (distance < 30 && distance >= 15) {
      Serial.println(distance);
      lastdistance = distance;
      action = 1;
      if (eyePosition != 2) {
        bool dirlift;
        if (!eyePosition) {
          dirlift = false;
          stepsupdown = stepper_eyes_up * 2 / 3;
        } else {
          dirlift = true;
          stepsupdown = stepper_eyes_up * 1 / 3;
        }
        StepperEyes(dirlift, stepsupdown); // Eyes to lift position
        Serial.print("Steps of ");
        Serial.print(stepsupdown);
        Serial.println(" for Eye's Lift");
        eyePosition = 2;
      }
      delay(200);
      servo_ear_left.writeMicroseconds(ear_left_open - ( 5 * lefttenth));
      servo_ear_right.writeMicroseconds(ear_right_open + (4 * lefttenth));
      Serial.println("Set Open");
    }
    else if (distance < 15 && distance > 7) {
      Serial.println(distance);
      lastdistance = distance;
      action = 1;
      if (eyePosition != 1) {
        eyesup();
      }
      servo_ear_right.writeMicroseconds(ear_right_closed);
      servo_ear_left.writeMicroseconds(ear_left_closed);
      delay(200);
      doorstate = STATE_CLOSED;
      Serial.println("State Closed");
      Serial.println("Set Closed");
    }
    if (firstrun) {
      firstrun = 0;
    }
  }
}

void SelectAMove() {
  if (selectedMove > 9) {
    selectedMove = 0;
  } //when all moves are played, repeat the moves from beginning
  if (selectedMove == 0) {
    Serial.println("Simple Close");
    // Return Normal...
    Serial.println("Finished Close");
  }
  else if (selectedMove == 1) {
    Serial.println("Simple Close");
    // Return Normal...
    Serial.println("Finished Close");
  }
  else if (selectedMove == 2) {
    Serial.println("Simple Close2");
    crazydoor2();
    Serial.println("Finished Close");
  }
  else if (selectedMove == 3) {
    Serial.println("Crazy Door");
    crazydoor();
    Serial.println("Finished Close");
  }
  else if (selectedMove == 4) {
    Serial.println("Slow");
    slow();
    Serial.println("Finished Close");
  }
  else if (selectedMove == 5) {
    Serial.println("Serious");
    serious();
    Serial.println("Finished Close");
  }
  else if (selectedMove == 6) {
    Serial.println("Troll Close");
    trollClose();
    Serial.println("Finished Close");
  }
  else if (selectedMove == 7) {
    Serial.println("Simple Close");
    // Return Normal...
    Serial.println("Finished Close");
  }
  else if (selectedMove == 8) {
    Serial.println("Matrix");
    matrix();
    Serial.println("Finished Close");
  }
  else if (selectedMove == 9) {
    Serial.println("Sneak");
    sneak();
    Serial.println("Finished Close");
  }
  selectedMove += 1; //swith to next move
}

void crazydoor() {
  delay(400);
  servo_ear_right.writeMicroseconds(ear_right_closed);
  delay(100);
  servo_ear_left.writeMicroseconds(ear_left_closed);
  delay(400);
  servo_ear_left.writeMicroseconds(ear_left_open);
  delay(100);
  servo_ear_right.writeMicroseconds(ear_right_open);
  delay(400);
  servo_ear_right.writeMicroseconds(ear_right_closed);
  delay(100);
  servo_ear_left.writeMicroseconds(ear_left_closed);
  delay(400);
  servo_ear_left.writeMicroseconds(ear_left_middle);
  delay(400);
  servo_ear_right.writeMicroseconds(ear_right_open);
  delay(400);
  servo_ear_left.writeMicroseconds(ear_left_open);
  delay(400);
  servo_ear_right.writeMicroseconds(ear_right_middle);
  delay(400);
  servo_ear_left.writeMicroseconds(ear_left_middle);
  delay(400);
  servo_ear_right.writeMicroseconds(ear_right_closed);
  delay(400);
  servo_ear_left.writeMicroseconds(ear_left_open);
  delay(400);
  servo_ear_right.writeMicroseconds(ear_right_open);
  delay(400);
  servo_ear_right.writeMicroseconds(ear_right_closed);
  delay(400);
  servo_ear_left.writeMicroseconds(ear_left_closed);
  delay(400);
  servo_ear_left.writeMicroseconds(ear_left_middle);
  delay(400);
  servo_ear_right.writeMicroseconds(ear_right_middle);
  delay(400);
}


void crazydoor2() {
  delay(400);
  servo_ear_right.writeMicroseconds(ear_right_closed);
  delay(100);
  servo_ear_left.writeMicroseconds(ear_left_closed);
  delay(400);
  servo_ear_left.writeMicroseconds(ear_left_middle);
  delay(100);
  servo_ear_right.writeMicroseconds(ear_right_middle);
  delay(400);
  servo_ear_right.writeMicroseconds(ear_right_closed);
  delay(100);
  servo_ear_left.writeMicroseconds(ear_left_closed);
  delay(400);
  servo_ear_left.writeMicroseconds(ear_left_middle);
  delay(400);
  servo_ear_right.writeMicroseconds(ear_right_middle);
  delay(400);
  servo_ear_left.writeMicroseconds(ear_left_open);
  delay(400);
  servo_ear_right.writeMicroseconds(ear_right_open);
  delay(400);
}


void sneak() {
  delay(400);
  servo_ear_right.writeMicroseconds(ear_right_closed);
  delay(100);
  servo_ear_left.writeMicroseconds(ear_left_closed);
  delay(400);
  servo_ear_right.writeMicroseconds(ear_right_closed - (1 * righttenth));
  delay(100);
  servo_ear_left.writeMicroseconds(ear_left_closed + (1 * lefttenth));
  delay(400);
  servo_ear_right.writeMicroseconds(ear_right_closed - (2 * righttenth));
  delay(100);
  servo_ear_left.writeMicroseconds(ear_left_closed + (2 * lefttenth));
  delay(400);
  servo_ear_right.writeMicroseconds(ear_right_closed - (3 * righttenth));
  delay(100);
  servo_ear_left.writeMicroseconds(ear_left_closed + (3 * lefttenth));
  delay(400);
  servo_ear_right.writeMicroseconds(ear_right_closed - (4 * righttenth));
  delay(100);
  servo_ear_left.writeMicroseconds(ear_left_closed + (4 * lefttenth));
  delay(400);
  servo_ear_right.writeMicroseconds(ear_right_open);
  delay(100);
  servo_ear_left.writeMicroseconds(ear_left_open);
  delay(400);
}

void matrix() {
  delay(400);
  servo_ear_right.writeMicroseconds(ear_right_closed - (5 * righttenth));
  delay(100);
  servo_ear_left.writeMicroseconds(ear_left_closed + (5 * lefttenth));
  delay(400);
  servo_ear_right.writeMicroseconds(ear_right_closed - (6 * righttenth));
  delay(100);
  servo_ear_left.writeMicroseconds(ear_left_closed + (6 * lefttenth));
  delay(400);
  servo_ear_right.writeMicroseconds(ear_right_closed - (7 * righttenth));
  delay(100);
  servo_ear_left.writeMicroseconds(ear_left_closed + (7 * lefttenth));
  delay(400);
  servo_ear_right.writeMicroseconds(ear_right_closed - (8 * righttenth));
  delay(100);
  servo_ear_left.writeMicroseconds(ear_left_closed + (8 * lefttenth));
  delay(400);
  servo_ear_right.writeMicroseconds(ear_right_closed - (9 * righttenth));
  delay(100);
  servo_ear_left.writeMicroseconds(ear_left_closed + (9 * lefttenth));
  delay(400);
  servo_ear_right.writeMicroseconds(ear_right_open);
  delay(100);
  servo_ear_left.writeMicroseconds(ear_left_open);
  delay(400);
}

void serious() {
  delay(400);
  servo_ear_right.writeMicroseconds(ear_right_closed);
  delay(100);
  servo_ear_left.writeMicroseconds(ear_left_closed);
  delay(400);
  servo_ear_right.writeMicroseconds(ear_right_open + (1 * righttenth));
  delay(100);
  servo_ear_left.writeMicroseconds(ear_left_closed + (1 * lefttenth));
  delay(400);
  servo_ear_right.writeMicroseconds(ear_right_open + (2 * righttenth));
  delay(100);
  servo_ear_left.writeMicroseconds(ear_left_closed + (2 * lefttenth));
  delay(400);
  servo_ear_right.writeMicroseconds(ear_right_open + (3 * righttenth));
  delay(100);
  servo_ear_left.writeMicroseconds(ear_left_closed + (3 * lefttenth));
  delay(400);
  servo_ear_right.writeMicroseconds(ear_right_open + (4 * righttenth));
  delay(100);
  servo_ear_left.writeMicroseconds(ear_left_closed + (4 * lefttenth));
  delay(400);
  servo_ear_right.writeMicroseconds(ear_right_open + (5 * righttenth));
  delay(100);
  servo_ear_left.writeMicroseconds(ear_left_closed + (5 * lefttenth));
  delay(400);
  servo_ear_right.writeMicroseconds(ear_right_open + (6 * righttenth));
  delay(100);
  servo_ear_left.writeMicroseconds(ear_left_closed + (6 * lefttenth));
  delay(400);
  servo_ear_right.writeMicroseconds(ear_right_open + (7 * righttenth));
  delay(100);
  servo_ear_left.writeMicroseconds(ear_left_closed + (7 * lefttenth));
  delay(400);
  servo_ear_right.writeMicroseconds(ear_right_open + (8 * righttenth));
  delay(100);
  servo_ear_left.writeMicroseconds(ear_left_closed + (8 * lefttenth));
  delay(400);
  servo_ear_right.writeMicroseconds(ear_right_open + (9 * righttenth));
  delay(100);
  servo_ear_left.writeMicroseconds(ear_left_closed + (9 * lefttenth));
  delay(400);
  servo_ear_right.writeMicroseconds(ear_right_open);
  delay(100);
  servo_ear_left.writeMicroseconds(ear_left_open);
  delay(400);
}

void trollClose() {
  delay(400);
  servo_ear_right.writeMicroseconds(ear_right_closed);
  delay(100);
  servo_ear_left.writeMicroseconds(ear_left_closed);
  delay(400);
  servo_ear_right.writeMicroseconds(ear_right_middle);
  delay(100);
  servo_ear_left.writeMicroseconds(ear_left_middle);
  delay(400);
  servo_ear_right.writeMicroseconds(ear_right_open);
  delay(100);
  servo_ear_left.writeMicroseconds(ear_left_open);
  delay(400);
}

void slow() {
  delay(400);
  servo_ear_right.writeMicroseconds(ear_right_closed);
  delay(100);
  servo_ear_left.writeMicroseconds(ear_left_closed);;
  delay(400);
  servo_ear_right.writeMicroseconds(ear_right_closed - (1 * righttenth));
  delay(100);
  servo_ear_left.writeMicroseconds(ear_left_closed + (1 * lefttenth));
  delay(400);
  servo_ear_right.writeMicroseconds(ear_right_closed - (2 * righttenth));
  delay(100);
  servo_ear_left.writeMicroseconds(ear_left_closed + (2 * lefttenth));
  delay(400);
  servo_ear_right.writeMicroseconds(ear_right_closed - (3 * righttenth));
  delay(100);
  servo_ear_left.writeMicroseconds(ear_left_closed + (3 * lefttenth));
  delay(400);
  servo_ear_right.writeMicroseconds(ear_right_closed - (4 * righttenth));
  delay(100);
  servo_ear_left.writeMicroseconds(ear_left_closed + (4 * lefttenth));
  delay(400);
  servo_ear_right.writeMicroseconds(ear_right_closed - (5 * righttenth));
  delay(100);
  servo_ear_left.writeMicroseconds(ear_left_closed + (5 * lefttenth));
  delay(400);
  servo_ear_right.writeMicroseconds(ear_right_closed - (6 * righttenth));
  delay(100);
  servo_ear_left.writeMicroseconds(ear_left_closed + (6 * lefttenth));
  delay(400);
  servo_ear_right.writeMicroseconds(ear_right_closed - (7 * righttenth));
  delay(100);
  servo_ear_left.writeMicroseconds(ear_left_closed + (7 * lefttenth));
  delay(400);
  servo_ear_right.writeMicroseconds(ear_right_closed - (8 * righttenth));
  delay(100);
  servo_ear_left.writeMicroseconds(ear_left_closed + (8 * lefttenth));
  delay(400);
  servo_ear_right.writeMicroseconds(ear_right_closed - (9 * righttenth));
  delay(100);
  servo_ear_left.writeMicroseconds(ear_left_closed + (9 * lefttenth));
  delay(400);
  servo_ear_right.writeMicroseconds(ear_right_open);
  delay(100);
  servo_ear_left.writeMicroseconds(ear_left_open);
  delay(400);
}

float getDistance() {
  delay(200);
  float distance;
  // float readdist[3];
  // for (int i = 0; i < 3; i++) {
  digitalWrite(sonar_trig_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(sonar_trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(sonar_trig_pin, LOW);
  unsigned long duration = pulseIn(sonar_echo_pin, HIGH);
  // readdist[i] = duration / 29.0 / 2.0;
  distance = duration / 29.0 / 2.0;
  // }
  // distance = ((readdist[1] + readdist[2] + readdist[3]) / 3);
  Serial.println(distance);
  return distance;
}

void OneStep(bool dir) {
  if (dir) {
    switch (step_number) {
      case 0:
        digitalWrite(STEPPER_PIN_1, HIGH);
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, LOW);
        break;
      case 1:
        digitalWrite(STEPPER_PIN_1, LOW);
        digitalWrite(STEPPER_PIN_2, HIGH);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, LOW);
        break;
      case 2:
        digitalWrite(STEPPER_PIN_1, LOW);
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, HIGH);
        digitalWrite(STEPPER_PIN_4, LOW);
        break;
      case 3:
        digitalWrite(STEPPER_PIN_1, LOW);
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, HIGH);
        break;
    }
  } else {
    switch (step_number) {
      case 0:
        digitalWrite(STEPPER_PIN_1, LOW);
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, HIGH);
        break;
      case 1:
        digitalWrite(STEPPER_PIN_1, LOW);
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, HIGH);
        digitalWrite(STEPPER_PIN_4, LOW);
        break;
      case 2:
        digitalWrite(STEPPER_PIN_1, LOW);
        digitalWrite(STEPPER_PIN_2, HIGH);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, LOW);
        break;
      case 3:
        digitalWrite(STEPPER_PIN_1, HIGH);
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, LOW);
    }
  }
  step_number++;
  if (step_number > 3) {
    step_number = 0;
  }
}
