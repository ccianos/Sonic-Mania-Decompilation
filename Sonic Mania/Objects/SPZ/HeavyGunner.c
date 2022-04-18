// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: HeavyGunner Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectHeavyGunner *HeavyGunner;

void HeavyGunner_Update(void)
{
    RSDK_THIS(HeavyGunner);
    StateMachine_Run(self->state);
}

void HeavyGunner_LateUpdate(void) {}

void HeavyGunner_StaticUpdate(void) {}

void HeavyGunner_Draw(void)
{
    RSDK_THIS(HeavyGunner);
    StateMachine_Run(self->stateDraw);
}

void HeavyGunner_Create(void *data)
{
    RSDK_THIS(HeavyGunner);

    if (!SceneInfo->inEditor) {
        if (globals->gameMode < MODE_TIMEATTACK) {
            if (data) {
                self->type      = voidToInt(data);
                self->active    = ACTIVE_NORMAL;
                self->visible   = true;
                self->drawOrder = Zone->objectDrawLow - 1;
                switch (self->type) {
                    case HEAVYGUNNER_HELI:
                        RSDK.SetSpriteAnimation(HeavyGunner->aniFrames, 6, &self->mainAnimator, true, 0);
                        RSDK.SetSpriteAnimation(HeavyGunner->aniFrames, 14, &self->pilotAnimator, true, 0);
                        RSDK.SetSpriteAnimation(HeavyGunner->aniFrames, 7, &self->frontAnimator, true, 0);
                        RSDK.SetSpriteAnimation(HeavyGunner->aniFrames, 8, &self->tBladeAnimator, true, 0);
                        RSDK.SetSpriteAnimation(HeavyGunner->aniFrames, 9, &self->rBladeAnimator, true, 0);
                        RSDK.SetSpriteAnimation(HeavyGunner->aniFrames, 10, &self->beaconAnimator, true, 0);
                        RSDK.SetSpriteAnimation(HeavyGunner->aniFrames, 11, &self->feetAnimator, true, 0);
                        RSDK.SetSpriteAnimation(HeavyGunner->aniFrames, 11, &self->feet2Animator, true, 2);
                        RSDK.SetSpriteAnimation(HeavyGunner->aniFrames, 0, &self->gunnerAnimator, true, 0);
                        self->drawFX    = FX_ROTATE;
                        self->rotation  = 64;
                        self->state     = HeavyGunner_StateHeli_WaitForPlayer;
                        self->stateDraw = HeavyGunner_Draw_Heli;
                        break;

                    case HEAVYGUNNER_EGGROBO:
                        RSDK.SetSpriteAnimation(HeavyGunner->aniFrames, 17, &self->feetAnimator, true, 0);
                        RSDK.SetSpriteAnimation(HeavyGunner->aniFrames, 16, &self->mainAnimator, true, RSDK.Rand(0, 4));
                        self->drawFX    = FX_SCALE | FX_ROTATE;
                        self->state     = HeavyGunner_StateEggRobo_Patrolling;
                        self->stateDraw = HeavyGunner_Draw_EggRobo;
                        self->scale.x   = 0x200;
                        self->scale.y   = 0x200;
                        self->targetScale  = 512;
                        self->parentOffset  = RSDK.Rand(-0x200000, 0x600000);
                        self->timer     = 240;
                        break;

                    case HEAVYGUNNER_GUN:
                        RSDK.SetSpriteAnimation(HeavyGunner->aniFrames, 20, &self->mainAnimator, true, 0);
                        self->drawFX    = FX_ROTATE;
                        self->rotation  = 64;
                        self->state     = HeavyGunner_StateGun_Thrown;
                        self->stateDraw = HeavyGunner_Draw_Simple;
                        break;

                    case HEAVYGUNNER_MISSILE_F:
                    case HEAVYGUNNER_MISSILE:
                        if (self->type == HEAVYGUNNER_MISSILE_F)
                            RSDK.SetSpriteAnimation(HeavyGunner->aniFrames, 29, &self->mainAnimator, true, 0);
                        else
                            RSDK.SetSpriteAnimation(HeavyGunner->aniFrames, 30, &self->mainAnimator, true, 0);
                        self->drawFX     = FX_SCALE | FX_ROTATE | FX_FLIP;
                        self->velocity.x = 0x40000;
                        self->velocity.y = -0x38000;
                        self->scale.x    = 0x100;
                        self->scale.y    = 0x100;
                        self->rotation   = -24;
                        self->state      = HeavyGunner_StateMissile_Launched;
                        self->stateDraw  = HeavyGunner_Draw_Missile;
                        self->drawOrder  = Zone->objectDrawLow;
                        break;

                    case HEAVYGUNNER_ESCAPE_HBH:
                        RSDK.SetSpriteAnimation(HeavyGunner->aniFrames, 5, &self->mainAnimator, true, 0);
                        self->velocity.x = -0x20000;
                        self->velocity.y = -0x20000;
                        self->state      = HeavyGunner_StateEscapeHBH_Hover;
                        self->stateDraw  = HeavyGunner_Draw_Simple;
                        self->drawOrder  = Zone->objectDrawLow;
                        break;

                    default: break;
                }
            }
            else {
                Zone->autoScrollSpeed = 0;
                self->active          = ACTIVE_XBOUNDS;
                self->updateRange.x   = 0x800000;
                self->updateRange.y   = 0x800000;
                self->state           = HeavyGunner_StateManager_SetupArena;
            }
        }
        else {
            destroyEntity(self);
        }
    }
}

void HeavyGunner_StageLoad(void)
{
    HeavyGunner->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/Boss.bin", SCOPE_STAGE);

    HeavyGunner->sfxFire       = RSDK.GetSfx("Stage/CannonFire.wav");
    HeavyGunner->sfxJet        = RSDK.GetSfx("Stage/RocketJet.wav");
    HeavyGunner->sfxHit        = RSDK.GetSfx("Stage/BossHit.wav");
    HeavyGunner->sfxDestroy    = RSDK.GetSfx("Global/Destroy.wav");
    HeavyGunner->sfxExplosion2 = RSDK.GetSfx("Stage/Explosion2.wav");
    HeavyGunner->sfxExplosion3 = RSDK.GetSfx("Stage/Explosion3.wav");
    HeavyGunner->sfxCharge     = RSDK.GetSfx("SPZ1/BazookaCharge.wav");
    HeavyGunner->sfxThrow      = RSDK.GetSfx("SPZ1/BazookaThrow.wav");
    HeavyGunner->sfxFlyIn      = RSDK.GetSfx("SPZ1/EggRoboFlyIn.wav");
    HeavyGunner->sfxWooshIn    = RSDK.GetSfx("SPZ1/HeliWooshIn.wav");
    HeavyGunner->sfxWooshOut   = RSDK.GetSfx("SPZ1/HeliWooshOut.wav");
    HeavyGunner->sfxFlip       = RSDK.GetSfx("SPZ1/RocketFlip.wav");
    HeavyGunner->sfxRumble     = RSDK.GetSfx("SPZ1/Rumble.wav");

    Soundboard_LoadSFX("SPZ1/HBHSurprise.wav", true, HeavyGunner_SurpriseCheckCB, NULL);
    Soundboard_LoadSFX("SPZ1/HeliProp.wav", true, HeavyGunner_HelicopterCheckCB, NULL);
    Soundboard_LoadSFX("SPZ1/RocketBurn.wav", true, HeavyGunner_BurnCheckCB, NULL);

    // Absolutely zero clue what these are for
    HeavyGunner->unused1 = 0x470000;
    HeavyGunner->unused2 = 0x2D0000;
    HeavyGunner->unused3 = 0x100000;
    HeavyGunner->unused4 = 0x80000;

    HeavyGunner->hitboxMissileF.left   = -32;
    HeavyGunner->hitboxMissileF.top    = -8;
    HeavyGunner->hitboxMissileF.right  = 32;
    HeavyGunner->hitboxMissileF.bottom = 8;

    HeavyGunner->hitboxMissile.left   = -16;
    HeavyGunner->hitboxMissile.top    = -8;
    HeavyGunner->hitboxMissile.right  = 16;
    HeavyGunner->hitboxMissile.bottom = 8;

    HeavyGunner->active = ACTIVE_NEVER;
}

bool32 HeavyGunner_HelicopterCheckCB(void)
{
    Vector2 range;

    range.x       = 0x800000;
    range.y       = 0x800000;
    bool32 active = false;

    foreach_active(HeavyGunner, gunner)
    {
        if (RSDK.CheckOnScreen(gunner, &range) && gunner->tBladeAnimator.animationID == 8)
            active = true;
    }
    return active;
}

bool32 HeavyGunner_SurpriseCheckCB(void)
{
    Vector2 range;

    range.x       = 0x800000;
    range.y       = 0x800000;
    bool32 active = false;

    foreach_active(HeavyGunner, gunner)
    {
        if (RSDK.CheckOnScreen(gunner, &range) && gunner->gunnerAnimator.animationID == 3 && gunner->mainAnimator.animationID != 12)
            active = true;
    }

    return active;
}

