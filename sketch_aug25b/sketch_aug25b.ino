// program used to control the voltage and current of the psu

const int cs = A0; // Current measurement from opamp 2-4V
const int ocp = 2; // Overcurrent protection
const int cA = 3; // Current rotary encoder pin A
const int cB = 4; // Current rotary encoder pin B
const int cButton = 5; // Current rotary encoder button
const int vA = 6; // Voltage rotary encoder pin A
const int vB = 7; // Voltage rotary encoder pin B
const int vButton = 8; // Voltage rotary encoder button
const int ocLed = 9; // Overcurrent led
const int pLed = 10; // Power led
uint16_t value = 0;
uint16_t currentVal = 0;
uint16_t currentLimit = 0;

void setup() {
  pinMode(cs, INPUT);
  pinMode(ocp, OUTPUT);
  pinMode(cA, INPUT);
  pinMode(cB, INPUT);
  pinMode(cButton, INPUT);
  pinMode(vA, INPUT);
  pinMode(vB, INPUT);
  pinMode(vButton, INPUT);
  pinMode(ocLed, OUTPUT);
  pinMode(pLed, OUTPUT);
  Serial.begin(115200);

}

int setCurrentLimit(){  // Function will set the current limit value
  value = analogRead(cA);
  return value;
  }



void loop() {
  currentVal = analogRead(cs);  // Read the current value
  currentLimit = 650;  // Read the current limit
  Serial.print(currentVal);
  Serial.print("\t");
  Serial.println(currentLimit);
  
  while (currentVal >= currentLimit){  // Compare value and limit
    digitalWrite(ocp, HIGH);  // Set ocp high if value >= limit
    currentVal = analogRead(cs);  
    currentLimit = 650;
    Serial.print(currentVal);
    Serial.print("\t");
    Serial.println(currentLimit);
  }
    
  digitalWrite(ocp, LOW);
}
