#pragma once

#include <libdragon.h>
#include <vector>
#include "../collision.h"
#include "../GameObjects/GameObjectList.h"

class GameState
{
public:
    GameState();
    virtual ~GameState();
    virtual void handleInput() = 0;
    virtual void update() = 0;
    virtual void renderT3d() = 0;
    virtual void renderRdpq() = 0;
    GameState* nextState = nullptr;
    bool pausable = true;

    virtual void testFunc() = 0;

    std::vector<AABB> mapColl_;

    //std::vector<GameObject*>* objectList;
    GameObjectList* objectList;
    

protected:
    static const int AUDIO_CHANNEL_SFX = 0;
    static const int AUDIO_CHANNEL_MUSIC = 1;

    uint64_t endTime;

    struct camera_t {
        T3DVec3 pos;
        T3DVec3 target;
        float targetDistanceFromPlayer;
        float distanceFromPlayer;
        float targetHeightFromPlayer;
        float heightFromPlayer;
        float rotAngle;
        float targetRotAngle;
        float targetFOV;
        float FOV;
    } camera;

    struct constants_t {
        float midHeightFromPlayer = 30;
        float midDistanceFromPlayer = 40;
        float midFOV = 0.128*T3D_PI;
        float closeHeightFromPlayer = 5;
        float closeDistanceFromPlayer = 30;
        float closeFOV = 0.212*T3D_PI;
        float cRotAngle = 0.25*T3D_PI;
    } constants; 

    virtual void initCamera();
    virtual void updateCamera();
    virtual void handleInputCamera();

    //bool envVisible;
    //int totalObjects;
    //int visibleObjects;
    //int triCount;
};