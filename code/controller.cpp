#include "controller.h"
#include "windows.h"
#include "xinput.h"

bool check_for_conected_controller()
{
    DWORD dwResult;
    for(DWORD i = 0; i < XUSER_MAX_COUNT; i++)
    {
        XINPUT_STATE state;
        ZeroMemory(&state, sizeof(XINPUT_STATE));
        dwResult = XInputGetState(i, &state);
        if(dwResult == ERROR_SUCCESS)
        {
            return true;
        }
    }
    
    return false;
}

void process_xinput(Controller* controller)
{
    DWORD dwResult;
    for(DWORD i = 0; i < XUSER_MAX_COUNT; i++)
    {
        XINPUT_STATE state;
        ZeroMemory(&state, sizeof(XINPUT_STATE));
        dwResult = XInputGetState(i, &state);
        if(dwResult == ERROR_SUCCESS)
        {
            XINPUT_GAMEPAD* pad = &state.Gamepad;
            controller->up = (pad->wButtons & XINPUT_GAMEPAD_DPAD_UP);
            controller->down = (pad->wButtons & XINPUT_GAMEPAD_DPAD_DOWN);
            controller->left = (pad->wButtons & XINPUT_GAMEPAD_DPAD_LEFT);
            controller->right = (pad->wButtons & XINPUT_GAMEPAD_DPAD_RIGHT);
            controller->A = (pad->wButtons & XINPUT_GAMEPAD_A);
            controller->B = (pad->wButtons & XINPUT_GAMEPAD_B);
            controller->X = (pad->wButtons & XINPUT_GAMEPAD_X);
            controller->Y = (pad->wButtons & XINPUT_GAMEPAD_Y);
            controller->thumbLX = pad->sThumbLX;
            controller->thumbLY = pad->sThumbLY;
            controller->thumbRX = pad->sThumbRX;
            controller->thumbRY = pad->sThumbRY; 
        }
    } 
}

