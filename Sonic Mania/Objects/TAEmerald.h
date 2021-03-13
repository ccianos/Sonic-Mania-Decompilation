#ifndef OBJ_TAEMERALD_H
#define OBJ_TAEMERALD_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectTAEmerald;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityTAEmerald;

// Object Struct
extern ObjectTAEmerald *TAEmerald;

// Standard Entity Events
void TAEmerald_Update();
void TAEmerald_LateUpdate();
void TAEmerald_StaticUpdate();
void TAEmerald_Draw();
void TAEmerald_Create(void* data);
void TAEmerald_StageLoad();
void TAEmerald_EditorDraw();
void TAEmerald_EditorLoad();
void TAEmerald_Serialize();

// Extra Entity Functions


#endif //!OBJ_TAEMERALD_H