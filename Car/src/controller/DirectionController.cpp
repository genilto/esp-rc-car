#include "controller/DirectionController.h"

DirectionController::DirectionController()
{
    this->resetState();
}
void DirectionController::resetState()
{
    this->_center = false;
    this->_totalRight = false;
    this->_partialRight = false;
    this->_totalLeft = false;
    this->_partialLeft = false;
}
void DirectionController::begin()
{
    pinMode(BROWN, OUTPUT);
    pinMode(WHITE, OUTPUT);
    pinMode(YELLOW, INPUT_PULLUP);
    pinMode(BLUE, INPUT_PULLUP);

    pinMode(DIRECTION_MOTOR_PIN_RIGHT, OUTPUT);
    pinMode(DIRECTION_MOTOR_PIN_LEFT, OUTPUT);

    digitalWrite(BROWN, HIGH);
    digitalWrite(WHITE, HIGH);

    this->commandStop();
}
void DirectionController::handle(String direction)
{
    this->readDirectionState();
    this->adjustDirection(direction);
}

void DirectionController::readDirectionState()
{
    this->resetState();

    // Liga o marrom
    digitalWrite(BROWN, LOW);
    digitalWrite(WHITE, HIGH);

    // Checa se está no fim de curso
    this->_totalRight = digitalRead(BLUE) == LOW;
    if (this->_totalRight) {
        DebugPrintln("Está todo para DIREITA");
        return;
    }

    this->_totalLeft = digitalRead(YELLOW) == LOW;
    if (this->_totalLeft) {
        DebugPrintln("Está todo para ESQUERDA");
        return;
    }

    // Liga o Branco
    digitalWrite(BROWN, HIGH);
    digitalWrite(WHITE, LOW);

    // Checa se está um pouco para direita ou para esquerda
    this->_partialRight = digitalRead(YELLOW) == LOW;
    if (this->_partialRight) {
        return;
        DebugPrintln("Está um pouco para DIREITA");
    }

    this->_partialLeft = digitalRead(BLUE) == LOW;
    if (this->_partialLeft) {
        DebugPrintln("Está um pouco para ESQUERDA");
        return;
    }

    this->_center = true;
    DebugPrintln("Está NO CENTRO");
}

void DirectionController::commandStop()
{
    digitalWrite(DIRECTION_MOTOR_PIN_RIGHT, LOW);
    digitalWrite(DIRECTION_MOTOR_PIN_LEFT, LOW);
}
void DirectionController::commandRight()
{
    analogWrite(DIRECTION_MOTOR_PIN_RIGHT, DIRECTION_MOTOR_POWER);
    digitalWrite(DIRECTION_MOTOR_PIN_LEFT, LOW);
}
void DirectionController::commandLeft()
{
    digitalWrite(DIRECTION_MOTOR_PIN_RIGHT, LOW);
    analogWrite(DIRECTION_MOTOR_PIN_LEFT, DIRECTION_MOTOR_POWER);
}
void DirectionController::adjustDirection(String direction)
{
    if (direction.equals("R")) {
        if (!this->_totalRight) {
            this->commandRight();
            return;
        }
    }
    if (direction.equals("L")) {
        if (!this->_totalLeft) {
            this->commandLeft();
            return;
        }
    }
    if (direction.equals("C")) {
        if (this->_totalRight || this->_partialRight) {
            this->commandLeft();
            return;
        }
        if (this->_totalLeft || this->_partialLeft) {
            this->commandRight();
            return;
        }
    }
    this->commandStop();
}
