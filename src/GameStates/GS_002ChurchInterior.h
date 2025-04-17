#pragma once

#include "GameState.h"


class GS_002ChurchInterior : public GameState {
public:
    GS_002ChurchInterior(T3DVec3 playerStartingPos);
    ~GS_002ChurchInterior();

    virtual void handleInput() override;
    virtual void update() override;
    virtual void renderT3d() override;
    virtual void renderRdpq() override;

    virtual void testFunc() override;

    static struct startPos_t {
        T3DVec3 CHURCH_SQUARE = {{0, 0, 110}};
    } startPos;

private:

    uint8_t colorAmbient[4] = {80, 80, 100, 0xFF};
    //uint8_t colorAmbient[4] = {20, 20, 50, 0xFF};

    T3DViewport viewport;
    T3DMat4FP* envMatFP;
};