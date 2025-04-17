#pragma once

#include <cmath>
#include <libdragon.h>
#include <t3d/t3d.h>
#include <t3d/t3dmath.h>
#include <t3d/t3dmodel.h>
#include <t3d/t3ddebug.h>
#include "GameState.h"

class GS_001ChurchSquare : public GameState {
public:
    GS_001ChurchSquare(T3DVec3 playerStartingPos);
    ~GS_001ChurchSquare();

    virtual void handleInput() override;
    virtual void update() override;
    virtual void renderT3d() override;
    virtual void renderRdpq() override;

    virtual void testFunc() override;

    static struct startPos_t {
        T3DVec3 CHURCH_INTERIOR = {{0, 0, 10}};
        T3DVec3 SHOP_INTERIOR = {{65, 0, 30}};
    } startPos;

private:
    uint8_t colorAmbient[4] = {80, 80, 100, 0xFF};

    uint64_t endTime;
    
    T3DViewport viewport;
    
    T3DMat4FP* envMatFP;

    rspq_block_t * dplDrawEnv;
};