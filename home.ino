#include<ESP8266WiFi.h>
#include<Adafruit_MQTT.h>
#include<Adafruit_MQTT_Client.h>

#define WIFI_SSID "<YOUR SSID>"
#define WIFI_PASS "<YOUR WIFI PASSWORD>"

#define MQTT_SERV "io.adafruit.com"
#define MQTT_PORT 1883
#define MQTT_NAME "<YOUR ADAFRUIT.IO USER NAME>"
#define MQTT_PASS "<YOUR AIO KEY>"

#define WiFi_LED D4
#define greenpin D0
#define yellowpin D1
#define redpin D2
#define fanpin D3

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, MQTT_SERV, MQTT_PORT, MQTT_NAME, MQTT_PASS);
Adafruit_MQTT_Subscribe geenled = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/feeds/geenled");
Adafruit_MQTT_Subscribe yellowled = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/feeds/yellowled");
Adafruit_MQTT_Subscribe redled = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/feeds/redled");
Adafruit_MQTT_Subscribe fan = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/feeds/fan");

void setup() {
    Serial.begin(115200);

    pinMode(WiFi_LED, OUTPUT);
    pinMode(greenpin, OUTPUT);
    pinMode(yellowpin, OUTPUT);
    pinMode(redpin, OUTPUT);
    pinMode(fanpin, OUTPUT);

    digitalWrite(greenpin, LOW);
    digitalWrite(yellowpin, LOW);
    digitalWrite(redpin, LOW);
    digitalWrite(fanpin, LOW);

    //Connect to WiFi
    Serial.printf("\n\nConnecting to %s%s", WIFI_SSID,"...");
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while(WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
    Serial.println("OK!");
    digitalWrite(WiFi_LED, LOW);

    //Subscribe to Led0 topic
    mqtt.subscribe(&geenled);
    mqtt.subscribe(&yellowled);
    mqtt.subscribe(&redled);
    mqtt.subscribe(&fan);
}

void loop() {
    //Connect/Reconnect to MQTT
    MQTT_connect();

    //Read from our subscribe queue until we ron out, or
    //wait up to 3 seconds for subcription to update
    Adafruit_MQTT_Subscribe * subscription;
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

    //ping the server to keep the mqtt connection alive
    if(!mqtt.ping()) {
        mqtt.disconnect();
    }
}

void MQTT_connect() {
    int8_t ret;
    // Stop if already connected
    if (mqtt.connected()) {
        return;
    }

    Serial.print("Connecting to MQTT... ");
    uint8_t retries = 3;
    while ((ret = mqtt.connect()) != 0) {// connect will return 0 for connected
        Serial.println(mqtt.connectErrorString(ret));
        Serial.println("Retrying MQTT connection in 3 seconds...");
        mqtt.disconnect();
        delay(3000);  // wait 5 seconds
        retries--;
        if (retries == 0) {
            // basically die and wait for WDT to reset me
            while (1);
        }
    }
    Serial.println("MQTT Connected!");
}
