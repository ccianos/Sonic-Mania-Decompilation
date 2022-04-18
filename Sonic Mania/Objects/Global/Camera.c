// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Camera Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectCamera *Camera;

void Camera_Update(void) {}

void Camera_LateUpdate(void)
{
    RSDK_THIS(Camera);

    self->lastPos.x = self->position.x;
    self->lastPos.y = self->position.y;

    StateMachine_Run(self->state);

    self->velocity.x = self->position.x - self->lastPos.x;
    self->velocity.y = self->position.y - self->lastPos.y;

    Camera_SetCameraBounds(self);

    if (self->shakePos.x) {
        if (self->shakePos.x <= 0)
            self->shakePos.x = -1 - self->shakePos.x;
        else
            self->shakePos.x = -self->shakePos.x;
    }

    if (self->shakePos.y) {
        if (self->shakePos.y <= 0)
            self->shakePos.y = -1 - self->shakePos.y;
        else
            self->shakePos.y = -self->shakePos.y;
    }

    if (!self->disableYOffset) {
        self->offset.y = maxVal(self->offset.y - (self->offset.y >> 3), 0);
    }
}

void Camera_StaticUpdate(void)
{
    if (Camera->centerBounds.x < 0x100000)
        Camera->centerBounds.x += 0x4000;

    if (Camera->centerBounds.y < 0x180000)
        Camera->centerBounds.y += 0x8000;
}

void Camera_Draw(void) {}

void Camera_Create(void *data)
{
    RSDK_THIS(Camera);
    int32 screen = voidToInt(data);
    self->offset.x = 0x80000;
    self->centerY  = ScreenInfo->centerY - 16;
    if (self->active != ACTIVE_NORMAL) {
        self->screenID = screen;
        RSDK.AddCamera(&self->center, ScreenInfo[screen].centerX << 16, ScreenInfo[screen].centerY << 16, false);
    }
    self->boundsOffset.x = 3;
    self->boundsOffset.y = 2;
    self->active         = ACTIVE_NORMAL;
    if (!Zone->setATLBounds) {
        self->boundsL = Zone->cameraBoundsL[self->screenID];
        self->boundsR = Zone->cameraBoundsR[self->screenID];
        self->boundsT = Zone->cameraBoundsT[self->screenID];
        self->boundsB = Zone->cameraBoundsB[self->screenID];

        if (self->target) {
            self->position.x = self->target->position.x;
            self->position.y = self->target->position.y;
            self->state      = Camera_State_Follow;
        }
        else {
            self->state = Camera_State_Roam;
        }
    }
}

void Camera_StageLoad(void)
{
    if (!RSDK.CheckStageFolder("Credits")) {
        for (int32 i = 0; i < RSDK.GetSettingsValue(SETTINGS_SCREENCOUNT); ++i) RSDK.ResetEntitySlot(SLOT_CAMERA1 + i, Camera->objectID, intToVoid(i));
        Camera->centerBounds.x = 0x100000;
        Camera->centerBounds.y = 0x180000;
    }
}

