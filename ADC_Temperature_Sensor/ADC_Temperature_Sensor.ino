#define NbADCvalues 800

uint16_t adcValueIndex = 0;
volatile uint32_t AnalogData = 0;

uint16_t ADCvalue[NbADCvalues];

uint32_t lastPrint = 0;
uint16_t intervalPrint = 100;

void setup() {
  for (int i = 0; i < NbADCvalues; i++) {
    ADCvalue[i] = 0;
  }
  Serial.begin(115200);
  ADMUX = 0b11001000;
  ADCSRA = 0b10101111;
  ADCSRA |= 0b01000000;
  ADCSRB = 0b00000000;
}

void loop() {
  uint32_t Now = millis();

  if ((Now - lastPrint) >= intervalPrint) {

    cli();
    uint32_t adcCopy = AnalogData;
    sei();

    float Temp = adcCopy * 1.0 / NbADCvalues;
    Temp = (Temp - 324.31 ) / 1.22;

    delay(1);
    Serial.println(Temp);
    lastPrint = Now;
  }
}

ISR(ADC_vect) {
  AnalogData -= ADCvalue[adcValueIndex];
  ADCvalue[adcValueIndex] = ADCL | (ADCH << 8);
  AnalogData += ADCvalue[adcValueIndex];
  adcValueIndex++;

  if (adcValueIndex == NbADCvalues) {
    adcValueIndex = 0;
  }
}


