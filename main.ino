#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h> 

//piny
#define SERVO_PIN 3   
#define RED_LED   4 
#define GREEN_LED 5  
#define ECHO_PIN  6 
#define TRIG_PIN  7
#define BUZZER    8 
#define RST_PIN   9   
#define SS_PIN    10  
// MOSI -> 11, MISO -> 12, SCK -> 13

//moduły
MFRC522 mfrc522(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27, 16, 2); //upewnij się, czy LCD ma 0x27, zdarza się, że nie
Servo mojZamek; // obiekt serwomechanizmu

bool ktosWpoblizu = false;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  
  lcd.init();
  lcd.backlight();
  
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  // konfiguracja serwa
  mojZamek.attach(SERVO_PIN);
  mojZamek.write(0); 
  
  // włączenie czerwonej diody na stałe 
  digitalWrite(RED_LED, HIGH);
  
  WyswietlCzuwanie();
}

void loop() {
  long odleglosc = MierzOdleglosc();
  
  // system reaguje <= 20 cm
  if (odleglosc > 0 && odleglosc <= 20) { 
    if (!ktosWpoblizu) {
      WyswietlProsbe();
      ktosWpoblizu = true;
    }
    
    // szukanie pestki/karty RFID
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
      
      String odczytanyTag = "";
      for (byte i = 0; i < mfrc522.uid.size; i++) {
        odczytanyTag.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
        odczytanyTag.concat(String(mfrc522.uid.uidByte[i], HEX));
      }
      odczytanyTag.toUpperCase();
      odczytanyTag = odczytanyTag.substring(1); 
      
      Serial.print("Zeskanowano UID: ");
      Serial.println(odczytanyTag);
      
      // sprawdzanie kodów (tutaj wpisz swoje kody)
      if (odczytanyTag == "F7 7F 25 15" || odczytanyTag == "03 02 33 2D" || odczytanyTag == "93 9E 40 16") {
        DostepPrzyznany();
      } 
      // odmowa innych kodów
      else {
        OdmowaDostepu();
      }
      
      ktosWpoblizu = false; 
    }
    
  } else {
    if (ktosWpoblizu) {
      WyswietlCzuwanie();
      ktosWpoblizu = false;
    }
  }
  delay(50); 
}

//funkcje pomocniczne

long MierzOdleglosc() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long czas = pulseIn(ECHO_PIN, HIGH, 20000); 
  return czas * 0.034 / 2;
}

void WyswietlCzuwanie() {
  lcd.clear();
  lcd.setCursor(0, 0); lcd.print("System gotowy.");
}

void WyswietlProsbe() {
  lcd.clear();
  lcd.setCursor(0, 0); lcd.print("Zbliz pestke");
  lcd.setCursor(0, 1); lcd.print("do czytnika");
}

void DostepPrzyznany() {
  lcd.clear();
  lcd.setCursor(0, 0); lcd.print("Dostep przyznany");
  lcd.setCursor(0, 1); lcd.print("Zapraszamy");
  
  digitalWrite(GREEN_LED, HIGH); // zapalenie zielonej diody
  mojZamek.write(90);            // otwarcie drzwi przez serwo
  
  // dźwięk powitalny
  tone(BUZZER, 1000, 150); delay(150);
  tone(BUZZER, 1400, 150); delay(150);
  tone(BUZZER, 1800, 300); delay(300);
  
  delay(2400); // odczekuje tak żeby było 3s
  
  digitalWrite(GREEN_LED, LOW);  // gasi zieloną diodę
  mojZamek.write(0);             // zamykanie drzwi
  digitalWrite(RED_LED, HIGH);   // czerwona na pewno świeci
}

void OdmowaDostepu() {
  lcd.clear();
  lcd.setCursor(0, 0); lcd.print("Brak dostepu!");
  lcd.setCursor(0, 1); lcd.print("Karta zablokowana");
  
  tone(BUZZER, 250); //alarm
  
  // mruganie czerwonej diody
  for (int i = 0; i < 6; i++) {
    digitalWrite(RED_LED, LOW);
    delay(250);
    digitalWrite(RED_LED, HIGH);
    delay(250);
  }
  
  noTone(BUZZER); // wyłączenie dźwięku po 3 sekundach
  digitalWrite(RED_LED, HIGH); //powrót czerwonej diody
}