#pragma once
#include <libdragon.h>
#include "../Fonts.h"
#include "GameInterrupt.h"
//#include "../globals.h"

class GI_Reckoner : public GameInterrupt
{
public:
    GI_Reckoner();
    virtual ~GI_Reckoner();
    virtual void handleInput() override;
    virtual void update() override;
    virtual void renderT3d() override;
    virtual void renderRdpq() override;

protected:
    char* pauseStr;

    sprite_t *bgSprite;
};