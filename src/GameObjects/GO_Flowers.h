#pragma once

#include "GameObject.h"

class GO_Flowers: public GameObject {
public:
    GO_Flowers(T3DVec3 position, float rotation, color_t color, std::string name="Flower");
    GO_Flowers(std::string name);
    virtual ~GO_Flowers();
    virtual void handleInput() override;
    virtual void update() override;
    virtual void renderT3d() override;
    virtual void renderRdpq() override;

    T3DVec3 position_;

private:
    T3DMat4 flowerMat;
    T3DMat4FP* flowerMatFP;
    static T3DModel* flowerModel;
    //static std::shared_ptr<T3DModel> keyModel;
    static uint8_t instanceCount;

    color_t color_;
};