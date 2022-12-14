#include <dummy.h>

#include <dummy.h>

#include <BluetoothSerial.h>


// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings

#define BLYNK_TEMPLATE_ID "TMPLC_9wwfMb"
#define BLYNK_DEVICE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "-ls7CxBRCpGBp08KK9S8h3nYW5n2rHlW"

// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <time.h>
#include <SPI.h>
#include <DHT.h>

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Abhishek";
char pass[] = "9066639178";

BlynkTimer timer;

int reading;

// NTP Server Details
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 16200;
const int daylightOffset_sec = 3600;

#define wifiLed 16  //D0
float tempe;
float soil;
float Humid;
float light;
// LED's
#define red 5
#define blue 32
#define green 27
#define yellow 12
#define soilpin 39
#define ldr 33

int buttonState;            // current reading from the input pin
int lastButtonState = LOW;  // previous reading from the input pin

unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 200;   // the debounce time; increase if the output flickers

// Screens
int displayScreenNum = 0;
int displayScreenNumMax = 4;

unsigned long lastTimer = 0;
unsigned long timerDelay = 10000;

#define DHTPIN 36
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// OLED Display
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

#define I2Cdisplay_SDA 21
#define I2Cdisplay_SCL 22

// Display anime
#define NUMFLAKES 10
#define LOGO_HEIGHT 16
#define LOGO_WIDTH 16
#define XPOS 0
#define YPOS 1
#define DELTAY 2

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

static const unsigned char PROGMEM logo_bmp[] = { B00000000, B11000000,
                                                  B00000001, B11000000,
                                                  B00000001, B11000000,
                                                  B00000011, B11100000,
                                                  B11110011, B11100000,
                                                  B11111110, B11111000,
                                                  B01111110, B11111111,
                                                  B00110011, B10011111,
                                                  B00011111, B11111100,
                                                  B00001101, B01110000,
                                                  B00011011, B10100000,
                                                  B00111111, B11100000,
                                                  B00111111, B11110000,
                                                  B01111100, B11110000,
                                                  B01110000, B01110000,
                                                  B00000000, B00110000 };
const uint8_t* bitmap = logo_bmp;
uint8_t w = LOGO_WIDTH;
uint8_t h = LOGO_HEIGHT;

unsigned char temperature_icon[] = {
  0b00000001, 0b11000000,  //        ###
  0b00000011, 0b11100000,  //       #####
  0b00000111, 0b00100000,  //      ###  #
  0b00000111, 0b11100000,  //      ######
  0b00000111, 0b00100000,  //      ###  #
  0b00000111, 0b11100000,  //      ######
  0b00000111, 0b00100000,  //      ###  #
  0b00000111, 0b11100000,  //      ######
  0b00000111, 0b00100000,  //      ###  #
  0b00001111, 0b11110000,  //     ########
  0b00011111, 0b11111000,  //    ##########
  0b00011111, 0b11111000,  //    ##########
  0b00011111, 0b11111000,  //    ##########
  0b00011111, 0b11111000,  //    ##########
  0b00001111, 0b11110000,  //     ########
  0b00000111, 0b11100000,  //      ######
};

unsigned char humidity_icon[] = {
  0b00000000, 0b00000000,  //
  0b00000001, 0b10000000,  //        ##
  0b00000011, 0b11000000,  //       ####
  0b00000111, 0b11100000,  //      ######
  0b00001111, 0b11110000,  //     ########
  0b00001111, 0b11110000,  //     ########
  0b00011111, 0b11111000,  //    ##########
  0b00011111, 0b11011000,  //    ####### ##
  0b00111111, 0b10011100,  //   #######  ###
  0b00111111, 0b10011100,  //   #######  ###
  0b00111111, 0b00011100,  //   ######   ###
  0b00011110, 0b00111000,  //    ####   ###
  0b00011111, 0b11111000,  //    ##########
  0b00001111, 0b11110000,  //     ########
  0b00000011, 0b11000000,  //       ####
  0b00000000, 0b00000000,  //
};