bool32 HeavyGunner_BurnCheckCB(void)
{
    Vector2 range;

    range.x       = 0x800000;
    range.y       = 0x800000;
    bool32 active = false;

    foreach_active(HeavyGunner, gunner)
    {
        if (RSDK.CheckOnScreen(gunner, &range) && gunner->exhaustAnimator.animationID == 25)
            active = true;
    }
    return active;
}

void HeavyGunner_HandleBGWrap(int multiplier)
{
    TileLayer *bg1 = RSDK.GetSceneLayer(0);
    for (int s = 0; s < bg1->scrollInfoCount; ++s) {
        bg1->scrollInfo[s].scrollPos += multiplier * bg1->scrollInfo[s].parallaxFactor;
    }

    TileLayer *bg2 = RSDK.GetSceneLayer(1);
    for (int s = 0; s < bg2->scrollInfoCount; ++s) {
        bg2->scrollInfo[s].scrollPos += multiplier * bg2->scrollInfo[s].parallaxFactor;
    }

    TileLayer *bg3 = RSDK.GetSceneLayer(2);
    for (int s = 0; s < bg3->scrollInfoCount; ++s) {
        bg3->scrollInfo[s].scrollPos += multiplier * bg3->scrollInfo[s].parallaxFactor;
    }
}

void HeavyGunner_DestroyAllMissiles(void)
{
    RSDK_THIS(HeavyGunner);

    foreach_active(HeavyGunner, gunner)
    {
        if (gunner != self) {
            if (gunner->type == HEAVYGUNNER_MISSILE_F || gunner->type == HEAVYGUNNER_MISSILE) {
                RSDK.PlaySfx(HeavyGunner->sfxFlip, false, 0xFF);
                gunner->velocity.x = -0x10000;
                RSDK.SetSpriteAnimation(-1, 0, &gunner->exhaustAnimator, true, 0);
                gunner->direction = FLIP_NONE;
                gunner->rotation  = 256;
                gunner->state     = HeavyGunner_StateMissile_Malfunction;
            }
        }
    }
}

void HeavyGunner_Draw_Heli(void)
{
    RSDK_THIS(HeavyGunner);
    Vector2 drawPos;

    drawPos.x = self->position.x + 0x1B0000;
    drawPos.y = self->position.y;
    RSDK.DrawSprite(&self->feetAnimator, &drawPos, false);
    RSDK.DrawSprite(&self->mainAnimator, NULL, false);
    RSDK.DrawSprite(&self->pilotAnimator, NULL, false);
    RSDK.DrawSprite(&self->frontAnimator, NULL, false);
    RSDK.DrawSprite(&self->tBladeAnimator, NULL, false);
    RSDK.DrawSprite(&self->rBladeAnimator, NULL, false);
    RSDK.DrawSprite(&self->beaconAnimator, NULL, false);
    RSDK.DrawSprite(&self->feet2Animator, NULL, false);
    RSDK.DrawSprite(&self->gunnerAnimator, NULL, false);
}

void HeavyGunner_Draw_EggRobo(void)
{
    RSDK_THIS(HeavyGunner);

    RSDK.DrawSprite(&self->feetAnimator, NULL, false);
    RSDK.DrawSprite(&self->mainAnimator, NULL, false);
    RSDK.DrawSprite(&self->armAnimator, NULL, false);
}

void HeavyGunner_Draw_Missile(void)
{
    RSDK_THIS(HeavyGunner);

    RSDK.DrawSprite(&self->mainAnimator, NULL, false);
    RSDK.DrawSprite(&self->exhaustAnimator, NULL, false);
}

void HeavyGunner_Draw_Simple(void)
{
    RSDK_THIS(HeavyGunner);
    RSDK.DrawSprite(&self->mainAnimator, NULL, false);
}

void HeavyGunner_Draw_FadeOut(void)
{
    RSDK_THIS(HeavyGunner);

    RSDK.FillScreen(0xF0F0F0, self->timer, self->timer - 0x80, self->timer - 0x100);
}

void HeavyGunner_StateManager_SetupArena(void)
{
    RSDK_THIS(HeavyGunner);

    if (++self->timer >= 8) {
        EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

        if (player1->position.x >= self->position.x - 0x1000000) {
            HeavyGunner->active = ACTIVE_ALWAYS;
            self->timer         = 0;
            self->active        = ACTIVE_NORMAL;

            EntityCamera *camera = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);
            self->position.x     = camera->position.x - 0x100000;
            self->position.y     = camera->position.y;

            Zone->playerBoundActiveL[0] = true;
            Zone->playerBoundActiveR[0] = true;
            HeavyGunner->boundsL        = Zone->cameraBoundsL[0];
            HeavyGunner->boundsR        = Zone->cameraBoundsR[0];
            HeavyGunner->boundsT        = Zone->cameraBoundsT[0];
            HeavyGunner->boundsB        = 0xC00;
            HeavyGunner->stageWrapActive       = true;

            int32 mult = -0x1E00 * ScreenInfo->position.y;

            TileLayer *bg1 = RSDK.GetSceneLayer(0);
            bg1->scrollPos += mult;
            bg1->parallaxFactor = 88;

            TileLayer *bg2 = RSDK.GetSceneLayer(1);
            bg2->scrollPos += mult;
            bg2->parallaxFactor = 88;

            TileLayer *bg3 = RSDK.GetSceneLayer(2);
            bg3->scrollPos += mult;
            bg3->parallaxFactor = 88;

            foreach_active(ParallaxSprite, sprite)
            {
                sprite->parallaxFactor.y = 0x5800;
                sprite->position.y -= mult;
            }

            Zone->autoScrollSpeed = player1->velocity.x;

            EntityHeavyGunner *robo1 = RSDK_GET_ENTITY(SceneInfo->entitySlot + 1, HeavyGunner);
            int x                    = robo1->position.x;
            int y                    = robo1->position.y;
            RSDK.ResetEntitySlot(SceneInfo->entitySlot + 1, HeavyGunner->objectID, intToVoid(HEAVYGUNNER_EGGROBO));
            robo1->position.x = x;
            robo1->position.y = y;
            robo1->parent     = RSDK_GET_ENTITY(SceneInfo->entitySlot + 4, HeavyGunner);
            robo1->angle      = 0;
            RSDK.SetSpriteAnimation(HeavyGunner->aniFrames, 22, &robo1->armAnimator, true, 0);

            EntityHeavyGunner *robo2 = RSDK_GET_ENTITY(SceneInfo->entitySlot + 2, HeavyGunner);
            x                        = robo2->position.x;
            y                        = robo2->position.y;
            RSDK.ResetEntitySlot(SceneInfo->entitySlot + 2, HeavyGunner->objectID, intToVoid(HEAVYGUNNER_EGGROBO));
            robo2->position.x = x;
            robo2->position.y = y;
            robo2->parent     = RSDK_GET_ENTITY(SceneInfo->entitySlot + 4, HeavyGunner);
            robo2->angle      = 80;
            RSDK.SetSpriteAnimation(HeavyGunner->aniFrames, 23, &robo2->armAnimator, true, 0);

            EntityHeavyGunner *robo3 = RSDK_GET_ENTITY(SceneInfo->entitySlot + 3, HeavyGunner);
            x                        = robo3->position.x;
            y                        = robo3->position.y;
            RSDK.ResetEntitySlot(SceneInfo->entitySlot + 3, HeavyGunner->objectID, intToVoid(HEAVYGUNNER_EGGROBO));
            robo3->position.x = x;
            robo3->position.y = y;
            robo3->parent     = RSDK_GET_ENTITY(SceneInfo->entitySlot + 4, HeavyGunner);
            robo3->angle      = 160;
            robo3->timer      = 0;
            robo3->state      = HeavyGunner_StateEggRobo_FlyIn;
            RSDK.SetSpriteAnimation(HeavyGunner->aniFrames, 19, &robo3->armAnimator, true, 0);

            EntityHeavyGunner *heli = RSDK_GET_ENTITY(SceneInfo->entitySlot + 4, HeavyGunner);
            x                       = heli->position.x;
            y                       = heli->position.y;
            RSDK.ResetEntitySlot(SceneInfo->entitySlot + 4, HeavyGunner->objectID, intToVoid(HEAVYGUNNER_HELI));
            heli->position.x = x;
            heli->position.y = y;

            self->state = HeavyGunner_StateManager_HandleStageWrap;
            Music_TransitionTrack(TRACK_HBHBOSS, 0.0125);
        }
    }
}

