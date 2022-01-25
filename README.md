# Lyliya Clock

Build your own LED clock easily with an esp866 and ledstrips

![End product](/assets/IMG_5729.jpeg)

## Features
- Weather information, display current weather and temperature (metric system), custom image for different weather (sun, cloud, rain, ...)
- Humidity, display current humidity
- Clock, well, it's a clock after all
- Wind Speed, display the wind speed
- Handle the timezone and hour change automaticly based on your location
- Reduce brightness during night
- Rotate between mode every 2 minutes
- REST API to change data without recompiling the project (mode/city)
- And more to come...

## Part needed
### Electronics
- ESP8266 - 2€
- 5V ledstrips (I use 95 LEDs with 60 LED/m to create those big pixels) - 12€
- Wire and soldering iron - Depend on what you take, but ~20€
- Jack alimentation connector - 1€
- 5V alimentation with enough Amp to handle your leds - 5€

Total cost : 40€ (20€ if you already have basic tools)

### 3D printed cases
TBD

## Construction
TBD

![Diffusion demo](/assets/IMG_2386.JPEG)  

I use paper for the LED diffusion, its way cheaper than plexiglass and still have that smooth display for the LED  
![Clock Without diffusion](/assets/IMG_2359.JPEG)

## Flashing the code
You can find the arduino project in `clock/clock.ino`

Open this file in the **Arduino IDE**, edit your **Open Weather Map API KEY**, your **Wifi informations** and edit the **city with the one you live in**

Then, upload the project on your ESP8266

## Usage
Plug the power in and voila !