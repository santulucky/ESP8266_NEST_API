/*
 *  HTTP over TLS (HTTPS) example sketch
 *
 *  This example demonstrates how to use
 *  WiFiClientSecure class to access HTTPS API.
 *  We fetch and display the status of
 *  esp8266/Arduino project continuous integration
 *  build.
 *
 *  Limitations:
 *    only RSA certificates
 *    no support of Perfect Forward Secrecy (PFS)
 *    TLSv1.2 is supported since version 2.4.0-rc1
 *
 *  Created by Ivan Grokhotkov, 2015.
 *  This example is in public domain.
 */

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

const char* ssid = "WIFI_SSID";
const char* password = "WIFI_PASSWORD";

//const char* host = "api.home.nest.com";//for getting access token
const char* host = "developer-api.nest.com"; //for controlling
const int httpsPort = 443;

const char* access_token = "ACCESS_TOKEN";

const char* thermostat_id="/devices/thermostats/THERMOSTAT_ID";

void setup() {
	Serial.begin(115200);
	Serial.println();
	Serial.print("connecting to ");
	Serial.println(ssid);
	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid, password);
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}
	Serial.println("");
	Serial.println("WiFi connected");
	Serial.println("IP address: ");
	Serial.println(WiFi.localIP());

	// Use WiFiClientSecure class to create TLS connection
	WiFiClientSecure client;
	Serial.print("connecting to ");
	Serial.println(host);
	if (!client.connect(host, httpsPort)) {
		Serial.println("connection failed");
		return;
	}

	String line;

	//code for getting access token

	//String url = "/oauth2/access_token";
	//Serial.print("requesting URL: ");
	//Serial.println(url);
	//
	//String PostData = "code=PINCODE&client_id=CLIENT_ID&client_secret=CLIENT_SECRET&grant_type=authorization_code";
	//
	//client.println(String("POST ") + url + " HTTP/1.1");
	//client.println(String("Host: ") + host);
	//client.println("Cache-Control: no-cache");
	//client.println("Content-Type: application/x-www-form-urlencoded");
	//client.print("Content-Length: ");
	//client.println(PostData.length());
	//client.println();
	//client.println(PostData);




	//code to read list of net devices
	//  client.print(String("GET ") + "/" + " HTTP/1.1\r\n" +
	//               "Host: " + host + "\r\n" +
	//               "Content-Type: application/json\r\n" +
	//               "Authorization: Bearer " + access_token + "\r\n" +
	//               "Connection: close\r\n\r\n");
	//
	//  Serial.println("request sent");
	//  
	//  line = client.readString();
	//
	//  loop to follow the redirection
	//  while(line.startsWith("HTTP/1.1 307")){
	//    int startp = line.indexOf("//")+2;
	//    int endp = line.indexOf(":", startp);
	//    String rhost=line.substring(startp,endp);
	//    int pend = line.indexOf("/",endp);
	//    int rport = (line.substring(endp+1,pend)).toInt();
	//    Serial.println(rhost);
	//    Serial.println(rport);
	//    if (!client.connect(rhost, rport)) {
	//      Serial.println("connection failed");
	//      return;
	//    }
	//    client.print(String("GET ") + "/" + " HTTP/1.1\r\n" +
	//             "Host: " + rhost + "\r\n" +
	//             "Content-Type: application/json\r\n" +
	//             "Authorization: Bearer " + access_token + "\r\n" +
	//             "Connection: close\r\n\r\n");
	//    line = client.readString();
	//  }
	//
	//  Serial.println(line);


	//code to change thermostat mode
	String PutData = "{\"hvac_mode\":\"heat-cool\"}";

	client.println(String("PUT ") + thermostat_id + " HTTP/1.1");
	client.println(String("Host: ") + host);
	client.println("Cache-Control: no-cache");
	client.println("Content-Type: application/json");
	client.println(String("Authorization: Bearer ")+ access_token);
	client.print("Content-Length: ");
	client.println(PutData.length());
	client.println();
	client.println(PutData);

	Serial.println("request sent");
	Serial.println(PutData.length());
	Serial.println("==============");

	line = client.readString();

	//loop to follow the redirection
	while(line.startsWith("HTTP/1.1 307")){
		int startp = line.indexOf("//")+2;
		int endp = line.indexOf(":", startp);
		String rhost=line.substring(startp,endp);
		int pend = line.indexOf("/",endp);
		int rport = (line.substring(endp+1,pend)).toInt();
		Serial.println(rhost);
		Serial.println(rport);
		if (!client.connect(rhost, rport)) {
			Serial.println("connection failed");
			return;
		}
		client.println(String("PUT ") + thermostat_id + " HTTP/1.1");
		client.println(String("Host: ") + rhost);
		client.println("Cache-Control: no-cache");
		client.println("Content-Type: application/json");
		client.println(String("Authorization: Bearer ")+ access_token);
		client.print("Content-Length: ");
		client.println(PutData.length());
		client.println();
		client.println(PutData);

		line = client.readString();
	}

	Serial.println("reply was:");
	Serial.println("==========");
	Serial.println(line);
	Serial.println("==========");
	Serial.println("closing connection");
}

void loop() {
}
