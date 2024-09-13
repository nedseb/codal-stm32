#pragma once

#include <string>

template <class T>
class BLECharateristic {
  public:
    BLECharacteristic<T>(std::string characteristicUUID, T value);

    T readValue();
    void writeValue(T value);

  private:
    T value;
    std::string uuid;
};