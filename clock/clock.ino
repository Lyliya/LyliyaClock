#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
#define ARDUINOJSON_USE_LONG_LONG 1 // we need to store long long
#include <FastLED.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <Arduino_JSON.h>

#define NUM_LEDS 95
#define DATA_PIN 5
#define BRIGHTNESS 64

#define OWMKEY "YOUR OWM KEY"

// MODE
#define MODE_NB 5
enum MODE
{
    FULL = 0,
    HOUR,
    TEMP,
    HUMIDITY,
    WIND
};

// Weather structure
typedef struct weather_s
{
    int humidity;
    int temperature;
    double wind_speed;
    long sunset;
    long sunrise;
    String type;
} weather_t;

String city = "YOUR CITY";

// Leds
CRGB leds[NUM_LEDS];
MODE mode = HOUR;

// WiFi
const char *ssid = "YOUR WIFI";
const char *password = "YOUR WIFI PASSWORD";

// Web Server
ESP8266WebServer server(80);

// NTP
WiFiUDP ntpUDP;
// Offset for UTC+1
int offset = 0;
NTPClient timeClient(ntpUDP, "fr.pool.ntp.org", offset, 60000);

// Meteo data
weather_t weather;

// Shape for numbers
int zero[] = {0, 1, 2, 37, 35, 38, 40, 75, 73, 76, 77, 78, -1};
int one[] = {2, 35, 40, 73, 78, -1};
int two[] = {0, 1, 2, 35, 40, 38, 39, 75, 76, 77, 78, -1};
int three[] = {0, 1, 2, 35, 40, 39, 38, 73, 78, 77, 76, -1};
int four[] = {0, 2, 37, 35, 38, 39, 40, 73, 78, -1};
int five[] = {0, 1, 2, 37, 38, 39, 40, 73, 78, 77, 76, -1};
int six[] = {0, 1, 2, 37, 38, 39, 40, 75, 73, 76, 77, 78, -1};
int seven[] = {0, 1, 2, 35, 40, 73, 78, -1};
int eight[] = {0, 1, 2, 37, 35, 38, 39, 40, 75, 73, 76, 77, 78, -1};
int nine[] = {0, 1, 2, 37, 35, 38, 39, 40, 73, 78, 77, 76, -1};
int *digits[] = {zero, one, two, three, four, five, six, seven, eight, nine};

// Custom colored shaped
typedef struct pixel_s
{
    int pos;
    CRGB color;
} pixel_t;

pixel_t degree[] = {
    {0, CRGB::White},
    {1, CRGB::White},
    {37, CRGB::White},
    {36, CRGB::White},
    {-1, CRGB::White}};

pixel_t minus[] = {
    {38, CRGB::White},
    {39, CRGB::White},
    {-1, CRGB::White}};

pixel_t snow[] = {
    {37, CRGB::White},
    {75, CRGB::White},
    {39, CRGB::White},
    {35, CRGB::White},
    {73, CRGB::White},
    {-1, CRGB::White}};

pixel_t thunder[] = {
    {38, CRGB::Grey},
    {75, CRGB::Grey},
    {39, CRGB::Grey},
    {74, CRGB::Grey},
    {36, CRGB::Grey},
    {35, CRGB::Grey},
    {40, CRGB::Grey},
    {73, CRGB::Yellow},
    {41, CRGB::Grey},
    {72, CRGB::Grey},
    {77, CRGB::Yellow},
    {-1, CRGB::Grey}};

pixel_t cloud[] = {
    {38, CRGB::Grey},
    {75, CRGB::Grey},
    {39, CRGB::Grey},
    {74, CRGB::Grey},
    {36, CRGB::Grey},
    {35, CRGB::Grey},
    {40, CRGB::Grey},
    {73, CRGB::Grey},
    {41, CRGB::Grey},
    {72, CRGB::Grey},
    {-1, CRGB::Grey}};

// "39,40,35,34,41,42,71,72,73,74,79,77"
pixel_t rain[] = {
    {39, CRGB::Grey},
    {40, CRGB::Grey},
    {35, CRGB::Grey},
    {34, CRGB::Grey},
    {41, CRGB::Grey},
    {42, CRGB::Grey},
    {71, CRGB::Blue},
    {72, CRGB::Grey},
    {73, CRGB::Blue},
    {74, CRGB::Grey},
    {79, CRGB::Blue},
    {77, CRGB::Blue},
    {-1, CRGB::Grey}};

/* pixel_t sun[] = {
  {37, CRGB::Yellow},
  {36, CRGB::Yellow},
  {35, CRGB::Yellow},
  {40, CRGB::Yellow},
  {39, CRGB::Yellow},
  {38, CRGB::Yellow},
  {75, CRGB::Yellow},
  {74, CRGB::Yellow},
  {73, CRGB::Yellow},
  {-1, CRGB::Yellow}
}; */

