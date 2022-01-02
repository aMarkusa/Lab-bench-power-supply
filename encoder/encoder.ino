const int A = 2;
const int B = 3;
int stateA;
int stateB;
uint16_t count = 0;



void setup() {
  pinMode(A, INPUT_PULLUP);
  pinMode(B, INPUT_PULLUP);
  Serial.begin(115200);
  stateA = digitalRead(A);
  stateB = digitalRead(B);
}

void loop() {
  int newStateA = digitalRead(A);
  int newStateB = digitalRead(B);
  if (newStateB != stateB) {
    if (newStateB != stateA) 
      count ++;
    else
      count --;
  }
  Serial.println(count);
  stateA = newStateA;
  stateB = newStateB;
}
