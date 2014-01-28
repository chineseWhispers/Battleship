/**********************************
 * Author: Clifton Thomas
 * Date: 3/28/13
 * Institution: Georgia Tech
 *
 * Title: Communication Module
 * Class: ECE2035
 * Assignment: Project 2
 **********************************/

#ifndef COMM_H
#define COMM_H

#include <string.h>
#include "mbed.h"

class commSerial: public Serial {
public:
    //constructor
    commSerial(PinName tx, PinName rx, int baudrate): Serial(tx,rx) {
        Serial::baud(baudrate);
    }
    
    //fcn to send data
    void sendData(char *str) {
        Serial::printf(str);
    }
    
    //fcn to receive data
    void receiveData(char *buffer) {
        Serial::scanf("%s", buffer);    
    }
};

#endif