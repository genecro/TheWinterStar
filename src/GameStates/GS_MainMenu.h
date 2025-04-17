#pragma once
#include <libdragon.h>
#include <t3d/t3d.h>
#include <t3d/t3dmath.h>
#include <t3d/t3dmodel.h>
#include <t3d/t3ddebug.h>
#include <string>
#include "../GameInterrupts/GI_Pause.h"
#include "../GameInterrupts/GI_FadeToNextGS.h"
#include "../GameInterrupts/GI_FadeIn.h"
#include "GameState.h"
//#include "GS_Intro.h"
//#include "GS_001ChurchSquare.h"
#include "../Fonts.h"
//#include "../globals.h"


#define NUM_MENU_CHOICES 5
#define MOVE_CURSOR_UP -1
#define MOVE_CURSOR_DOWN 1

class GS_MainMenu : public GameState
{
public:
    GS_MainMenu();
    ~GS_MainMenu();
    virtual void handleInput() override;
    virtual void update() override;
    virtual void renderT3d() override;
    virtual void renderRdpq() override;

    virtual void testFunc() override;

private:
    /*struct camera_t {
        T3DVec3 pos;
        T3DVec3 target;
    } camera;*/

    ////t3d
    T3DViewport viewport;
    T3DMat4 modelMat;
    T3DMat4FP* modelMatFP;
    uint8_t colorAmbient[4] = {80, 80, 100, 0xFF};
    uint8_t colorDir[4]     = {0xEE, 0xAA, 0xAA, 0xFF};
    T3DVec3 lightDirVec;
    T3DModel *model;
    float rotAngle;
    bool rotAngleReverse;
    float zTrans;
    bool zTransUp;
    rspq_block_t *dplDraw = NULL;

    sprite_t *titleSprite;
    int titleSpriteX, titleSpriteY;

    struct menuChoice {
        char const* choiceName;
        bool enabled;
    };

    void updateMainMenuState();
    void updateNewGameState();

    struct nameEntry_t {
        bool caps;
        int numCols;
        std::string newPlayerName;
        int nameCursor;
        int LETTERS_IN_ALPHABET;
        int capsConv;
        int maxNameLen;
    } nameEntry;

    menuChoice menuChoices[NUM_MENU_CHOICES];
    int menuCursor;

    int textX, textY;

    int moveCursor(int currCursor, int direction);

    int currState;

    void drawTitleLogo();
    void drawMainMenu();

    void drawNameEntry();
};