#include <ESP8266WiFi.h>

const char *ssid = HOME_WIFI_SSID;
const char *password = HOME_WIFI_PASS;
const char *host = SERVER_HOST_ADDR_URL;

const int soilPower = 14;
const int sleepTimeSeconds = 60 * 60; //1 hour in seconds

// the setup routine runs once when you press reset:
void setup()
{
    Serial.begin(115200);
    Serial.setTimeout(2000);
    while (!Serial)
    {
    }

    // We start by connecting to a WiFi network
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    // make the pushbutton's pin an input:
    pinMode(soilPower, OUTPUT);
    digitalWrite(soilPower, LOW);
    float moistureValue = readSoil();
    sendMoistureLevel(moistureValue);
    ESP.deepSleep(sleepTimeSeconds * 1e6);
}

float readSoil()
{

    // read the input pin:
    digitalWrite(soilPower, HIGH);
    delay(100); // delay in between reads for stability
    int sensorValue = analogRead(A0);
    float voltage = sensorValue * (1.0 / 1023.0);
    digitalWrite(soilPower, LOW);
    Serial.print("Soil Moisture Level = ");
    Serial.println(voltage);
    return voltage;
}

void sendMoistureLevel(float moistureLevel)
{
    Serial.print("connecting to ");
    Serial.println(host);

    // Use WiFiClient class to create TCP connections
    WiFiClient client;
    const int httpPort = 80;
    if (!client.connect(host, httpPort))
    {
        Serial.println("connection failed");
        return;
    }

    // We now create a URI for the request
    String url = "/Soil-Moisture-Alert";
    Serial.print("Requesting URL: ");
    Serial.println(url);

    Serial.print("Sending Moisture Level = ");
    Serial.println(moistureLevel);

    // This will send the request to the server
    client.print(String("GET ") + url + "?soilMoistureAmount=" + String(moistureLevel) + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
    delay(500);

    // Read all the lines of the reply from server and print them to Serial
    while (client.available())
    {
        String line = client.readStringUntil('\r');
        Serial.print(line);
    }

    Serial.println();
    Serial.println("closing connection");
}

void loop()
{
}
