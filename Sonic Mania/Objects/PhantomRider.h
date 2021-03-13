#ifndef OBJ_PHANTOMRIDER_H
#define OBJ_PHANTOMRIDER_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectPhantomRider;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityPhantomRider;

// Object Struct
extern ObjectPhantomRider *PhantomRider;

// Standard Entity Events
void PhantomRider_Update();
void PhantomRider_LateUpdate();
void PhantomRider_StaticUpdate();
void PhantomRider_Draw();
void PhantomRider_Create(void* data);
void PhantomRider_StageLoad();
void PhantomRider_EditorDraw();
void PhantomRider_EditorLoad();
void PhantomRider_Serialize();

// Extra Entity Functions


#endif //!OBJ_PHANTOMRIDER_H