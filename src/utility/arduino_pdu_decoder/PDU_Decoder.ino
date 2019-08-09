//decode a pdu format message
#include "pdu_decoder.h"

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  delay(1000); //delay a little toavoid triggering a restart

  Serial.println(pdu_decode("0CC8F71D14969741F977FD07"));   //0C(decimal 12) followed by How are you?
  Serial.println(pdu_decode("C8F71D14969741F977FD07", 12)); //How are you?
  Serial.println(pdu_decode("A0C8F71D14969741F977FD07A2A2D373507A0E9ABFDFEFF7FB0D6AD7C76890B9EE0685DD645098CD06D1D1659099FDBE97E573D03DCD6683C4ECF7BB0D0ABBC9203ABA0CBA97C37474590EBAA7D96C90B80C3287D37250D84D06D1D165D0B9EC2ECBC3F4F4DB3D07DDD36C36C8CC7ED7E5E9393A0432A6D9ECB4FB0CA2A3D373501D0E4ACF41EAFA9C0E32D7DD"));
}

void loop() {
}
