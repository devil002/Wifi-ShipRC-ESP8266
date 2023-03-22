#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Servo.h>

Servo servo_1;

const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWD";

const int port = 80;
ESP8266WebServer server(port);

// define L298N or L293D motor control pins
int MotorForward = 0;   // GPIO0(D3) -> IN1 
int MotorBackward = 2;  // GPIO2(D4) -> IN2  

// define L298N or L293D enable pins 
int MotorENB = 4; // GPIO4(D2) -> Motor-A Enable 14

int motor_speed = 800;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  // initialize motor control pins as output
  pinMode(MotorForward, OUTPUT);
  pinMode(MotorBackward, OUTPUT);

  // initialize motor enable pins as output
  pinMode(MotorENB, OUTPUT);

  servo_1.attach(16); // Attaching Servo to D0

  Serial.begin(115200);
  Serial.print("Connecting to : ");
  Serial.println(ssid);

  WiFi.persistent(false);
  WiFi.mode(WIFI_OFF);
  WiFi.mode(WIFI_STA);

  WiFi.disconnect();
  WiFi.hostname("relay.local");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    pinMode(LED_BUILTIN, LOW);
    delay(250);
    pinMode(LED_BUILTIN, HIGH);
    delay(250);
    Serial.print(".");
  }

  Serial.println("Connection established!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Port: " +String(port));
  Serial.print("Hostnmae :");
  Serial.println(WiFi.getHostname());

  server.on("/move", HTTP_GET, handleMoveRequest);
  server.on("/action", HTTP_GET, handleActionRequest);
  server.onNotFound(handleNotFound);
  server.begin();
}

void loop() {
  server.handleClient();
}

void handleMoveRequest() {
  if (!server.hasArg("dir")) {
    server.send(404, "text / plain", "Move: undefined");
    return;
  }
  String direction = server.arg("dir");
  if (direction.equals("F")) {
    forward();
    server.send(200, "text / plain", "Move: forward");
  }
  else if (direction.equals("B")) {
    backward();
    server.send(200, "text / plain", "Move: backward");
  }
  else  if (direction.equals("S")) {
    stop_motors();
    server.send(200, "text / plain", "Move: stop");
  }
  else  if (direction.equals("L")) {
    turn_left();
    server.send(200, "text / plain", "Turn: Left");
  }
  else  if (direction.equals("R")) {
    turn_right();
    server.send(200, "text / plain", "Turn: Right");
  }
  else {
    server.send(404, "text / plain", "Move: undefined");
  }
}


void handleActionRequest() {
  if (!server.hasArg("type")) {
    server.send(404, "text / plain", "Action: undefined");
    return;
  }
  String type = server.arg("type");
  if (type.equals("1")) {
    motor_speed = 400;
    Serial.println("Mode: Action 1 and speed "+String(motor_speed));
    server.send(200, "text / plain", "Action 1");
  }
  else if (type.equals("2")) {
    motor_speed = 470;
    Serial.println("Mode: Action 2 and speed "+String(motor_speed));
    server.send(200, "text / plain", "Action 2");
  }
  else if (type.equals("3")) {
    motor_speed = 610;
    Serial.println("Mode: Action 3 and speed "+String(motor_speed));
    server.send(200, "text / plain", "Action 3");
  }
  else if (type.equals("4")) {
    motor_speed = 680;
    Serial.println("Mode: Action 4 and speed "+String(motor_speed));
    server.send(200, "text / plain", "Action 4");
  }
  else if (type.equals("5")) {
    motor_speed = 750;
    Serial.println("Mode: Action 5 and speed "+String(motor_speed));
    server.send(200, "text / plain", "Action 5");
  }
  else if (type.equals("6")) {
    motor_speed = 820;
    Serial.println("Mode: Action 6 and speed "+String(motor_speed));
    server.send(200, "text / plain", "Action 6");
  }
  else if (type.equals("7")) {
    motor_speed = 890;
    Serial.println("Mode: Action 7 and speed "+String(motor_speed));
    server.send(200, "text / plain", "Action 7");
  }
  else if (type.equals("8")) {
    motor_speed = 960;
    Serial.println("Mode: Action 8 and speed "+String(motor_speed));
    server.send(200, "text / plain", "Action 8");
  }
  else {
    server.send(404, "text / plain", "Action: undefined");
  }
}

void handleNotFound() {
  server.send(404, "text / plain", "404: Not found");
}


void stop_motors() {
  Serial.println("Stop");
  analogWrite(MotorENB, motor_speed);
  digitalWrite(MotorENB,LOW);
  digitalWrite(MotorForward,LOW);
  digitalWrite(MotorBackward,LOW);
  servo_1.write (90);
}

void backward() {
  Serial.println("Backward with speed "+String(motor_speed));
  analogWrite(MotorENB, motor_speed);
  //digitalWrite(MotorENB,HIGH);
  digitalWrite(MotorBackward,HIGH);
  digitalWrite(MotorForward,LOW);
  servo_1.write (90);  //90
}

void forward() {
  Serial.println("Forward with speed "+String(motor_speed));
  analogWrite(MotorENB, motor_speed);
  //digitalWrite(MotorENB,HIGH);
  digitalWrite(MotorForward,HIGH);
  digitalWrite(MotorBackward,LOW);
  servo_1.write (90); //90
}

void turn_left() {
  Serial.println("Left with speed "+String(motor_speed));
  analogWrite(MotorENB, motor_speed);
  //digitalWrite(MotorENB,HIGH);
  digitalWrite(MotorForward,HIGH);
  digitalWrite(MotorBackward,LOW);
  Serial.println("Left with 5");
  servo_1.write (5); //5-85
}

void turn_right() {
  Serial.println("Right with speed "+String(motor_speed));
  analogWrite(MotorENB, motor_speed);
  //digitalWrite(MotorENB,HIGH);
  digitalWrite(MotorForward,HIGH);
  digitalWrite(MotorBackward,LOW);
  Serial.println("Right with 180");
  servo_1.write (180);  //95-180
}
