#pragma once

#include "GameObject.h"

class GO_Key: public GameObject {
public:
    GO_Key(T3DVec3 position);
    GO_Key(std::string name);
    virtual ~GO_Key();
    virtual void handleInput() override;
    virtual void update() override;
    virtual void renderT3d() override;
    virtual void renderRdpq() override;

    T3DVec3 position_;

private:
    T3DMat4 keyMat;
    T3DMat4FP* keyMatFP;
    static T3DModel* keyModel;
    //static std::shared_ptr<T3DModel> keyModel;
    static uint8_t instanceCount;
};