**System wejścia za pomocą karty**

Jest to dość prosty projekt inteligentnego zamka, który możesz wykonać samodzielnie.

Jak on działa?
- system czuwania - gdy w pobliżu (mniej niż 20 cm) nie stoi żaden obiekt, na ekranie LCD widnieje napis "System gotowy"
- uruchomienie systemu - gdy użytkownik podejdzie na mniej niż 20 cm od czujnika, uruchomi się czujnik oraz zmieni się napis na "zbliż pestkę do czytnika"
- wejście - poprawna pestka zapala zieloną diodę, wydaje miły dźwięk oraz otwiera drzwi za pomocą servo
- blokada - dwie pestki są niezarejestrowane, po przybliżeniu włącza się ciągły sygnał, czerwony LED mruga, a drzwi nie otwierają się 
- logi - w monitorze otrzymujemy informacje o kodzie pestki/karty RFID 

Potrzebny sprzęt:
- Arduino UNO bądź jego odpowiednik
- płytka stykowa
- ekran LCD 
- skaner RFID
- pestki / karty RFID
- servo 
- czujnik odległości 
- buzzer
- kable żeńsko - męskie
- kable męsko - męskie
- rezystory 
- zielona lampka LED
- czerwona lampka LED

**Krok 1 - oczytanie kodów**
Zanim ruszymy do naszego kodu głównego oraz całego systemu wejścia, musimy zeskanować UID z 3 pestek/kart (lub więcej, jeśli masz). Pamiętaj, aby zostawić dwie lub trzy, które nie zostaną wpisane do naszego systemu. 

Należy połączyć skaner RFID z naszym Arduino:  
3.3V -> PIN 3.3V
GND  -> PIN GND
RST -> PIN 9
SDA(SS) -> PIN 10
MOSI -> PIN 11
MISO -> PIN 12
SCK -> PIN 13

**WAŻNE: nie możesz używać innych pinów**
Dlaczego? Arduino UNO w tych miejscach posiada sprzętowy kontroler SPI, dzięki czemu możemy otrzymać prawidłowe dane z pestek/kart.

Aby zeskanować pestki/karty, można użyć biblioteki MFRC522-spi-i2c-uart-async i wybrać rfid_default_keys.

Uruchamiając ten kod włączamy Serial Monitor i zbliżamy pestkę/kartę do czytnika. Na monitorze otrzymujemy informację o UID pestki/ karty. Kopiujemy je i zapisujemy na później. 

**Krok 2 - system otwierania drzwi**
Po zgraniu i zapisaniu UID pestek/kart zabieramy się za najważniejszą część projektu. 
Podłączenia:
buzzer -> PIN 8
GND -> PIN GND (lub przez rezystor, jeśli chcesz go ściszyć)

**czujnik**
trig -> PIN  7
echo -> PIN  6
GND -> PIN GND
VCC  -> PIN 5V

**LED**
Zielony LED:
Anoda (dłuższa nóżka +) -> PIN 5
Katoda (krótsza nóżka -) -> rezystor -> PIN GND

Czerwony LED:
Anoda (dłuższa nóżka +) -> PIN 4
Katoda (krótsza nóżka -) -> rezystor -> PIN GND

**LCD**
GND -> PIN GND
VCC  -> PIN 5V
SDA  -> PIN SDA
SCL  -> PIN SCL

**servo**
orange/yellow -> PIN 3
red -> 5V/VIN
brown -> GND

Jako że mamy wiele pinów do GND, warto połączyć je na płytce w jednym rzędzie i jeden kabel zaprowadzić do GND. 

Na servo można przykleić papierowe drzwi dla efektu.

Kod znajduje się w pliku o nazwie main.cpp

**Uruchomienie**
Wszystko gotowe? Podłącz Arduino i wgraj kod. Jeśli wszystko dobrze jest podłączone, na ekranie pojawi się napis "System gotowy" lub "Zbliż pestkę do czytnika", w zależności od tego, czy jakiś obiekt nie stoi bliżej niż 20cm od czytnika. Zbliż pestki/karty i zobacz, co się stanie!

 
