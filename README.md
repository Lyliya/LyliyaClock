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

### 3D printed case

Print the two part body `3d/` and `3d/`  
![body](/assets/body.png)

Print the LED box and glue them together  
![box](/assets/ledbox.png)

Print 2x LED plate and glue them together  
![plate](/assets/plate.png)

## Construction

### Wiring

Fix your led in the LED plate making a zigzag pattern (see wiring under) using double sided tape

Connect your led in a XY matrix, make sure the white arrows follow the path in a alternative way, or your LED row wont work  
Replace the Arduino with your ESP8266 (idk how to do that in Tinkercad), and your 5V power supply with yours, wiring is the same
![Wiring](/assets/connection.png)

When your LED are secured on the plate, glue the led boxes on top and slide the plate in the notch on the body, and close everything together, You should have something like this
![Clock Without diffusion](/assets/IMG_2386.JPEG)  

I use paper for the LED diffusion, its way cheaper than plexiglass and still have that smooth display for the LED  
![Diffusion test](/assets/IMG_2359.JPEG)

Cut a piece of paper to the dimension of the clock, and glue it to the side with super glue

## Flashing the code
You can find the arduino project in `clock/clock.ino`

Open this file in the **Arduino IDE**, edit your **Open Weather Map API KEY**, your **Wifi informations** and edit the **city with the one you live in**

Then, upload the project on your ESP8266

## Usage
Plug the power in and voila !