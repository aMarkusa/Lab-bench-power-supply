// program used to control the voltage and current of the psu

const int vMes = A0;   // Voltage measurement from opamp 2-4V
const int cMes = A1;   // Current measurement from opamp 2-4V
const int cA = 2;      // Current rotary encoder pin A
const int cB = 4;      // Current rotary encoder pin B
int caState;           // Current rotary encoder pin A state
int cbState;           // Current rotary encoder pin B state
const int vA = 5;      // Voltage rotary encoder pin A
const int vB = 6;      // Voltage rotary encoder pin B
int vaState;           // Voltage rotary encoder pin A state
int vbState;           // Voltage rotary encoder pin B state
const int ocp = 7;     // Overcurrent protection
const int pLed = 8;    // Power led
const int outSw = 12;  // Output switch
uint16_t currentVal = 0; // Current value read from Cmes. 10-bit
uint16_t currentLim = 0; // Current limit set by encoder. 10-bit
uint16_t voltageLim = 0; // Voltage limit set by encoder. 10-bot
uint8_t count = 0;

void setup() {
  // pinmodes of every I/O
  pinMode(cMes, INPUT);
  pinMode(vMes, INPUT);
  pinMode(cA, INPUT);
  pinMode(cB, INPUT);
  pinMode(vA, INPUT);
  pinMode(vB, INPUT);
  pinMode(ocp, OUTPUT);
  pinMode(pLed, OUTPUT);
  pinMode(outSw, INPUT);
  pinMode(caState, INPUT_PULLUP);
  pinMode(cbState, INPUT_PULLUP);
  pinMode(vaState, INPUT_PULLUP);
  pinMode(vbState, INPUT_PULLUP);
  Serial.begin(115200);
}

int setCurrentLimit(){  // Function will set the current limit value
  uint16_t value = analogRead(cA);
  return value;
}

int setVoltageLimit(voltageLim){  // Function will set the voltage limit
  voltageLim = map(voltageLim, 0, 1023, 0, 255);
  analogWrite(Vset, voltageLim);
  return 0;
}



void loop() {
  currentLimit = setCurrentLimit(); // Read the current limit
  currentVal = analogRead(cMes);  // Read the current value           
 

  while (currentVal >= currentLimit) {  // Compare value and limit
    digitalWrite(ocp, HIGH);            // Set ocp high if value >= limit
    currentVal = analogRead(cMes);
    currentLimit = setCurrentLimit();
  
  }

  digitalWrite(ocp, LOW);
}
