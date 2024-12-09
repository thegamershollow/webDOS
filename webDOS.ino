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
int btnBState; // create a variable for button B state

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
    delay(100) // set a 100 ms delay
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
        Screen.print(2,ssid,true); // print ssid that shield is trying to connect to
        // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
        status = WiFi.begin(ssid, password); // connect to wifi network that is specified by the SSID variable and PASSWORD variable
        // wait 10 seconds for connection:
        delay(10000);
    }
    server.begin();
    // you're connected now, so print out the status:
    printWifiStatus(); 
}
void loop() {
    Serial.println("Listening for incoming clients");
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
                    client.println("<title>webDOS</title>"); // set webpage title
                    client.println("<style>html,body{height:100%;} #dos {width: 100%; height: 100%;}</style>"); // set width and height of the window
                    client.println("<link rel='stylesheet' href='https://v8.js-dos.com/latest/js-dos.css'>"); // load the js-dos css style sheet
                    client.println("<script src='https://v8.js-dos.com/latest/js-dos.js'></script>"); // load the js-dos javascript file
                    client.println("</head>");
                    client.println("<body>");
                    client.println("<div id='dos' style='width: 100%; height: 100%'></div>"); // set width and height to 100%
                    client.println("<script>Dos(document.getElementById('dos'), {url: 'https://raw.githubusercontent.com/thegamershollow/webDOS/refs/heads/main/bundle.jsdos',});</script>"); // load the js-dos package
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
        Serial.println("client disonnected"); // print status to serial
        digitalWrite(LED_USER, LOW); // set led to off state
    }
}
void printWifiStatus() {
    IPAddress ip = WiFi.localIP(); // get the ip address of the webserver running on the arduino
    // convert the ip address into a c string
    uint8_t IP_array[4]={ip[0],ip[1],ip[2],ip[3]};
    String strIP= String(IP_array[0])+"."+String(IP_array[1])+"."+String(IP_array[2])+"."+String(IP_array[3]);
    const char* ipCStr = strIP.c_str();
    Screen.clean(); // clear screen
    Serial.print("WiFi SSID: "); // print wifi ssid to serial
    Serial.println(WiFi.SSID()); // print wifi ssid to serial
    Serial.print("IP Address: "); // print ip address to serial
    Serial.println(strIP); // print ip address to serial
    Serial.print("Port: "); // print port to serial
    Serial.println(port); // print port to serial
    Screen.print(0, "WiFi SSID:"); // print wifi ssid to screen
    Screen.print(1, WiFi.SSID()); // print wifi ssid to screen
    Screen.print(2,"IP Address:"); // print ip address to screen
    Screen.print(3, ipCStr, true); // print ip address to screen
    digitalWrite(LED_AZURE, HIGH); // set the AZURE led to high
    // print the received signal strength:
    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");
}
