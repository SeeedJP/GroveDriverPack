// BOARD Seeed Wio 3G
// GROVE UART <-> Grove - Speech Recognizer (SKU#101020232)

#include <GroveDriverPack.h>

WioCellular Wio;

GroveBoard Board;
GroveSpeechRecognizer Speech(&Board.UART);

void CommandReceived(GroveSpeechRecognizer::COMMAND_TYPE command) {
  SerialUSB.println((int)command);
}

void setup() {
  delay(200);
  SerialUSB.begin(115200);

  Wio.Init();
  Wio.PowerSupplyGrove(true);
  delay(500);
  
  Board.UART.Enable(9600, 8, HalUART::PARITY_NONE, 1);
  Speech.Init();
  
  Speech.AttachCommandReceived(CommandReceived);
}

void loop() {
  Speech.DoWork();
}
