//
// Created by paul on 24-1-2021.
//

#include "BroadcastPrint.h"

size_t BroadcastPrint::write(uint8_t b) {
    if (this->wifi != nullptr) {
        this->wifi->write(b);
    }
    if (this->ethernet != nullptr) {
        this->ethernet->write(b);
    }
    if (this->serial != nullptr) {
        this->serial->write(b);
    }
    return 1;
}

void BroadcastPrint::setWifi(Print *print) {
    this->wifi = print;
}

void BroadcastPrint::setEthernet(Print *print) {
    this->ethernet = print;
}

void BroadcastPrint::setSerial(Print *print) {
    this->serial = print;
}

BroadcastPrint* BroadcastPrint::getInstance() {
    if (singleton == nullptr) {
        singleton = new BroadcastPrint();
    }
    return singleton;
}

BroadcastPrint* BroadcastPrint::singleton;