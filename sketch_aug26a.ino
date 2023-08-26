#include <DHT.h>
#include <WiFiNINA.h>
#include <ThingSpeak.h>

// Replace with your network credentials
char ssid[] = "ramansharma";
char password[] = "kuku1234";

// Replace with your ThingSpeak channel and API key
unsigned long channelID = 2251838;
char *apiKey = "0PFGI3ENQLBYU426";

// Pin connected to DHT22 data pin
#define DHTPIN 2

// Uncomment the type of DHT sensor you're using
#define DHTTYPE DHT22 // DHT 22 (AM2302)

DHT dht(DHTPIN, DHTTYPE);

WiFiClient client;

void setup() {
  Serial.begin(9600);

  // Connect to Wi-Fi
  while (WiFi.begin(ssid, password) != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
dht.begin();    
  // Initialize ThingSpeak
  ThingSpeak.begin(client);
}

void loop() {
  // Read humidity value from DHT22 sensor
  float humidity = dht.readHumidity();

  if (isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    delay(2000);
    return;
  }

  // Print humidity value to serial monitor
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println("%");

  // Update ThingSpeak channel with humidity value
  ThingSpeak.setField(1, humidity);

  // Send the data to ThingSpeak
  int response = ThingSpeak.writeFields(channelID, apiKey);

  if (response == 200) {
    Serial.println("Data sent to ThingSpeak successfully");
  } else {
    Serial.print("Error sending data to ThingSpeak. HTTP error code: ");
    Serial.println(response);
  }

  delay(30000); // Wait for 30 seconds before sending the next data
}