void HeavyGunner_StateManager_HandleStageWrap(void)
{
    RSDK_THIS(HeavyGunner);

    self->position.x += Zone->autoScrollSpeed;

    if (HeavyGunner->stageWrapActive) {
        if (self->position.x >= 0x49800000) {
            HeavyGunner_HandleBGWrap(0x80000);
            foreach_active(HeavyGunner, gunner) { gunner->position.x -= 0x8000000; }
            for (int p = 0; p < Player->playerCount; ++p) {
                EntityPlayer *player = RSDK_GET_ENTITY(p, Player);
                player->position.x -= 0x8000000;
            }
            foreach_active(Ring, ring) { ring->position.x -= 0x8000000; }
            foreach_active(ImageTrail, imageTrail)
            {
                imageTrail->position.x -= 0x8000000;
                imageTrail->currentPos.x -= 0x8000000;
                for (int i = 0; i < ImageTrail_TrackCount; ++i) imageTrail->statePos[i].x -= 0x8000000;
            }
        }

        Zone->cameraBoundsL[0] = (self->position.x >> 16) - ScreenInfo->centerX;
        Zone->cameraBoundsR[0] = (self->position.x >> 16) + ScreenInfo->centerX;
        Zone->playerBoundsL[0] = Zone->cameraBoundsL[0] << 16;
        Zone->playerBoundsR[0] = Zone->cameraBoundsR[0] << 16;
        EntityCamera *camera   = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);
        camera->boundsL        = Zone->cameraBoundsL[0];
        camera->boundsR        = Zone->cameraBoundsR[0];

        if (self->flyInTimer == 120) {
            if (!RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x800000, 0x40))
                self->position.y += 0x80000;
            Zone->cameraBoundsB[0] = (self->position.y >> 16) + 168;
            Zone->cameraBoundsT[0] = Zone->cameraBoundsB[0] - ScreenInfo->height;
        }
        else {
            self->flyInTimer++;
        }
    }
    else if (self->position.x < 0x49800000) {
        Zone->cameraBoundsL[0] = (self->position.x >> 16) - ScreenInfo->centerX;
        Zone->cameraBoundsR[0] = (self->position.x >> 16) + ScreenInfo->centerX;
        Zone->playerBoundsL[0] = Zone->cameraBoundsL[0] << 16;
        Zone->playerBoundsR[0] = Zone->cameraBoundsR[0] << 16;
        EntityCamera *camera   = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);
        camera->boundsL        = Zone->cameraBoundsL[0];
        camera->boundsR        = Zone->cameraBoundsR[0];

        if (self->flyInTimer == 120) {
            if (!RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x800000, 0x40))
                self->position.y += 0x80000;
            Zone->cameraBoundsB[0] = (self->position.y >> 16) + 168;
            Zone->cameraBoundsT[0] = Zone->cameraBoundsB[0] - ScreenInfo->height;
        }
        else {
            self->flyInTimer++;
        }
    }
    else {
        Zone->cameraBoundsL[0]      = HeavyGunner->boundsL;
        Zone->cameraBoundsR[0]      = HeavyGunner->boundsR;
        Zone->cameraBoundsT[0]      = HeavyGunner->boundsT;
        Zone->cameraBoundsB[0]      = HeavyGunner->boundsB;
        Zone->playerBoundActiveR[0] = false;
        for (int p = 0; p < Player->playerCount; ++p) {
            EntityPlayer *player = RSDK_GET_ENTITY(p, Player);
            if (player->stateInput == Player_ProcessP1Input) {
                player->stateInput = HeavyGunner_Input_LockedP1;
            }
            else if (player->stateInput == Player_ProcessP2Input_Player) {
                player->stateInput = HeavyGunner_Input_LockedP2;
            }
            else if (player->stateInput == Player_ProcessP2Input_AI) {
                player->stateInput = HeavyGunner_Input_LockedP2_AI;
            }
        }
        self->state = HeavyGunner_StateManager_HandlePathChange;
    }

    if (Zone->autoScrollSpeed > 0x70000) {
        Zone->autoScrollSpeed -= 0x2000;
        if (Zone->autoScrollSpeed < 0x70000)
            Zone->autoScrollSpeed = 0x70000;
    }
    else if (Zone->autoScrollSpeed < 0x70000) {
        Zone->autoScrollSpeed += 0x2000;
        if (Zone->autoScrollSpeed > 0x70000)
            Zone->autoScrollSpeed = 0x70000;
    }

    for (int32 p = 0; p < Player->playerCount; ++p) {
        EntityPlayer *player = RSDK_GET_ENTITY(p, Player);

        if (player->groundVel < Zone->autoScrollSpeed - 0x20000)
            player->groundVel = Zone->autoScrollSpeed - 0x20000;
        player->direction = FLIP_NONE;
        player->topSpeed  = Zone->autoScrollSpeed + 0x20000;

        if (player->state == Player_State_KnuxGlideLeft) {
            player->timer            = 0;
            player->animator.frameID = 6;
            player->state            = Player_State_KnuxGlideRight;
            player->abilitySpeed     = Zone->autoScrollSpeed - 0x18000;
        }
        else if (player->state == Player_State_KnuxGlideRight) {
            if (player->abilitySpeed < Zone->autoScrollSpeed - 0x18000) {
                player->abilitySpeed = Zone->autoScrollSpeed - 0x18000;
            }
        }

        if (player->groundVel > player->topSpeed)
            player->groundVel = player->topSpeed;
    }
}

void HeavyGunner_StateManager_HandlePathChange(void)
{
    RSDK_THIS(HeavyGunner);

    EntityCamera *camera = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);
    if (camera->position.x <= 0x51800000) {
        for (int32 p = 0; p < Player->playerCount; ++p) {
            EntityPlayer *player = RSDK_GET_ENTITY(p, Player);

            if (player->onGround) {
                if (player->groundVel < 0x80000)
                    player->groundVel = 0x80000;
            }
            else if (player->velocity.x < 0x80000) {
                player->velocity.x = 0x80000;
            }
            player->direction = FLIP_NONE;

            if (player->state == Player_State_KnuxGlideLeft) {
                player->timer            = 0;
                player->animator.frameID = 6;
                player->state            = Player_State_KnuxGlideRight;
                player->abilitySpeed     = Zone->autoScrollSpeed;
            }
            else if (player->state == Player_State_KnuxGlideRight) {
                if (player->abilitySpeed < Zone->autoScrollSpeed) {
                    player->abilitySpeed = Zone->autoScrollSpeed;
                }
            }
        }
    }
    else {
        camera->position.x -= 0x10000000;
        self->position.x = camera->position.x - 0x100000;
        self->position.y = camera->position.y - 0x800000;
        HeavyGunner_HandleBGWrap(0x100000);
        for (int32 p = 0; p < Player->playerCount; ++p) {
            EntityPlayer *player = RSDK_GET_ENTITY(p, Player);
            player->position.x -= 0x10000000;
            if (player->velocity.x > Zone->autoScrollSpeed)
                Zone->autoScrollSpeed = player->velocity.x;
        }

        foreach_active(Ring, ring) { ring->position.x -= 0x10000000; }
        foreach_active(ImageTrail, imageTrail)
        {
            imageTrail->position.x -= 0x10000000;
            imageTrail->currentPos.x -= 0x10000000;
            for (int32 i = 0; i < ImageTrail_TrackCount; ++i) imageTrail->statePos[i].x -= 0x10000000;
        }

        for (int32 p = 0; p < Player->playerCount; ++p) {
            EntityPlayer *player = RSDK_GET_ENTITY(p, Player);

            if (player->stateInput == HeavyGunner_Input_LockedP1) {
                player->stateInput = Player_ProcessP1Input;
            }
            else if (player->stateInput == HeavyGunner_Input_LockedP2) {
                player->stateInput = Player_ProcessP2Input_Player;
            }
            else if (player->stateInput == HeavyGunner_Input_LockedP2_AI) {
                player->stateInput = Player_ProcessP2Input_AI;
            }
        }
        Zone->playerBoundActiveR[0] = true;
        HeavyGunner->stageWrapActive       = true;
        self->state                 = HeavyGunner_StateManager_HandleStageWrap;

        EntityHeavyGunner *heli = RSDK_GET_ENTITY(SceneInfo->entitySlot + 4, HeavyGunner);
        heli->position.x        = camera->position.x - 0x1200000;
        heli->position.y        = camera->position.y - 0xC00000;
        RSDK.SetSpriteAnimation(HeavyGunner->aniFrames, 1, &heli->gunnerAnimator, true, 0);
        heli->state = HeavyGunner_StateHeli_WaitForPlayer;
        for (int32 c = 3 - heli->nextRoboID; c > 0; --c) {
            EntityHeavyGunner *robo = RSDK_GET_ENTITY(SceneInfo->entitySlot + c, HeavyGunner);
            robo->position.x        = camera->position.x - 0x200000;
            robo->position.y        = camera->position.y - 0xC00000;
        }
    }
}

void HeavyGunner_Input_LockedP1(void)
{
    RSDK_THIS(Player);
    Player_ProcessP1Input();

    if (self->state != Player_State_None) {
        self->up        = false;
        self->down      = false;
        self->jumpPress = false;
        self->jumpHold  = false;
    }
}

void HeavyGunner_Input_LockedP2(void)
{
    RSDK_THIS(Player);
    Player_ProcessP2Input_Player();

    self->up        = false;
    self->down      = false;
    self->jumpPress = false;
    self->jumpHold  = false;
    if (self->stateInput == Player_ProcessP2Input_AI)
        self->stateInput = HeavyGunner_Input_LockedP2_AI;
}

void HeavyGunner_Input_LockedP2_AI(void)
{
    RSDK_THIS(Player);
    Player_ProcessP2Input_AI();

    self->up        = false;
    self->down      = false;
    self->jumpPress = false;
    self->jumpHold  = false;
    if (self->stateInput == Player_ProcessP2Input_Player)
        self->stateInput = HeavyGunner_Input_LockedP2;
}

