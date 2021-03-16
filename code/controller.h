#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "stdint.h"

struct Controller
{
    // temporal
    bool x_was_press = false;

    bool up;
    bool down;
    bool left;
    bool right;
    bool A;
    bool B;
    bool X;
    bool Y; 
    int16_t thumbLX;
    int16_t thumbRX;
    int16_t thumbLY;
    int16_t thumbRY;   
};

bool check_for_conected_controller();
void process_xinput(Controller* controller);

#endif
