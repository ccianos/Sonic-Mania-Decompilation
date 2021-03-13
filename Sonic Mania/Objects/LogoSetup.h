#ifndef OBJ_LOGOSETUP_H
#define OBJ_LOGOSETUP_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int timer;
    ushort sfx_Sega;
} ObjectLogoSetup;

// Entity Class
typedef struct {
    RSDK_ENTITY
    void (*state)();
    void (*stateDraw)();
    int timer;
} EntityLogoSetup;

// Object Struct
extern ObjectLogoSetup *LogoSetup;

// Standard Entity Events
void LogoSetup_Update();
void LogoSetup_LateUpdate();
void LogoSetup_StaticUpdate();
void LogoSetup_Draw();
void LogoSetup_Create(void* data);
void LogoSetup_StageLoad();
void LogoSetup_EditorDraw();
void LogoSetup_EditorLoad();
void LogoSetup_Serialize();

// Extra Entity Functions
bool32 LogoSetup_PNGCallback();
void LogoSetup_CESAScreen();
void LogoSetup_SegaScreen();
void LogoSetup_Unknown2();
void LogoSetup_Unknown3();
void LogoSetup_Unknown4();

#endif //!OBJ_LOGOSETUP_H