void HeavyGunner_StateEggRobo_Patrolling(void)
{
    RSDK_THIS(HeavyGunner);
    EntityHeavyGunner *parent = self->parent;

    RSDK.ProcessAnimation(&self->mainAnimator);
    RSDK.ProcessAnimation(&self->feetAnimator);
    RSDK.ProcessAnimation(&self->armAnimator);

    int32 angle = 0;
    if (self->position.x <= parent->position.x)
        angle = RSDK.Sin256(self->angle);
    else
        angle = RSDK.Sin256(self->angle) - 64;

    self->position.x += (angle << 9) + Zone->autoScrollSpeed;
    self->rotation = (RSDK.Sin256(self->angle) >> 4) - 32;
    self->angle    = (self->angle + 1) & 0xFF;

    if (self->scale.x < self->targetScale)
        self->scale.x += 2;
    else if (self->scale.x > self->targetScale)
        self->scale.x -= 2;

    self->scale.y = self->scale.x;
    if (self->scale.x <= 0x200)
        self->drawOrder = Zone->objectDrawLow - 1;
    else
        self->drawOrder = Zone->objectDrawHigh;
    if (parent->state == HeavyGunner_StateHeli_EscapeMissile)
        self->velocity.y = -0x8000;
    else
        self->velocity.y += ((parent->position.y + self->parentOffset - self->position.y) >> 8) - (self->velocity.y >> 4);

    self->position.y += self->velocity.y;
    if (--self->timer <= 0) {
        self->timer    = RSDK.Rand(120, 240);
        self->parentOffset = RSDK.Rand(-0x200000, 0x600000);
        switch (RSDK.Rand(0, 8)) {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4: self->targetScale = 0x200; break;
            case 5:
            case 6: self->targetScale = 0x180; break;
            case 7: self->targetScale = 0x300; break;
            default: break;
        }
    }
}

void HeavyGunner_StateEggRobo_FlyIn(void)
{
    RSDK_THIS(HeavyGunner);

    if (++self->timer == 120) {
        self->position.x = self->parent->position.x - 0x1800000;
        self->position.y = self->parent->position.y + 0x800000;
        self->scale.x    = 0x400;
        self->scale.y    = 0x400;
        self->velocity.x = 0x6A000;
        self->drawOrder  = Zone->objectDrawHigh;
        RSDK.PlaySfx(HeavyGunner->sfxFlyIn, false, 0xFF);
        self->state = HeavyGunner_StateEggRobo_ThrowGun;
    }
}

void HeavyGunner_StateEggRobo_ThrowGun(void)
{
    RSDK_THIS(HeavyGunner);

    RSDK.ProcessAnimation(&self->mainAnimator);
    RSDK.ProcessAnimation(&self->feetAnimator);
    RSDK.ProcessAnimation(&self->armAnimator);
    self->velocity.x -= 0x1000;
    self->position.x += self->velocity.x + Zone->autoScrollSpeed;
    self->rotation = (self->velocity.x - 0x40000) >> 13;
    self->position.y += self->velocity.y + ((self->parent->position.y - self->position.y + 0x400000) >> 6);
    if (self->velocity.x < 0 && self->position.x < self->parent->position.x + 0x600000) {
        self->drawOrder = Zone->objectDrawLow - 1;
        RSDK.SetSpriteAnimation(HeavyGunner->aniFrames, 21, &self->armAnimator, true, 0);
        self->state              = HeavyGunner_StateEggRobo_ThrownGun;
        EntityHeavyGunner *child = CREATE_ENTITY(HeavyGunner, intToVoid(HEAVYGUNNER_GUN), self->position.x + 0x80000, self->position.y - 0x80000);
        child->velocity.x        = -0x34000;
        child->velocity.y        = -0x80000;
        child->parent            = self->parent;
        RSDK.PlaySfx(HeavyGunner->sfxThrow, false, 0xFF);
    }

    if (self->scale.x > 0x200) {
        self->scale.x -= 4;
        self->scale.y = self->scale.x;
    }
}

void HeavyGunner_StateEggRobo_ThrownGun(void)
{
    RSDK_THIS(HeavyGunner);

    RSDK.ProcessAnimation(&self->mainAnimator);
    RSDK.ProcessAnimation(&self->feetAnimator);
    RSDK.ProcessAnimation(&self->armAnimator);
    self->position.x += Zone->autoScrollSpeed;
    self->velocity.y += ((self->parent->position.y - self->position.y + 0x400000) >> 8) - (self->velocity.y >> 4);
    self->position.y += self->velocity.y;

    if (self->armAnimator.frameID >= self->armAnimator.frameCount - 1) {
        RSDK.SetSpriteAnimation(HeavyGunner->aniFrames, 18, &self->armAnimator, true, 0);
        self->timer = 0;
        self->state = HeavyGunner_StateEggRobo_Patrolling;
    }
}

void HeavyGunner_StateGun_Thrown(void)
{
    RSDK_THIS(HeavyGunner);
    EntityHeavyGunner *parent = self->parent;

    self->velocity.y += 0x4000;
    self->rotation = (self->rotation - 16) & 0x1FF;
    self->position.x += self->velocity.x + Zone->autoScrollSpeed;
    self->position.y += self->velocity.y;

    if (self->position.x < parent->position.x - 0x180000)
        self->velocity.x = 0;
    if (self->velocity.y > 0) {
        if (self->position.y > parent->position.y - 0x200000) {
            RSDK.SetSpriteAnimation(HeavyGunner->aniFrames, 1, &parent->gunnerAnimator, true, 0);
            RSDK.PlaySfx(Player->sfxGrab, false, 255);
            self->drawOrder = Zone->hudDrawOrder;
            self->state     = HeavyGunner_StateGun_Grabbed;
            self->timer     = 0x200;
            self->stateDraw = HeavyGunner_Draw_FadeOut;
        }
    }
}

void HeavyGunner_StateGun_Grabbed(void)
{
    RSDK_THIS(HeavyGunner);

    self->timer -= 64;
    if (!self->timer)
        destroyEntity(self);
}

void HeavyGunner_StateMissile_Launched(void)
{
    RSDK_THIS(HeavyGunner);

    RSDK.ProcessAnimation(&self->mainAnimator);
    self->velocity.x -= 0x1800;
    self->velocity.y += 0x3800;
    self->position.x += self->velocity.x + Zone->autoScrollSpeed;
    self->position.y += self->velocity.y;
    self->scale.x += ((0x200 - self->scale.x) >> 3);
    self->scale.y = self->scale.x;

    if (self->rotation < 0)
        self->rotation++;

    if (++self->timer == 32) {
        self->scale.x = 0x200;
        self->timer   = self->missileID + 48;
        self->scale.y = 0x200;
        RSDK.SetSpriteAnimation(HeavyGunner->aniFrames, 26, &self->exhaustAnimator, true, 0);
        self->state = HeavyGunner_StateMissile_BlastOff;
        RSDK.PlaySfx(HeavyGunner->sfxJet, false, 255);
    }
}

void HeavyGunner_StateMissile_BlastOff(void)
{
    RSDK_THIS(HeavyGunner);

    RSDK.ProcessAnimation(&self->mainAnimator);
    RSDK.ProcessAnimation(&self->exhaustAnimator);

    if (self->velocity.x < 0x80000)
        self->velocity.x += 0x3800;
    self->position.x += self->velocity.x + Zone->autoScrollSpeed;
    if (self->exhaustAnimator.animationID == 26 && self->exhaustAnimator.frameID == self->exhaustAnimator.frameCount - 1)
        RSDK.SetSpriteAnimation(HeavyGunner->aniFrames, 25, &self->exhaustAnimator, true, 0);

    if (!(Zone->timer & 3)) {
        EntityExplosion *explosion = CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_BOSSPUFF), self->position.x - 0x180000, self->position.y);
        explosion->drawOrder       = Zone->objectDrawLow - 1;
        explosion->drawFX          = FX_ROTATE;
        explosion->rotation        = 128;
    }
    if (--self->timer <= 0) {
        self->timer = 0;
        self->state = HeavyGunner_StateMissile_FindFloor;
    }
}

void HeavyGunner_StateMissile_FindFloor(void)
{
    RSDK_THIS(HeavyGunner);

    self->position.x += Zone->autoScrollSpeed;
    self->position.y += 0x20000;

    if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x100000, true)) {
        self->direction = FLIP_X;
        if (self->type == HEAVYGUNNER_MISSILE)
            self->velocity.x = -0x10000;
        else
            self->velocity.x = -0x20000;
        self->state = HeavyGunner_StateMissile_AttackPlayer;
    }
}

