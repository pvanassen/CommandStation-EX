//
// Created by paul on 24-1-2021.
//

#ifndef COMMANDSTATION_EX_BROADCASTPRINT_H
#define COMMANDSTATION_EX_BROADCASTPRINT_H

#include <Arduino.h>


class BroadcastPrint: public Print {
private:
    Print* wifi = nullptr;
    Print* ethernet = nullptr;
    Print* serial = nullptr;
    static BroadcastPrint *singleton;
public:
    virtual size_t write(uint8_t b);
    void setWifi(Print *print);
    void setEthernet(Print *print);
    void setSerial(Print *print);
    static BroadcastPrint* getInstance();
};


#endif //COMMANDSTATION_EX_BROADCASTPRINT_H
