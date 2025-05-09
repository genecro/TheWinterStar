#include "GI_Reckoner.h"

#define STICK_DEADZONE 8
#define TOP_MENU_NUM_ITEMS 3

enum {
    TOP_MENU = -1,
    MAP_STATE = 0,
    INV_STATE = 1,
    WB_STATE = 2,

};

GI_Reckoner::GI_Reckoner() {
    pauseStr = "Paused";
    pauseInterrupt = true;

    bgSprite = sprite_load("rom:/reckonerBG.sprite");

    wbSprite = sprite_load("rom:/sprites/RKWB.sprite");
    invSprite = sprite_load("rom:/sprites/RKINV.sprite");
    mapSprite = sprite_load("rom:/sprites/RKMAP.sprite");

    wbSpriteDark = sprite_load("rom:/sprites/RKWBdark.sprite");
    invSpriteDark = sprite_load("rom:/sprites/RKINVdark.sprite");
    mapSpriteDark = sprite_load("rom:/sprites/RKMAPdark.sprite");

    currState = TOP_MENU;

    topMenu_t.index = 0;

    invMenu_t.invNum = 3333;
    invMenu_t.scale = 2.0;

    glyphDrawer = new NumberGlyphDrawer();
}

GI_Reckoner::~GI_Reckoner() {
    sprite_free(bgSprite);
    sprite_free(wbSprite);
    sprite_free(invSprite);
    sprite_free(mapSprite);
    sprite_free(wbSpriteDark);
    sprite_free(invSpriteDark);
    sprite_free(mapSpriteDark);
    delete glyphDrawer;
}

void GI_Reckoner::handleInput() {
    joypad_buttons_t keys = joypad_get_buttons_pressed(JOYPAD_PORT_1);
    int yAxisPressed = joypad_get_axis_pressed(JOYPAD_PORT_1, JOYPAD_AXIS_STICK_Y);
    int yAxisHeld = joypad_get_axis_held(JOYPAD_PORT_1, JOYPAD_AXIS_STICK_Y);

    switch (currState) {
        case TOP_MENU:
            if(yAxisPressed < 0 || keys.d_down) {
                if(topMenu_t.index+1 < TOP_MENU_NUM_ITEMS) {
                    topMenu_t.index++;
                }
            }

            if(yAxisPressed > 0 || keys.d_up) {
                if(topMenu_t.index > 0) {
                    topMenu_t.index--;
                }
            }

            if(keys.a) {
                currState = topMenu_t.index;
            }

            if(keys.b || keys.start) {
                timeToDestroy = true;
            }
            break;

        case MAP_STATE:
            if(keys.b) {
                currState = TOP_MENU;
            }

            if(keys.start) {
                timeToDestroy = true;
            }
            break;
        
        case INV_STATE:
            if(keys.b) {
                currState = TOP_MENU;
            }

            if(keys.r) {
                if(invMenu_t.scale < 16) {
                    invMenu_t.scale += 1.0f;
                }
            }

            if(keys.l) {
                if(invMenu_t.scale > 0) {
                    invMenu_t.scale -= 1.0f;
                }
            }

            if(keys.start) {
                timeToDestroy = true;
            }

            if(yAxisHeld>0) {
                invMenu_t.invNum++;
            }
            else if (yAxisHeld < 0) {
                if(invMenu_t.invNum > 0) {
                    invMenu_t.invNum--;
                }
            }
            break;
        
        case WB_STATE:
            if(keys.b) {
                currState = TOP_MENU;
            }

            if(keys.start) {
                timeToDestroy = true;
            }
            break;
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
    rdpq_mode_alphacompare(1);
    //debugf("setting mode blender\n");
    //rdpq_mode_combiner(RDPQ_COMBINER_FLAT);
    //rdpq_mode_blender(RDPQ_BLENDER_MULTIPLY);
    //debugf("blitting sprite\n");
    rdpq_sprite_blit(bgSprite, display_get_width()/2.0f, display_get_height()/2.0f, &(rdpq_blitparms_t){
        .cx = bgSprite->width/2.0,
        .cy = bgSprite->height/2.0,
    });

    rdpq_sprite_blit(topMenu_t.index == 0 ? mapSprite : mapSpriteDark, display_get_width()/4.0f, display_get_height()/3.0f, &(rdpq_blitparms_t){
        .cx=8,
        .cy=8,
        .scale_x=2,
        .scale_y=2,
    });
    rdpq_sprite_blit(topMenu_t.index == 1 ? invSprite : invSpriteDark, display_get_width()/4.0f, display_get_height()/2.0f, &(rdpq_blitparms_t){
        .cx=8,
        .cy=8,
        .scale_x=2,
        .scale_y=2,
    });
    rdpq_sprite_blit(topMenu_t.index == 2 ? wbSprite : wbSpriteDark, display_get_width()/4.0f, 2.0f*display_get_height()/3.0f, &(rdpq_blitparms_t){
        .cx=8,
        .cy=8,
        .scale_x=2,
        .scale_y=2,
    });
    //debugf("finished blitting sprite\n");
   

    switch (currState) {
        case TOP_MENU:
            rdpq_text_printf(&(rdpq_textparms_t) {
                .style_id=FONTSTYLE_ORANGE,
            }, FONT_PIXELFRAKTUR_24, display_get_width()/2, display_get_height()/2, pauseStr);

            switch (topMenu_t.index) {
                case MAP_STATE:
                    rdpq_text_printf(&(rdpq_textparms_t) {
                        .style_id=FONTSTYLE_ORANGE,
                    }, FONT_PIXELFRAKTUR_16, display_get_width()/4.0f+20, display_get_height()/3.0f+6, "Plot");
                    break;

                case INV_STATE:
                    rdpq_text_printf(&(rdpq_textparms_t) {
                        .style_id=FONTSTYLE_ORANGE,
                    }, FONT_PIXELFRAKTUR_16, display_get_width()/4.0f+20, display_get_height()/2.0f+6, "Hoard");
                    break;

                case WB_STATE:
                    rdpq_text_printf(&(rdpq_textparms_t) {
                        .style_id=FONTSTYLE_ORANGE,
                    }, FONT_PIXELFRAKTUR_16, display_get_width()/4.0f+20, 2.0f*display_get_height()/3.0f+6, "Wordbook");
                    break;
            }
            break;
        
        case INV_STATE:
            
            //
            rdpq_text_printf(&(rdpq_textparms_t) {
                .style_id=FONTSTYLE_ORANGE,
            }, FONT_PIXELFRAKTUR_16, display_get_width()/2.0f-20, display_get_height()/2.0f, std::to_string(invMenu_t.invNum).c_str());
            //
            //rdpq_mode_push();
            glyphDrawer->drawGlyph(invMenu_t.invNum, display_get_width()/2.0f+15, display_get_height()/2.0f-10, RGBA32(0xF8, 0x7D, 0x36, 0xFF), invMenu_t.scale);
            //rdpq_mode_pop();
            
            break;
    }
}