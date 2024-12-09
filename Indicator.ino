#define M 3
#define POT A0

#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978



#define dataPin 10
#define latchPin 11
#define clockPin 12

#define DEBOUNCE_THRESHOLD 1
#define SMOOTHING_FACTOR 0.1
int smoothedValue = 0;
int previousTimer = -1;
int timer = 0;

int melodyDelayMillis =50 ;

bool melodyplay;
unsigned long currentMillis;  
unsigned long previousMillis = 0; 
unsigned long melodyStartMillis = 0;
unsigned long countdownMillis = 0;
byte digts[10] = {
  B1111110,  
  B00110000,  
  B01101101,  
  B01111001, 
  B00110011,  
  B01011011,  
  B1011111,  
  B1110000,  
  B1111111,  
  B1111011  
};

int newTimer;
int melody[] = {
  NOTE_E4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_G4, NOTE_F4, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_B3, NOTE_B3, NOTE_B3, NOTE_C4,
  NOTE_D4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_E4, NOTE_E4, NOTE_E4,
  NOTE_E4, NOTE_E4, NOTE_D4, NOTE_D4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_D4, NOTE_D4, NOTE_C4, NOTE_C4
};



void Melodyplay(){
  for(int thisNote = 0; thisNote < 34; thisNote++){
    tone(M,melody[thisNote]);
        
    }
}

void setup() {

  Serial.begin(9600);


  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(M, OUTPUT);
  pinMode(POT, INPUT);

  digitalWrite(latchPin, OUTPUT);
}


void Indicator(int num){
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, digts[num] );
  digitalWrite(latchPin, HIGH);
  delay(50);
}


void loop() {
  int rawPotValue  = analogRead(POT);
  smoothedValue = (int)(SMOOTHING_FACTOR * rawPotValue + (1 - SMOOTHING_FACTOR) * smoothedValue);
  newTimer = map(smoothedValue, 0, 1024, 0, 9);
  Serial.println(smoothedValue);


  if (abs(newTimer - previousTimer) >= DEBOUNCE_THRESHOLD) {
    previousTimer = newTimer;  
    timer = newTimer;         
    Indicator(timer);          
  }

  delay(50);
  currentMillis = millis();
  if (timer == 0 && !melodyplay) {
    if (currentMillis - melodyDelayMillis >= 3000) { 
      melodyplay = true; 
      Melodyplay(); 

    }
  }

  if (currentMillis - countdownMillis >= 1000) {
    countdownMillis = currentMillis;
    
    if (timer > 0) {
      timer--;
      Indicator(timer);  
    }
  }

  if (timer+10 == 0 && !melodyplay) {
    melodyplay = true;
    Melodyplay();

  }
  
  if (currentMillis - previousMillis >= 1000) {
    previousMillis = currentMillis;
    if (timer > 0) {
      timer--;
      Indicator(timer);  
    }
     if (melodyplay) {
    return;  // Прерываем выполнение, чтобы мелодия продолжала играть
  }
  }


}
