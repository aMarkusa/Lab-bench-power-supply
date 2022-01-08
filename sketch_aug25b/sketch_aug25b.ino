// program used to control the voltage and current of the psu

// pin definition
#define vMes A0   // Voltage measurement from opamp 2-4V
#define cMes A1   // Current measurement from opamp 2-4V
#define Vset 3    // voltage set signal going to LT1085
#define cA 2      // Current rotary encoder pin A
#define cB 4      // Current rotary encoder pin B
#define vA 5      // Voltage rotary encoder pin A
#define vB 6      // Voltage rotary encoder pin B
#define ocp 7     // Overcurrent protection
#define pLed 8    // Power led
#define outSw 12  // Output switch

// value definition
int caState;           // Current rotary encoder pin A state
int cbState;           // Current rotary encoder pin B state
int vaState;           // Voltage rotary encoder pin A state
int vbState;           // Voltage rotary encoder pin B state
uint16_t currentVal = 0; // Current value read from Cmes. 10-bit
uint16_t currentLim = 0; // Current limit set by encoder. 10-bit
uint16_t voltage = 0; // Voltage limit set by encoder. 10-bit
uint16_t voltageMesVal = 0;  // Measured voltage value from voltage divider 
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

  // read states of encoders
  caState = digitalRead(cA);
  cbState = digitalRead(cB);
  vaState = digitalRead(vA);
  vbState = digitalRead(vB);

   Serial.begin(115200);
}

void setCurrentLimit(){  // Function will set the current limit value
  int newStateA = digitalRead(cA);
  int newStateB = digitalRead(cB);
    if(newStateB != caState) {  // If-statement for adjusting currentlim value
      if(newStateB != cbState) 
        currentLim ++;
      else
        currentLim --;
    }
  if(currentLim > 255)  // Value cant be over 255
    currentLim = 255;
}

void setVoltage(){  // Function will set the voltage limit
  int newStateA = digitalRead(vA);
  int newStateB = digitalRead(vB);
    if(newStateB != vaState) {  // If-statement for adjusting voltage value
      if(newStateB != vbState) 
        voltage ++;
      else
        voltage --;
    }
  if(voltage > 255)  // Value cant be over 255
    voltage = 255;
  analogWrite(Vset, voltage);
}

float measureVoltage(){
  voltageMesVal = 15/1023 * analogRead(vMes);  // 10-bit voltage value(1023 = 15V)
  return roundf(voltageMesVal * 100) / 100;  // return voltage with 2 decimals
}

void loop() {
  if(digitalRead(outSw) == 1)  // Check if output is high
    digitalWrite(pLed, HIGH);  // power led on
  else
    digitalWrite(pLed, LOW);  // poer led off
  setCurrentLimit(); // Read the current limit
  setVoltage();  
  currentVal = analogRead(cMes);  // Read the current value 
  if(currentVal > currentLim){  // Compare the current and the limit
    digitalWrite(ocp, HIGH); // Limit the current if val > limit
  }
  else{
    digitalWrite(ocp, LOW);
  }          
 
}
