#include "GS.h"
#include "../GameObjects/GO.h"
#include "../GameInterrupts/GI.h"
#include "../globals.h"

GS_001ChurchSquare::startPos_t GS_001ChurchSquare::startPos;

GS_003ShopInterior::GS_003ShopInterior(T3DVec3 startingPlayerPos) {
    viewport = t3d_viewport_create();
    t3d_mat4_identity(&envMat);
    envMatFP = (T3DMat4FP*)malloc_uncached(sizeof(T3DMat4FP));
    envModel = t3d_model_load("rom:/shopInterior.t3dm");

    t3d_viewport_set_projection(viewport, 75.0f, 20.0f, 200.0f);

    global::thePlayer->position_ = startingPlayerPos;

    initCamera();

    objectList = new GameObjectList();
    objectList->push(new GO_Door<GS_001ChurchSquare>((T3DVec3){{-9,0,-17}}, 0.5f*T3D_PI, 4, GS_001ChurchSquare::startPos.SHOP_INTERIOR));
}

GS_003ShopInterior::~GS_003ShopInterior() {
    t3d_model_free(envModel);
    free_uncached(envMatFP);
    delete objectList;
}

void GS_003ShopInterior::handleInput() {
    joypad_buttons_t keys = joypad_get_buttons_pressed(JOYPAD_PORT_1);
    if(keys.start) {
        global::GameInterruptStack->push_back(new GI_Pause());
    }

    if(keys.b) {
        
    }

    global::thePlayer->handleInput();
    handleInputCamera();
    objectList->handleInput();
}

void GS_003ShopInterior::update() {
    global::thePlayer->update();
    updateCamera();

    t3d_mat4_from_srt_euler(&envMat,
        (float[3]){ 0.05f, 0.05f, 0.05f},
        (float[3]){0.0f, 0.0f, 0.0f},
        (float[3]){0,0,0}
    );
    t3d_mat4_to_fixed(envMatFP, &envMat);

    objectList->update();
}

void GS_003ShopInterior::renderT3d() {
    t3d_viewport_set_projection(viewport, camera.FOV, 10.0f, 200.0f);
    t3d_viewport_attach(&viewport);

    t3d_screen_clear_color(RGBA32(160, 110, 200, 0xFF));
    t3d_screen_clear_depth();

    t3d_light_set_ambient(colorAmbient);

    t3d_viewport_look_at(viewport, camera.pos, camera.target, {{0, 1, 0}});

    t3d_matrix_push_pos(1);
    objectList->renderT3d();
    global::thePlayer->renderT3d();
    t3d_matrix_set(envMatFP, true);
    t3d_model_draw(envModel);
    t3d_matrix_pop(1);
}

void GS_003ShopInterior::renderRdpq() {
    objectList->renderRdpq();
    global::thePlayer->renderRdpq();
}

void GS_003ShopInterior::testFunc() {
    debugf("Test func shopInterior\n");
}


void GS_003ShopInterior::initCamera() {
    camera.FOV = 0.27*T3D_PI;
    camera.pos = (T3DVec3){{-15, 10, 15}};
    camera.target = global::thePlayer->position_;
}

void GS_003ShopInterior::handleInputCamera() {
    
}

void GS_003ShopInterior::updateCamera() {
    camera.target = global::thePlayer->position_;
    camera.target += {{0,2,0}};
    if(camera.target.z>9.0f) {
        camera.target.z = 9.0f;
    }
    if(camera.target.x<-8.5f) {
        camera.target.x = -8.5f;
    }
}