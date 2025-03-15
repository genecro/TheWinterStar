#pragma once

#include "GameState.h"

class GS_003ShopInterior : public GameState {
public:
    GS_003ShopInterior(T3DVec3 playerStartingPos);
    ~GS_003ShopInterior();

    virtual void handleInput() override;
    virtual void update() override;
    virtual void renderT3d() override;
    virtual void renderRdpq() override;

    virtual void testFunc() override;

    static struct startPos_t {
        T3DVec3 CHURCH_SQUARE = {{-9,0,-12}};
    } startPos;

private:
    uint8_t colorAmbient[4] = {80, 80, 100, 0xFF};

    T3DViewport viewport;
    T3DMat4 envMat;
    T3DMat4FP* envMatFP;
    T3DModel* envModel;

    void initCamera() override;
    void handleInputCamera() override;
    void updateCamera() override;
};