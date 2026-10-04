#include "BLEServiceGBG.h"

#include <Arduino.h>
#include <btstack.h>
#include <ble/att_db_util.h>
#include <pico/async_context.h>
#include <pico/cyw43_arch.h>
#include <list>
#include <LocklessQueue.h>
#include <BluetoothLock.h>

BLEServiceGBG::BLEServiceGBG()
  : BLEService(BLEUUID(SERVICE_UUID)) {
  const int rxbuff = 2000;
  const int txbuff = 2000;
  _txSize = 200;
  _rx = new BLECharacteristic(BLEUUID(CHARACTERISTIC_UUID_RX), BLEWrite, "GBG RX");
  _rx->setCallbacks(this);
  _tx = new BLECharacteristic(BLEUUID(CHARACTERISTIC_UUID_TX), BLERead | BLENotify, "GBG TX");
  _tx->setCallbacks(this);
  addCharacteristic(_rx);
  addCharacteristic(_tx);
  _rxQueue = new LocklessQueue<uint8_t>(rxbuff);
  _txQueue = new LocklessQueue<uint8_t>(txbuff);
  _overflow = false;
  _flushAlarm = -1;
  _flushTimeout = 0;
  _lastFlush = millis();

  _ctrl = new BLECharacteristic(BLEUUID(CHARACTERISTIC_UUID_RX), BLEWrite, "GBG CTRL");
  _ctrl->setCallbacks(this);
  _telm = new BLECharacteristic(BLEUUID(CHARACTERISTIC_UUID_TX), BLERead | BLENotify, "GBG TELM");
  _telm->setCallbacks(this);
  addCharacteristic(_ctrl);
  addCharacteristic(_telm);

}

BLEServiceGBG::~BLEServiceGBG() {
  BluetoothLock b;
  delete _rx;
  delete _tx;
  delete _rxQueue;
  delete _txQueue;
  async_context_remove_at_time_worker(cyw43_arch_async_context(), &_flushwork);
}

void BLEServiceGBG::begin() {
}

void BLEServiceGBG::begin(unsigned long baud) {
  (void)baud;
  begin();
}

void BLEServiceGBG::begin(unsigned long baud, uint16_t cfg) {
  (void)baud;
  (void)cfg;
  begin();
}

void BLEServiceGBG::end() {
  panic("unsupported BLEUart::end");
}

void BLEServiceGBG::setAutoflush(uint32_t ms) {
  _flushTimeout = ms;
}

BLEServiceGBG::operator bool() {
  if (con_handle != 0) {
    return true;
  } else {
    return false;
  }
}

int BLEServiceGBG::read() {
  uint8_t ret;
  if (_rxQueue->read(&ret)) {
    return ret;
  } else {
    return -1;
  }
}

int BLEServiceGBG::peek() {
  uint8_t ret;
  if (_rxQueue->peek(&ret)) {
    return ret;
  } else {
    return -1;
  }
}

int BLEServiceGBG::available() {
  return _rxQueue->available();
}

bool BLEServiceGBG::overflow() {
  BluetoothLock b;
  bool ovf = _overflow;
  _overflow = false;
  return ovf;
}

size_t BLEServiceGBG::write(uint8_t c) {
  if (_txQueue->write(c)) {
    // We can just buffer it up for now, but set reminder alarm
    BluetoothLock b;
    if ((_flushTimeout > 0) && (_flushAlarm < 0)) {
      _flushwork.do_work = _flushWorkCB;
      _flushwork.user_data = (void *)this;
      async_context_add_at_time_worker_in_ms(cyw43_arch_async_context(), &_flushwork, _flushTimeout);
      _flushAlarm = 1;
    }
    return 1;
  }
  // Write buffer is full, update characteristic
  flush();
  return _txQueue->write(c);
}

void BLEServiceGBG::flush() {
  async_context_remove_at_time_worker(cyw43_arch_async_context(), &_flushwork);

  _flushAlarm = -1;
  uint8_t b[_txSize];
  size_t len = 0;
  while (len < _txSize) {
    uint8_t r;
    if (!_txQueue->read(&r)) {
      break;
    }
    b[len++] = r;
  }
  _tx->setValue(b, len);
  _lastFlush = millis();
}

void BLEServiceGBG::onWrite(BLECharacteristic *c) {
  if (c == _rx) {
    auto len = c->valueLen();
    const char *data = (const char *)c->valueData();
    for (size_t off = 0; off < len; off++) {
      if (!_rxQueue->write(data[off])) {
        _overflow = true;
      }
    }
  }
}