void HeavyGunner_StateMissile_AttackPlayer(void)
{
    RSDK_THIS(HeavyGunner);

    RSDK.ProcessAnimation(&self->mainAnimator);
    RSDK.ProcessAnimation(&self->exhaustAnimator);
    self->position.x += self->velocity.x + Zone->autoScrollSpeed;
    RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x100000, 64);

    Hitbox *hitbox = NULL;
    if (self->type == HEAVYGUNNER_MISSILE)
        hitbox = &HeavyGunner->hitboxMissile;
    else
        hitbox = &HeavyGunner->hitboxMissileF;

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, hitbox)) {
            if (Player_CheckAttacking(player, self) && self->type == HEAVYGUNNER_MISSILE) {
                if (player->onGround) {
                    player->groundVel = Zone->autoScrollSpeed - 0x20000;
                }
#if RETRO_USE_PLUS
                else if (player->state != Player_State_MightyHammerDrop) {
#endif
                    player->velocity.y = -player->velocity.y;
                    if (player->velocity.y > -0x40000)
                        player->velocity.y = -0x40000;
#if RETRO_USE_PLUS
                }
#endif
                self->velocity.x = -0x10000;
                RSDK.SetSpriteAnimation(-1, 0, &self->exhaustAnimator, true, 0);
                self->direction = FLIP_NONE;
                self->rotation  = 256;
                self->state     = HeavyGunner_StateMissile_Malfunction;
                RSDK.PlaySfx(HeavyGunner->sfxHit, false, 255);
                RSDK.PlaySfx(HeavyGunner->sfxFlip, false, 255);
            }
            else {
                Player_CheckHit(player, self);
                RSDK.PlaySfx(HeavyGunner->sfxExplosion3, false, 255);
                int32 x                = RSDK.Cos512(self->rotation) << 11;
                int32 y                = RSDK.Sin512(self->rotation) << 11;
                EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Move, x + self->position.x, y + self->position.y);
                RSDK.SetSpriteAnimation(Explosion->aniFrames, EXPLOSION_BOSS, &debris->animator, true, 0);
                debris->velocity.x = self->velocity.x + Zone->autoScrollSpeed;
                debris->drawOrder  = Zone->objectDrawLow;
                debris->timer      = 41;
                destroyEntity(self);
            }
        }
    }
}

void HeavyGunner_StateMissile_Malfunction(void)
{
    RSDK_THIS(HeavyGunner);

    RSDK.ProcessAnimation(&self->mainAnimator);
    self->position.x += self->velocity.x + Zone->autoScrollSpeed;
    self->position.x += RSDK.Cos512(self->rotation) << 8;
    self->position.y += (RSDK.Sin512(self->rotation) - 0x200) << 8;
    self->rotation += 32;

    if (!(Zone->timer & 3)) {
        int32 x = RSDK.Cos512(self->rotation) << 11;
        int32 y = RSDK.Sin512(self->rotation) << 11;
        CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_BOSSPUFF), self->position.x - x, self->position.y - y);
    }

    if (self->rotation == 0x400) {
        EntityHeavyGunner *parent = self->parent;
        if (parent->gunnerAnimator.animationID == 3) {
            EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Move, self->position.x, self->position.y);
            RSDK.SetSpriteAnimation(Explosion->aniFrames, 2, &debris->animator, true, 0);
            debris->velocity.x = Zone->autoScrollSpeed;
            debris->drawOrder  = Zone->objectDrawHigh;
            debris->drawFX     = FX_SCALE;
            debris->scale.x    = 0x300;
            debris->scale.y    = 0x300;
            debris->timer      = 52;
            --parent->missileID;
            destroyEntity(self);
            RSDK.PlaySfx(HeavyGunner->sfxExplosion3, false, 255);
        }
        else {
            self->rotation   = 0;
            self->state      = HeavyGunner_StateMissile_ReturnToSender;
            self->velocity.x = 0;
            RSDK.SetSpriteAnimation(HeavyGunner->aniFrames, 3, &parent->gunnerAnimator, true, 0);
            RSDK.PlaySfx(HeavyGunner->sfxJet, false, 255);
        }
    }
}

void HeavyGunner_StateMissile_ReturnToSender(void)
{
    RSDK_THIS(HeavyGunner);
    EntityHeavyGunner *parent = self->parent;

    RSDK.ProcessAnimation(&self->mainAnimator);

    self->position.y += self->velocity.y;
    self->position.x += self->velocity.x + Zone->autoScrollSpeed;
    self->velocity.y += ((parent->position.y - self->position.y) >> 8) - (self->velocity.y >> 4);
    if (self->velocity.x >= -0x20000) {
        if (self->velocity.x > 0x70000)
            self->velocity.x = 0x70000;
    }
    else {
        self->velocity.x = -0x20000;
    }

    if (self->rotation <= -0x200 || self->direction) {
        self->rotation  = 0;
        self->direction = FLIP_X;
        self->velocity.x -= RSDK.Cos512(0) << 6;
        if (Zone->timer & 3) {
            if ((Zone->timer & 3) == 2) {
                int32 x                = 0x600 * RSDK.Cos512(self->rotation);
                int32 y                = 0x600 * RSDK.Sin512(self->rotation);
                EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Move, self->position.x - x, self->position.y - y);
                RSDK.SetSpriteAnimation(Explosion->aniFrames, 2, &debris->animator, true, 0);
                debris->velocity.x = Zone->autoScrollSpeed;
                debris->drawOrder  = Zone->objectDrawHigh;
                debris->drawFX     = FX_SCALE;
                debris->scale.x    = RSDK.Rand(0x80, 0x180);
                debris->scale.y    = debris->scale.x;
                debris->timer      = 52;
            }
        }
        else {
            int32 x                = RSDK.Cos512(self->rotation) << 11;
            int32 y                = RSDK.Sin512(self->rotation) << 11;
            EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Move, x + self->position.x, y + self->position.y);
            RSDK.SetSpriteAnimation(Explosion->aniFrames, EXPLOSION_BOSSPUFF, &debris->animator, true, 0);
            debris->velocity.x = (RSDK.Cos512(self->rotation) << 8) + Zone->autoScrollSpeed;
            debris->velocity.y = 16 * RSDK.Cos512(self->rotation);
            debris->drawOrder  = Zone->objectDrawLow;
            debris->timer      = 41;
        }

        if (self->position.x - parent->position.x < 0xA00000) {
            if (parent->nextRoboID >= 3) {
                parent->velocity.y = 0;
                parent->timer      = 0;
                parent->state      = HeavyGunner_StateHeli_IncomingMissile;
                self->state        = HeavyGunner_StateMissile_AttackGunner;
            }
            else {
                parent->nextRoboID++;
                self->parent  = RSDK_GET_ENTITY(RSDK.GetEntityID(parent) - parent->nextRoboID, HeavyGunner);
                parent->state = HeavyGunner_StateHeli_EscapeMissile;
                self->state   = HeavyGunner_StateMissile_AttackRobo;
                RSDK.PlaySfx(HeavyGunner->sfxWooshOut, false, 255);
            }
            self->groundVel = 256;
        }
    }
    else {
        self->rotation -= 4;
        self->velocity.x += RSDK.Cos512(self->rotation) << 6;

        if (Zone->timer & 3) {
            if ((Zone->timer & 3) == 2) {
                int32 x                = 0x600 * RSDK.Cos512(self->rotation);
                int32 y                = 0x600 * RSDK.Sin512(self->rotation);
                EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Move, self->position.x - x, self->position.y - y);
                RSDK.SetSpriteAnimation(Explosion->aniFrames, EXPLOSION_BOSS, &debris->animator, true, 0);
                debris->velocity.x = Zone->autoScrollSpeed;
                debris->drawOrder  = Zone->objectDrawHigh;
                debris->drawFX     = FX_SCALE;
                debris->scale.x    = RSDK.Rand(0x80, 0x180);
                debris->scale.y    = debris->scale.x;
                debris->timer      = 52;
            }
        }
        else {
            int32 x                = RSDK.Cos512(self->rotation) << 11;
            int32 y                = RSDK.Sin512(self->rotation) << 11;
            EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Move, self->position.x - (x << 11), self->position.y - y);
            RSDK.SetSpriteAnimation(Explosion->aniFrames, EXPLOSION_BOSSPUFF, &debris->animator, true, 0);
            debris->velocity.x = Zone->autoScrollSpeed - (RSDK.Cos512(self->rotation) << 8);
            debris->velocity.y = -16 * RSDK.Cos512(self->rotation);
            debris->drawOrder  = Zone->objectDrawLow;
            debris->timer      = 41;
        }
    }
}