void Camera_SetCameraBounds(EntityCamera *entity)
{
    RSDKScreenInfo *screen = &ScreenInfo[entity->screenID];
    screen->position.x     = (entity->position.x >> 0x10) + entity->lookPos.x - screen->centerX;
    screen->position.y     = (entity->position.y >> 0x10) + entity->lookPos.y - entity->centerY;

    if (screen->position.x < entity->boundsL)
        screen->position.x = entity->boundsL;

    if (screen->width + screen->position.x > entity->boundsR)
        screen->position.x = entity->boundsR - screen->width;

    if (screen->position.y < entity->boundsT)
        screen->position.y = entity->boundsT;

    if (screen->height + screen->position.y > entity->boundsB)
        screen->position.y = entity->boundsB - screen->height;

    screen->position.x += entity->shakePos.x;
    screen->position.y += entity->shakePos.y;
    entity->center.x = screen->position.x + screen->centerX;
    entity->center.y = screen->position.y + screen->centerY;
}
EntityCamera *Camera_SetTargetEntity(int32 screen, void *t)
{
    Entity *target = (Entity *)t;

    foreach_all(Camera, camera)
    {
        if (camera->screenID == screen) {
            camera->target  = target;
            camera->position.x = target->position.x;
            camera->position.y = target->position.y;
            foreach_return camera;
        }
    }
    return NULL;
}
void Camera_ShakeScreen(int32 screen, int32 shakeX, int32 shakeY)
{
    foreach_all(Camera, camera)
    {
        if (camera->screenID == screen) {
            camera->shakePos.x = shakeX;
            camera->shakePos.y = shakeY;
            foreach_break;
        }
    }
}
void Camera_HandleHBounds(void)
{
    RSDK_THIS(Camera);
    RSDKScreenInfo *screen = &ScreenInfo[self->screenID];

    if (Zone->cameraBoundsL[self->screenID] > self->boundsL) {
        if (screen->position.x > Zone->cameraBoundsL[self->screenID])
            self->boundsL = Zone->cameraBoundsL[self->screenID];
        else
            self->boundsL = screen->position.x;
    }

    if (Zone->cameraBoundsL[self->screenID] < self->boundsL) {
        if (screen->position.x <= self->boundsL) {
            int32 off         = self->boundsL - self->boundsOffset.x;
            self->boundsL = off;
            if (self->velocity.x < 0) {
                self->boundsL += (self->velocity.x >> 0x10);
                if (self->boundsL < Zone->cameraBoundsL[self->screenID])
                    self->boundsL = Zone->cameraBoundsL[self->screenID];
            }
        }
        else {
            self->boundsL = Zone->cameraBoundsL[self->screenID];
        }
    }

    if (Zone->cameraBoundsR[self->screenID] < self->boundsR) {
        if (screen->width + screen->position.x < Zone->cameraBoundsR[self->screenID])
            self->boundsR = Zone->cameraBoundsR[self->screenID];
        else
            self->boundsR = screen->width + screen->position.x;
    }

    if (Zone->cameraBoundsR[self->screenID] > self->boundsR) {
        if (screen->width + screen->position.x >= self->boundsR) {
            self->boundsR += self->boundsOffset.x;
            if (self->velocity.x > 0) {
                self->boundsR = (self->velocity.x >> 0x10) + self->boundsR;
                if (self->boundsR > Zone->cameraBoundsR[self->screenID])
                    self->boundsR = Zone->cameraBoundsR[self->screenID];
            }
        }
        else {
            self->boundsR = Zone->cameraBoundsR[self->screenID];
        }
    }

    Zone->playerBoundsL[self->screenID] = self->boundsL << 16;
    Zone->playerBoundsR[self->screenID] = self->boundsR << 16;
}
void Camera_HandleVBounds(void)
{
    RSDK_THIS(Camera);
    RSDKScreenInfo *screen = &ScreenInfo[self->screenID];

    if (Zone->cameraBoundsT[self->screenID] > self->boundsT) {
        if (screen->position.y <= self->boundsT)
            self->boundsT = screen->position.y + self->boundsOffset.y;
        else
            self->boundsT = screen->position.y;
    }

    if (Zone->cameraBoundsT[self->screenID] < self->boundsT) {
        if (screen->position.y <= self->boundsT) {
            int32 off         = self->boundsT - self->boundsOffset.y;
            self->boundsT = off;
            if (self->velocity.y < 0) {
                self->boundsT = (self->velocity.y >> 0x10) + off;
                if (self->boundsT < Zone->cameraBoundsT[self->screenID])
                    self->boundsT = Zone->cameraBoundsT[self->screenID];
            }
        }
        else {
            self->boundsT = Zone->cameraBoundsT[self->screenID];
        }
    }

    if (Zone->cameraBoundsB[self->screenID] < self->boundsB) {
        if (screen->height + screen->position.y >= self->boundsB)
            self->boundsB -= 2;
        else
            self->boundsB = screen->height + screen->position.y;
    }

    if (Zone->cameraBoundsB[self->screenID] > self->boundsB) {
        if (screen->height + screen->position.y >= self->boundsB) {
            self->boundsB += self->boundsOffset.y;
            if (self->velocity.y > 0) {
                self->boundsB = (self->velocity.y >> 0x10) + self->boundsB;
                if (self->boundsB > Zone->cameraBoundsB[self->screenID])
                    self->boundsB = Zone->cameraBoundsB[self->screenID];
            }
        }
        else {
            self->boundsB = Zone->cameraBoundsB[self->screenID];
        }
    }

    Zone->playerBoundsT[self->screenID] = self->boundsT << 16;
    Zone->playerBoundsB[self->screenID] = self->boundsB << 16;
}

