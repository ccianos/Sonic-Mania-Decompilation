#ifndef OBJ_TURNTABLE_H
#define OBJ_TURNTABLE_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort aniFrames;
} ObjectTurntable;

// Entity Class
typedef struct {
    RSDK_ENTITY
    byte type;
    int angleVel;
    byte activePlayers;
    Vector2 size;
    Vector2 origin;
    int field_74;
    int field_78[4];
    int field_88[4];
    int field_98[4];
    Hitbox hitbox;
    Animator animator;
} EntityTurntable;

// Object Struct
extern ObjectTurntable *Turntable;

// Standard Entity Events
void Turntable_Update(void);
void Turntable_LateUpdate(void);
void Turntable_StaticUpdate(void);
void Turntable_Draw(void);
void Turntable_Create(void* data);
void Turntable_StageLoad(void);
void Turntable_EditorDraw(void);
void Turntable_EditorLoad(void);
void Turntable_Serialize(void);

// Extra Entity Functions
void Turntable_SetupSize(void);


#endif //!OBJ_TURNTABLE_H