void HeavyGunner_StateMissile_AttackRobo(void)
{
    RSDK_THIS(HeavyGunner);
    EntityHeavyGunner *parent = self->parent;

    RSDK.ProcessAnimation(&self->mainAnimator);

    int32 angle = RSDK.ATan2(self->position.x - parent->position.x, self->position.y - parent->position.y);
    self->groundVel += 16;
    self->rotation   = 2 * angle;
    self->velocity.x = self->groundVel * RSDK.Cos512(self->rotation);
    self->velocity.y = self->groundVel * RSDK.Sin512(self->rotation);
    self->position.x += Zone->autoScrollSpeed - self->velocity.x;
    self->position.y -= self->velocity.y;

    if (self->scale.x >= parent->scale.x) {
        if (self->scale.x > parent->scale.x)
            self->scale.x -= 2;
    }
    else {
        self->scale.x += 2;
    }

    self->scale.y   = self->scale.x;
    self->drawOrder = parent->drawOrder;
    if (Zone->timer & 3) {
        if ((Zone->timer & 3) == 2) {
            int32 x                = 0x600 * RSDK.Cos512(self->rotation);
            int32 y                = 0x600 * RSDK.Sin512(self->rotation);
            EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Move, self->position.x - x, self->position.y - y);
            RSDK.SetSpriteAnimation(Explosion->aniFrames, 2, &debris->animator, true, 0);
            debris->velocity.x = Zone->autoScrollSpeed;
            debris->drawOrder  = Zone->objectDrawHigh;
            debris->drawFX     = FX_SCALE;
            debris->scale.x    = RSDK.Rand(0x80, 0x180);
            debris->scale.y    = debris->scale.x;
            debris->timer      = 52;
        }
    }
    else {
        int32 x                = RSDK.Cos512(self->rotation) << 11;
        int32 y                = RSDK.Sin512(self->rotation) << 11;
        EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Move, x + self->position.x, y + self->position.y);
        RSDK.SetSpriteAnimation(Explosion->aniFrames, EXPLOSION_BOSSPUFF, &debris->animator, true, 0);
        debris->velocity.x = (RSDK.Cos512(self->rotation) << 8) + Zone->autoScrollSpeed;
        debris->velocity.y = 16 * RSDK.Cos512(self->rotation);
        debris->drawOrder  = Zone->objectDrawLow;
        debris->timer      = 41;
    }

    int32 rx = (self->position.x - parent->position.x) >> 16;
    int32 ry = (self->position.y - parent->position.y) >> 16;
    if (rx * rx + ry * ry < 0x100) {
        for (int32 i = 0; i < 12; ++i) {
            int32 x                = self->position.x + RSDK.Rand(-0xC0000, 0xC0000);
            int32 y                = self->position.y + RSDK.Rand(-0xC0000, 0xC0000);
            EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Fall, x, y);
            RSDK.SetSpriteAnimation(HeavyGunner->aniFrames, 28, &debris->animator, true, i >> 1);
            debris->velocity.x = Zone->autoScrollSpeed + RSDK.Rand(-0x20000, 0x20000);
            debris->velocity.y = RSDK.Rand(-0x20000, 0x20000) - 0x40000;
            debris->gravityStrength    = 0x3800;
            debris->scale      = parent->scale;
            debris->rotSpeed   = RSDK.Rand(-24, 24);
            debris->drawOrder  = Zone->objectDrawLow;
            if (i >> 1 < 3)
                debris->drawFX = FX_SCALE | FX_ROTATE;
        }

        EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Move, parent->position.x, parent->position.y);
        RSDK.SetSpriteAnimation(Explosion->aniFrames, EXPLOSION_ENEMY, &debris->animator, true, 0);
        debris->velocity.x    = Zone->autoScrollSpeed;
        debris->drawOrder     = Zone->objectDrawHigh;
        debris->timer         = 32;
        HeavyGunner->stageWrapActive = false;

        int32 plane = RSDK.Rand(0, 2);
        foreach_active(Player, player) { player->collisionPlane = plane; }

        --parent->parent->missileID;
        destroyEntity(self);
        destroyEntity(parent);
        RSDK.PlaySfx(HeavyGunner->sfxDestroy, false, 255);
        RSDK.PlaySfx(HeavyGunner->sfxExplosion3, false, 255);
        HeavyGunner_DestroyAllMissiles();
    }
}

void HeavyGunner_StateMissile_AttackGunner(void)
{
    RSDK_THIS(HeavyGunner);
    EntityHeavyGunner *parent = self->parent;

    RSDK.ProcessAnimation(&self->mainAnimator);

    int32 angle = RSDK.ATan2(self->position.x - parent->position.x, self->position.y - parent->position.y);
    self->groundVel += 16;
    self->rotation   = 2 * angle;
    self->velocity.x = self->groundVel * RSDK.Cos512(self->rotation);
    self->velocity.y = self->groundVel * RSDK.Sin512(self->rotation);
    self->position.x += Zone->autoScrollSpeed - self->velocity.x;
    self->position.y -= self->velocity.y;
    if (Zone->timer & 3) {
        if ((Zone->timer & 3) == 2) {
            int32 x                = 0x600 * RSDK.Cos512(self->rotation);
            int32 y                = 0x600 * RSDK.Sin512(self->rotation);
            EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Move, self->position.x - x, self->position.y - y);
            RSDK.SetSpriteAnimation(Explosion->aniFrames, 2, &debris->animator, true, 0);
            debris->velocity.x = Zone->autoScrollSpeed;
            debris->drawOrder  = Zone->objectDrawHigh;
            debris->drawFX     = FX_SCALE;
            debris->scale.x    = RSDK.Rand(0x80, 0x180);
            debris->scale.y    = debris->scale.x;
            debris->timer      = 52;
        }
    }
    else {
        int32 x                = RSDK.Cos512(self->rotation) << 11;
        int32 y                = RSDK.Sin512(self->rotation) << 11;
        EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Move, x + self->position.x, y + self->position.y);
        RSDK.SetSpriteAnimation(Explosion->aniFrames, EXPLOSION_BOSSPUFF, &debris->animator, true, 0);
        debris->velocity.x = (RSDK.Cos512(self->rotation) << 8) + Zone->autoScrollSpeed;
        debris->velocity.y = 16 * RSDK.Cos512(self->rotation);
        debris->drawOrder  = Zone->objectDrawLow;
        debris->timer      = 41;
    }

    int32 rx = (self->position.x - parent->position.x) >> 16;
    int32 ry = (self->position.y - parent->position.y) >> 16;
    if (rx * rx + ry * ry < 0x300) {
        for (int32 i = 0; i < 12; ++i) {
            int32 x                = self->position.x + RSDK.Rand(0x60000, 0x180000);
            int32 y                = self->position.y + RSDK.Rand(-0xC0000, 0xC0000);
            EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Fall, x, y);
            RSDK.SetSpriteAnimation(HeavyGunner->aniFrames, 28, &debris->animator, true, i >> 1);
            debris->velocity.x = Zone->autoScrollSpeed + RSDK.Rand(-0x20000, 0x20000);
            debris->velocity.y = RSDK.Rand(-0x20000, 0x20000) - 0x40000;
            debris->gravityStrength    = 0x3800;
            debris->rotSpeed   = RSDK.Rand(-24, 24);
            debris->drawOrder  = Zone->objectDrawHigh;
            if (i >> 1 < 3)
                debris->drawFX = FX_ROTATE;
        }

        EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Fall, self->position.x, self->position.y - 0x200000);
        RSDK.SetSpriteAnimation(HeavyGunner->aniFrames, 13, &debris->animator, true, 0);
        debris->velocity.x = Zone->autoScrollSpeed + RSDK.Rand(-0x20000, 0x20000);
        debris->velocity.y = RSDK.Rand(-0x20000, 0x20000) - 0x40000;
        debris->gravityStrength    = 0x3800;
        debris->drawOrder  = Zone->objectDrawHigh;

        debris = CREATE_ENTITY(Debris, Debris_State_Move, self->position.x + 0x60000, self->position.y - 0x40000);
        RSDK.SetSpriteAnimation(Explosion->aniFrames, EXPLOSION_BOSS, &debris->animator, true, 0);
        debris->velocity.x = Zone->autoScrollSpeed;
        debris->drawOrder  = Zone->objectDrawHigh;
        debris->drawFX     = FX_SCALE;
        debris->scale.x    = 768;
        debris->scale.y    = 768;
        debris->timer      = 52;

        destroyEntity(self);
        RSDK.SetSpriteAnimation(HeavyGunner->aniFrames, 12, &parent->mainAnimator, true, 0);
        RSDK.SetSpriteAnimation(HeavyGunner->aniFrames, 12, &parent->frontAnimator, true, 1);
        RSDK.SetSpriteAnimation(HeavyGunner->aniFrames, 12, &parent->tBladeAnimator, true, 2);
        RSDK.SetSpriteAnimation(HeavyGunner->aniFrames, 12, &parent->beaconAnimator, true, 3);
        RSDK.SetSpriteAnimation(-1, 0, &parent->pilotAnimator, true, 0);
        --parent->missileID;
        parent->state          = HeavyGunner_StateHeli_Exploding;
        SceneInfo->timeEnabled = false;
        Player_GiveScore(RSDK_GET_ENTITY(SLOT_PLAYER1, Player), 1000);
        RSDK.PlaySfx(HeavyGunner->sfxExplosion3, false, 255);
        self->defeated = true; // ? self was destroyed, this wont do a thing right??
        HeavyGunner_DestroyAllMissiles();
    }
}

void HeavyGunner_StateHeli_WaitForPlayer(void)
{
    RSDK_THIS(HeavyGunner);

    if (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x > self->position.x) {
        self->velocity.x = 0x40000;
        self->velocity.y = 0;
        self->drawFX     = FX_ROTATE;
        self->state      = HeavyGunner_StateHeli_WooshIn;
        Camera_ShakeScreen(0, 12, 12);
        RSDK.PlaySfx(HeavyGunner->sfxRumble, false, 255);
        RSDK.PlaySfx(HeavyGunner->sfxWooshIn, false, 255);
        Zone->deathBoundary[0] += 0x8000000;
    }
}

