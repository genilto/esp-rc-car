#include "controller/AcelerationController.h"

AcelerationController::AcelerationController()
{
    this->resetState();
}
void AcelerationController::resetState()
{
    this->_aceleration.direction = "S";
    this->_aceleration.power = 0;
}
void AcelerationController::begin()
{
    pinMode(MAIN_MOTOR_POWER, OUTPUT);
    pinMode(MAIN_MOTOR_PIN_FORWARD, OUTPUT);
    pinMode(MAIN_MOTOR_PIN_BACKWARD, OUTPUT);

    this->commandStop();
}
void AcelerationController::handle(AcelerationData aceleration)
{
    this->_aceleration = aceleration;

    if (this->_aceleration.direction == "F") {
        this->commandForward();
        return;
    }
    if (this->_aceleration.direction == "B") {
        this->commandBackward();
        return;
    }
    this->commandStop();
}

void AcelerationController::commandStop()
{
    digitalWrite(MAIN_MOTOR_POWER, LOW);
    digitalWrite(MAIN_MOTOR_PIN_FORWARD, LOW);
    digitalWrite(MAIN_MOTOR_PIN_BACKWARD, LOW);
}
void AcelerationController::commandForward()
{
    digitalWrite(MAIN_MOTOR_POWER, HIGH);
    analogWrite(MAIN_MOTOR_PIN_FORWARD, this->_aceleration.power);
    digitalWrite(MAIN_MOTOR_PIN_BACKWARD, LOW);
}
void AcelerationController::commandBackward()
{
    digitalWrite(MAIN_MOTOR_POWER, HIGH);
    digitalWrite(MAIN_MOTOR_PIN_FORWARD, LOW);
    analogWrite(MAIN_MOTOR_PIN_BACKWARD, this->_aceleration.power);
}
