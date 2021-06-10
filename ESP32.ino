#include <WiFi.h>
#include <WebServer.h>

#include "DHT.h"

#define DHTPIN 27    // Digital pin connected to the DHT sensor

#define DHTTYPE DHT11   // DHT 11

DHT dht(DHTPIN, DHTTYPE);

const char *ssid = "Freebox_Infopix";
const char *password = "1f0pix_64";
WebServer server(80);

const int led = 14;
bool etatLed = 0;
char texteEtatLed[2][10] = {"ÉTEINTE!","ALLUMÉE!"};

void handleRoot()
{
    String page = "<!DOCTYPE html>";

    page += "<html>";
    page += "<head>";
    page += "<link rel='preconnect' href='https://fonts.gstatic.com'>";
    page += "<link href='https://fonts.googleapis.com/css2?family=Roboto:wght@300&display=swap' rel='stylesheet'>";
    page += "</head>";
    page += "<title>ESP32</title>";
    page += "<meta charset='UTF-8' name='viewport' content='width=device-width, initial-scale=1'>";
    page += "<body style='max-width:400px'>";
    page += "<header class='w3-container'>";
    page += "<h1 style='font-family: 'Roboto', sans-serif;'>Infopix</h1>";
    page += "</header>";
    page += "<div class='w3-container'>";
    page += "<hr>";
    page += "<div class='w3-row'>";
    page += "<div class='w3-col s9 w3-container'>";
    page += "<a href='/on'>ON</a> / <a href='/off'>OFF</a>";
    page += "<p>État : "; page +=texteEtatLed[etatLed]; page += "</p>";
    page += "</div>";
    page += "</div>";
    page += "<hr>";
    page += "<div class='w3-col s9 w3-container'>";
    page += "<div class='w3-row'>";
    page += "<h3>Température : "; page += dht.readTemperature(); + "</h3>";
    page += "<h3>Humidité : "; page += dht.readHumidity(); + "</h3>";
    page += "</div>";
    page += "</div>";
    page += "<hr>";
    page += "<div class='w3-row'>";
    page += "<div class='w3-col s9 w3-container'>";
    page += "<h3>Chauffage : </h3>";
    page += "<input type='range' list='tickmarks' value='0' oninput='this.nextElementSibling.value = this.value'>";
    page += "<output>0</output>%";
    page += "<datalist id='tickmarks'>";
    page += "<option value='0'>";
    page += "<option value='10'>";
    page += "<option value='20'>";
    page += "<option value='30'>";
    page += "<option value='40'>";
    page += "<option value='50'>";
    page += "<option value='60'>";
    page += "<option value='70'>";
    page += "<option value='80'>";
    page += "<option value='90'>";
    page += "<option value='100'>";
    page += "</datalist>";
    page += "</div>";
    page += "</div>";
    page += "<hr>";
    page += "</div>";
    page += "<footer class='w3-container'>";
    page += "</footer>";
    page += "</body>";
    page += "</html>";

    server.setContentLength(page.length());
    server.send(200, "text/html", page);
}

void handleOn()
{
    etatLed = 1;
    digitalWrite(led, HIGH);
    server.sendHeader("Location","/");
    server.send(303);
}

void handleOff()
{
    etatLed = 0;
    digitalWrite(led, LOW);
    server.sendHeader("Location","/");
    server.send(303);
}

void handleNotFound()
{
    server.send(404, "text/plain", "404: Not found");
}

void setup()
{
    Serial.begin(115200);
    delay(1000);
    Serial.println("\n");
    dht.begin();
    pinMode(led, OUTPUT);
    digitalWrite(led, LOW);

    WiFi.persistent(false);
    WiFi.begin(ssid, password);
    Serial.print("Tentative de connexion...");

    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(100);
    }

    Serial.println("\n");
    Serial.println("Connexion etablie!");
    Serial.print("Adresse IP: ");
    Serial.println(WiFi.localIP());

    server.on("/", handleRoot);
    server.on("/on", handleOn);
    server.on("/off", handleOff);
    server.onNotFound(handleNotFound);
    server.begin();

    Serial.println("Serveur web actif!");
}

void loop()
{
    server.handleClient();
}
