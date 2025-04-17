#include "GS.h"
#include "../GameObjects/GO.h"
#include "../GameInterrupts/GI.h"
#include "../globals.h"
#include "../collision.h"

GS_004ForestA::startPos_t GS_004ForestA::startPos;

GS_004ForestA::GS_004ForestA(T3DVec3 startingPlayerPos) {
    viewport = t3d_viewport_create();
    t3d_mat4_identity(&envMat);
    envMatFP = (T3DMat4FP*)malloc_uncached(sizeof(T3DMat4FP));
    envModel = t3d_model_load("rom:/forestA.t3dm");
    collisionTris = collision::loadCollTriangles("rom:/forestA.bin");

    envScale = 0.25f;
    collision::scaleTriangles(&collisionTris, envScale);

    t3d_viewport_set_projection(viewport, 75.0f, 20.0f, 200.0f);

    global::thePlayer->position_ = startingPlayerPos + (T3DVec3){{0, global::thePlayer->objectWidth_, 0}};

    initCamera();

    objectList = new GameObjectList();
    //objectList->push(new GO_Door<GS_001ChurchSquare>((T3DVec3){{-9,0,-17}}, 0.5f*T3D_PI, 4, GS_001ChurchSquare::startPos.SHOP_INTERIOR));
}

GS_004ForestA::~GS_004ForestA() {
    t3d_model_free(envModel);
    free_uncached(envMatFP);
    delete objectList;
}

void GS_004ForestA::handleInput() {
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

void GS_004ForestA::update() {
    global::thePlayer->update();
    updateCamera();

    

    t3d_mat4_from_srt_euler(&envMat,
        (float[3]){ envScale, envScale, envScale},
        (float[3]){0.0f, 0.0f, 0.0f},
        (float[3]){0,0,0}
    );
    t3d_mat4_to_fixed(envMatFP, &envMat);

    objectList->update();
}

void GS_004ForestA::renderT3d() {
    t3d_viewport_set_projection(viewport, camera.FOV, 10.0f, 200.0f);
    t3d_viewport_attach(&viewport);

    rdpq_mode_fog(RDPQ_FOG_STANDARD);
    rdpq_set_fog_color((color_t){20, 20, 50, 0xFF});

    t3d_screen_clear_color(RGBA32(160, 110, 200, 0xFF));
    t3d_screen_clear_depth();

    t3d_fog_set_enabled(true);
    t3d_fog_set_range(50.0f, 125.0f);

    t3d_light_set_ambient(colorAmbient);

    t3d_viewport_look_at(viewport, camera.pos, camera.target, {{0, 1, 0}});

    t3d_matrix_push_pos(1);
    objectList->renderT3d();
    global::thePlayer->renderT3d();
    t3d_matrix_set(envMatFP, true);
    t3d_model_draw(envModel);
    t3d_matrix_pop(1);
}

void GS_004ForestA::renderRdpq() {
    objectList->renderRdpq();
    global::thePlayer->renderRdpq();
}

void GS_004ForestA::testFunc() {
    debugf("Test func forestA\n");
}