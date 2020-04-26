// BOARD Seeed Wio LTE M1/NB1(BG96)
// GROVE UART <-> Grove - MP3 V3 -Music Player (SKU#107020069)

#include <GroveDriverPack.h>

#define VOLUME  (2)

WioCellular Wio;

GroveBoard Board;
GroveMP3V3 MP3(&Board.UART);

void setup() {
  delay(200);
  SerialUSB.begin(115200);

  Wio.Init();
  Wio.PowerSupplyGrove(true);
  
  Board.UART.Enable(9600, 8, HalUART::PARITY_NONE, 1);
  if (!MP3.Init())
  {
    SerialUSB.println("Grove-MP3V3 not found.");
    return;
  }

  int n = MP3.QueryNumberOfMusic();
  SerialUSB.print("Number of music = ");
  SerialUSB.println(n);

  MP3.SetVolume(VOLUME);

  for (int i = 0; i < n; ++i)
  {
    SerialUSB.print("Play ");
    SerialUSB.println(i);
    
    MP3.Play(i);
    while (MP3.QueryStatus() == GroveMP3V3::STATUS_PLAY) delay(100);
  }
}

void loop() {
}
