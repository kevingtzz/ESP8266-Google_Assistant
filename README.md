# ESP8266-Google_Assistant
ESP8266 module connected with google assistant or google home by MQTT protocol on Adafruit broker.
## Let's get started 🚀
First you need to have your local machine preparate to work with the ESP8266 module, in this case we will use the ESP8266 ESP-12E NodeMCU board, for that I recommend to you check this web site [How to programming ESP8266 ESP-12E NodeMCU](https://www.instructables.com/id/Programming-ESP8266-ESP-12E-NodeMCU-Using-Arduino-/).
## Prerequisites 📋
* Install the Adafruit MQTT Client library, you can find it in the Arduino IDE library manager.
* Create a free acount in the [Adafruit IO broker](https://io.adafruit.com/).
* Create a free ("Maker") account in this web site [IFTTT](https://platform.ifttt.com/), it is advisable to do it with the google account.
### Installation 🔧
* Change this parameters in the code...
````
#define WIFI_SSID "<YOUR SSID>"
#define WIFI_PASS "<YOUR WiFi PASSWORD>"
````
`````
#define MQTT_SERV "io.adafruit.com"
#define MQTT_PORT 1883
#define MQTT_NAME "<YOUR ADAFRUIT.IO USER NAME>"
#define MQTT_PASS "<YOUR AIO KEY>"
``````
You can find the **AIO Key** in your Dashboards in the Adafruit.io website. 
### Running the tests ⚙️
* Define the pins you want to use, in this case we will define four pins for **MQTT Subscriptions** and one that will light up when there is wifi connection, like this.
````
#define WiFi_LED D4
#define greenpin D0
#define yellowpin D1
#define redpin D2
#define fanpin D3
```````
* So then let's create your **Feeds** in your **Adafruit account** like this.
![ScreenShot](https://github.com/kevingtzz/ESP8266-Google_Assistant/blob/master/Documentation%20IMG/Screen%20Shot%202018-11-21%20at%2017.20.16.png)
* Create a **Dashboar** and add your **Feeds**:
![ScreenShot](https://github.com/kevingtzz/ESP8266-Google_Assistant/blob/master/Documentation%20IMG/Screen%20Shot%202018-11-21%20at%2017.34.30.png)
* You must define your **Feed Subscriptions**.
`````
Adafruit_MQTT_Subscribe geenled = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/feeds/geenled");
Adafruit_MQTT_Subscribe yellowled = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/feeds/yellowled");
Adafruit_MQTT_Subscribe redled = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/feeds/redled");
Adafruit_MQTT_Subscribe fan = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/feeds/fan");
``````
* Instance them in the **setup function** in your code:
`````
mqtt.subscribe(&geenled);
mqtt.subscribe(&yellowled);
mqtt.subscribe(&redled);
mqtt.subscribe(&fan);
`````
* How you can see, the name of my **Feeds** are **geenled**,**yellowled**,**redled** and **fan** you must change these feeds for yours in the mentioned parts of code and in the logic code below:
````
    while((subscription = mqtt.readSubscription(3000))) {
        //If we're in here, a subscription update...
        if(subscription == &geenled) {
            //Print the new value to the serial monitor
            Serial.print("Green LED: ");
            Serial.println((char*)geenled.lastread);

            //If the new value is "ON", turn the light on.
            //Otherwise, turn it off.
            if(!strcmp((char*)geenled.lastread, "ON")) {
                digitalWrite(greenpin, HIGH);
            }else {
                digitalWrite(greenpin, LOW);
            }
        }
        if(subscription == &yellowled) {
            //Print the new value to the serial monitor
            Serial.print("Yellow LED: ");
            Serial.println((char*)yellowled.lastread);

            //If the new value is "ON", turn the light on.
            //Otherwise, turn it off.
            if(!strcmp((char*)yellowled.lastread, "ON")) {
                digitalWrite(yellowpin, HIGH);
            }else {
                digitalWrite(yellowpin, LOW);
            }
        }
        if(subscription == &redled) {
            //Print the new value to the serial monitor
            Serial.print("Red LED: ");
            Serial.println((char*)redled.lastread);

            //If the new value is "ON", turn the light on.
            //Otherwise, turn it off.
            if(!strcmp((char*)redled.lastread, "ON")) {
                digitalWrite(redpin, HIGH);
            }else {
                digitalWrite(redpin, LOW);
            }
        }
        if(subscription == &fan) {
            //Print the new value to the serial monitor
            Serial.print("Fan: ");
            Serial.println((char*)fan.lastread);

            //If the new value is "ON", turn the light on.
            //Otherwise, turn it off.
            if(!strcmp((char*)fan.lastread, "ON")) {
                digitalWrite(fanpin, HIGH);
            }else {
                digitalWrite(fanpin, LOW);
            }
        }
    }
 `````
At this moment, when you upload the code to your board you should be able to control your pins from the adafruit page.
#### Almost there...
Now we just need to download the **IFTTT** mobile app in your device and configure the **apletts**.
* The trigger is by **Google Assistant**
* the action is by **Adafruit**, configure your **Adafruit account** with **IFTTT** in the action and then you will see your feeds.
Now you should be able to controle your pins with **Google Assistant**
### All done.
