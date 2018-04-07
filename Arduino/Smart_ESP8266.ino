\#include <ESP8266WiFi.h>
#include <Servo.h>
#include <ESP8266HTTPClient.h>

 
const char* ssid = //insert your SSID here;
const char* password = //insert your Password here;
 
int ledPin = 13; // GPIO13
int analogIn = A0; // GPIO13
int ledState = LOW;

int flagopen = 0;
int flagclose = 0;

WiFiServer server(80);

Servo myservo;
int distance;
 
void setup() 
{
  Serial.begin(115200);
  delay(10);

  // Variable inits
  ledState = LOW;

  //Init LED output
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, ledState);

 
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) 
  {
      delay(500);
      Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

  myservo.attach(0);
}
 
void loop() 
{
  
  int photocell_value = analogRead(analogIn);
  
  HTTPClient http;  //Declare an object of class HTTPClient
 
  http.begin("http://ronakgune.com/smart/smart.txt");  //Specify request destination
  int httpCode = http.GET(); //Send the request
 
  if (httpCode > 0) 
  {
      //flagopen = 0;
      //flagclose = 0; 
      //Check the returning code
 
      String payload = http.getString();   //Get the request response payload
      Serial.println(payload);                     //Print the response payload
      
      if(payload == "stop")
      {
        myservo.detach();
      }

      else if(payload == "automatic")
      {
        Serial.print("in automatic");
        photocell_value = analogRead(analogIn);
      
        if(photocell_value == 1024)
        {
          for(int i=0;i<10;i++)
          {
            myservo.attach(0);
            // attaches the servo on pin 9 to the servo object
            delay(15);
            myservo.write(-50);
            // sets the servo position according to the scaled value
            delay(1000);
            // waits for it to get to the position
          
          myservo.detach();
          }
          delay(50000);
        }
        else
        {

            for(int i=0;i<10;i++)
            {
            myservo.attach(0);
            // attaches the servo on pin 9 to the servo object
            delay(15);
            myservo.write(179);
            // sets the servo position according to the scaled value
            delay(1000); // waits for it to get to the position
          
          myservo.detach();
            }
            delay(50000);
        }
      }     
      
      else if(payload == "open" || payload == "manual")
      {
        Serial.print("itworks");
        myservo.attach(0);
        // attaches the servo on pin 9 to the servo object
        delay(15);
        myservo.write(-50);
        // sets the servo position according to the scaled value
        delay(1000);
        // waits for it to get to the position
        myservo.detach();
        delay(1000);
      }
       
      else if(payload = "close" || payload == "manual")
      {
        Serial.print("itcloses");
        myservo.attach(0);
        // attaches the servo on pin 9 to the servo object
        delay(15);
        myservo.write(179);
        // sets the servo position according to the scaled value
        delay(1000); // waits for it to get to the position
        myservo.detach();
        delay(1000);
      }   
  }
  
  http.end();   //Close connection
  delay(500);    //Send a request every 5 seconds
 
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) 
  {
    return;
  }
 
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available())
  {
    delay(1);
  }
 
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  // Find which, if any, commmand was sent:
  
  // This is a little hacky, we should parse the query string
  // then check for the existance of named variables, but this is a bit quicker to program - alls we do
  // is check if the command string is found, and if it is, execute the command
  if (request.indexOf("cmd=TURN_ON_LED") != -1)
  {
    // if the command is turn on the led, turn it on
    ledState = HIGH;
  }
  else if (request.indexOf("cmd=TURN_OFF_LED") != -1)
  {
     // if the command is turn off the led, turn it off
    ledState = LOW;    
  }
  // Update the LED based on ledState
  digitalWrite(ledPin, ledState);


  // Always update the photocell value anytime there's a request
  // NOTE: We have the cmd=RELOAD_PHOTOCELL command because we need a way
  // to update the photocell without changing the led state for the user
  //int photocell_value = analogRead(analogIn);
  
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
 
  client.print("Led pin is now: ");
 
  if(ledState == HIGH) {
    client.print("On");
  } else {
    client.print("Off");
  }
  client.println("<br><br>");
  client.println("<a href=\"?cmd=TURN_ON_LED\"><button>Turn On </button></a>");
  client.println("<a href=\"?cmd=TURN_OFF_LED\"><button>Turn Off </button></a>");

  client.println("<p>The photocell's value is:<b> ");
  client.println(photocell_value);
 
  client.println("</b></p>");
  client.println("<a href=\"?cmd=RELOAD_PHOTOCELL\"><button>Check for new photocell value</button></a>");
  
  client.println("</html>");
 
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");  
}