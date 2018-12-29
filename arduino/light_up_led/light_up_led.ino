// Blow Out LED Ave creates an LED that you can blow out. It automatically relights after 2 seconds
//
// Copyright 2018, Paul H. Dietz

// LED Connections
#define PLUS A1               // High side of the resistor
#define MEASURE A0            // Low side of resistor and anode of LED
// Cathode of LED goes to ground

#define NUMSAMPLES 32         // Number of samples to keep
#define MINJUMP 15000           // Minimum jump for blow out

uint32_t sensedata[NUMSAMPLES];
int dataptr = 0;
int buffull = 0;

void setup() {
  Serial.begin(115200);                     // Initialize serial communication
  pinMode(MEASURE, INPUT);
  //  pinMode(PLUS, /);
  pinMode(A2, OUTPUT);
  digitalWrite(A2, LOW);
  //  digitalWrite(PLUS, /);                 // Turn on the LED

  //  pinMode(PLUS, INPUT);/

  analogRead(PLUS);

  for (uint16_t i=0; i<NUMSAMPLES; i++) {
    sensedata[i] = 0xFFFFFF;
  }
}

void loop() {
  int cnt;
  uint32_t sum = 0;

  // Sum 256 adc readings (to reduce adc noise)
  for (cnt = 0; cnt < 256; cnt++)
  {
    sum = sum + analogRead(MEASURE);
  }
                    // Output sum so we can watch with Serial Plotter

  uint32_t avg = 0;
  for (uint16_t i=0; i<NUMSAMPLES; i++) {
    avg += sensedata[i];
  }

  avg /= NUMSAMPLES;
  int32_t delta = sum - avg;

  Serial.println(sum);
//  Serial.println(delta);

  
  if (delta > MINJUMP) {
    pinMode(PLUS, OUTPUT);
    digitalWrite(PLUS, HIGH);
    delay(2000); 
    digitalWrite(PLUS, LOW);
    pinMode(PLUS, INPUT);
    analogRead(PLUS);
    delay(1000);
  }
  
  {
    sensedata[dataptr] = sum;               // Store the latest data in the buffer
    dataptr = (dataptr + 1) % NUMSAMPLES;
  }

  delay(1);
}
