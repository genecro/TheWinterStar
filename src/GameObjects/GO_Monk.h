#pragma once

#include "GO_NPC.h"

class GO_Monk : public GO_NPC {
public:
    GO_Monk(std::string name, T3DVec3 position, float rotation, color_t color);
    virtual ~GO_Monk();
    virtual void handleInput() override;
    virtual void update() override;
    virtual void renderT3d() override;
    virtual void renderRdpq() override;

private:
    T3DMat4 monkMat;
    T3DMat4FP* monkMatFP;
    static T3DModel *modelMonk;
    //static std::shared_ptr<T3DModel> modelMonk;

    static uint8_t instanceCount;

    color_t color_;
};