void Camera_SetupLerp(int32 type, int32 screen, int32 x, int32 y, int32 speed)
{
    foreach_all(Camera, camera)
    {
        if (camera->screenID == screen) {
            camera->startLerpPos.x = camera->position.x;
            camera->startLerpPos.y = camera->position.y;
            camera->endLerpPos.x   = x;
            camera->endLerpPos.y   = y;
            camera->lerpType       = type;
            camera->lerpSpeed      = speed;
            camera->state          = Camera_State_HandleLerp;
            foreach_break;
        }
    }
}

// States
void Camera_State_Roam(void)
{
    RSDK_THIS(Camera);
    int32 speed = 0x100000;
    if (!ControllerInfo[CONT_P1].keyA.down)
        speed = 0x40000;

    if (ControllerInfo[CONT_P1].keyUp.down) {
        self->position.y -= speed;
    }
    else if (ControllerInfo[CONT_P1].keyDown.down) {
        self->position.y += speed;
    }
    if (ControllerInfo[CONT_P1].keyLeft.down) {
        self->position.x -= speed;
    }
    else if (ControllerInfo[CONT_P1].keyRight.down) {
        self->position.x += speed;
    }

    self->position.x = self->position.x >> 0x10;
    self->position.y = self->position.y >> 0x10;
    RSDKScreenInfo *screen = &ScreenInfo[self->screenID];

    if (self->position.x >= screen->centerX) {
        if (self->position.x > Zone->cameraBoundsR[self->screenID] - screen->centerX)
            self->position.x = Zone->cameraBoundsR[self->screenID] - screen->centerX;
    }
    else {
        self->position.x = screen->centerX;
    }

    if (self->position.y >= screen->centerY) {
        if (self->position.y > Zone->cameraBoundsB[self->screenID] - screen->centerY)
            self->position.y = Zone->cameraBoundsB[self->screenID] - screen->centerY;
        self->position.x <<= 0x10;
        self->position.y <<= 0x10;
    }
    else {
        self->position.x <<= 0x10;
        self->position.y <<= 0x10;
    }
}
void Camera_State_Follow(void)
{
    RSDK_THIS(Camera);
    if (self->target) {
        Camera_HandleHBounds();
        Camera_HandleVBounds();
        Entity *target = self->target;
        target->position.x += self->targetMoveVel.x;
        if (target->position.x <= self->position.x + self->offset.x) {
            if (target->position.x < self->position.x - self->offset.x) {
                int32 pos = target->position.x + self->offset.x - self->position.x;
                if (pos < -Camera->centerBounds.x)
                    pos = -Camera->centerBounds.x;
                self->position.x += pos;
            }
            target->position.x -= self->targetMoveVel.x;
        }
        else {
            int32 pos = target->position.x - self->position.x - self->offset.x;
            if (pos > Camera->centerBounds.x)
                pos = Camera->centerBounds.x;
            self->position.x += pos;
            target->position.x -= self->targetMoveVel.x;
        }

        target->position.y += self->targetMoveVel.y;
        int32 adjust = target->position.y - self->adjustY;
        if (adjust <= self->position.y + self->offset.y) {
            if (adjust < self->position.y - self->offset.y) {
                int32 pos = target->position.y + self->offset.y - self->position.y - self->adjustY;
                if (pos < -Camera->centerBounds.y)
                    pos = -Camera->centerBounds.y;
                self->position.y += pos;
            }
            target->position.y -= self->targetMoveVel.y;
        }
        else {
            int32 pos = adjust - self->position.y - self->offset.y;
            if (pos > Camera->centerBounds.y)
                pos = Camera->centerBounds.y;
            self->position.y += pos;
            target->position.y -= self->targetMoveVel.y;
        }
    }
}
void Camera_State_HLock(void)
{
    RSDK_THIS(Camera);
    if (self->target) {
        Camera_HandleHBounds();
        Entity *target = self->target;
        target->position.x += self->targetMoveVel.x;
        if (target->position.x <= self->position.x + self->offset.x) {
            if (target->position.x < self->position.x - self->offset.x) {
                int32 pos = target->position.x + self->offset.x - self->position.x;
                if (pos < -Camera->centerBounds.x)
                    pos = -Camera->centerBounds.x;
                self->position.x = self->position.x + pos;
            }
            target->position.x -= self->targetMoveVel.x;
        }
        else {
            int32 pos = target->position.x - self->position.x - self->offset.x;
            if (pos > Camera->centerBounds.x)
                pos = Camera->centerBounds.x;
            self->position.x = self->position.x + pos;
            target->position.x -= self->targetMoveVel.x;
        }
    }
}
void Camera_State_VLock(void)
{
    RSDK_THIS(Camera);
    if (self->target) {
        Camera_HandleVBounds();
        Entity *target = self->target;
        target->position.y += self->targetMoveVel.y;
        int32 adjust = target->position.y - self->adjustY;
        if (adjust <= self->position.y + self->offset.y) {
            if (adjust < self->position.y - self->offset.y) {
                int32 pos = target->position.y + self->offset.y - self->position.y - self->adjustY;
                if (pos < -Camera->centerBounds.y)
                    pos = -Camera->centerBounds.y;
                self->position.y = self->position.y + pos;
            }
            target->position.y -= self->targetMoveVel.y;
        }
        else {
            int32 pos = adjust - self->position.y - self->offset.y;
            if (pos > Camera->centerBounds.y)
                pos = Camera->centerBounds.y;
            self->position.y = self->position.y + pos;
            target->position.y -= self->targetMoveVel.y;
        }
    }
}
void Camera_State_HandleLerp(void)
{
    RSDK_THIS(Camera);
    self->lerpPercent += self->lerpSpeed;

    switch (self->lerpType) {
        default: break;
        case 0:
            MathHelpers_Lerp(&self->position, self->lerpPercent, self->startLerpPos.x, self->startLerpPos.y, self->endLerpPos.x,
                                 self->endLerpPos.y);
            break;
        case 1:
            MathHelpers_LerpSin1024(&self->position, self->lerpPercent, self->startLerpPos.x, self->startLerpPos.y, self->endLerpPos.x,
                                 self->endLerpPos.y);
            break;
        case 2:
            MathHelpers_Lerp2Sin1024(&self->position, self->lerpPercent, self->startLerpPos.x, self->startLerpPos.y, self->endLerpPos.x,
                                 self->endLerpPos.y);
            break;
        case 3:
            MathHelpers_LerpSin512(&self->position, self->lerpPercent, self->startLerpPos.x, self->startLerpPos.y, self->endLerpPos.x,
                                 self->endLerpPos.y);
            break;
    }

    if (self->lerpPercent >= 0x100) {
        self->lerpPercent = 0;
        self->state       = StateMachine_None;
    }
}

#if RETRO_INCLUDE_EDITOR
void Camera_EditorDraw(void)
{
    RSDK_THIS(Camera);

    // Camera preview :)
    if (showGizmos()) {
        DrawHelpers_DrawRectOutline(self->position.x, self->position.y, WIDE_SCR_XSIZE << 16, SCREEN_YSIZE << 16, 0xFF0000);
    }
}

void Camera_EditorLoad(void) {}
#endif

void Camera_Serialize(void) {}
