#include "WiFiS3.h"
#include <WiFiUdp.h>
#include <NTPClient.h>
#include "ArduinoGraphics.h"
#include "Arduino_LED_Matrix.h"

char ssid[] = "XXXXXXXXXXXXXXXXXXXXXX";     // Enter your WiFi network name
char pass[] = "XXXXXXXXXXXXXXXX"; // Enter your WiFi password

WiFiUDP ntpUDP;
// Time offset set to 19800 seconds (5 hours 30 minutes) for Indian Standard Time
NTPClient timeClient(ntpUDP, "pool.ntp.org", 19800, 60000); 

ArduinoLEDMatrix matrix;

void setup() {
  matrix.begin();
  
  // Connect to WiFi
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  
  timeClient.begin();
}

void loop() {
  timeClient.update();
  
  int hours = timeClient.getHours();
  int minutes = timeClient.getMinutes();
  
  // Convert 24-hour time to 12-hour format
  String ampm = (hours >= 12) ? " PM" : " AM";
  if (hours == 0) {
    hours = 12;
  } else if (hours > 12) {
    hours -= 12;
  }
  
  // Format the text string (e.g., "  4:05 PM  ")
  String timeString = "  " + String(hours) + ":";
  if (minutes < 10) {
    timeString += "0";
  }
  timeString += String(minutes) + ampm + "  ";
  
  // Scroll the time on the LED matrix
  matrix.beginDraw();
  matrix.stroke(0xFFFFFFFF);
  matrix.textScrollSpeed(80); 
  
  matrix.textFont(Font_5x7);
  matrix.beginText(0, 1, 0xFFFFFF);
  matrix.print(timeString);
  matrix.endText(SCROLL_LEFT);
  
  matrix.endDraw();
}