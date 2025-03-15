#include "GO_Monk.h"
#include "../GameInterrupts/GI.h"

T3DModel* GO_Monk::modelMonk = nullptr;
uint8_t GO_Monk::instanceCount = 0;

GO_Monk::GO_Monk(std::string name, T3DVec3 position, float rotation, color_t color) {
    name_ = name;
    position_ = position;
    rotation_ = rotation;
    color_ = color;

    objectWidth_ = 4;

    t3d_mat4_identity(&monkMat);
    monkMatFP = (T3DMat4FP*)malloc_uncached(sizeof(T3DMat4FP));

    instanceCount++;
    if(!modelMonk) {
        modelMonk = t3d_model_load("rom:/monk.t3dm");
    }
}

GO_Monk::~GO_Monk() {
    free_uncached(monkMatFP);
    instanceCount--;
    if(instanceCount==0) {
        t3d_model_free(modelMonk);
        modelMonk = nullptr;
    }
}

void GO_Monk::handleInput() {
    joypad_buttons_t btn = joypad_get_buttons_pressed(JOYPAD_PORT_1);

    if(btn.a && global::thePlayer->canInteract(position_, objectWidth_)) {
            global::GameInterruptStack->push_back(new GI_Dialog(name_, 
            "Greetings. I am " + name_ + ". By our singing we afford this " +
            "town its sparkflow. The cling of our song begets a shaking " +
            "in the lift besetting us. Above us sit workings which wend " +
            "the shaking into sparkflow and spread it wirelessly throughout " +
            "Stargard."));
    }
}

void GO_Monk::update() {
    t3d_mat4_from_srt_euler(&monkMat,
      (float[3]){0.05f, 0.05f, 0.05f},
      (float[3]){0.0f, rotation_, 0.0f},
      position_.v
    );
    t3d_mat4_to_fixed(monkMatFP, &monkMat);
    GameObject::checkCollision();
}

void GO_Monk::renderT3d() {
    rdpq_sync_pipe();
    rdpq_set_prim_color(color_);
    t3d_matrix_set(monkMatFP, true);
    t3d_model_draw(modelMonk);
}

void GO_Monk::renderRdpq() {

}