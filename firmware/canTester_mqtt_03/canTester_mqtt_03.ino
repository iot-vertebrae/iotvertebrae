/*

* * * Works with ESP32-S3 * * *

Good for an initial test in the car to check that the board and the car's OBD connector work as they should

- Blue LED blinks when CAN replies are received from the car
- Yellow LED is on when the engine rpm is over 2000 rpm
- Engine rpms are continously printed over the serial on USB-port

*/

#include "driver/twai.h"

#include <WiFi.h>
#include "IoT-02_wifiMng.h"
#include "IoT-02_mqttCredentials.h"
#include "IoT-02_mqttTopics.h"
#include <PubSubClient.h>
#include <ESPmDNS.h>
#include <ArduinoJson.h>
#define N_SJD 500

#define MINIMUM_DELAY_ALLOWING_MULTITASKING 50

WiFiClient espClient;
PubSubClient client(espClient);
#define MAC_SIZE 15
char sMac[MAC_SIZE];

TaskHandle_t TaskMqtt;

StaticJsonDocument<N_SJD> doc,docPub; // https://arduinojson.org/

// #define BLUE_LED      10
// #define YELLOW_LED    11
#define BLUE_LED      2
#define YELLOW_LED    2
#define BT_IO0 0

#define RX_PIN        26
#define TX_PIN        27
//#define CAN_RS        38

#define CANPID_RPM    0x0C
#define CAN_REQST_ID  0x7DF 
#define CAN_REPLY_ID  0x7E8

void mqttconnect() {
  const TickType_t xDelay = 5000 / portTICK_PERIOD_MS; // 5 seconds

  /* Loop until reconnected */
  while (!client.connected()) {
    Serial.print("MQTT connecting ...");
    /* client ID */
    String clientId = "IoT-02_" + String(sMac); // <-------   Unique name in every device
    /* connect now */
    if (client.connect(clientId.c_str(), mqtt_user, mqtt_password)) {
      //if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      /* subscribe topic */
      client.subscribe(TOPIC_REQUEST_MAC); // <-------   Subscription to MQTT(S) topic
      /*client.subscribe(String("/" + String(sMac) + TOPIC_LED_W).c_str());
      client.subscribe(String("/" + String(sMac) + TOPIC_LED_R).c_str());
      client.subscribe(String("/" + String(sMac) + TOPIC_LED_Y).c_str());
      client.subscribe(String("/" + String(sMac) + TOPIC_LED_G).c_str());
      client.subscribe(String("/" + String(sMac) + TOPIC_LATCHING_RELAY).c_str());
      client.subscribe(String("/" + String(sMac) + TOPIC_REQUEST_LATCHING_RELAY_STATE).c_str());
      client.subscribe(String("/" + String(sMac) + TOPIC_REQUEST_LDR).c_str());
      client.subscribe(String("/" + String(sMac) + TOPIC_REQUEST_T).c_str());
      client.subscribe(String("/" + String(sMac) + TOPIC_REQUEST_RH).c_str());
      client.subscribe(String("/" + String(sMac) + TOPIC_REQUEST_P).c_str());
      client.subscribe(String("/" + String(sMac) + TOPIC_REQUEST_ALT).c_str());
      client.subscribe(String("/" + String(sMac) + TOPIC_SMALL_TEXT).c_str());
      client.subscribe(String("/" + String(sMac) + TOPIC_MEDIUM_TEXT).c_str());
      client.subscribe(String("/" + String(sMac) + TOPIC_BIG_TEXT).c_str());
      client.subscribe(String("/" + String(sMac) + TOPIC_JSON_INPUT_REQ).c_str());
      client.subscribe(String("/" + String(sMac) + TOPIC_JSON_LEDS).c_str());
      client.subscribe(String("/" + String(sMac) + TOPIC_JSON_BME_REQ).c_str());
      client.subscribe(String("/" + String(sMac) + TOPIC_SYNC_LDR).c_str());
      client.subscribe(String("/" + String(sMac) + TOPIC_SYNC_JSONIN).c_str());
      Serial.println("Subscriptions to: ");
      Serial.println(String("/" + String(sMac) + TOPIC_SYNC_LDR).c_str());
      Serial.println(String("/" + String(sMac) + TOPIC_SYNC_JSONIN).c_str());*/
      client.subscribe(String("/" + String(sMac) + TOPIC_CAN_JSON_MSG_IN).c_str());
    } else {
      Serial.print("failed, status code =");
      Serial.print(client.state());
      Serial.println("try again in 5 seconds");
      /* Wait 5 seconds before retrying */
      //delay(5000);
      vTaskDelay(xDelay);
    }
  }
}

