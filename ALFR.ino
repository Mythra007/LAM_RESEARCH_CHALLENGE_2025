const int irPins[8] = {A0, A1, A2, A3, A4, A5, A6, A7};

const int trigPin = 3;
const int echoPin = 4;
const int obstacleDistanceCM = 5;

const int leftPWM = 10;
const int leftIn1 = 9;
const int leftIn2 = 8;

const int rightPWM = 5;
const int rightIn1 = 7;
const int rightIn2 = 6;

const int BLACK_THR = 90;  
const int WHITE_THR = 80; 
const int TH = 10;            

int straightSpeed = 160;
const int reduction_factor = -0.2;

void setup() {
  Serial.begin(115200);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(leftPWM, OUTPUT);
  pinMode(leftIn1, OUTPUT);
  pinMode(leftIn2, OUTPUT);

  pinMode(rightPWM, OUTPUT);
  pinMode(rightIn1, OUTPUT);
  pinMode(rightIn2, OUTPUT);

  stopMotors();
  delay(300);
}

void loop() {
  long d = readUltrasonicCM();
  if (d > 0 && d <= obstacleDistanceCM) {
    stopMotors();
    Serial.print("Obstacle at: "); Serial.println(d);
    while (true) {
      delay(200);
      long dd = readUltrasonicCM();
      if (dd > obstacleDistanceCM + 5) break;
    }
    delay(20);
  }
  int s[8];
  for (int i = 0; i < 8; i++) s[i] = analogRead(irPins[i]);

  for (int i = 0; i < 8; i++) { Serial.print(String(s[i]) + " "); }
  Serial.println();

  bool AllWhite = (((s[4]+s[3])/2 < WHITE_THR) && (s[7] + s[6] + s[5])/3 < WHITE_THR && (s[2] + s[1] + s[0])/3<WHITE_THR);
  bool AllBlack = (
      ((s[4] + s[3]) / 2 > BLACK_THR) &&
      ((s[7] + s[6] + s[5]) / 3 > BLACK_THR) &&
      ((s[2] + s[1] + s[0]) / 3 > BLACK_THR)
  );

  if (AllWhite) {
    Serial.println("ALL WHITE → BACKWARD");

    digitalWrite(leftIn1, LOW);
    digitalWrite(leftIn2, HIGH);
    analogWrite(leftPWM, 140); 

    digitalWrite(rightIn1, LOW);
    digitalWrite(rightIn2, HIGH);
    analogWrite(rightPWM, 140);

    delay(200); 
    return;
  }
  int sumLeft  = s[7] + s[6] + s[5];
  int sumRight = s[2] + s[1] + s[0];

  int diff = sumLeft - sumRight;


  if (diff > TH) {
    turnLeftSharp(straightSpeed, straightSpeed*reduction_factor);
    Serial.print("TURN LEFT | diff="); Serial.println(diff);
  }

  else if (diff < -TH) {
    turnRightSharp(straightSpeed*reduction_factor, straightSpeed);
    Serial.print("TURN RIGHT | diff="); Serial.println(diff);
  }

  else {
    forward(straightSpeed, straightSpeed);
    Serial.print("STRAIGHT | diff="); Serial.println(diff);
  }
  // if (AllBlack) {
  //   Serial.println("ALL BLACK → FORWARD FAST");
  //   digitalWrite(leftIn1, HIGH);
  //   digitalWrite(leftIn2, LOW);
  //   analogWrite(leftPWM, straightSpeed);

  //   digitalWrite(rightIn1, HIGH);
  //   digitalWrite(rightIn2, LOW);
  //   analogWrite(rightPWM, straightSpeed);

  //   delay(150);
  //   return;
  // }
  delay(100);
}
void forward(int L, int R) {
  digitalWrite(leftIn1, HIGH);
  digitalWrite(leftIn2, LOW);
  analogWrite(leftPWM, L);

  digitalWrite(rightIn1, HIGH);
  digitalWrite(rightIn2, LOW);
  analogWrite(rightPWM, R);
  delay(20);
}

void turnRightSharp(int Lspeed, int Rspeed) {
  digitalWrite(leftIn1, LOW);
  digitalWrite(leftIn2, HIGH);
  analogWrite(leftPWM, Lspeed);
  digitalWrite(rightIn1, HIGH);
  digitalWrite(rightIn2, LOW);
  analogWrite(rightPWM, Rspeed);
  delay(20);
}

void turnLeftSharp(int Lspeed, int Rspeed) {
  digitalWrite(leftIn1, HIGH);
  digitalWrite(leftIn2, LOW);
  analogWrite(leftPWM, Lspeed);
  digitalWrite(rightIn1, LOW);
  digitalWrite(rightIn2, HIGH);
  analogWrite(rightPWM, Rspeed);
  delay(20);
}



void stopMotors() {
  analogWrite(leftPWM, 0);
  analogWrite(rightPWM, 0);
  digitalWrite(leftIn1, LOW);
  digitalWrite(leftIn2, LOW);
  digitalWrite(rightIn1, LOW);
  digitalWrite(rightIn2, LOW);
  delay(20);
}

long readUltrasonicCM() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long dur = pulseIn(echoPin, HIGH, 30000);
  if (dur == 0) return -1;

  return dur / 58;
}
