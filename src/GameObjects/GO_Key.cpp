#include "GO_Key.h"
#include "../globals.h"
#include "../Misc/Inventory.h"

T3DModel* GO_Key::keyModel = nullptr;
uint8_t GO_Key::instanceCount = 0;

GO_Key::GO_Key(T3DVec3 position) {
    position_ = position;
    objectWidth_ = 2;
    rotation_ = 0;

    t3d_mat4_identity(&keyMat);
    keyMatFP = (T3DMat4FP*)malloc_uncached(sizeof(T3DMat4FP));
    instanceCount++;
    if(!keyModel) {
        keyModel = t3d_model_load("rom:/keyGold.t3dm");
    }
}

GO_Key::~GO_Key() {
    free_uncached(keyMatFP);
    instanceCount--;
    if(instanceCount == 0) {
        t3d_model_free(keyModel);
        keyModel=nullptr;
    }
}

void GO_Key::handleInput() {

}

void GO_Key::update() {
    rotation_ += 0.007;
    if(rotation_ >= 2*T3D_PI) {
        rotation_ -= 2*T3D_PI;
    }

    t3d_mat4_from_srt_euler(&keyMat,
        (float[3]){0.1f, 0.1f, 0.1f},
        (float[3]){T3D_PI/4, rotation_, 0.0f},
        (float[3]){0,10,50}
    );
    t3d_mat4_to_fixed(keyMatFP, &keyMat);

    if(global::thePlayer->isTouching(position_, objectWidth_)) {
        //global::thePlayer->inventory_.keys++;
        global::thePlayer->addItem(KEY_ITEM_ID, 1);
        global::thePlayer->addItem(POTION_ITEM_ID, 94);
        global::gameState->objectList->remove(this);
    }
}

void GO_Key::renderT3d() {
    t3d_matrix_set(keyMatFP, true);
    t3d_model_draw(keyModel);
}

void GO_Key::renderRdpq() {

}

