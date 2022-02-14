#include "ESP8266WiFi.h"

const int BUTTON_PIN = 4;    // Define pin the button is connected to
const int ON_BOARD_LED = 2;  // Define pin the on-board LED is connected to
const int RGB_G_PIN = 12;    // RGB Green LED
const int RGB_R_PIN = 15;    // RGB Green LED
const int RGB_B_PIN = 13;    // RGB Green LED
const int LDR_PIN = A0;      // Define the analog pin the LDR is connected to

void setup() {
  pinMode(ON_BOARD_LED, OUTPUT);       // Initialize the LED_BUILTIN pin as an output
  pinMode(BUTTON_PIN, INPUT_PULLUP);  // Initialize button pin with built-in pullup.
  digitalWrite(ON_BOARD_LED, HIGH);    // Ensure LED is off
  Serial.begin(115200);               // Set comm rate to 115200

  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  Serial.println("Setup done");
}

void loop() {
  // put your main code here, to run repeatedly:
  int btn_Status = HIGH;
  int lightIntensity;

  lightIntensity = analogRead(LDR_PIN);  // Read the light intensity
  // analogWrite( RGB_G_PIN, map(lightIntensity, 40, 1023, 0, 1023));
  analogWrite( RGB_G_PIN, map(lightIntensity, 40, 1023,  0, 256));
  analogWrite( RGB_R_PIN, map(lightIntensity, 40, 1023,  0, 256));
  analogWrite( RGB_B_PIN, map(lightIntensity, 40, 1023,  0, 256));
  btn_Status = digitalRead (BUTTON_PIN);  // Check status of button
  if (btn_Status == LOW) {                // Button pushed, so do something
    Serial.print("Light Intensity Reading: ");
    Serial.println(lightIntensity);
    Serial.println("scan start");
    digitalWrite(ON_BOARD_LED, LOW);       // Turn LED ON
    // WiFi.scanNetworks will return the number of networks found
    int n = WiFi.scanNetworks();
    Serial.println("scan done");
    if (n == 0)
      Serial.println("no networks found");
    else
    {
      Serial.print(n);
      Serial.println(" networks found");
      for (int i = 0; i < n; ++i)
      {
        // Print SSID and RSSI for each network found
        Serial.print(i + 1);
        Serial.print(": ");
        Serial.print(WiFi.SSID(i));
        Serial.print(" (");
        Serial.print(WiFi.RSSI(i));
        Serial.print(")");
        Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " : Unsecure" : " : Encrypted");
        delay(10);
      }
    }
    Serial.println("");
    digitalWrite(ON_BOARD_LED, HIGH);    // Turn LED Off
  }
}