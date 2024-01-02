#include <Arduino.h>
#include "debug.h"

#ifndef DirectionController_h
#define DirectionController_h

// Sensores de direção
#define BROWN D8
#define WHITE D7
#define YELLOW D6
#define BLUE D5

#define DIRECTION_MOTOR_PIN_LEFT D1
#define DIRECTION_MOTOR_PIN_RIGHT D2
#define DIRECTION_MOTOR_POWER 200

class DirectionController {
    private:
        bool _center = false;
        bool _totalRight = false;
        bool _partialRight = false;
        bool _totalLeft = false;
        bool _partialLeft = false;

        void resetState();
        void readDirectionState();
        void adjustDirection(String direction);
        void commandStop();
        void commandRight();
        void commandLeft();
    public:
        DirectionController();
        void begin();
        void handle(String direction);
};

#endif