unsigned char arrow_down_icon[] = {
  0b00001111, 0b11110000,  //     ########
  0b00011111, 0b11111000,  //    ##########
  0b00011111, 0b11111000,  //    ##########
  0b00011100, 0b00111000,  //    ###    ###
  0b00011100, 0b00111000,  //    ###    ###
  0b00011100, 0b00111000,  //    ###    ###
  0b01111100, 0b00111110,  //  #####    #####
  0b11111100, 0b00111111,  // ######    ######
  0b11111100, 0b00111111,  // ######    ######
  0b01111000, 0b00011110,  //  ####      ####
  0b00111100, 0b00111100,  //   ####    ####
  0b00011110, 0b01111000,  //    ####  ####
  0b00001111, 0b11110000,  //     ########
  0b00000111, 0b11100000,  //      ######
  0b00000011, 0b11000000,  //       ####
  0b00000001, 0b10000000,  //        ##
};

unsigned char sun_icon[] = {
  0b00000000, 0b00000000,  //
  0b00100000, 0b10000010,  //   #     #     #
  0b00010000, 0b10000100,  //    #    #    #
  0b00001000, 0b00001000,  //     #       #
  0b00000001, 0b11000000,  //        ###
  0b00000111, 0b11110000,  //      #######
  0b00000111, 0b11110000,  //      #######
  0b00001111, 0b11111000,  //     #########
  0b01101111, 0b11111011,  //  ## ######### ##
  0b00001111, 0b11111000,  //     #########
  0b00000111, 0b11110000,  //      #######
  0b00000111, 0b11110000,  //      #######
  0b00010001, 0b11000100,  //    #   ###   #
  0b00100000, 0b00000010,  //   #           #
  0b01000000, 0b10000001,  //  #      #      #
  0b00000000, 0b10000000,  //         #
};

// Wifi status for blynk
void checkBlynkStatus() {  // called every 2 seconds by SimpleTimer
}

// Create display marker for each screen*
void displayIndicator(int displayNumber) {
  int xCoordinates[5] = { 44, 54, 64, 74, 84 };
  for (int i = 0; i < 5; i++) {
    if (i == displayNumber) {
      display.fillCircle(xCoordinates[i], 60, 2, WHITE);
    } else {
      display.drawCircle(xCoordinates[i], 60, 2, WHITE);
    }
  }
}

//SCREEN NUMBER 0: DATE AND TIME
void displayLocalTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");

  //GET DATE
  //Get full weekday name
  char weekDay[10];
  strftime(weekDay, sizeof(weekDay), "%a", &timeinfo);
  //Get day of month
  char dayMonth[4];
  strftime(dayMonth, sizeof(dayMonth), "%d", &timeinfo);
  //Get abbreviated month name
  char monthName[5];
  strftime(monthName, sizeof(monthName), "%b", &timeinfo);
  //Get year
  char year[6];
  strftime(year, sizeof(year), "%Y", &timeinfo);

  //GET TIME
  //Get hour (12 hour format)
  /*char hour[4];
  strftime(hour, sizeof(hour), "%I", &timeinfo);*/

  //Get hour (24 hour format)
  char hour[4];
  strftime(hour, sizeof(hour), "%H", &timeinfo);
  //Get minute
  char minute[4];
  strftime(minute, sizeof(minute), "%M", &timeinfo);

  //Display Date and Time on OLED display
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(3);
  display.setCursor(19, 5);
  display.print(hour);
  display.print(":");
  display.print(minute);
  display.setTextSize(1);
  display.setCursor(16, 40);
  display.print(weekDay);
  display.print(", ");
  display.print(dayMonth);
  display.print(" ");
  display.print(monthName);
  display.print(" ");
  display.print(year);
  displayIndicator(displayScreenNum);
  display.display();
  digitalWrite(yellow, LOW);
}


// SCREEN NUMBER 1: TEMPERATURE
void displayTemperature() {
  display.clearDisplay();
  display.setTextSize(2);
  display.drawBitmap(15, 5, temperature_icon, 16, 16, 1);
  display.setCursor(35, 5);
  float temperature = tempe;
  display.print(temperature);
  display.cp437(true);
  display.setTextSize(1);
  display.print(" ");
  display.write(167);
  display.print("C");
  display.setCursor(0, 34);
  display.setTextSize(1);
  display.print("Humidity: ");
  display.print(Humid);
  display.print(" %");
  display.setCursor(0, 44);
  display.setTextSize(1);
  display.print("soil: ");
  display.print(soil);
  display.print(" hpa");
  displayIndicator(displayScreenNum);
  display.display();
  digitalWrite(red, HIGH);
}

