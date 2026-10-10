
/*
  LAM - Case 1
  Pump 1: 50% duty cycle
  Pump 2: 100% duty cycle
  Float LOW: Pump 1 OFF, solenoid OFF
  Pump 2 remains ON
*/

// Replace these with the actual ESP32 GPIO numbers
const int P1_RPWM =  /* GPIO for R_PWM1 */;
const int P1_LPWM =  /* GPIO for L_PWM1 */;
const int P1_REN  =  /* GPIO for R_EN1  */;
const int P1_LEN  =  /* GPIO for L_EN1  */;

const int P2_RPWM =  /* GPIO for R_PWM2 */;
const int P2_LPWM =  /* GPIO for L_PWM2 */;
const int P2_REN  =  /* GPIO for R_EN2  */;
const int P2_LEN  =  /* GPIO for L_EN2  */;

const int FLOAT_PIN = /* GPIO connected to FS */;
const int SOLENOID_PIN = /* GPIO connected to SV */;

// 8-bit PWM: 0 to 255
const int P1_DUTY = 128;  // approximately 50%
const int P2_DUTY = 255;  // 100%

void setup() {
  Serial.begin(115200);

  pinMode(P1_REN, OUTPUT);
  pinMode(P1_LEN, OUTPUT);
  pinMode(P2_REN, OUTPUT);
  pinMode(P2_LEN, OUTPUT);

  pinMode(P1_RPWM, OUTPUT);
  pinMode(P1_LPWM, OUTPUT);
  pinMode(P2_RPWM, OUTPUT);
  pinMode(P2_LPWM, OUTPUT);

  pinMode(FLOAT_PIN, INPUT_PULLUP);
  pinMode(SOLENOID_PIN, OUTPUT);

  // Start with motor drivers disabled
  digitalWrite(P1_REN, LOW);
  digitalWrite(P1_LEN, LOW);
  digitalWrite(P2_REN, LOW);
  digitalWrite(P2_LEN, LOW);

  analogWrite(P1_RPWM, 0);
  analogWrite(P1_LPWM, 0);
  analogWrite(P2_RPWM, 0);
  analogWrite(P2_LPWM, 0);

  // Assumption: LOW de-energizes the solenoid driver
  digitalWrite(SOLENOID_PIN, LOW);

  delay(500);

  // Enable motor drivers
  digitalWrite(P1_REN, HIGH);
  digitalWrite(P1_LEN, HIGH);
  digitalWrite(P2_REN, HIGH);
  digitalWrite(P2_LEN, HIGH);

  // Pump 2 runs continuously at 100%
  analogWrite(P2_RPWM, P2_DUTY);
  analogWrite(P2_LPWM, 0);

  Serial.println("Case 1 controller started");
}

void loop() {
  int floatState = digitalRead(FLOAT_PIN);

  if (floatState == LOW) {
    // High sump level: stop incoming flow
    analogWrite(P1_RPWM, 0);
    analogWrite(P1_LPWM, 0);

    // Turn solenoid driver OFF
    digitalWrite(SOLENOID_PIN, LOW);

    Serial.println("FLOAT TRIGGERED: P1 OFF, P2 ON, SOLENOID OFF");
  }
  else {
    // Normal operation: Pump 1 at 50%
    analogWrite(P1_RPWM, P1_DUTY);
    analogWrite(P1_LPWM, 0);

    // Turn solenoid driver ON
    digitalWrite(SOLENOID_PIN, HIGH);

    Serial.println("NORMAL: P1 50%, P2 100%, SOLENOID ON");
  }

  delay(100);
}
 