#include <Wire.h>
#include <ESP8266WiFi.h>
#include <Adafruit_GFX.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET  -1   //Reset pin for oled
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

DynamicJsonDocument doc(16384);

const char *ssid = "NombreDeTuRedWIFI"; //Cambiar por el nombre de tu Wifi
const char *password = "ContraseñaWifi"; //Cambiar por la contraseña del Wifi

String API_Key = "TuAPIKey"; // Cambiar por tu Api Key
int httpCode;
String payload;

void setup()
{
  Wire.begin(0, 2);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { //turning on the oled
    for(;;);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 6);
  display.print("Noticias del dia");
  display.setCursor(0, 32);
  display.print("Creado por");
  display.setCursor(0, 38);
  display.print("Electronica De Una");   // To make it intresting, you an change the name, and make your own. 
  display.display();
  delay(2000);

  display.clearDisplay();
  WiFi.begin(ssid, password);

  display.setCursor(5, 16);
  display.print("Conectando");
   while (WiFi.status() != WL_CONNECTED)
   {
    display.print("*");
    display.display();
    delay(500);
   }
   display.setCursor(5,40);
   display.print("WiFi Conectado");
   display.display();  //to clear the old screen
   delay(1000);
}

void loop ()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    WiFiClient client;
    HTTPClient http;

    http.begin(client,"http://newsapi.org/v2/top-headlines?country=ar&pageSize=5&apiKey=" + API_Key);
    httpCode = http.GET();

    if (httpCode > 0)
    {
      payload = http.getString();
      
      payload.replace("á","a");
      payload.replace("é","e");
      payload.replace("í","i");
      payload.replace("ó","o");
      payload.replace("ú","u");
      payload.replace("ñ","ni");
      payload.replace("“","'");
      payload.replace("”","'");
      
      deserializeJson(doc, payload);
      
      for(int i=0; i<5; i++) {
        String titulo = doc["articles"][i]["title"];
        String fuente = doc["articles"][i]["source"]["name"];
        
        display.clearDisplay();
        display.display();

        display.setCursor(2, 0);
        display.print(titulo);
        display.setCursor(2, 56);
        display.print("Fuente: " + fuente);  
        display.display();
        delay(7000);
      }
    }
    http.end();
  }
  delay(60000);
}
