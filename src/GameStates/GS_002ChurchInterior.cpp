//#include "GS_002ChurchInterior.h"
//#include "GS_001ChurchSquare.h"
#include "GS.h"
#include "../GameObjects/GO.h"
#include "../GameInterrupts/GI.h"
#include "../globals.h"

GS_002ChurchInterior::GS_002ChurchInterior(T3DVec3 playerStartingPos) {
    debugf("Creating churchInterior\n");
    viewport = t3d_viewport_create();
    t3d_mat4_identity(&envMat);
    envMatFP = (T3DMat4FP*)malloc_uncached(sizeof(T3DMat4FP));
    envModel = t3d_model_load("rom:/churchInterior.t3dm");
    //envModel = t3d_model_load("rom:/churchInteriorFixed.t3dm");
    //envModel = t3d_model_load("rom:/churchInteriorCull.t3dm");

    /*
    T3DModelIter it = t3d_model_iter_create(envModel, T3D_CHUNK_TYPE_OBJECT);
    while(t3d_model_iter_next(&it)) {
        rspq_block_begin();
        t3d_model_draw_object(it.object, NULL);
        it.object->userBlock = rspq_block_end();
    }
    */

    t3d_viewport_set_projection(viewport, 75.0f, 20.0f, 200.0f);

    global::thePlayer->position_ = playerStartingPos;

    initCamera();
    global::thePlayer->camState_.closeness = CAM_MID;
    global::thePlayer->camState_.followPlayerRot = false;
    constants.closeHeightFromPlayer = 5;

    objectList = new GameObjectList();
    objectList->push(new GO_Monk("Red Monk", {{0,0,-167.0f}}, 0.0f, RGBA32(0xFF, 0, 0, 0xFF)));
    objectList->push(new GO_Monk("Black Monk", {{12,0,-162.0f}}, T3D_PI*0.25, RGBA32(0, 0, 0, 0xFF)));
    objectList->push(new GO_Monk("White Monk", {{-12,0,-162.0f}}, -T3D_PI*0.25, RGBA32(0xFF, 0xFF, 0xFF, 0xFF)));
    objectList->push(new GO_Portal<GS_001ChurchSquare>((T3DVec3){{0,0,130}}, 0, 24, GS_001ChurchSquare::startPos.CHURCH_INTERIOR));

    global::GameInterruptStack->push_back(new GI_FadeIn());
}

GS_002ChurchInterior::~GS_002ChurchInterior() {
    t3d_model_free(envModel);
    free_uncached(envMatFP);
    delete objectList;
}

void GS_002ChurchInterior::handleInput() {
    joypad_buttons_t keys = joypad_get_buttons_pressed(JOYPAD_PORT_1);
    if(keys.start) {
        global::GameInterruptStack->push_back(new GI_Pause());
    }
    global::thePlayer->handleInput();

    objectList->handleInput();
    handleInputCamera();
}

void GS_002ChurchInterior::update() {
    global::thePlayer->update();

    t3d_mat4_from_srt_euler(&envMat,
      (float[3]){0.1f, 0.1f, 0.1f},
      (float[3]){0.0f, 0.0f, 0.0f},
      (float[3]){0,0,0}
    );
    t3d_mat4_to_fixed(envMatFP, &envMat);

    objectList->update();
    updateCamera();

    /*
    T3DFrustum frustum = viewport.viewFrustum;
    t3d_frustum_scale(&frustum, 0.1f);

    envVisible = t3d_frustum_vs_aabb_s16(&frustum, envModel->aabbMin, envModel->aabbMax);

    if(envVisible) {
        const T3DBvh *bvh = t3d_model_bvh_get(envModel);
        if(bvh) {
            t3d_model_bvh_query_frustum(bvh, &frustum);
        }
        else {
            T3DModelIter it = t3d_model_iter_create(envModel, T3D_CHUNK_TYPE_OBJECT);
            while(t3d_model_iter_next(&it)) {
                it.object->isVisible = t3d_frustum_vs_aabb_s16(&frustum, it.object->aabbMin, it.object->aabbMax);
            }
        }
    }
    */
}

void GS_002ChurchInterior::renderT3d() {
    t3d_viewport_set_projection(viewport, camera.FOV, 20.0f, 200.0f);
    
    t3d_viewport_attach(&viewport);
    
    //rdpq_sync_pipe();
    rdpq_mode_fog(RDPQ_FOG_STANDARD);
    rdpq_set_fog_color((color_t){20, 20, 50, 0xFF});
    
    //t3d_screen_clear_color(RGBA32(160, 110, 200, 0xFF));
    t3d_screen_clear_color(RGBA32(110, 110, 110, 0xFF));
    t3d_screen_clear_depth();

    //rdpq_sync_pipe();
    t3d_fog_set_enabled(true);
    t3d_fog_set_range(50.0f, 125.0f);

    t3d_light_set_ambient(colorAmbient);

    t3d_viewport_look_at(viewport, camera.pos, camera.target, {{0, 1, 0}});
    /*
    totalObjects = 0;
    visibleObjects = 0;
    triCount = 0;

    t3d_matrix_push(envMatFP);
    //t3d_matrix_push_pos(1);

    if(envVisible) {
        T3DModelState state = t3d_model_state_create();
        T3DModelIter it = t3d_model_iter_create(envModel, T3D_CHUNK_TYPE_OBJECT);
        while(t3d_model_iter_next(&it)) {
            if(it.object->isVisible) {
                t3d_model_draw_material(it.object->material, &state);
                rspq_block_run(it.object->userBlock);
                it.object->isVisible = false;
                ++visibleObjects;
                triCount += it.object->triCount;
            }
            ++totalObjects;
        }
    }

    t3d_matrix_pop(1);
    */
    t3d_matrix_push_pos(1);
    
    objectList->renderT3d();
    
    global::thePlayer->renderT3d();
    t3d_matrix_set(envMatFP, true);
    rdpq_sync_pipe();
    t3d_model_draw(envModel);
    t3d_matrix_pop(1);
}

void GS_002ChurchInterior::renderRdpq() {
    objectList->renderRdpq();
    global::thePlayer->renderRdpq();
    //rdpq_text_printf(&(rdpq_textparms_t){}, FONT_FREE_12, display_get_width()-110, 73, "Total objs: %d", totalObjects);
    //rdpq_text_printf(&(rdpq_textparms_t){}, FONT_FREE_12, display_get_width()-110, 85, "Vis objs: %d", visibleObjects);
    //rdpq_text_printf(&(rdpq_textparms_t){}, FONT_FREE_12, display_get_width()-110, 97, "Tricount: %d", triCount);
}

void GS_002ChurchInterior::testFunc() {
    debugf("Test func churchInterior\n");
}