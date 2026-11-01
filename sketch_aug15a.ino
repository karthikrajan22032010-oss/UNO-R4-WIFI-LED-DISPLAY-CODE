#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Onboard LED on ESP32-C3 Dual USB board is connected to GPIO 8
#define ONBOARD_LED 8

// OLED Display setup (SDA = GPIO 4, SCL = GPIO 5)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  // 1. Initialize Onboard LED (Active LOW logic: LOW = ON, HIGH = OFF)
  pinMode(ONBOARD_LED, OUTPUT);
  digitalWrite(ONBOARD_LED, LOW); // Turn LED ON

  // 2. Initialize I2C Bus for ESP32-C3 (SDA = GPIO 4, SCL = GPIO 5)
  Wire.begin(4, 5);
  Wire.setClock(400000); // Fast 400kHz mode

  // 3. Initialize OLED Display (Address 0x3C)
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    for (;;); // Stop execution if screen is missing
  }
}

// --- PHASE 1: SHOW FULL TEXT (WITHOUT 2026) ---
void showFullText() {
  digitalWrite(ONBOARD_LED, LOW); // Turn LED ON
  
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  // Line 1: HAPPY 80th
  display.setTextSize(1);
  display.setCursor(32, 10);
  display.println("HAPPY 80th");

  // Line 2: INDEPENDENCE
  display.setCursor(28, 26);
  display.println("INDEPENDENCE");

  // Line 3: DAY
  display.setTextSize(2);
  display.setCursor(46, 42);
  display.println("DAY");

  display.display();
  delay(3000); // Display greeting for 3 seconds
}

// --- PHASE 2: FLAG RISING ANIMATION (BOTTOM TO TOP) ---
void playFlagRisingVideo() {
  int poleX = 15;
  int poleTopY = 4;
  int poleBottomY = 60;
  int flagWidth = 40;
  int flagHeight = 21;

  // Animate flag moving from bottom (Y=38) up to top (Y=4)
  for (int flagY = 38; flagY >= 4; flagY -= 2) {
    
    // Pulse the LED on/off during the rising animation
    digitalWrite(ONBOARD_LED, (flagY % 4 == 0) ? LOW : HIGH);

    display.clearDisplay();

    // 1. Draw Flag Pole & Top Knob
    display.fillRect(poleX, poleTopY, 3, poleBottomY - poleTopY, SSD1306_WHITE);
    display.fillCircle(poleX + 1, poleTopY, 2, SSD1306_WHITE);

    // 2. Draw Flag Frame at current height
    int flagX = poleX + 3;
    display.drawRect(flagX, flagY, flagWidth, flagHeight, SSD1306_WHITE);

    // 3. Top Band (Saffron/Orange)
    display.fillRect(flagX, flagY, flagWidth, 7, SSD1306_WHITE);

    // 4. Middle Band (White) with Ashoka Chakra Circle
    display.drawCircle(flagX + 20, flagY + 10, 2, SSD1306_WHITE);

    // 5. Bottom Band (Green)
    display.fillRect(flagX, flagY + 14, flagWidth, 7, SSD1306_WHITE);

    // 6. Labels on side
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(68, 20);
    display.println("INDIA");
    
    display.setCursor(68, 36);
    display.println("80th");

    display.display();
    delay(100); // Animation speed
  }

  // Turn LED fully back ON when flag reaches the top
  digitalWrite(ONBOARD_LED, LOW);
  
  // Hold top hoisted flag position for 2 seconds
  delay(2000);
}

void loop() {
  // Step 1: Display greeting text
  showFullText();

  // Step 2: Play rising flag video animation loop
  playFlagRisingVideo();

  // Loop automatically repeats!
}