

//http://blog.zakkemble.co.uk/getting-bluetooth-modules-talking-to-each-other/

#define LED        6

#include <inttypes.h>

#define DESIRED_NAME "HC06 NAME"
#define DESIRED_BAUD_RATE "8"
#define DESIRED_PIN  "1111"
/*
  1---------1200
  2---------2400
  3---------4800
  4---------9600
  5---------19200
  6---------38400
  7---------57600
  8---------115200
*/

uint32_t baudRates[] = {1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200};
uint8_t N_baudRates = (sizeof(baudRates)) / (sizeof(baudRates[0]));
uint32_t HC06_baudRate = 0;


uint8_t nameSet = false;
uint8_t baudSet = false;
uint8_t pinSet = false;

void testBaudRates() {
  uint8_t i;
  Serial1.end();
  delay(1000);

  for (i = 0; i < N_baudRates; i++) {
    Serial.print("Test baud rate ");
    Serial.println(baudRates[i]);

    Serial1.begin(baudRates[i]);
    delay(100);

    Serial1.print("AT");

    delay(1000);
    if (Serial1.available()) {
      uint8_t rx_byte = Serial1.read();
      if (rx_byte == 'O') {
        Serial.print("Baud Rate = ");
        Serial.println(baudRates[i]);
        HC06_baudRate = baudRates[i];
        Serial1.end();
        return;
      }
    }
    Serial1.end();
    delay(100);
  }
}

void configHC06() {
  Serial.println(" ");
  Serial.println("Config HC-06 module");
  Serial.println(" ");

  //Set bluetooth NAME
  Serial.print("AT+NAME");
  Serial.println(DESIRED_NAME);

  Serial1.print("AT+NAME");
  Serial1.print(DESIRED_NAME);


  delay(1000);

  while (Serial1.available()) {
    Serial.write(Serial1.read());
  }
  Serial.println(" ");
  Serial.println(" ");

  //Set bluetooth BAUD RATE
  Serial.print("AT+BAUD");
  Serial.println(DESIRED_BAUD_RATE);

  Serial1.print("AT+BAUD");
  Serial1.print(DESIRED_BAUD_RATE);
  delay(1000);

  while (Serial1.available()) {
    Serial.write(Serial1.read());
  }
  Serial.println(" ");
  Serial.println(" ");

  //Set bluetooth PIN
  Serial.print("AT+PIN");
  Serial.println(DESIRED_PIN);

  Serial1.print("AT+PIN");
  Serial1.print(DESIRED_PIN);

  delay(1000);

  while (Serial1.available()) {
    Serial.write(Serial1.read());
  }
  Serial.println(" ");
  Serial.println(" ");

}

void setup() {
  Serial.begin(115200);

  pinMode(LED, OUTPUT);

  while (HC06_baudRate == 0) {
    testBaudRates();
  }

  Serial1.begin(HC06_baudRate);

  configHC06();

  Serial.println("Your HC-06 is configured with the desired parameters");
  Serial.println("Now this terminal is able to send and receive data through bluetooth module");
  Serial.println("If you changed the baud rate on this config, please run this code again before other tests");

}



void loop() {
  while (Serial1.available()) {
    Serial.write(Serial1.read());
  }
  while (Serial.available()) {
    Serial1.write(Serial.read());
  }
}