/*
      Serial.println((word)doc["ID"],HEX);
      Serial.printf("{ ID: 0x%X\n",(word)doc["ID"]);
      int n = (byte)doc["n"];
      for(int i = 0; i < n; i++){
        Serial.printf("[%d]: 0x%X, ",i,(byte)doc["msg"][i]);
      }
      Serial.println("}");
*/      


int nCANsend () {
  static unsigned int n = 250;
  static byte byOutA=0xFF,byOutB=0x00,byMask=0x01;

  n++;

  byOutA = ~byMask;
  byOutB = byMask;
  byMask = (byMask==0x80)?0x01:byMask<<1;

  twai_message_t message;
  message.identifier = (word)doc["ID"];
  message.extd = (byte)doc["extd"];
  message.rtr = (byte)doc["rtr"];
  message.data_length_code = (byte)doc["n"];
  for(int i = 0; i < message.data_length_code; i++){
    message.data[i] = (byte)doc["msg"][i];
  }  
  /* message.data[0] = 2;
  message.data[1] = 1;
  message.data[2] = CANPID_RPM;
  message.data[3] = byOutA;
  message.data[4] = byOutB;
  message.data[5] = 0;
  message.data[6] = (byte)(n & 0xFF);
  message.data[7] = (byte)(n >> 8); 
  //return twai_transmit(&message, 0);*/

  if (twai_transmit(&message, pdMS_TO_TICKS(1000)) == ESP_OK) {
    Serial.println("Message queued for transmission");
    
    if (message.extd) {
        Serial.println("Message is in Extended Format\n");
    } else {
        Serial.println("Message is in Standard Format\n");
    }
    Serial.printf("ID is 0x%X\n", message.identifier);
    if (!(message.rtr)) {
        for (int i = 0; i < message.data_length_code; i++) {
            Serial.printf("Data byte %d = 0x%X\n", i, message.data[i]);
        }
    }
    
    return 1;
  } else {
    Serial.println("Failed to queue message for transmission");
    return 0;
  }
}

void receivedCallback(char* topic, byte* payload, unsigned int length) {
  String szTopic = String(topic), szPayload = "";
  char topicBuf[250];
  
  Serial.print("Topic: ");
  Serial.println(topic);

  Serial.print("payload: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    szPayload += (char)payload[i];
  }
  Serial.println();

  if (szTopic == String("/" + String(sMac) + TOPIC_CAN_JSON_MSG_IN).c_str()) {
    DeserializationError error = deserializeJson(doc, payload);
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      return;
    }else{
      Serial.println((word)doc["ID"],HEX);
      Serial.printf("{ ID: 0x%X\n",(word)doc["ID"]);
      int n = (byte)doc["n"];
      if((byte)doc["rtr"] == 0){ // Dades
        for(int i = 0; i < n; i++){
          Serial.printf("[%d]: 0x%X, ",i,(byte)doc["msg"][i]);
        }        
      }else{
        Serial.println("Asking for an answer");
      }
      Serial.println("}");
      nCANsend();
    }
  }
}

void vSetupMqtt() {
  /* set SSL/TLS certificate */
  // espClient.setCACert(ca_cert);
  /* configure the MQTT server with IPaddress and port */
  client.setServer(mqtt_server, mqtt_port);
  /* this receivedCallback function will be invoked
    when client received subscribed topic */
  client.setCallback(receivedCallback);
}

void vReconnectWifiMqtt() {
  vSetupWifi();
  szGetMac().toCharArray(sMac, MAC_SIZE);
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  vSetupMqtt();
  mqttconnect();
}

void vTaskMqtt( void * pvParameters ){
  byte byI2cAddr;
  
  Serial.print("vTaskMqtt running on core ");
  Serial.println(xPortGetCoreID());
  for(;;){
    if (!client.connected()) {
      vReconnectWifiMqtt();
    }
    client.loop();
    vTaskDelay(MINIMUM_DELAY_ALLOWING_MULTITASKING);
  }
  vTaskDelete(NULL); // There is an infinite loop before. This line will never be reached.
}

boolean bPressedButton(int nWhichOne) {
  if (digitalRead(nWhichOne))
    return false;
  return true;
}

