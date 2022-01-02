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
uint16_t voltage = 0; // Voltage limit set by encoder. 10-bit
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
  // read states of encoders
  caState = digitalRead(cA);
  cbState = digitalRead(cB);
  vaState = digitalRead(vA);
  vbState = digitalRead(vB);
  // set current and voltage to 0
}

void setCurrentLimit(){  // Function will set the current limit value
  int newStateA = digitalRead(cA);
  int newStateB = digitalRead(cB);
    if(newStateB != caState) {
      if(newStateB != cbState) 
        currentLim ++;
      else
        currentLim --;
    }
  if(currentLim > 255) 
    currentLim = 255;
}

void setVoltage(){  // Function will set the voltage limit
  int newStateA = digitalRead(vA);
  int newStateB = digitalRead(vB);
    if(newStateB != vaState) {
      if(newStateB != vbState) 
        voltage ++;
      else
        voltage --;
    }
  if(voltage > 255) 
    voltage = 255;
  analogWrite(Vset, voltage);
}



void loop() {
  setCurrentLimit(); // Read the current limit
  setVoltageLimit();
  currentVal = analogRead(cMes);  // Read the current value 
  if(currentVal > currentLim){  // Compare the current and the limit
    digitalWrite(ocp, HIGH); // Limit the current iv val > limit
  }
  else{
    digitalWrite(ocp, LOW);
  }          
 

 

  digitalWrite(ocp, LOW);
}
