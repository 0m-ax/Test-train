int DDC_PIN = 7;
#include "queue.c";
enum MessageType {SpeedMessage};
typedef struct locoMesssage {
  unsigned char loco;
} LocoMesssage;
typedef struct speedMessageData {
  unsigned char loco;
  bool direction;
  unsigned char speed;
} SpeedMessageData;
typedef union messageData {
  SpeedMessageData speedMessageData;
  LocoMesssage locoMessage;
} MessageData;
typedef struct message {
  MessageType messageType;
  MessageData data;
} Message;

typedef struct lineMessage {
  unsigned char data[5];
  unsigned int len;
} LineMessage;
int OneDelay = 58;
int ZeroDelay = 100;

struct Queue* messageQueue = queue_create(10);


void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(DDC_PIN, OUTPUT);
  Serial.begin(9200);
}



void loop() {
  Message message;
  message.data.speedMessageData.loco = 40;
  message.data.speedMessageData.direction = true;
  message.data.speedMessageData.speed = 10;
  message.messageType = SpeedMessage;
  LineMessage lineMessage;
  lineMessage.len = 0;
  if (message.data.locoMessage.loco > 127) {
    //todo this is a long message
  } else {
    lineMessage.data[lineMessage.len] = 0b01111111 & message.data.locoMessage.loco;
    lineMessage.len++;
  }
  if (message.messageType == SpeedMessage) {

    unsigned char data = 0b01000000;
    data = (data & 0b11011111) | (message.data.speedMessageData.direction ? 0b00100000 : 0b00000000);
    data = (data & 0b11110000) | (message.data.speedMessageData.speed >> 1);
    //    data = (data & 0b11101111) | (message.data.speedMessageData.speed << 4);

    lineMessage.data[lineMessage.len] = data;
    lineMessage.len++;
  }
  lineMessage.data[2] = lineMessage.data[0] ^ lineMessage.data[1];
  lineMessage.len++;

  for (int i = 0; i < 16; i++) {
    digitalWrite(DDC_PIN, HIGH);
    delayMicroseconds(OneDelay);
    digitalWrite(DDC_PIN, LOW);
    delayMicroseconds(OneDelay);
  }
  for (int part = 0; part < lineMessage.len; part++) {
    digitalWrite(DDC_PIN, HIGH);
    delayMicroseconds(ZeroDelay);
    digitalWrite(DDC_PIN, LOW);
    delayMicroseconds(ZeroDelay);
    for (int offset = 0; offset < 8; offset++) {
      if (((lineMessage.data[part] << offset) & 0b10000000) == 0b10000000) {
        digitalWrite(DDC_PIN, HIGH);
        delayMicroseconds(OneDelay);
        digitalWrite(DDC_PIN, LOW);
        delayMicroseconds(OneDelay);
      } else {
        digitalWrite(DDC_PIN, HIGH);
        delayMicroseconds(ZeroDelay);
        digitalWrite(DDC_PIN, LOW);
        delayMicroseconds(ZeroDelay);
      }
    }
  }
  digitalWrite(DDC_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delayMicroseconds(OneDelay);
  digitalWrite(DDC_PIN, LOW);   // turn the LED on (HIGH is the voltage level)
  delayMicroseconds(OneDelay);

}
