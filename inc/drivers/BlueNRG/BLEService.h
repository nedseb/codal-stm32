#pragma once

#include <string>

class BLEService {
  public:
    BLEService(std::string serviceUUID);

  private:
    std::string uuid;
};