void HeavyGunner_StateHeli_WooshIn(void)
{
    RSDK_THIS(HeavyGunner);

    RSDK.ProcessAnimation(&self->mainAnimator);
    RSDK.ProcessAnimation(&self->pilotAnimator);
    RSDK.ProcessAnimation(&self->frontAnimator);
    RSDK.ProcessAnimation(&self->tBladeAnimator);
    RSDK.ProcessAnimation(&self->rBladeAnimator);
    RSDK.ProcessAnimation(&self->beaconAnimator);
    RSDK.ProcessAnimation(&self->feetAnimator);
    RSDK.ProcessAnimation(&self->feet2Animator);
    RSDK.ProcessAnimation(&self->gunnerAnimator);

    self->position.x += self->velocity.x;
    self->position.x += Zone->autoScrollSpeed;
    self->position.y += 0x10000;
    self->rotation = (self->velocity.x + 0x40000) >> 13;
    if (self->rotation < 0)
        self->rotation = 0;
    RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x800000, true);
    if (self->position.x > Zone->playerBoundsL[0] + ((ScreenInfo->centerX - 16) << 16))
        self->state = HeavyGunner_StateHeli_FindFloor;
}

void HeavyGunner_StateHeli_FindFloor(void)
{
    RSDK_THIS(HeavyGunner);

    RSDK.ProcessAnimation(&self->mainAnimator);
    RSDK.ProcessAnimation(&self->pilotAnimator);
    RSDK.ProcessAnimation(&self->frontAnimator);
    RSDK.ProcessAnimation(&self->tBladeAnimator);
    RSDK.ProcessAnimation(&self->rBladeAnimator);
    RSDK.ProcessAnimation(&self->beaconAnimator);
    RSDK.ProcessAnimation(&self->feetAnimator);
    RSDK.ProcessAnimation(&self->feet2Animator);
    RSDK.ProcessAnimation(&self->gunnerAnimator);
    self->velocity.x -= 0x1000;
    self->position.x += self->velocity.x;
    self->position.x += Zone->autoScrollSpeed;
    self->position.y += 0x10000;
    self->rotation = (self->velocity.x + 0x40000) >> 13;
    if (self->rotation < 0)
        self->rotation = 0;

    bool32 collided = RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x800000, true);
    if (self->velocity.x < 0 && collided) {
        if (self->position.x < Zone->playerBoundsL[0] + ((ScreenInfo->centerX - 16) << 16)) {
            self->velocity.x = 0;
            self->drawFX     = 0;
            if (self->nextRoboID > 0)
                self->timer = 240;
            self->state = HeavyGunner_StateHeli_HandleAttacks;
        }
    }
}

void HeavyGunner_StateHeli_HandleAttacks(void)
{
    RSDK_THIS(HeavyGunner);

    RSDK.ProcessAnimation(&self->mainAnimator);
    RSDK.ProcessAnimation(&self->pilotAnimator);
    RSDK.ProcessAnimation(&self->frontAnimator);
    RSDK.ProcessAnimation(&self->tBladeAnimator);
    RSDK.ProcessAnimation(&self->rBladeAnimator);
    RSDK.ProcessAnimation(&self->beaconAnimator);
    RSDK.ProcessAnimation(&self->feetAnimator);
    RSDK.ProcessAnimation(&self->feet2Animator);
    RSDK.ProcessAnimation(&self->gunnerAnimator);
    self->position.x += Zone->autoScrollSpeed;
    RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x800000, 64);
    self->position.y += RSDK.Sin256(self->angle) << 11;
    self->angle = (self->angle + 3) & 0xFF;
    if (self->gunnerAnimator.animationID == 3)
        self->timer = 0;

    if (++self->timer == 320) {
        RSDK.SetSpriteAnimation(HeavyGunner->aniFrames, 2, &self->gunnerAnimator, true, 0);
        RSDK.PlaySfx(HeavyGunner->sfxCharge, false, 255);
    }

    if (self->timer == 380) {
        RSDK.SetSpriteAnimation(HeavyGunner->aniFrames, 1, &self->gunnerAnimator, true, 0);
        self->timer      = 0;
        self->state      = HeavyGunner_StateHeli_ShotsFired;
        self->drawFX     = FX_ROTATE;
        self->velocity.x = -0x30000;

        int32 dudID = RSDK.Rand(0, 3);
        EntityHeavyGunner *child =
            CREATE_ENTITY(HeavyGunner, intToVoid((dudID == 0) + HEAVYGUNNER_MISSILE_F), self->position.x - 0x200000, self->position.y - 0x100000);
        child->velocity.y -= 0x10000;
        child->parent   = self;
        child->missileID = 0;
        ++self->missileID;

        child =
            CREATE_ENTITY(HeavyGunner, intToVoid((dudID == 1) + HEAVYGUNNER_MISSILE_F), self->position.x - 0x200000, self->position.y - 0x100000);
        child->parent   = self;
        child->missileID = 15 * ((dudID != 1) + 1);
        ++self->missileID;

        child =
            CREATE_ENTITY(HeavyGunner, intToVoid((dudID == 2) + HEAVYGUNNER_MISSILE_F), self->position.x - 0x200000, self->position.y - 0x100000);
        child->velocity.y += 0x10000;
        child->parent   = self;
        child->missileID = 30 * ((dudID != 2) + 1);
        ++self->missileID;

        EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Move, self->position.x - 0x200000, self->position.y - 0x140000);
        RSDK.SetSpriteAnimation(HeavyGunner->aniFrames, 27, &debris->animator, true, 0);
        debris->timer      = 22;
        debris->drawOrder  = Zone->objectDrawHigh;
        debris->velocity.x = Zone->autoScrollSpeed;
        RSDK.PlaySfx(HeavyGunner->sfxFire, false, 255);
    }
}

void HeavyGunner_StateHeli_ShotsFired(void)
{
    RSDK_THIS(HeavyGunner);

    RSDK.ProcessAnimation(&self->mainAnimator);
    RSDK.ProcessAnimation(&self->pilotAnimator);
    RSDK.ProcessAnimation(&self->frontAnimator);
    RSDK.ProcessAnimation(&self->tBladeAnimator);
    RSDK.ProcessAnimation(&self->rBladeAnimator);
    RSDK.ProcessAnimation(&self->beaconAnimator);
    RSDK.ProcessAnimation(&self->feetAnimator);
    RSDK.ProcessAnimation(&self->feet2Animator);
    RSDK.ProcessAnimation(&self->gunnerAnimator);

    self->velocity.x += 0x1800;
    self->position.x += self->velocity.x + Zone->autoScrollSpeed;
    self->rotation = self->velocity.x >> 14;
    RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x800000, 64);
    self->position.y += RSDK.Sin256(self->angle) << 11;
    self->angle = (self->angle + 3) & 0xFF;
    if (self->rotation > 0)
        self->rotation = 0;

    if (self->position.x > Zone->playerBoundsL[0] + ((ScreenInfo->centerX - 16) << 16)) {
        self->velocity.x = 0;
        self->drawFX     = 0;
        self->state      = HeavyGunner_StateHeli_HandleAttacks;
    }
}

void HeavyGunner_StateHeli_EscapeMissile(void)
{
    RSDK_THIS(HeavyGunner);

    RSDK.ProcessAnimation(&self->mainAnimator);
    RSDK.ProcessAnimation(&self->pilotAnimator);
    RSDK.ProcessAnimation(&self->frontAnimator);
    RSDK.ProcessAnimation(&self->tBladeAnimator);
    RSDK.ProcessAnimation(&self->rBladeAnimator);
    RSDK.ProcessAnimation(&self->beaconAnimator);
    RSDK.ProcessAnimation(&self->feetAnimator);
    RSDK.ProcessAnimation(&self->feet2Animator);
    RSDK.ProcessAnimation(&self->gunnerAnimator);
    self->position.x += self->velocity.x + Zone->autoScrollSpeed;
    self->position.y += self->velocity.y;
    self->velocity.y -= 0x4000;
}

void HeavyGunner_StateHeli_IncomingMissile(void)
{
    RSDK_THIS(HeavyGunner);

    RSDK.ProcessAnimation(&self->mainAnimator);
    RSDK.ProcessAnimation(&self->pilotAnimator);
    RSDK.ProcessAnimation(&self->frontAnimator);
    RSDK.ProcessAnimation(&self->tBladeAnimator);
    RSDK.ProcessAnimation(&self->rBladeAnimator);
    RSDK.ProcessAnimation(&self->beaconAnimator);
    RSDK.ProcessAnimation(&self->feetAnimator);
    RSDK.ProcessAnimation(&self->feet2Animator);

    self->position.x += self->velocity.x + Zone->autoScrollSpeed;
    self->velocity.x -= 0xE00;
    if (self->velocity.y < 0x400)
        self->velocity.y += 16;
    self->position.y += self->velocity.y * RSDK.Sin256(self->angle);
    self->angle = (self->angle + 6) & 0xFF;

    if (!self->timer) {
        self->timer = 1;
        RSDK.SetSpriteAnimation(-1, 0, &self->gunnerAnimator, true, 0);
        CREATE_ENTITY(HeavyGunner, intToVoid(HEAVYGUNNER_ESCAPE_HBH), self->position.x - 0x80000, self->position.y - 0x20000);

        EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Fall, self->position.x - 0x300000, self->position.y - 0x240000);
        RSDK.SetSpriteAnimation(HeavyGunner->aniFrames, 20, &debris->animator, true, 0);
        debris->velocity.y = -0x20000;
        debris->velocity.x = Zone->autoScrollSpeed - 0x20000;
        debris->gravityStrength    = 0x3800;
        debris->drawFX     = FX_ROTATE;
        debris->rotSpeed   = -16;
        debris->drawOrder  = Zone->objectDrawHigh;
    }
}

