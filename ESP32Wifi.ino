#include <WiFi.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

extern "C"
{
  #include "linkedlist.h"
}

#define SSID "SASKTEL01G9"
#define PASSWORD "v35j87v85tkm"
#define PEER_IP "172.16.1.157"
#define PEER_PORT 8200
#define BUFFER_SIZE 1024
#define AVERAGE_NUM 3
#define AVERAGE_CM_DOOR_OPEN 50
#define PAUSE_TIME_MS 30000
#define MS_DELAY 1000

const int trigPin = 23;
const int echoPin = 22;

int udp_socket = -1;
int skip = 0;
linked_list *ll = createLinkedList(AVERAGE_NUM);


int is_door_open(double average)
{
  if (average <= AVERAGE_CM_DOOR_OPEN)
    return 1;

  return 0;
}

char *convert_chararr(unsigned long num, char *buffer)
{

  snprintf(buffer, BUFFER_SIZE, "%lu", num);

  return buffer;
}

unsigned long get_distance_cm(int trigPin, int echoPin)
{

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);


  return pulseIn(echoPin, HIGH) * 0.034/2;
}

int createUDPsocket()
{
  if (udp_socket != -1)
  {
    return -1;
  }

  udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
  if (udp_socket < 0)
  {
    Serial.println("Failed to create UDP socket");
    return -1;
  }

  return 0;
}

int send_message(char *peer_ip, int peer_port, char *message)
{
  
  if (udp_socket < 0)
  {
    Serial.println("UDP socket not initialzed");
    return -1;
  }

  struct sockaddr_in peer_addr = {.sin_family = AF_INET, .sin_port = htons(peer_port)};

  if (inet_pton(AF_INET, peer_ip, &(peer_addr.sin_addr)) <= 0)
  {
    Serial.println("Something went wrong with IP");
    return -1;
  }


  if (sendto(udp_socket, message, strlen(message) + 1, 0, (struct sockaddr *)&peer_addr, sizeof(peer_addr)) < 0)
  {
    Serial.println("Failed to sendto");
    Serial.println(errno);
    return -1;
  }

  return 0;
}


int connect_wifi(const char *ssid, const char *pass)
{
	
  const unsigned long timeout = 15000;
  unsigned long startTime = millis();

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED && (millis() - startTime < timeout))
  {
    delay(100);
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    return 0;
  }

  else
  {
    return 1;
  }
}




void setup() {

  Serial.begin(115200);

  delay(5000);

  Serial.println("Starting:");

  if (connect_wifi(SSID, PASSWORD) == 0)
  {
    Serial.println("Connected! IP: " + WiFi.localIP().toString());
  }
  else
  {
    Serial.println("Connection Failed");
    return;
  }


  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  for (int i = 0; i < AVERAGE_NUM; i++)
  {
    /* enqueue very big number to make average large */
    enqueue(ll, createNodeSet(1000));
  }

  if (createUDPsocket() < 0)
  {
    Serial.println("Socket not created properly");
  }

}



void loop() {
  char buffer[BUFFER_SIZE];
  unsigned long distance_cm = get_distance_cm(trigPin, echoPin);
  send_message(PEER_IP, PEER_PORT, "Loop iter");
  enqueue(ll, createNodeSet(distance_cm));
  if (skip == 0)
  {
    if (is_door_open(getAverage(ll)))
    {
      send_message(PEER_IP, PEER_PORT, "Door Detected Open!");
      
      skip = (int)PAUSE_TIME_MS/MS_DELAY;
    }
  }
  
  if (skip != 0)
    skip--;

  delay(MS_DELAY);
}