pixel_t sun[]{
    {0, CRGB::Yellow},
    {37, CRGB::Yellow},
    {38, CRGB::Yellow},
    {75, CRGB::Yellow},
    {76, CRGB::Yellow},
    {77, CRGB::Yellow},
    {78, CRGB::Yellow},
    {79, CRGB::Yellow},
    {72, CRGB::Yellow},
    {41, CRGB::Yellow},
    {34, CRGB::Yellow},
    {3, CRGB::Yellow},
    {2, CRGB::Yellow},
    {1, CRGB::Yellow},
    {36, CRGB::Goldenrod},
    {74, CRGB::Goldenrod},
    {39, CRGB::Goldenrod},
    {35, CRGB::Goldenrod},
    {40, CRGB::Goldenrod},
    {73, CRGB::Goldenrod},
    {-1, CRGB::Yellow}};

pixel_t moon[]{
    {37, CRGB::Grey},
    {38, CRGB::Grey},
    {75, CRGB::Grey},
    {77, CRGB::Grey},
    {78, CRGB::Grey},
    {79, CRGB::Grey},
    {3, CRGB::Grey},
    {2, CRGB::Grey},
    {1, CRGB::Grey},
    {36, CRGB::Grey},
    {74, CRGB::Grey},
    {39, CRGB::Grey},
    {35, CRGB::Grey},
    {73, CRGB::Grey},
    {-1, CRGB::Grey}};

pixel_t drop[] = {
    {75, CRGB::Blue},
    {38, CRGB::Blue},
    {77, CRGB::Blue},
    {78, CRGB::Blue},
    {79, CRGB::Blue},
    {72, CRGB::Blue},
    {71, CRGB::Blue},
    {42, CRGB::Blue},
    {34, CRGB::Blue},
    {35, CRGB::Blue},
    {36, CRGB::Blue},
    {2, CRGB::Blue},
    {39, CRGB::Blue},
    {40, CRGB::Blue},
    {41, CRGB::Blue},
    {73, CRGB::Blue},
    {74, CRGB::Blue},
    {-1, CRGB::Blue},
};

pixel_t percent[] = {
    {76, CRGB::White},
    {74, CRGB::White},
    {40, CRGB::White},
    {34, CRGB::White},
    {37, CRGB::White},
    {79, CRGB::White},
    {-1, CRGB::White},
};

pixel_t wind[] = {
    {37, CRGB::White},
    {34, CRGB::White},
    {3, CRGB::White},
    {2, CRGB::White},
    {35, CRGB::White},
    {36, CRGB::White},
    {74, CRGB::White},
    {75, CRGB::White},
    {73, CRGB::White},
    {72, CRGB::White},
    {79, CRGB::White},
    {-1, CRGB::White}};

int getBrightness(int hour)
{
    if (hour > 8 && hour < 21)
    {
        return 64;
    }
    else
    {
        return 1;
    }
}

void drawCustomShape(pixel_t *shape, int offset)
{
    int y = 0;
    int dir = 1;

    for (int i = 0; shape[i].pos != -1; i++)
    {
        dir = ((shape[i].pos / 19) % 2 != 0) ? -1 : 1;
        leds[shape[i].pos + (offset * dir)] = shape[i].color;
    }
}

void addMidPoint(CRGB color)
{
    leds[1 * 19 + 9] = color;
    leds[3 * 19 + 9] = color;
}

void addnumber(int *n, int offset)
{
    int y = 0;
    int dir = 1;

    const CRGB color = CRGB::White; // CRGB(random(0, 255), random(0, 255), random(0, 255));
    for (int i = 0; n[i] != -1; i++)
    {
        dir = ((n[i] / 19) % 2 != 0) ? -1 : 1;
        leds[n[i] + (offset * dir)] = color;
    }
}

void displayHours(int hours, int minutes, int seconds)
{
    int h1 = (hours / 10) % 10;
    int h2 = hours % 10;
    int m1 = (minutes / 10) % 10;
    int m2 = minutes % 10;

    addnumber(digits[h1], 1);
    addnumber(digits[h2], 5);
    addMidPoint((seconds % 2 == 0) ? CRGB::White : CRGB::Black);
    addnumber(digits[m1], 11);
    addnumber(digits[m2], 15);
}

void fullMode()
{
    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds[i] = CRGB::White;
    }
}

void setup()
{
    delay(1000);
    timeClient.begin();

    FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
    FastLED.setBrightness(getBrightness(timeClient.getHours()));

    leds[2 * 19 + 8] = CRGB::White;
    leds[2 * 19 + 9] = CRGB::White;
    leds[2 * 19 + 10] = CRGB::White;
    FastLED.show();

    Serial.begin(9600);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.print("Connecting..\n");
    }

    // You are now connected

    // WebServer
    server.on("/mode", HTTP_POST, changeMode);
    server.on("/city", HTTP_POST, changeCity);

    // Get Weather first data
    updateWeather();

    Serial.print("Ip address: ");
    Serial.println(WiFi.localIP());
    server.begin();
}

int saveMinute = -1;

