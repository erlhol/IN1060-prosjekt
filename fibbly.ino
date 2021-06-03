#include <Adafruit_NeoPixel.h> // importerer neopixel-biblioteket

// pins til arduinoen
const int tiltSvitsj = 8;
const int vibrasjonsMotor = 7;
const int lysStripe = 6;

// verdier til led-stip
const int antPixler = 53;
const int delayValue = 100; // sier noe om hastigheten på lysene
                            //lavere verdi er høyere hastighet
                            // gitt i millisekunder

// verdier til tiltSvitsjen                           
int forrigeSvitsjTilstand;
int tilstandNaa;
int antRist = 0;

unsigned long millisSek = 0; // holder styr på antmillisekunder siden
                             // programmet ble startet

// verdier for å resette antRist hvert 5. sekund
unsigned long startMillis = 0;
unsigned long periode = 5000;

// verdier for å initiere lysing hvert 1.5 minutt
// i det endelige produktet ville dette vært byttet ut med kommunikasjon fra annen arduino
unsigned long vennLyserPeriode = 90000; // hvert 90. sekund
unsigned long startVennLyser = 0;

boolean timerPaa = false;

// oppretter et neoPixel objekt
Adafruit_NeoPixel pixels(antPixler, lysStripe, NEO_GRB + NEO_KHZ800);

void setup() {
  pinMode(tiltSvitsj,INPUT);
  pinMode(vibrasjonsMotor,OUTPUT);
  pixels.begin();
  forrigeSvitsjTilstand = digitalRead(tiltSvitsj);
  tilstandNaa = digitalRead(tiltSvitsj);
}

void loop() {
   tilstandNaa = digitalRead(tiltSvitsj);
   if (tilstandNaa != forrigeSvitsjTilstand) {
    timerPaa = true;
    antRist ++;
   }
   forrigeSvitsjTilstand = tilstandNaa;

   if (antRist >10) {
     // for å forhindre at antRist samler seg opp ettersom tiden går
       duHarRistet();
       antRist = 0;
    }
    resetTrykk();
    vennLyser();
}    

void resetTrykk() {
  millisSek = millis();
    if (millisSek - startMillis >= periode) {
          startMillis = millisSek;
          antRist = 0;
    }
}

void vennLyser() {
  millisSek = millis();
    if (millisSek - startVennLyser >= vennLyserPeriode) {
          startVennLyser = millisSek;
          vennHarRistet();
    }
}

void vennHarRistet() {
  // ikke implementert kommunikasjon mellom arduinoene
  // i koden har vi satt at vennen rister hver 90. sekund
  pixels.clear();
       for(int i=0; i<antPixler; i++) {
        pixels.setPixelColor(i,pixels.Color(0,255,0));
        pixels.setBrightness(255);
        pixels.show();
        delay(delayValue);
       }
      
}

void duHarRistet() {
      pixels.clear();
      int styrke = 255;
      for(int i=0; i<antPixler; i++) {
        pixels.setPixelColor(i, pixels.Color(0, 255, 255));
        pixels.setBrightness(styrke);
        styrke -= 5;
        pixels.show();
        vibrer();
        delay(delayValue);
        skruAvVibrasjon();
       }
}

void vibrer() {
  digitalWrite(vibrasjonsMotor,HIGH);
}

void skruAvVibrasjon() {
  digitalWrite(vibrasjonsMotor,LOW);
}
