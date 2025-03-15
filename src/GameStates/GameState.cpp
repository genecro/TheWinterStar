#include "GameState.h"
#include "../GameObjects/GO.h"
#include "../GameInterrupts/GI.h"
#include "../globals.h"

#define CAMERA_ROTATE_SPEED 0.05f
#define CAMERA_DISTANCE_SPEED 5.0f
#define CAMERA_HEIGHT_SPEED 5.0f
#define CAMERA_FOV_SPEED 0.05f

GameState::GameState()
{
    //textureMan::clearTextures();
    //modelMan::clearModels();

    //wav64_open(&m_cursorMove, "rom:/cursor)move.wav64");
    //mixer_ch_set_vol(AUDIO_CHANNEL_SFX, 0.50f, 0.50f);
}

GameState::~GameState()
{
    
}

void GameState::initCamera() {
    constants.midHeightFromPlayer = 40;
    constants.midDistanceFromPlayer = 60;
    constants.midFOV = 0.128*T3D_PI;
    constants.closeHeightFromPlayer = 10;
    constants.closeDistanceFromPlayer = 40;
    constants.closeFOV = 0.212*T3D_PI;
    constants.cRotAngle = 0.25*T3D_PI;

    camera.targetDistanceFromPlayer = constants.midDistanceFromPlayer;
    camera.distanceFromPlayer = constants.midDistanceFromPlayer;
    camera.targetHeightFromPlayer = constants.midHeightFromPlayer;
    camera.heightFromPlayer = constants.midHeightFromPlayer;
    camera.targetFOV = constants.midFOV;
    camera.FOV = constants.midFOV;
    camera.rotAngle = 0;
    camera.targetRotAngle = camera.rotAngle;

    endTime = get_ticks();
}

void GameState::handleInputCamera() {
    joypad_buttons_t keys = joypad_get_buttons_pressed(JOYPAD_PORT_1);
    
    if(keys.c_left) {
        if(global::thePlayer->camState_.followPlayerRot) {
            global::thePlayer->camState_.followPlayerRot = false;
            camera.targetRotAngle = round(camera.rotAngle * 4.0 / T3D_PI) * T3D_PI * 0.25;
        }
        camera.targetRotAngle -= constants.cRotAngle;
    }

    if(keys.c_right) {
        if(global::thePlayer->camState_.followPlayerRot) {
            global::thePlayer->camState_.followPlayerRot = false;
            camera.targetRotAngle = round(camera.rotAngle * 4.0 / T3D_PI) * T3D_PI * 0.25;
        }
        camera.targetRotAngle += constants.cRotAngle;
    }

    if(keys.r) {
        global::thePlayer->camState_.followPlayerRot = !global::thePlayer->camState_.followPlayerRot;
        if(!global::thePlayer->camState_.followPlayerRot) {
            camera.targetRotAngle = round(camera.rotAngle * 4.0 / T3D_PI) * T3D_PI * 0.25;
        }
        else {
            camera.rotAngle = -global::thePlayer->rotation_;
        }
    }

    if(keys.c_down && global::thePlayer->camState_.closeness < CAM_MID) {
        global::thePlayer->camState_.closeness++;
        if(global::thePlayer->camState_.closeness==CAM_MID) {
            camera.targetDistanceFromPlayer=constants.midDistanceFromPlayer;
            camera.targetHeightFromPlayer=constants.midHeightFromPlayer;
            camera.targetFOV = constants.midFOV;
        }
    }

    if(keys.c_up && global::thePlayer->camState_.closeness > CAM_CLOSE) {
        global::thePlayer->camState_.closeness--;
        if(global::thePlayer->camState_.closeness==CAM_CLOSE) {
            camera.targetDistanceFromPlayer=constants.closeDistanceFromPlayer;
            camera.targetHeightFromPlayer=constants.closeHeightFromPlayer;
            camera.targetFOV = constants.closeFOV;
        }
    }
}

void GameState::updateCamera() {
    
    if(camera.rotAngle < camera.targetRotAngle) {
        camera.rotAngle += CAMERA_ROTATE_SPEED * global::frameTimeMultiplier;
        if(camera.rotAngle > camera.targetRotAngle) {
            if(camera.targetRotAngle >= 2.0f*T3D_PI) {
                camera.targetRotAngle -= 2.0f*T3D_PI;
            }
            camera.rotAngle = camera.targetRotAngle;
        }
    }
    else if(camera.rotAngle > camera.targetRotAngle) {
        camera.rotAngle -= CAMERA_ROTATE_SPEED * global::frameTimeMultiplier;
        if(camera.rotAngle < camera.targetRotAngle) {
            if(camera.targetRotAngle < 0) {
                camera.targetRotAngle += 2.0f*T3D_PI;
            }
            camera.rotAngle = camera.targetRotAngle;
        }
    }

    if(camera.distanceFromPlayer < camera.targetDistanceFromPlayer) {
        camera.distanceFromPlayer += CAMERA_DISTANCE_SPEED * global::frameTimeMultiplier;
        if(camera.distanceFromPlayer > camera.targetDistanceFromPlayer) {
            camera.distanceFromPlayer = camera.targetDistanceFromPlayer;
        }
    }

    else if(camera.distanceFromPlayer > camera.targetDistanceFromPlayer) {
        camera.distanceFromPlayer -= CAMERA_DISTANCE_SPEED * global::frameTimeMultiplier;
        if(camera.distanceFromPlayer < camera.targetDistanceFromPlayer) {
            camera.distanceFromPlayer = camera.targetDistanceFromPlayer;
        }
    }

    if(camera.heightFromPlayer < camera.targetHeightFromPlayer) {
        camera.heightFromPlayer += CAMERA_HEIGHT_SPEED * global::frameTimeMultiplier;
        if(camera.heightFromPlayer > camera.targetHeightFromPlayer) {
            camera.heightFromPlayer = camera.targetHeightFromPlayer;
        }
    }

    else if(camera.heightFromPlayer > camera.targetHeightFromPlayer) {
        camera.heightFromPlayer -= CAMERA_HEIGHT_SPEED * global::frameTimeMultiplier;
        if(camera.heightFromPlayer < camera.targetHeightFromPlayer) {
            camera.heightFromPlayer = camera.targetHeightFromPlayer;
        }
    }

    if(camera.FOV < camera.targetFOV) {
        camera.FOV += CAMERA_FOV_SPEED * global::frameTimeMultiplier;
        if(camera.FOV > camera.targetFOV) {
            camera.FOV = camera.targetFOV;
        }
    }

    else if(camera.FOV > camera.targetFOV) {
        camera.FOV -= CAMERA_FOV_SPEED * global::frameTimeMultiplier;
        if(camera.FOV < camera.targetFOV) {
            camera.FOV = camera.targetFOV;
        }
    }

    if(global::thePlayer->camState_.followPlayerRot){
        camera.rotAngle = -global::thePlayer->rotation_ - 0.5*T3D_PI;
    }
    
    camera.pos = {{
        global::thePlayer->position_.x + camera.distanceFromPlayer * fm_sinf(camera.rotAngle),
        global::thePlayer->position_.y + camera.heightFromPlayer,
        global::thePlayer->position_.z + camera.distanceFromPlayer * fm_cosf(camera.rotAngle)
    }};

    camera.target = global::thePlayer->position_;

    if(global::thePlayer->camState_.closeness==CAM_MID) {
        camera.target += {{0,5,0}};
    }

    if(global::thePlayer->camState_.closeness==CAM_CLOSE) {
        camera.target += {{0,10,0}};
    }
}