#pragma once
#include <libdragon.h>
#include <vector>
#include <map>
#include "GameInterrupts/GameInterrupt.h"
#include "GameObjects/GO_Player.h"
#include "GameStates/GameState.h"


#define INTERACTION_ANGLE T3D_PI/6.0f

namespace global
{
    extern float elapsedSeconds;
    extern std::vector<GameInterrupt*>* GameInterruptStack;
    extern GO_Player* thePlayer;
    extern GameState* gameState;
    extern float usPerFrame;
    const extern float US_60FPS;
    extern float frameTimeMultiplier;
    extern surface_t* disp;
    //extern joypad_inputs_t input;
}