#include <Arduino.h>
#include "debug.h"
#include "AcelerationData.h"

#ifndef AcelerationController_h
#define AcelerationController_h

// Sensores de aceleração
#define SD2 9
#define SD3 10

#define MAIN_MOTOR_POWER D4
#define MAIN_MOTOR_PIN_FORWARD SD3
#define MAIN_MOTOR_PIN_BACKWARD SD2

class AcelerationController {
    private:
        AcelerationData _aceleration;

        void resetState();
        void commandStop();
        void commandForward();
        void commandBackward();
    public:
        AcelerationController();
        void begin();
        void handle(AcelerationData aceleration);
};

#endif
