//#include "GS_001ChurchSquare.h"
//#include "GS_MainMenu.h"
//#include "GS_002ChurchInterior.h"
//#include "GS_003ShopInterior.h"
#include "../GameObjects/GO.h"
#include "../GameInterrupts/GI.h"
#include "GS.h"

GS_002ChurchInterior::startPos_t GS_002ChurchInterior::startPos;
GS_003ShopInterior::startPos_t GS_003ShopInterior::startPos;

GS_001ChurchSquare::GS_001ChurchSquare(T3DVec3 playerStartingPos) {
    debugf("Creating churchSquare\n");
    viewport = t3d_viewport_create();
    t3d_mat4_identity(&envMat);
    envMatFP = (T3DMat4FP*)malloc_uncached(sizeof(T3DMat4FP));
    envModel = t3d_model_load("rom:/chTwHTx.t3dm");

    t3d_viewport_set_projection(viewport, 75.0f, 20.0f, 200.0f);

    global::thePlayer->position_ = playerStartingPos;

    initCamera();
    global::thePlayer->camState_.closeness = CAM_MID;
    global::thePlayer->camState_.followPlayerRot = false;

    objectList = new GameObjectList();
    objectList->push(new GO_Key((T3DVec3){{0,10,50}}));
    objectList->push(new GO_Flowers((T3DVec3){{-30, 0, 30}}, 0, RGBA32(140, 75, 255, 0xFF)));
    objectList->push(new GO_Flowers((T3DVec3){{-30, 0, 45}}, 2.0f*T3D_PI/3.0f, RGBA32(255, 255, 75, 0xFF)));
    objectList->push(new GO_Flowers((T3DVec3){{-30, 0, 60}}, -0.5f*T3D_PI, RGBA32(255, 75, 75, 0xFF)));
    objectList->push(new GO_Flowers((T3DVec3){{30, 0, 30}}, T3D_PI/8.0f, RGBA32(140, 75, 255, 0xFF)));
    objectList->push(new GO_Flowers((T3DVec3){{30, 0, 45}}, T3D_PI, RGBA32(255, 255, 75, 0xFF)));
    objectList->push(new GO_Flowers((T3DVec3){{30, 0, 60}}, 4.0f*T3D_PI/3.0f, RGBA32(255, 75, 75, 0xFF)));
    objectList->push(new GO_Portal<GS_002ChurchInterior>((T3DVec3){{0,0,0}}, 0, 8.0f, GS_002ChurchInterior::startPos.CHURCH_SQUARE));
    objectList->push(new GO_Door<GS_003ShopInterior>((T3DVec3){{70, 0, 30}}, T3D_PI, 4, GS_003ShopInterior::startPos.CHURCH_SQUARE));

    endTime = get_ticks();
    
    global::GameInterruptStack->push_back(new GI_FadeIn());
}

GS_001ChurchSquare::~GS_001ChurchSquare() {
    t3d_model_free(envModel);
    free_uncached(envMatFP);
    delete objectList;
}

void GS_001ChurchSquare::handleInput() {
    joypad_buttons_t keys = joypad_get_buttons_pressed(JOYPAD_PORT_1);
    //if(keys.a) {
    //    objectList->push(new GO_Flowers(global::thePlayer->position_, ((float)rand()/RAND_MAX)*2.0f*T3D_PI, RGBA32(255*((float)rand()/RAND_MAX), 255*((float)rand()/RAND_MAX), 255*((float)rand()/RAND_MAX), 0xFF)));
    //}
    if(keys.start) {
        global::GameInterruptStack->push_back(new GI_Pause());
    }
    global::thePlayer->handleInput();
    handleInputCamera();

    objectList->handleInput();
}

void GS_001ChurchSquare::update() {
    check_map_collision(&global::thePlayer->position_, mapColl_, MAP_COLL_AABB_COUNT);
    global::thePlayer->update();
    updateCamera();

    t3d_mat4_from_srt_euler(&envMat,
      (float[3]){0.1f, 0.1f, 0.1f},
      (float[3]){0.0f, 0.0f, 0.0f},
      (float[3]){0,0,0}
    );
    t3d_mat4_to_fixed(envMatFP, &envMat);

    objectList->update();
}

void GS_001ChurchSquare::renderT3d() {
    t3d_viewport_set_projection(viewport, camera.FOV, 20.0f, 200.0f);
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

void GS_001ChurchSquare::renderRdpq() {
    objectList->renderRdpq();
    global::thePlayer->renderRdpq();
}

void GS_001ChurchSquare::testFunc() { 
    debugf("Test func churchSquare\n");
}