// SCREEN NUMBER 2: HUMIDITY
void displayHumidity() {
  display.clearDisplay();
  display.setTextSize(2);
  display.drawBitmap(15, 5, humidity_icon, 16, 16, 1);
  display.setCursor(35, 5);
  float humidity = Humid;
  display.print(humidity);
  display.print(" %");
  display.setCursor(0, 34);
  display.setTextSize(1);
  display.print("Temperature: ");
  display.print(tempe);
  display.cp437(true);
  display.print(" ");
  display.write(167);
  display.print("C");
  display.setCursor(0, 44);
  display.setTextSize(1);
  display.print("soil: ");
  display.print(soil);
  display.print(" hpa");
  displayIndicator(displayScreenNum);
  display.display();
  digitalWrite(red, LOW);
  digitalWrite(blue, HIGH);
}

// SCREEN NUMBER 3: PRESSURE
void displayPressure() {
  display.clearDisplay();
  display.setTextSize(2);
  display.drawBitmap(0, 5, arrow_down_icon, 16, 16, 1);
  display.setCursor(20, 5);
  display.print(soil);
  display.setTextSize(1);
  display.print(" hpa");
  display.setCursor(0, 34);
  display.setTextSize(1);
  display.print("Temperature: ");
  display.print(tempe);
  display.cp437(true);
  display.print(" ");
  display.write(167);
  display.print("M");
  display.setCursor(0, 44);
  display.setTextSize(1);
  display.print("Humidity: ");
  display.print(Humid);
  display.print(" %");
  displayIndicator(displayScreenNum);
  display.display();
  digitalWrite(blue, LOW);
  digitalWrite(green, HIGH);
}

// SCREEN NUMBER 4: LUMINOSITY
void displayLDR() {
  display.clearDisplay();
  display.setTextSize(2);
  display.drawBitmap(33, 5, sun_icon, 16, 16, 1);
  display.setCursor(53, 5);
  int ldrReading = light;
  display.print(ldrReading);
  display.print(" %");
  display.setTextSize(1);
  display.setCursor(0, 34);
  display.print("Temperature: ");
  display.print(tempe);
  display.print(" ");
  display.cp437(true);
  display.write(167);
  display.print("C");
  display.setCursor(0, 44);
  display.setTextSize(1);
  display.print("Humidity: ");
  display.print(Humid);
  display.print(" %");
  display.setCursor(0, 44);
  displayIndicator(displayScreenNum);
  display.display();
  digitalWrite(green, LOW);
  digitalWrite(yellow, HIGH);
}

void updateScreen() {
  if (displayScreenNum == 0) {
    displayLocalTime();
  } else if (displayScreenNum == 1) {
    displayTemperature();
  } else if (displayScreenNum == 2) {
    displayHumidity();
  } else if (displayScreenNum == 3) {
    displayPressure();
  } else {
    displayLDR();
  }
}

void setup() {
  // Debug console
  Serial.begin(115200);

  pinMode(wifiLed, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(soilpin, INPUT);

  dht.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }

  display.clearDisplay();
  display.setTextColor(WHITE);

  Blynk.begin(auth, ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected.");

  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

void loop() {
  Blynk.run();
  timer.run();

  Humid = dht.readHumidity();
  tempe = dht.readTemperature();

  //read the state of the touch
  if (touchRead(T0) > 30) {
    reading = HIGH;

  } else {
    reading = LOW;
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi Not Connected");
    digitalWrite(wifiLed, LOW);  //Turn OFF WiFi LED
    Blynk.begin(auth, ssid, pass);
  } else {
    digitalWrite(wifiLed, HIGH);  //Turn ON WiFi LED
  }

  float tem = analogRead(33);
  light = (tem / 3200) * 100;

  tem = analogRead(36);
  soil = tem - 2000;

  // Change screen when the pushbutton is pressed
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  Blynk.virtualWrite(V1, tempe);
  Blynk.virtualWrite(V2, Humid);
  Blynk.virtualWrite(V3, soil);
  Blynk.virtualWrite(V4, light);

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == HIGH) {
        updateScreen();
        Serial.println(displayScreenNum);
        if (displayScreenNum < displayScreenNumMax) {
          displayScreenNum++;
        } else {
          displayScreenNum = 0;
        }
        lastTimer = millis();
      }
    }
  }
  lastButtonState = reading;

  // Change screen every 15 seconds (timerDelay variable)
  if ((millis() - lastTimer) > timerDelay) {
    updateScreen();
    Serial.println(displayScreenNum);
    if (displayScreenNum < displayScreenNumMax) {
      displayScreenNum++;
    } else {
      displayScreenNum = 0;
    }
    lastTimer = millis();
  }
}
