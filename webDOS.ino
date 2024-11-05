// includes
#include <AZ3166WiFi.h>
#include <OledDisplay.h>
#include <RGB_LED.h>

char ssid[] = ""; // wifi ssid
char password[] = ""; // wifi password
int keyIndex = 0; // key index
int status = WL_IDLE_STATUS; // set status variable to wireless idle status
int port = 80; // set server port
WiFiServer server(port); // init wifi server
int btnAState; // create a variable for button A state
int btnBState; // create a variable for button B stat

void setup() {
    Screen.init(); // init screen
    Serial.begin(115200); //Initialize serial and wait for port to open:
    pinMode(LED_USER, OUTPUT); // init user led
    pinMode(LED_AZURE, OUTPUT); // init azure led
    pinMode(USER_BUTTON_A, INPUT); // set pinmode of button A to input
    pinMode(USER_BUTTON_B, INPUT); // set pinmode of button B to input
    btnAState = digitalRead(USER_BUTTON_A); // get button A state and set it to variable
    btnBState = digitalRead(USER_BUTTON_B); // get button B state and set it to variable
    Screen.print("LOADING..."); // print loading to the screen
    // check for the presence of the shield:
    if (WiFi.status() == WL_NO_SHIELD) {
        Serial.println("WiFi shield not present");
        Screen.clean(); // clear oled screen
        Screen.print("WiFi Shield not present", true);
        // don't continue:
        while (true);
    }
    const char* fv = WiFi.firmwareVersion(); // set variable to wifi firmware version
    Serial.printf("Wi-Fi firmware: %s\r\n", fv); // print wifi firmware version
    // attempt to connect to Wifi network:
    while (status != WL_CONNECTED) {
        Serial.print("Attempting to connect to SSID: "); // print status
        Serial.println(ssid); // print ssid
        Screen.clean(); // clear screen
        Screen.print("Attemting to connect to SSID:", true); 
        Screen.print(2,ssid,true);
        // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
        status = WiFi.begin(ssid, password);
        // wait 10 seconds for connection:
        delay(10000);
    }
    server.begin();
    // you're connected now, so print out the status:
    printWifiStatus(); 
}
void loop() {
    Serial.println("listening for incoming clients");
    // listen for incoming clients
    WiFiClient client = server.available();
    Serial.println("available");
    if (client)
    {
        Serial.println("new client");
        // an http request ends with a blank line
        boolean currentLineIsBlank = true;
        while (client.connected())
        {
            digitalWrite(LED_USER, HIGH);
            if (client.available())
            {
                char c = client.read();
                Serial.write(c);
                // if you've gotten to the end of the line (received a newline
                // character) and the line is blank, the http request has ended,
                // so you can send a reply
                if (c == '\n' && currentLineIsBlank) {
                    client.println("HTTP/1.1 200 OK");
                    client.println("nontent-Type: text/html");
                    client.println("Connection: close");
                    client.println("");
                    client.println("<!doctype html>"); // setup html
                    client.println("<html lang='en-us'>");
                    client.println("<head>");
                    client.println("<meta charset='UTF-8'>");
                    client.println("<title>DOSBox</title>"); // set webpage title
                    client.println("<style>html,body{height:100%;} #dos {width: 100%; height: 100%;}</style>");
                    client.println("<link rel='stylesheet' href='https://v8.js-dos.com/latest/js-dos.css'>");
                    client.println("<script src='https://v8.js-dos.com/latest/js-dos.js'></script>");
                    client.println("</head>");
                    client.println("<body>");
                    client.println("<div id='dos' style='width: 100%; height: 100%'></div>");
                    client.println("<script>Dos(document.getElementById('dos'), {url: 'https://raw.githubusercontent.com/thegamershollow/webDOS/refs/heads/main/bundle.jsdos',});</script>");
                    client.println("</body>");
                    client.println("</html>");
                    break;
                }
                if (c == '\n') {
                    // you're starting a new line
                    currentLineIsBlank = true;
                    } else if (c != '\r') {
                    // you've gotten a character on the current line
                    currentLineIsBlank = false;
                }
            }
        }
        // give the web browser time to receive the data
        delay(1);
        // close the connection:
        client.stop();
        Serial.println("client disonnected");
        digitalWrite(LED_USER, LOW);
    }
}
void printWifiStatus() {
    // print the SSID of the network you're attached to:
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());
    // print your WiFi shield's IP address:
    IPAddress ip = WiFi.localIP();
    uint8_t IP_array[4]={ip[0],ip[1],ip[2],ip[3]};
    String strIP= String(IP_array[0])+"."+String(IP_array[1])+"."+String(IP_array[2])+"."+String(IP_array[3]);
    const char* ipCStr = strIP.c_str();
    Screen.clean();
    Serial.print("WiFi SSID: ");
    Serial.println(WiFi.SSID());
    Serial.print("IP Address: ");
    Serial.println(strIP);
    Serial.print("Port: ");
    Serial.println(port);
    Screen.print(0, "WiFi SSID:");
    Screen.print(1, WiFi.SSID());
    Screen.print(2,"IP Address:");
    Screen.print(3, ipCStr, true);
    digitalWrite(LED_AZURE, HIGH);
    // print the received signal strength:
    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");
}
