#include <WiFi.h>
#include <MQTT.h>

#include "DCMotor.h" 
// imports DCMotor class which can be found in the last version

const char ssid[] = "Premarital Homicide";
const char pass[] = "lilmimicoe";

const char mqtt_broker[]="test.mosquitto.org";
const char mqtt_topic[]="group11/command";
const char mqtt_status[]="group11/status";

int MQTT_PORT=1883;

WiFiClient net;
MQTTClient client;

void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nconnecting...");
  while (!client.connect("1tfMax0g3PahDg0vd5")) {  // connection ID should be unique
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");

  client.subscribe(mqtt_topic);
  client.subscribe(mqtt_status);
  // client.unsubscribe("/hello");
}

bool BUTTON;

void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
  BUTTON = (payload == "1");

  // Note: Do not use the client in the callback to publish, subscribe or
  // unsubscribe as it may cause deadlocks when other things arrive while
  // sending and receiving acknowledgments. Instead, change a global variable,
  // or push to a queue and handle it in the loop after calling `client.loop()`.
}

const uint8_t PIN[2] = {26,27}; // motor driver ports
DCMotor motor = DCMotor(PIN, true, false);

uint8_t mode;
int random_delay;

void setup() {
  WiFi.begin(ssid, pass);

  // Note: Local domain names (e.g. "Computer.local" on OSX) are not supported
  // by Arduino. You need to set the IP address directly.
  client.begin(mqtt_broker, MQTT_PORT, net);
  client.onMessage(messageReceived);

  pinMode(PIN[0], OUTPUT);
  pinMode(PIN[1], OUTPUT);
  Serial.begin(9600);
  connect();

  mode = 0; // set initial mode to 0
  client.publish(mqtt_status, "Mode 0: Waiting for user command...");
  BUTTON = false;
}

void loop() {
  client.loop();
  delay(10);  // <- fixes some issues with WiFi stability

  if (!client.connected()) {
    connect();
  }

  switch(mode) {
    case 0x0: // waiting for button input
      if(!BUTTON)
        break;

      mode++;
      client.publish(mqtt_status, "Mode 1: Randomizing...");
      BUTTON = false;
      break;

    case 0x1: // randomize (spinning the roulette)
      motor.setClockwise(true);
      motor.setRotate(true);

      random_delay = random(5000, 10000); // random a 'roulette spin time' within 5 to 10 seconds
      delay(random_delay);
      
      motor.setRotate(false);
      mode++;
      BUTTON = false;
      client.publish(mqtt_status, "Mode 2: Randomization completed, Waiting for user to lift off the button...");
      break;
    
    case 0x2: // user must lift off the button first to continue to the next mode
      if(BUTTON) {
        client.publish(mqtt_topic, "reset");
        delay(1000); // <- these 2 lines are here to handle spamming
        break;
      }

      mode++;
      client.publish(mqtt_status, "Mode 3: Push the button to continue...");
      break;

    case 0x3: // wait user to push the button to restart
      if(!BUTTON)
        break;

      // Program can reach this part if user pushes.
      mode = 0;
      BUTTON = false;
      client.publish(mqtt_status, "Mode 0: Waiting for user input...");

      motor.setClockwise(false);
      motor.setRotate(true);
      break;
  }
}