int CANsend (void) {
  static unsigned int n = 250;
  static byte byOutA=0xFF,byOutB=0x00,byMask=0x01;

  n++;

  byOutA = ~byMask;
  byOutB = byMask;
  byMask = (byMask==0x80)?0x01:byMask<<1;
  
  twai_message_t message;
  message.identifier = CAN_REQST_ID;
  message.extd = 0;
  message.rtr = 0;
  message.data_length_code = 8;
  message.data[0] = 2;
  message.data[1] = 1;
  message.data[2] = CANPID_RPM;
  message.data[3] = byOutA;
  message.data[4] = byOutB;
  message.data[5] = 0;
  message.data[6] = (byte)(n & 0xFF);
  message.data[7] = (byte)(n >> 8); 
  //return twai_transmit(&message, 0);

  if (twai_transmit(&message, pdMS_TO_TICKS(1000)) == ESP_OK) {
    Serial.println("Message queued for transmission");
    
    if (message.extd) {
        Serial.println("Message is in Extended Format\n");
    } else {
        Serial.println("Message is in Standard Format\n");
    }
    Serial.printf("ID is 0x%X\n", message.identifier);
    if (!(message.rtr)) {
        for (int i = 0; i < message.data_length_code; i++) {
            Serial.printf("Data byte %d = 0x%X\n", i, message.data[i]);
        }
    }
    
    return 1;
  } else {
    Serial.println("Failed to queue message for transmission");
    return 0;
  }
}

void CANrecv (void) {
  char output[N_SJD];
  
  twai_message_t message;
  
  if (twai_receive(&message, 0) != ESP_OK) {
    return;
  }
  Serial.println("CANrecv{");
  digitalWrite(BLUE_LED, HIGH);
  delay(20);
  digitalWrite(BLUE_LED, LOW);

  //Process received message
  if (message.extd) {
      Serial.println("Message is in Extended Format\n");
  } else {
      Serial.println("Message is in Standard Format\n");
  }
  Serial.printf("message.identifier & 0x00FF = 0x%X\n",message.identifier & 0x00FF);
  docPub["ID"] = (word)message.identifier;
  docPub["extd"] = (byte)message.extd;
  docPub["rtr"] = (byte)message.rtr;
  docPub["n"] = (byte)message.data_length_code;
  
  if (!(message.rtr)) {
    for (int i = 0; i < message.data_length_code; i++) {
      Serial.printf("Data byte %d = 0x%X\n", i, (byte)message.data[i]);
      docPub["msg"][i] = (byte)message.data[i];
    }
  }
  Serial.println("}CANrecv");
  serializeJson(docPub, output);
  client.publish( String("/" + String(sMac) + TOPIC_CAN_JSON_MSG_OUT).c_str(), output);
  Serial.println("Mqtt publishes:");
  Serial.print("Topic: ");
  Serial.println(String("/" + String(sMac) + TOPIC_CAN_JSON_MSG_OUT).c_str());
  Serial.print("Payload: ");
  Serial.println(output);
}

void setup() {
  Serial.begin(115200);
  Serial.println(__FILE__);
  
  // Serial.setTxTimeoutMs(0); // prevent slow serial prints if no usb
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(BT_IO0, INPUT);
  // pinMode(CAN_RS, OUTPUT);    // INPUT (high impedance) = slope control mode, OUTPUT = see next line
  // digitalWrite(CAN_RS, LOW);  // LOW = high speed mode, HIGH = low power mode (listen only)
  twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT((gpio_num_t)TX_PIN, (gpio_num_t)RX_PIN, TWAI_MODE_NORMAL);  // TWAI_MODE_NORMAL, TWAI_MODE_NO_ACK or TWAI_MODE_LISTEN_ONLY
  twai_timing_config_t t_config  = TWAI_TIMING_CONFIG_500KBITS();
  twai_filter_config_t f_config  = TWAI_FILTER_CONFIG_ACCEPT_ALL();
  twai_driver_install(&g_config, &t_config, &f_config);
  twai_start();

  xTaskCreatePinnedToCore(
                    vTaskMqtt,   /* Task function. */
                    "vTaskMqtt",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &TaskMqtt,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */   
}

void loop() {
  static boolean bIO0wasPressed = false;
  boolean bIO0currentState = bPressedButton(BT_IO0); 
  static unsigned int n = 1;
   
  if (bIO0wasPressed != bIO0currentState) {
    delay(2);
    if (bIO0currentState) {
      Serial.print(n++);
      Serial.println(") Button 0 pressed (BOOT)");
      CANsend();
    }
    bIO0wasPressed = bIO0currentState;
  } 
  CANrecv();

  vTaskDelay(MINIMUM_DELAY_ALLOWING_MULTITASKING);
}
