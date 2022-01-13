// program used to control the voltage and current of the psu

#include <U8glib.h>


// pin definition
#define vMes 14   // Voltage measurement from opamp 2-4V
#define cMes 15   // Current measurement from opamp 2-4V
#define Vset 3    // voltage set signal going to LT1085
#define cA 2      // Current rotary encoder pin A
#define cB 4      // Current rotary encoder pin B
#define vA 5      // Voltage rotary encoder pin A
#define vB 6      // Voltage rotary encoder pin B
#define ocp 7     // Overcurrent protection
#define pLed 8    // Power led
#define outSw 11  // Output switch

//U8GLIB_SSD1306_128X64(U8G_I2C_OPT_NONE)

// value definition
int caState;           // Current rotary encoder pin A state
int cbState;           // Current rotary encoder pin B state
int vaState;           // Voltage rotary encoder pin A state
int vbState;           // Voltage rotary encoder pin B state
uint16_t currentVal = 0; // Current value read from Cmes. 10-bit
int16_t currentLim = 0; // Current limit set by encoder. 10-bit
int16_t voltageVal = 0;  // Measured voltage value from voltage divider 
float voltage = 0.0; // 0-15V
float current = 0.0; // 0-1A

void setup() {
  // pinmodes of every I/O
  pinMode(cMes, INPUT);
  pinMode(vMes, INPUT);
  pinMode(cA, INPUT_PULLUP);
  pinMode(cB, INPUT_PULLUP);
  pinMode(vA, INPUT_PULLUP);
  pinMode(vB, INPUT_PULLUP);
  pinMode(ocp, OUTPUT);
  pinMode(pLed, OUTPUT);
  pinMode(outSw, INPUT);

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
  if(newStateA != caState) {  // If-statement for adjusting currentlim value
    if(newStateA != cbState) 
      currentLim ++;
    else
      currentLim --;
  }
  if(currentLim > 255)  // 236 = 1A
    currentLim = 255;
  if(currentLim < 0)
    currentLim = 0;
  caState = newStateA;  // Set the new state of encoder
  cbState = newStateB;

  //Serial.print(currentLim);
  //Serial.print("              ");
}

void setVoltage(){  // Function will set the voltage limit
  int newStateA = digitalRead(vA);
  int newStateB = digitalRead(vB);
  if(newStateA != vaState) {  // If-statement for adjusting currentlim value
    if(newStateA != vbState) 
      voltageVal ++;
    else
      voltageVal --;
  }
  if(voltageVal > 218)  // 218 = 15V
    voltageVal = 218;
  if(voltageVal < 0)
    voltageVal = 0;
  vaState = newStateA;  // Set the new state of the encoder
  vbState = newStateB;
  analogWrite(Vset, voltageVal);  // Write the voltage value to the opamp
  
  //Serial.println(voltageVal);
  //Serial.print(currentLim);
  //Serial.print("--------");
}

void measureVoltage(){  // function used to measure output voltage
  float error = 0;  // mean error
  voltage = 15.0*analogRead(vMes)/1012.0 + error;  // 0-15V. 1012 = 15V
  
  //Serial.println(analogRead(vMes));
  //Serial.println(voltage,1);
}

void measureCurrent(){
  currentVal = analogRead(cMes);  // 10-bit value for the current
  current = map(currentVal, 0, 1023, 0, 1000);  // Map the value to mA/1000 = A
  currentVal = map(currentVal, 0, 1023, 0, 255);  // Map for if statement in loop
  Serial.println(current/1000);
}

void draw(){  // used for graphics on oled 

}

void loop() {
  if(digitalRead(outSw) == 1)  // Check if output is high
    digitalWrite(pLed, HIGH);  // power led on
  else
    digitalWrite(pLed, LOW);  // poer led off
  setCurrentLimit(); // Read the current limit
  setVoltage();  // Set the voltage 
  measureVoltage();  // Measure the voltage
  measureCurrent();  // Measure the current
  if(currentVal > currentLim){  // Compare the current and the limit
    digitalWrite(ocp, HIGH); // Limit the current if val > limit
  }
  else{
    digitalWrite(ocp, LOW);
  }  

  /* picture loop
  u8g.firstPage();  
  do {
    draw();
  } while( u8g.nextPage() );*/ 
 
}
