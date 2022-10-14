#pragma once

#include <cstdint>

constexpr uint8_t EVT_COMMAND_COMPLETE = 0x0E;
constexpr uint8_t LE_EVENT_CODE        = 0x3E;

constexpr uint8_t LE_EVENT_CONNECTION_COMPLETE                 = 0x01;
constexpr uint8_t LE_EVENT_ADVERTISING_REPORT                  = 0x02;
constexpr uint8_t LE_EVENT_CONNECTION_UPDATE_COMPLETE          = 0x03;
constexpr uint8_t LE_EVENT_READ_REMOTE_USED_FEATURE_COMPLETE   = 0x04;
constexpr uint8_t LE_EVENT_LONG_TERM_KEY_REQUESTED             = 0x05;
constexpr uint8_t LE_EVENT_REMOTE_CONN_PARAM_REQUEST           = 0x06;
constexpr uint8_t LE_EVENT_DATA_LENGTH_CHANGE                  = 0x07;
constexpr uint8_t LE_EVENT_READ_LOCAL_P256_PUBLIC_KEY_COMPLETE = 0x08;
constexpr uint8_t LE_EVENT_DHKEY_COMPLETE                      = 0x09;
constexpr uint8_t LE_EVENT_ENHANCED_CONNECTION_COMPLETE        = 0x0A;
constexpr uint8_t LE_EVENT_DIRECT_ADVERTISING_REPORT           = 0x0B;