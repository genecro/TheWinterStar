#pragma once

#include "GameObject.h"
#include "../globals.h"

class GO_NPC: public GameObject {
public:
    GO_NPC();
    GO_NPC(std::string name);
    virtual ~GO_NPC();
    virtual void handleInput() override;
    virtual void update() override;
    virtual void renderT3d() override;
    virtual void renderRdpq() override;
};