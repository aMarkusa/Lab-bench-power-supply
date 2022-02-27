// program used to control the voltage and current of the psu

#include <Wire.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


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

//U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);

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

  // Setup display
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  delay(1000);
  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(WHITE);

  // Initialize pin changer interrupts
  PCICR |= B00000100;  // PCIE2
  PCMSK2 |= B01110100; // pins 2,4,5,6

   Serial.begin(115200); 
}

// Interrupt sequence
ISR (PCINT2_vect){
  static unsigned long lastInterruptTime = 0;
  unsigned long interruptTime = millis();
  if (interruptTime - lastInterruptTime > 2){  // Check if 2ms has passed. Removes encoder "bouncing"
    int newStateAv = digitalRead(vA);  // Reads all the states
    int newStateBv = digitalRead(vB);
    int newStateAc = digitalRead(cA);
    int newStateBc = digitalRead(cB);
    if(newStateAv != vaState) {  // If statement = 1, adjust voltage
      if(newStateAv != vbState) 
        voltageVal ++;
      else
        voltageVal --;
      voltageVal = constrain(voltageVal, 0, 218);  // 218 = 15V
      analogWrite(Vset, voltageVal);  // Write the voltage value to the opamp
    }
    if(newStateAc != caState){  // If statement = 1, adjust current
      if(newStateAc != cbState) 
        currentLim ++;
      else
        currentLim --;

      currentLim = constrain(currentLim, 0, 255);  // 255 = 1A
    }
    caState = newStateAc;  // Set the new states of encoders
    cbState = newStateBc;
    vaState = newStateAv; 
    vbState = newStateBv;
  } 
  lastInterruptTime = interruptTime;
}

void measureVoltage(){  // function used to measure output voltage
  //float error = 0;  // mean error
  voltage = 15.0*analogRead(vMes)/1012.0;  // 0-15V. 1012 = 15V
  
  //Serial.println(analogRead(vMes));
  //Serial.println(voltage,1);
}

void measureCurrent(){
  currentVal = analogRead(cMes);  // 10-bit value for the current
  current = map(currentVal, 0, 1023, 0, 1000);  // Map the value to mA/1000 = A
  current = 1.0*current/1000.0;
  currentVal = map(currentVal, 0, 1023, 0, 255);  // Map for if statement in loop
  //Serial.println(current/1000);
}

void draw(float v, float c){  // used for graphics on oled 
  display.setCursor(0, 10);
  display.print(v);
  display.println(" V");
  display.print(c);
  display.print(" A");
  display.display();
  display.clearDisplay();
}

void loop() {
  Serial.println(currentLim);
  if(digitalRead(outSw) == 1)  // Check if output is high
    digitalWrite(pLed, HIGH);  // power led on
  else
    digitalWrite(pLed, LOW);  // power led off
  measureVoltage();  // Measure the voltage
  measureCurrent();  // Measure the current
  if(currentVal > currentLim){  // Compare the current and the limit
    digitalWrite(ocp, HIGH); // Limit the current if val > limit
  }
  else{
    digitalWrite(ocp, LOW);
  } 
  //Serial.println(voltageVal);
  draw(voltage, current);
}
