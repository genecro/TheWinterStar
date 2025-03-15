#include "GO_Flowers.h"
#include "../globals.h"

T3DModel* GO_Flowers::flowerModel = nullptr;
uint8_t GO_Flowers::instanceCount = 0;

GO_Flowers::GO_Flowers(T3DVec3 position, float rotation, color_t color, std::string name) {
    position_ = position;
    rotation_ = rotation;
    color_ = color;
    name_=name;

    t3d_mat4_identity(&flowerMat);
    flowerMatFP = (T3DMat4FP*)malloc_uncached(sizeof(T3DMat4FP));
    instanceCount++;
    if(!flowerModel) {
        flowerModel = t3d_model_load("rom:/flower.t3dm");
    }
}

GO_Flowers::~GO_Flowers() {
    free_uncached(flowerMatFP);
    instanceCount--;
    if(instanceCount == 0) {
        t3d_model_free(flowerModel);
        flowerModel=nullptr;
    }
}

void GO_Flowers::handleInput() {

}

void GO_Flowers::update() {
    t3d_mat4_from_srt_euler(&flowerMat,
        (float[3]){0.5f, 0.5f, 0.5f},
        (float[3]){0.0f, rotation_, 0.0f},
        position_.v
    );
    t3d_mat4_to_fixed(flowerMatFP, &flowerMat);
}

void GO_Flowers::renderT3d() {
    rdpq_set_prim_color(color_);
    t3d_matrix_set(flowerMatFP, true);
    t3d_model_draw(flowerModel);
}

void GO_Flowers::renderRdpq() {

}

