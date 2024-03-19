#include <Arduino.h>
#include <FlexCAN_T4.h>
#include "Logger.h"

FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> Can0;
Logger logger;
void canSniff(const CAN_message_t &msg);

void setup() {
  // put your setup code here, to run once:
  Can0.begin();
  Can0.setBaudRate(1000000);
  Can0.setMaxMB(16);
  Can0.enableFIFO();
  Can0.enableFIFOInterrupt();
  Can0.onReceive(canSniff);
  Can0.mailboxStatus();

  logger.setup();
  logger.initializeFile(
    "sgamplifier",
    {
      "amplifier output"
    }
  );

  Serial.begin(115200);

  pinMode(13, OUTPUT);

  digitalWrite(13, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
}

void canSniff(const CAN_message_t &msg) {
  int rawAmplifierOutput = 0;
  int ambientTemp = 0;


  switch(msg.id) {
    case 0x4E2: // TODO update ids
      Serial.print("id: ");
      Serial.print(msg.id, HEX);
      rawAmplifierOutput = (msg.buf[0] << 8) | msg.buf[1];
      ambientTemp = (msg.buf[4] << 8) | msg.buf[5]; // attempting to read ambient temp
      Serial.print("  ");
      Serial.println(rawAmplifierOutput);
      logger.addData("sgamplifier", "amplifier output", (float) rawAmplifierOutput);
      break;
    default:
    //Serial.print("No message");
    break;
  }

  logger.writeRow("sgamplifier");
  // semi-updated print 
}