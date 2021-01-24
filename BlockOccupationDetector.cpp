
#include "BlockOccupationDetector.h"

BlockOccupationDetector::BlockOccupationDetector(byte pinA, byte pinB, byte pinC, byte pinRead,
                                                 byte detectorEnablePins, byte detectorEnablePin[]) {
    selectPinArray[0] = pinA;
    selectPinArray[1] = pinB;
    selectPinArray[2] = pinC;
    this->pinRead = pinRead;
    pinMode(pinA, OUTPUT);
    pinMode(pinB, OUTPUT);
    pinMode(pinC, OUTPUT);
    pinMode(pinRead, INPUT);
    setDetectors(detectorEnablePins, detectorEnablePin);
}

BlockOccupationDetector::BlockOccupationDetector(byte pinA, byte pinB, byte pinC, byte pinRead,
                                                 byte detectorEnablePins, byte detectorEnablePin[],
                                                 void (*blockOccupied)(byte), void (*blockReleased)(byte)):
        BlockOccupationDetector(pinA, pinB, pinC, pinRead, detectorEnablePins, detectorEnablePin) {
    this->blockOccupied = blockOccupied;
    this->blockReleased = blockReleased;
}

void BlockOccupationDetector::setDetectors(byte detectorEnablePins, byte detectorEnablePin[]) {
    byte detectorEnablePinSize = detectorEnablePins;

    detectors = detectorEnablePinSize;

    pinEnable = new byte[detectors];
    lastState = new byte[detectors];
    lastStateChange = (unsigned long**) new unsigned long[detectors][8];
    blockStates = new byte[detectors];
    states = new bool[(detectors * 8)];

    for (int nDetector = 0; nDetector != detectors; nDetector++) {
        pinEnable[nDetector] = detectorEnablePin[nDetector];
        pinMode(detectorEnablePin[nDetector], OUTPUT);
        digitalWrite(detectorEnablePin[nDetector], HIGH);
        for (int i=0;i!=8;i++) {
            bitWrite(lastState[nDetector], i, false);
            lastStateChange[nDetector][i] = 0;
            bitWrite(blockStates[nDetector], i, false);
        }
    }
}

void BlockOccupationDetector::tick() {
    now = millis();
    if (++block == 8) {
        block = 0;
    }
    if (++detector == detectors) {
        detector = 0;
    }

    for (int pin=0; pin<3; pin++) {
        digitalWrite(selectPinArray[pin], block & (1<<pin) ? HIGH : LOW);
    }
    digitalWrite(pinEnable[detector], LOW);

    lastBlock.changed = false;

    int value = 0;
    for (int sample=0;sample!=samples;sample++) {
        value += analogRead(this->pinRead);
    }

    bool occupied = (value / samples) < 600;
    if (bitRead(lastState[detector], block) != occupied) {
        lastStateChange[detector][block] = now;
        bitWrite(lastState[detector], block,  occupied);
    }
    else if ((now - lastStateChange[detector][block]) > debounceDelay) {
        if (bitRead(blockStates[detector], block) != occupied) {
            bitWrite(blockStates[detector], block, occupied);

            int absoluteBlock = (detector * 8) + block;

            lastBlock.changed = true;
            lastBlock.absoluteBlock = absoluteBlock;
            lastBlock.occupied = occupied;

            if (blockOccupied != NULL && blockReleased != NULL) {
                if (occupied) {
                    blockOccupied(absoluteBlock);
                } else {
                    blockReleased(absoluteBlock);
                }
            }
        }
    }

    digitalWrite(pinEnable[detector], HIGH);
}

byte BlockOccupationDetector::firstAvailableSensor() {
    return (detectors * 8);
}

bool* BlockOccupationDetector::getLastKnownStates() {
    byte cnt = 0;
    for (int nDetector=0;nDetector != detectors; nDetector++) {
        for (int nBlock = 0; nBlock != 8; nBlock++) {
            states[cnt++] = bitRead(blockStates[nDetector], nBlock);
        }
    }

    return states;
}