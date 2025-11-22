#include <OneWire.h>
#include <DallasTemperature.h>
#include <SoftwareSerial.h>

// ------------------ DS18B20 setup ------------------
#define ONE_WIRE_BUS 2  // Data pin for DS18B20
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// ------------------ pH sensor setup ------------------
#define PH_PIN A0  // Analog pin for pH sensor

#define PH4_VOLTAGE 2.5
#define PH7_VOLTAGE 2.1
#define PH_STEP (4.0 - 7.0) / (PH4_VOLTAGE - PH7_VOLTAGE)

// ------------------ ESP8266 setup ------------------
#define ESP_RX 7  // Arduino receives from ESP TX
#define ESP_TX 8  // Arduino transmits to ESP RX
SoftwareSerial espSerial(ESP_RX, ESP_TX);

const char* ssid = "AirFiber-Aafee7";
const char* password = "xequ2eexaiS1aeR0";

// ------------------ Setup ------------------
void setup() {
  Serial.begin(9600);
  espSerial.begin(115200);  // ESP8266 baud rate (default may vary)
  delay(1000);

  Serial.println("Initializing sensors...");
  sensors.begin();
  pinMode(PH_PIN, INPUT);
  Serial.println("DS18B20 and pH sensor ready.");

  // ------------------ Connect to Wi-Fi ------------------
  Serial.println("Connecting to Wi-Fi via ESP8266...");
  espSerial.println("AT+RST");  // Reset ESP
  delay(2000);

  sendCommand("AT", 1000);  // Check communication
  sendCommand("AT+CWMODE=1", 1000);  // Set ESP to station mode
  String cmd = "AT+CWJAP=\"" + String(ssid) + "\",\"" + String(password) + "\"";
  sendCommand(cmd.c_str(), 5000);  // Connect to Wi-Fi
  Serial.println("Wi-Fi connection attempt done.");
}

// ------------------ Main loop ------------------
void loop() {
  // --- Temperature reading ---
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);
  if (tempC == DEVICE_DISCONNECTED_C) {
    Serial.println("Error: DS18B20 not found!");
  } else {
    Serial.print("Temperature: ");
    Serial.print(tempC);
    Serial.println(" °C");
  }

  // --- pH reading ---
  int phValue = analogRead(PH_PIN);
  float voltage = phValue * (5.0 / 1023.0);
  float ph = 7.0 + ((PH7_VOLTAGE - voltage) / PH_STEP);

  Serial.print("pH Raw Value: ");
  Serial.print(phValue);
  Serial.print(" | Voltage: ");
  Serial.print(voltage);
  Serial.print("V | pH: ");
  Serial.println(ph);

  delay(2000);
}

// ------------------ Helper function to send commands to ESP ------------------
String sendCommand(const char* cmd, const int timeout) {
  String response = "";
  espSerial.println(cmd);
  long int time = millis();
  while ((time + timeout) > millis()) {
    while (espSerial.available()) {
      char c = espSerial.read();
      response += c;
    }
  }
  Serial.print("Command: "); Serial.println(cmd);
  Serial.print("Response: "); Serial.println(response);
  return response;
}

if (temperature < 28 || temperature > 32) {
    sendTelegramAlert("Warning: Temperature out of range! Current value: " + String(temperature));
}

if (pH < 6.5 || pH > 8.5) {
    sendTelegramAlert("Warning: pH level out of range! Current value: " + String(pH));
}


// ----------- Alert logic for temperature and pH -----------
if (temperature < 28 || temperature > 32) {
    activateAlert(1000); // Alert for abnormal temperature
}

if (pH < 6.5 || pH > 8.5) {
    activateAlert(1200); // Alert for abnormal pH
}

















