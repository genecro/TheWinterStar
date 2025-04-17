#include "GI_Reckoner.h"

enum {

};

GI_Reckoner::GI_Reckoner() {
    pauseStr = "Paused";
    pauseInterrupt = true;

    bgSprite = sprite_load("rom:/reckonerBG.sprite");
}

GI_Reckoner::~GI_Reckoner() {

}

void GI_Reckoner::handleInput() {
    joypad_buttons_t keys = joypad_get_buttons_pressed(JOYPAD_PORT_1);

    if(keys.start) {
        timeToDestroy = true;
    }
}

void GI_Reckoner::update() {

}

void GI_Reckoner::renderT3d() {

}

void GI_Reckoner::renderRdpq() {
    rdpq_sync_pipe();
    //debugf("setting mode\n");
    rdpq_set_mode_standard();
    //debugf("setting alpha compare\n");
    //rdpq_mode_alphacompare(1);
    //debugf("setting mode blender\n");
    //rdpq_mode_combiner(RDPQ_COMBINER_FLAT);
    rdpq_mode_blender(RDPQ_BLENDER_MULTIPLY);
    //debugf("blitting sprite\n");
    rdpq_sprite_blit(bgSprite, display_get_width()/2, display_get_height()/2, &(rdpq_blitparms_t){
        .cx = bgSprite->width/2.0,
        .cy = bgSprite->height/2.0,
    });
    //debugf("finished blitting sprite\n");

    rdpq_text_printf(&(rdpq_textparms_t) {
        .style_id=FONTSTYLE_ORANGE,
    }, FONT_PIXELFRAKTUR_24, display_get_width()/2-20, display_get_height()/2, pauseStr);
}