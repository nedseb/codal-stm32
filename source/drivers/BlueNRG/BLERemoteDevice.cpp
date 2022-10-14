#include "BLERemoteDevice.h"

using namespace std;

vector<BLERemoteDevice> BLERemoteDevice::buildFromAdvertisingReports(deque<BLEAdvertisingReport> reports)
{
    vector<BLERemoteDevice> devices;
    size_t idx;

    for (BLEAdvertisingReport report : reports) {
        idx = isReportInVector(devices, report);

        if (idx < devices.size()) {
            if (!report.name.empty()) {
                devices[idx].name = report.name;
            }
            devices[idx].rssi = report.rssi;

            for (auto sd : report.serviceData) {
                devices[idx].serviceDatas.push_back(sd);
            }
        }
        else {
            BLERemoteDevice dev;
            dev.address      = report.address;
            dev.name         = report.name;
            dev.rssi         = report.rssi;
            dev.serviceDatas = report.serviceData;
            devices.push_back(dev);
        }
    }

    return devices;
}

size_t BLERemoteDevice::isReportInVector(vector<BLERemoteDevice>& devices, BLEAdvertisingReport& report)
{
    for (size_t i = 0; i < devices.size(); ++i) {
        if (devices[i].address == report.address) {
            return i;
        }
    }

    return devices.size();
}