void HeavyGunner_StateHeli_Exploding(void)
{
    RSDK_THIS(HeavyGunner);

    RSDK.ProcessAnimation(&self->mainAnimator);
    RSDK.ProcessAnimation(&self->rBladeAnimator);
    RSDK.ProcessAnimation(&self->feetAnimator);
    RSDK.ProcessAnimation(&self->feet2Animator);

    self->position.x += self->velocity.x + Zone->autoScrollSpeed;
    if (self->velocity.x < 0x8000)
        self->velocity.x += 0xE00;
    ++self->timer;
    self->position.y += self->velocity.y * RSDK.Sin256(self->angle);
    self->angle = (self->angle + 6) & 0xFF;

    if ((self->timer & 7) == 2)
        RSDK.PlaySfx(HeavyGunner->sfxExplosion2, false, 255);

    if (!(self->timer & 0x1F))
        RSDK.PlaySfx(HeavyGunner->sfxHit, false, 255);

    if (!(self->timer & 3)) {
        int32 x = self->position.x + RSDK.Rand(-0x300000, 0x300000);
        int32 y = self->position.y + RSDK.Rand(-0x300000, 0x300000);

        EntityExplosion *explosion = CREATE_ENTITY(Explosion, intToVoid(RSDK.Rand(EXPLOSION_BOSS, EXPLOSION_BOSSPUFF + 1)), x, y);
        explosion->drawOrder       = Zone->objectDrawHigh;
        explosion->drawFX          = FX_SCALE;
        explosion->scale.x         = RSDK.Rand(0x100, 0x300);
        explosion->scale.y         = explosion->scale.x;
    }

    if (self->timer > 60) {
        self->timer = 0;

        EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Fall, self->position.x, self->position.y);
        RSDK.SetSpriteAnimation(HeavyGunner->aniFrames, 11, &debris->animator, true, self->feetAnimator.frameID);
        debris->velocity.y = -0x10000;
        debris->velocity.x = Zone->autoScrollSpeed - 0x10000;
        debris->gravityStrength    = 0x3800;
        debris->drawOrder  = Zone->objectDrawHigh;

        debris = CREATE_ENTITY(Debris, Debris_State_Fall, self->position.x + 0x1B0000, self->position.y);
        RSDK.SetSpriteAnimation(HeavyGunner->aniFrames, 11, &debris->animator, true, self->feet2Animator.frameID);
        debris->velocity.y = -0x10000;
        debris->velocity.x = Zone->autoScrollSpeed + 0x10000;
        debris->gravityStrength    = 0x3800;
        debris->drawOrder  = Zone->objectDrawLow - 1;
        RSDK.SetSpriteAnimation(-1, 0, &self->feetAnimator, true, 0);
        RSDK.SetSpriteAnimation(-1, 0, &self->feet2Animator, true, 0);
        self->state = HeavyGunner_StateHeli_ExplodeAndFall;
    }
}

void HeavyGunner_StateHeli_ExplodeAndFall(void)
{
    RSDK_THIS(HeavyGunner);

    RSDK.ProcessAnimation(&self->mainAnimator);
    RSDK.ProcessAnimation(&self->rBladeAnimator);
    self->position.x += self->velocity.x + Zone->autoScrollSpeed;
    self->velocity.y += 0x3800;
    self->position.y += self->velocity.y;
    if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x200000, true))
        self->velocity.y = -(self->velocity.y >> 1);

    if ((++self->timer & 7) == 2)
        RSDK.PlaySfx(HeavyGunner->sfxExplosion2, false, 255);
    if (!(self->timer & 0x1F))
        RSDK.PlaySfx(HeavyGunner->sfxHit, false, 255);

    if (!(self->timer & 3)) {
        int32 x                      = self->position.x + RSDK.Rand(-0x300000, 0x300000);
        int32 y                      = self->position.y + RSDK.Rand(-0x300000, 0x300000);
        EntityExplosion *explosion = CREATE_ENTITY(Explosion, intToVoid(RSDK.Rand(EXPLOSION_BOSS, EXPLOSION_BOSSPUFF + 1)), x, y);
        explosion->drawOrder       = Zone->objectDrawHigh;
        explosion->drawFX          = FX_SCALE;
        explosion->scale.x         = RSDK.Rand(0x100, 0x200);
        explosion->scale.y         = explosion->scale.x;
    }

    if (self->timer > 200) {
        self->drawOrder = Zone->hudDrawOrder;
        self->state     = HeavyGunner_StateHeli_FadeOutDestroy;
        self->timer     = 768;
        self->stateDraw = HeavyGunner_Draw_FadeOut;
        Music_TransitionTrack(TRACK_STAGE, 0.0125);
        RSDK.PlaySfx(HeavyGunner->sfxExplosion3, false, 255);

        EntityCamera *camera = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);
        camera->position.x   = 0x57C00000;
        if (camera->position.y <= 0x6600000) {
            camera->position.y = 0x2EC0000;
            if (camera->position.y > 0x3800000)
                camera->position.y = 0x5EC0000;
        }
        else {
            camera->position.y = 0x8EC0000;
        }

        foreach_active(Player, player)
        {
            player->position.x = 0x57C00000;
            player->position.y = camera->position.y;
        }

        foreach_all(SignPost, signPost)
        {
            signPost->position.y   = camera->position.y;
            Zone->cameraBoundsL[0] = (signPost->position.x >> 16) - 512;
            RSDK.ObjectTileGrip(signPost, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x180000, 0x40);
        }

        Zone->autoScrollSpeed       = 0;
        Zone->cameraBoundsR[0]      = HeavyGunner->boundsR;
        Zone->playerBoundActiveR[0] = false;
        destroyEntitySlot(SceneInfo->entitySlot - 4);
    }
}

void HeavyGunner_StateEscapeHBH_Hover(void)
{
    RSDK_THIS(HeavyGunner);

    RSDK.ProcessAnimation(&self->mainAnimator);
    self->velocity.y += 0x3800;
    self->position.x += self->velocity.x + Zone->autoScrollSpeed;
    self->position.y += self->velocity.y;

    if (++self->timer == 30) {
        self->timer = 0;
        RSDK.SetSpriteAnimation(HeavyGunner->aniFrames, 4, &self->mainAnimator, true, 0);
        self->state = HeavyGunner_StateEscapeHBH_FlyAway;
        RSDK.PlaySfx(HeavyGunner->sfxJet, false, 0xFF);
    }
}

void HeavyGunner_StateEscapeHBH_FlyAway(void)
{
    RSDK_THIS(HeavyGunner);

    RSDK.ProcessAnimation(&self->mainAnimator);
    self->velocity.y -= 0x6000;
    self->position.x += self->velocity.x + Zone->autoScrollSpeed;
    self->position.y += self->velocity.y;
    if (++self->timer == 60)
        destroyEntity(self);
}

void HeavyGunner_StateHeli_FadeOutDestroy(void)
{
    RSDK_THIS(HeavyGunner);

    self->timer -= 8;
    if (!self->timer)
        destroyEntity(self);
}

#if RETRO_INCLUDE_EDITOR
void HeavyGunner_EditorDraw(void)
{
    RSDK_THIS(HeavyGunner);

    RSDK.SetSpriteAnimation(HeavyGunner->aniFrames, 6, &self->mainAnimator, true, 0);
    RSDK.SetSpriteAnimation(HeavyGunner->aniFrames, 14, &self->pilotAnimator, true, 0);
    RSDK.SetSpriteAnimation(HeavyGunner->aniFrames, 7, &self->frontAnimator, true, 0);
    RSDK.SetSpriteAnimation(HeavyGunner->aniFrames, 8, &self->tBladeAnimator, true, 0);
    RSDK.SetSpriteAnimation(HeavyGunner->aniFrames, 9, &self->rBladeAnimator, true, 0);
    RSDK.SetSpriteAnimation(HeavyGunner->aniFrames, 10, &self->beaconAnimator, true, 0);
    RSDK.SetSpriteAnimation(HeavyGunner->aniFrames, 11, &self->feetAnimator, true, 0);
    RSDK.SetSpriteAnimation(HeavyGunner->aniFrames, 11, &self->feet2Animator, true, 2);
    RSDK.SetSpriteAnimation(HeavyGunner->aniFrames, 0, &self->gunnerAnimator, true, 0);
    self->drawFX   = FX_ROTATE;
    self->rotation = 64;

    HeavyGunner_Draw_Heli();
}

void HeavyGunner_EditorLoad(void) { HeavyGunner->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/Boss.bin", SCOPE_STAGE); }
#endif

void HeavyGunner_Serialize(void) {}