void loop()
{
    // Handle server client
    server.handleClient();

    // Clear all led
    FastLED.clear();
    // Update time
    timeClient.update();

    int currentMinute = timeClient.getMinutes();
    if (currentMinute != saveMinute)
    {
        if (currentMinute % 2 == 0)
        {
            mode = (MODE)(mode + 1);
            if (mode >= MODE_NB)
            {
                mode = (MODE)(1);
            }
        }
        else if (currentMinute % 5 == 0)
        {
            updateWeather();
        }
        saveMinute = currentMinute;
    }

    FastLED.setBrightness(getBrightness(timeClient.getHours()));

    // Exec mode function to draw on led
    switch (mode)
    {
    case FULL:
        fullMode();
        break;
    case HOUR:
        displayHours(timeClient.getHours(), currentMinute, timeClient.getSeconds());
        break;
    case TEMP:
        displayTemp();
        break;
    case HUMIDITY:
        displayHumidity();
        break;
    case WIND:
        displayWindSpeed();
        break;
    default:
        displayHours(timeClient.getHours(), timeClient.getMinutes(), timeClient.getSeconds());
    }

    // Show led
    FastLED.show();

    // Wait
    delay(1000);
}

void updateWeather()
{
    // Get weather info on OpenWeatherMap

    Serial.println("Get weather data");

    String serverPath = "http://api.openweathermap.org/data/2.5/weather?units=metric&q=" + city + "&APPID=" + OWMKEY;
    WiFiClient client;
    HTTPClient http;

    http.begin(client, serverPath.c_str());
    // Send HTTP GET request
    int httpResponseCode = http.GET();
    Serial.println(httpResponseCode);
    String jsonBuffer = http.getString();

    JSONVar json = JSON.parse(jsonBuffer);

    if (JSON.typeof(json) == "undefined")
    {
        Serial.println("Parsing input failed!");
        return;
    }
    weather.temperature = json["main"]["temp"];
    weather.humidity = json["main"]["humidity"];
    weather.wind_speed = json["wind"]["speed"];
    weather.type = json["weather"][0]["main"];
    weather.sunset = json["sys"]["sunset"];
    weather.sunrise = json["sys"]["sunrise"];

    if ((int)json["timezone"] != offset)
    {
        offset = json["timezone"];
        timeClient.setTimeOffset(offset);
    }
}

void displayTemp()
{
    int offset = 9;
    int temperature = weather.temperature;

    if (weather.temperature < 0)
    {
        temperature *= -1;
        drawCustomShape(minus, 6);
    }

    int t1 = (temperature / 10) % 10;
    int t2 = temperature % 10;

    if (t1 != 0)
    {
        addnumber(digits[t1], offset);
        offset += 4;
    }
    addnumber(digits[t2], offset);

    drawCustomShape(degree, offset + 4);

    if (weather.type == "Clouds")
    {
        drawCustomShape(cloud, 1);
    }
    else if (weather.type == "Thunderstorm")
    {
        drawCustomShape(thunder, 1);
    }
    else if (weather.type == "Drizzle")
    {
        drawCustomShape(cloud, 1);
    }
    else if (weather.type == "Rain")
    {
        drawCustomShape(rain, 1);
    }
    else if (weather.type == "Snow")
    {
        drawCustomShape(snow, 1);
    }
    else if (weather.type == "Clear")
    {
        unsigned long now = timeClient.getEpochTime() + offset;
        if (now < weather.sunrise || now > weather.sunset)
        {
            drawCustomShape(moon, 1);
        }
        else
        {
            drawCustomShape(sun, 1);
        }
    }
}

void displayHumidity()
{
    // Show humidity
    // Get humidity and display with water drop
    int humidity = weather.humidity;
    if (humidity >= 100)
        humidity = 99;
    int h1 = (weather.humidity / 10) % 10;
    int h2 = weather.humidity % 10;

    drawCustomShape(drop, 1);
    if (h1 != 0)
        addnumber(digits[h1], 7);
    addnumber(digits[h2], 11);

    drawCustomShape(percent, 15);
}

void displayWindSpeed()
{
    // Show wind speed
    // Get wind speed, display wind image and wind speed
    int wind_speed = weather.wind_speed * 3.6;
    drawCustomShape(wind, 1);

    int offset = 7;
    if (wind_speed >= 100)
    {
        addnumber(digits[wind_speed / 100], offset);
    }
    offset += 4;
    if (wind_speed >= 10)
    {
        addnumber(digits[wind_speed / 10], offset);
        offset += 4;
    }
    addnumber(digits[wind_speed % 10], offset);
}

void changeMode()
{
    if (server.arg("mode") == "full")
    {
        mode = FULL;
    }
    else if (server.arg("mode") == "clock")
    {
        mode = HOUR;
    }
    else if (server.arg("mode") == "temp")
    {
        mode = TEMP;
    }
    else if (server.arg("mode") == "humidity")
    {
        mode = HUMIDITY;
    }
    else if (server.arg("mode") == "wind")
    {
        mode = WIND;
    }
    else
    {
        mode = HOUR;
    }

    server.send(200, "text/plain", "OK");
}

void changeCity()
{
    if (server.arg("city"))
    {
        city = server.arg("city");
    }

    server.send(200, "text/plain", "OK");

    updateWeather();
}
