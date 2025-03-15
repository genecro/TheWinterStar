#pragma once

#include <cmath>
#include <libdragon.h>
#include <t3d/t3d.h>
#include <t3d/t3dmath.h>
#include <t3d/t3dmodel.h>
#include <t3d/t3ddebug.h>
#include "GameState.h"
#include "../globals.h"
#include "../collision.h"

#define AABB_BORDER 2
#define MAP_COLL_AABB_COUNT 8

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
    //T3DVec3 playerStartingPos_;
    
    uint8_t colorAmbient[4] = {80, 80, 100, 0xFF};

    uint64_t endTime;
    
    T3DViewport viewport;
    T3DMat4 envMat;
    T3DMat4FP* envMatFP;
    T3DModel* envModel;

    rspq_block_t * dplDrawEnv;

    const AABB mapColl_[MAP_COLL_AABB_COUNT] = {
        {{-70-AABB_BORDER, 90-AABB_BORDER}, {70+AABB_BORDER, 90+AABB_BORDER}}, //south wall
        {{-69.5-AABB_BORDER, -5-AABB_BORDER}, {-69.5+AABB_BORDER, 90+AABB_BORDER}}, //west wall
        {{-70-AABB_BORDER, -5-AABB_BORDER}, {-7.5+AABB_BORDER, -5+AABB_BORDER}}, //northwest wall
        {{-7.5-AABB_BORDER, -5-AABB_BORDER}, {7.5+AABB_BORDER, -5+AABB_BORDER}}, //church entrance
        {{7.5-AABB_BORDER, -5-AABB_BORDER}, {70+AABB_BORDER, -5+AABB_BORDER}}, //northeast wall
        {{69.5-AABB_BORDER, -5-AABB_BORDER}, {69.5+AABB_BORDER, 90+AABB_BORDER}}, //east wall
        {{-19.5-AABB_BORDER, -6-AABB_BORDER}, {-6+AABB_BORDER, 8+AABB_BORDER}}, //west tower
        {{6-AABB_BORDER, -6-AABB_BORDER}, {19.5+AABB_BORDER, 8+AABB_BORDER}} //east tower
    };

    
};