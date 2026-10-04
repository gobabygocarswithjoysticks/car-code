/*
    Based on
    BLEServiceUART - v6.2.0
    Copyright (c) 2026 Earle F. Philhower, III.  GNU LGPL
*/


#pragma once

#include <_needsbt.h>
#include <Arduino.h>
#include "BLEService.h"
#include "BLECharacteristic.h"
#include <pico/async_context.h>
#include <LocklessQueue.h>

class BLEServiceGBG : public BLEService, public BLECharacteristicCallbacks, public arduino::HardwareSerial {
  public:
    static constexpr const char *SERVICE_UUID = "25210001-ad15-4c78-9ef6-cba8e277fd8d";
    static constexpr const char *CHARACTERISTIC_UUID_RX = "25210002-ad15-4c78-9ef6-cba8e277fd8d";
    static constexpr const char *CHARACTERISTIC_UUID_TX = "25210003-ad15-4c78-9ef6-cba8e277fd8d";
    static constexpr const char *CHARACTERISTIC_UUID_RC_CTRL = "25210012-ad15-4c78-9ef6-cba8e277fd8d";
    static constexpr const char *CHARACTERISTIC_UUID_RC_TELEM = "25210013-ad15-4c78-9ef6-cba8e277fd8d";

    BLEServiceGBG();
    ~BLEServiceGBG();

    void begin();
    void begin(unsigned long baud) override;
    void begin(unsigned long baud, uint16_t cfg) override;

    void end();

    void setAutoflush(uint32_t ms = 50);

    operator bool();

    int read();
    int peek();
    int available();
    bool overflow();

    size_t write(uint8_t c);

    void flush();

    using Print::write;

  private:
    async_at_time_worker_t _flushwork;
    static void _flushWorkCB(async_context_t *context, struct async_work_on_timeout *timeout) {
      (void) context;
      ((BLEServiceGBG *)(timeout->user_data))->flush();
    }

    static int64_t _flushcb(alarm_id_t t, void *data) {
      (void)t;
      ((BLEServiceGBG *)data)->flush();
      return 0;
    }

    // The host has sent us data...
    void onWrite(BLECharacteristic *c) override;

    BLECharacteristic *_rx;
    BLECharacteristic *_tx;
    BLECharacteristic *_ctrl;
    BLECharacteristic *_telm;
    LocklessQueue<uint8_t> *_rxQueue;
    bool _overflow;
    LocklessQueue<uint8_t> *_txQueue;
    size_t _txSize;

    alarm_id_t _flushAlarm;  // Current (if any) alarm
    uint32_t _lastFlush;     // Time we last flushed data
    uint32_t _flushTimeout;  // ms to sit on a buffer before forcing a flush on a callback
};
