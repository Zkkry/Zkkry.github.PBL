
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <DHT.h>

// =====================================
// 1. Configuration
// =====================================
const char* ssid = "Camelia";
const char* password = "Camelia123";

// Google Apps Script URL
const char* scriptURL = "https://script.google.com/macros/s/AKfycbwPevaFi9A5aMIroJ_Xyu9ACxCaJCZfJvc-6jYkiUPSsI3V9tcJhA-pTk3jccwdnDhr/exec";

// DHT11 Sensor
#define DHTPIN 16
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

// Send data every 10 minutes
const unsigned long sendInterval = 600000UL;

// Retry after 1 minute if unsuccessful
const unsigned long retryInterval = 60000UL;

unsigned long previousMillis = 0;
unsigned long waitInterval = 0;

// Sensor validation range
const float TEMP_MIN = 0.0;
const float TEMP_MAX = 50.0;
const float HUM_MIN = 20.0;
const float HUM_MAX = 90.0;


// =====================================
// 2. WiFi Connection
// =====================================
bool connectWiFi() {

  // Already connected
  if (WiFi.status() == WL_CONNECTED) {
    return true;
  }

  // Stop previous connection attempts
  WiFi.disconnect(true);
  delay(1000);

  WiFi.mode(WIFI_STA);
  WiFi.setSleep(false);
  WiFi.setAutoReconnect(false);

  Serial.println();
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  unsigned long startTime = millis();

  // Allow 30 seconds to connect
  while (WiFi.status() != WL_CONNECTED &&
         millis() - startTime < 30000UL) {

    delay(500);
    Serial.print(".");
  }

  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {

    Serial.println("WiFi Connected!");

    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    Serial.print("Signal Strength: ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");

    return true;

  } else {

    Serial.println("WiFi connection failed!");

    Serial.print("WiFi Status Code: ");
    Serial.println(WiFi.status());

    // Cancel failed connection
    WiFi.disconnect(true);

    return false;
  }
}


// =====================================
// 3. Sensor Validation
// =====================================
bool validateSensorReading(
  float temperature,
  float humidity
) {

  if (isnan(temperature) || isnan(humidity))
    return false;

  if (temperature < TEMP_MIN ||
      temperature > TEMP_MAX)
    return false;

  if (humidity < HUM_MIN ||
      humidity > HUM_MAX)
    return false;

  return true;
}


// =====================================
// 4. Read DHT11 Sensor
// =====================================
bool readSensor(
  float &temperature,
  float &humidity
) {

  for (int attempt = 1; attempt <= 3; attempt++) {

    humidity = dht.readHumidity();
    temperature = dht.readTemperature();

    if (validateSensorReading(
          temperature, humidity)) {

      return true;
    }

    Serial.print("Sensor reading failed. Attempt ");
    Serial.println(attempt);

    delay(2000);
  }

  return false;
}


// =====================================
// 5. Send Data to Google Sheets
// =====================================
bool sendDataToGoogleSheets(
  float temperature,
  float humidity
) {

  if (WiFi.status() != WL_CONNECTED) {

    Serial.println("WiFi disconnected!");
    return false;
  }

  WiFiClientSecure client;

  // For laboratory testing only
  client.setInsecure();

  HTTPClient http;

  http.setTimeout(15000);
  http.setFollowRedirects(
    HTTPC_STRICT_FOLLOW_REDIRECTS
  );

  String url = String(scriptURL) +
    "?temperature=" + String(temperature, 2) +
    "&humidity=" + String(humidity, 2);

  if (!http.begin(client, url)) {

    Serial.println("Failed to open URL!");
    return false;
  }

  Serial.println("Sending data to Google Sheets...");

  int httpResponseCode = http.GET();

  bool success = false;

  if (httpResponseCode > 0) {

    Serial.print("HTTP Response Code: ");
    Serial.println(httpResponseCode);

    String response = http.getString();

    Serial.println(response);

    success = (
      httpResponseCode == 200 &&
      response.indexOf("success") >= 0
    );

  } else {

    Serial.print("HTTP Error: ");
    Serial.println(
      http.errorToString(httpResponseCode)
    );
  }

  http.end();

  return success;
}


// =====================================
// 6. Setup
// =====================================
void setup() {

  Serial.begin(115200);

  delay(1000);

  Serial.println();
  Serial.println("========================");
  Serial.println("ESP32 DHT11 Data Logger");
  Serial.println("========================");

  dht.begin();

  delay(2000);

  // First reading immediately
  previousMillis = millis();
  waitInterval = 0;
}


// =====================================
// 7. Main Loop
// =====================================
void loop() {

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis <
      waitInterval) {

    delay(10);
    return;
  }

  // Check WiFi before reading sensor
  if (!connectWiFi()) {

    Serial.println("Retrying WiFi in 1 minute.");

    previousMillis = millis();
    waitInterval = retryInterval;

    return;
  }

  float temperature;
  float humidity;

  // Read DHT11 sensor
  if (!readSensor(temperature, humidity)) {

    Serial.println("Sensor reading failed.");
    Serial.println("Retrying in 1 minute.");

    previousMillis = millis();
    waitInterval = retryInterval;

    return;
  }

  // Display sensor readings
  Serial.println("------------------------");

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" C");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  // Send data to Google Sheets
  bool sent = sendDataToGoogleSheets(
    temperature,
    humidity
  );

  previousMillis = millis();

  if (sent) {

    Serial.println("Data uploaded successfully!");
    Serial.println("Next reading in 10 minutes.");

    waitInterval = sendInterval;

  } else {

    Serial.println("Data upload failed!");
    Serial.println("Retrying in 1 minute.");

    waitInterval = retryInterval;
  }

  Serial.println("------------------------");
}
