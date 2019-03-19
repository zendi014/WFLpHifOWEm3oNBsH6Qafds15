#include <Arduino.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <SocketIoClient.h>
#include <ArduinoJson.h>

////////////////////////////////// GLOBAL DEFINE //////////////////////////////////////
WiFiClient client;
SocketIoClient webSocket;

const char* ssid     = "i7559";
const char* password = "123456789";
char path[] = "/";
char host[] = "192.168.137.1";//WLAN IP (SERVER)
int port = 2208;




////////////////////////////////// LOOP AND SETUP //////////////////////////////////////

void setup() {

    #if defined(ESP8266)
      Serial.begin(115200);
    #else
      Serial.begin(38400);
    #endif

    Serial.setDebugOutput(true);


    for(uint8_t t = 4; t > 0; t--) {
        Serial.printf("[SETUP] BOOT WAIT %d...\n", t);
        Serial.flush();
        delay(500);
    }

    wifi_connection();

    webSocket.on("connect", connection);
    webSocket.begin(host, port);
}





void loop() {
    webSocket.loop();
    webSocket.on("message", message);
    webSocket.on("web_app_soi", message);

    webSocket.on("srv_emit_to_wemos", message);
}




/////////////////////// BINDING CONNECTION ///////////////////////////////////////////

void wifi_connection(){
    Serial.print("\n");
    Serial.printf("WiFi Connecting to :: ");
    Serial.printf(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.printf(".");
    }

    Serial.print("\n");
    Serial.printf("WiFi connected");
    Serial.printf("IP address: ");
    Serial.print(WiFi.localIP());

    app_connection();
}





void app_connection(){
    Serial.print("\n");
    Serial.printf("App Connecting to :: ");
    Serial.printf(host);

    if (client.connect(host, port)) {
        Serial.printf("\nApp Server Connected\n");
    } else {
        Serial.printf("\nApp Server Connection failed\n");
    }

    Serial.print("");
}




void message(const char * payload, size_t length) {
//    Serial.printf("connection message: %s\n", payload);
    parse_data(payload);//parse data
}




void parse_data(String msg){
    StaticJsonBuffer<2048> JSONBuffer;//Memory pool
    JsonObject& parsed = JSONBuffer.parseObject(msg);
    if (parsed.success()) {
         StaticJsonBuffer<200> jsonBuffer;
         JsonObject& dt = jsonBuffer.createObject();
         dt["your_id"]  = parsed["your_id"];
         dt["to_id"]  = parsed["to_id"];
         dt["msg_text"] = parsed["msg_text"];

         char jsondt[8000];
         dt.printTo(jsondt);
         webSocket.emit("wemos_emit_to_srv", jsondt);//here replying
    }
}


void connection(const char * payload, size_t length) {
    //Serial.printf("connection message: %s\n", payload);
    webSocket.emit("wemos_connected", "{\"is_connect\":\"1\"}");
}




void broadcast_test(){
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& dt = jsonBuffer.createObject();
    dt["status"]  = "replying from WEMOS";
    dt["code"]  = "200";

    char jsondt[100];
    dt.printTo(jsondt);

            //dt.prettyPrintTo(Serial);
            //Serial.println(jsondt);
    webSocket.emit("res_web_app_soi", jsondt);//here replying
}
