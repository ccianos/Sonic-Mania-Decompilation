// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: ItemBox Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectItemBox *ItemBox;

void ItemBox_Update(void)
{
    RSDK_THIS(ItemBox);
    StateMachine_Run(self->state);

#if RETRO_USE_PLUS
    if (self->type == ITEMBOX_STOCK) {
        if (self->contentsAnimator.animationID == 2 || self->contentsAnimator.animationID == 7 || self->contentsAnimator.animationID == 8) {
            if (globals->characterFlags == 0x1F && globals->gameMode == MODE_ENCORE) {
                RSDK.SetSpriteAnimation(ItemBox->aniFrames, 8, &self->contentsAnimator, false, 0);
            }
            else {
                RSDK.SetSpriteAnimation(ItemBox->aniFrames, 7, &self->contentsAnimator, false, 0);
                if (globals->gameMode == MODE_ENCORE) {
                    int32 id = 0;
                    while ((1 << self->contentsAnimator.frameID) & globals->characterFlags) {
                        if (++self->contentsAnimator.frameID > 4)
                            self->contentsAnimator.frameID = 0;
                        if (++id > 5) {
                            RSDK.SetSpriteAnimation(ItemBox->aniFrames, 8, &self->contentsAnimator, false, 0);
                            RSDK.PrintLog(PRINT_NORMAL, "Bad Change Item State");
                        }
                    }
                }
            }
        }
    }
#endif
}

void ItemBox_LateUpdate(void) {}

void ItemBox_StaticUpdate(void)
{
    if (!(Zone->timer & 3)) {
        RSDK.RotatePalette(0, 60, 63, true);
        RSDK.RotatePalette(1, 60, 63, true);
    }
}

void ItemBox_Draw(void)
{
    RSDK_THIS(ItemBox);
    if (!self->hidden) {
        if (self->isContents) {
            if (SceneInfo->currentDrawGroup == Zone->playerDrawHigh) {
                self->drawFX = FX_NONE;
                RSDK.DrawSprite(&self->contentsAnimator, &self->contentsPos, false);
            }
            else {
                self->drawFX    = FX_FLIP;
                self->inkEffect = INK_NONE;
                RSDK.DrawSprite(&self->boxAnimator, NULL, false);

                RSDK.AddDrawListRef(Zone->playerDrawHigh, SceneInfo->entitySlot);
            }
        }
        else {
            self->inkEffect = INK_NONE;
            RSDK.DrawSprite(&self->boxAnimator, NULL, false);
            RSDK.DrawSprite(&self->contentsAnimator, &self->contentsPos, false);

            self->inkEffect = INK_ADD;
            RSDK.DrawSprite(&self->overlayAnimator, NULL, false);

            self->inkEffect = INK_NONE;
            RSDK.DrawSprite(&self->debrisAnimator, NULL, false);
        }
    }
}

void ItemBox_Create(void *data)
{
    RSDK_THIS(ItemBox);
    if (data)
        self->type = voidToInt(data);
    if (self->state != ItemBox_State_Broken) {
        RSDK.SetSpriteAnimation(ItemBox->aniFrames, 0, &self->boxAnimator, true, 0);
        RSDK.SetSpriteAnimation(ItemBox->aniFrames, 2, &self->contentsAnimator, true, 0);
        RSDK.SetSpriteAnimation(ItemBox->aniFrames, 3, &self->overlayAnimator, true, 0);
        RSDK.SetSpriteAnimation(ItemBox->aniFrames, 4, &self->debrisAnimator, true, 0);

        EntityPlayer *player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        switch (self->type) {
            case ITEMBOX_1UP_SONIC:
            case ITEMBOX_1UP_TAILS:
            case ITEMBOX_1UP_KNUX:
#if RETRO_USE_PLUS
            case ITEMBOX_1UP_MIGHTY:
            case ITEMBOX_1UP_RAY:
#endif
                if (globals->gameMode == MODE_TIMEATTACK) {
                    self->type = ITEMBOX_RING;
                }
#if RETRO_USE_PLUS
                else if (globals->gameMode == MODE_ENCORE) {
                    self->type = ITEMBOX_STOCK;
                }
#endif
                else {
                    switch (player->characterID) {
                        case ID_SONIC: self->type = ITEMBOX_1UP_SONIC; break;
                        case ID_TAILS: self->type = ITEMBOX_1UP_TAILS; break;
                        case ID_KNUCKLES: self->type = ITEMBOX_1UP_KNUX; break;
#if RETRO_USE_PLUS
                        case ID_MIGHTY: self->type = ITEMBOX_1UP_MIGHTY; break;
                        case ID_RAY: self->type = ITEMBOX_1UP_RAY; break;
#endif
                        default: break;
                    }
                }
                self->contentsAnimator.frameID = self->type;
                break;
#if RETRO_USE_PLUS
            case ITEMBOX_SWAP:
            case ITEMBOX_RANDOM:
                if (globals->gameMode == MODE_ENCORE || globals->gameMode == MODE_COMPETITION)
                    self->contentsAnimator.frameID = self->type;
                else
                    destroyEntity(self);
#endif
                break;
            default: self->contentsAnimator.frameID = self->type; break;
        }
    }

    self->drawFX = FX_FLIP;
    if (!SceneInfo->inEditor) {
        self->direction *= FLIP_Y;
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x400000;
        self->updateRange.y = 0x400000;
        self->visible       = true;
        if (self->planeFilter > 0 && ((uint8)self->planeFilter - 1) & 2)
            self->drawOrder = Zone->objectDrawHigh;
        else
            self->drawOrder = Zone->objectDrawLow;

        self->alpha = 0xFF;
        if (self->state == ItemBox_State_Broken) {
            RSDK.SetSpriteAnimation(ItemBox->aniFrames, 1, &self->boxAnimator, true, self->boxAnimator.frameID);
        }
        else if (self->hidden) {
            self->state = StateMachine_None;
        }
        else if (self->isFalling) {
            self->active = ACTIVE_NORMAL;
            self->state  = ItemBox_State_Falling;
        }
        else if (LRZConvItem && self->lrzConvPhys) {
            self->active = ACTIVE_NORMAL;
            self->state  = ItemBox_State_Conveyor;
        }
        else {
            self->state = ItemBox_State_Normal;
        }
    }
}

void ItemBox_StageLoad(void)
{
    ItemBox->aniFrames           = RSDK.LoadSpriteAnimation("Global/ItemBox.bin", SCOPE_STAGE);
    ItemBox->hitbox.left         = -15;
    ItemBox->hitbox.top          = -16;
    ItemBox->hitbox.right        = 15;
    ItemBox->hitbox.bottom       = 16;
    ItemBox->hiddenHitbox.left   = -15;
    ItemBox->hiddenHitbox.top    = -24;
    ItemBox->hiddenHitbox.right  = 15;
    ItemBox->hiddenHitbox.bottom = 16;

    DEBUGMODE_ADD_OBJ(ItemBox);

    if (globals->gameMode == MODE_COMPETITION) {
        if (globals->itemMode == ITEMS_RANDOM) {
            foreach_all(ItemBox, entity) { entity->type = ITEMBOX_RANDOM; }
        }
        else if (globals->itemMode == ITEMS_TELEPORT) {
            foreach_all(ItemBox, entity) { entity->type = ITEMBOX_SWAP; }
        }
    }

    ItemBox->sfxDestroy   = RSDK.GetSfx("Global/Destroy.wav");
    ItemBox->sfxTeleport  = RSDK.GetSfx("Global/Teleport.wav");
    ItemBox->sfxHyperRing = RSDK.GetSfx("Global/HyperRing.wav");
#if RETRO_USE_PLUS
    ItemBox->sfxPowerDown = RSDK.GetSfx("Stage/PowerDown.wav");
    ItemBox->sfxRevovery  = RSDK.GetSfx("Global/Recovery.wav");
#endif
}

void ItemBox_DebugDraw(void)
{
    RSDK_THIS(ItemBox);
    DebugMode->subtypeCount = ITEMBOX_COUNT;
    RSDK.SetSpriteAnimation(ItemBox->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);

    RSDK.SetSpriteAnimation(ItemBox->aniFrames, 2, &DebugMode->animator, true, DebugMode->itemSubType);
    Vector2 drawPos;
    drawPos.x = self->position.x;
    drawPos.y = self->position.y - 0x30000;
    RSDK.DrawSprite(&DebugMode->animator, &drawPos, false);
}
void ItemBox_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);

    EntityItemBox *itemBox            = CREATE_ENTITY(ItemBox, NULL, self->position.x, self->position.y);
    itemBox->type                     = DebugMode->itemSubType;
    itemBox->contentsAnimator.frameID = DebugMode->itemSubType;
}

void ItemBox_State_Broken(void)
{
    RSDK_THIS(ItemBox);
    if (LRZConvItem)
        LRZConvItem_HandleLRZConvPhys(self);
    ItemBox_HandleFallingCollision();
}
void ItemBox_State_ContentsShown(void)
{
    RSDK_THIS(ItemBox);
    if (LRZConvItem && self->lrzConvPhys) {
        LRZConvItem_HandleLRZConvPhys(self);
    }
    else {
        ItemBox_HandleFallingCollision();
    }

    if (self->contentsSpeed < 0) {
        self->contentsPos.y += self->contentsSpeed;
        self->contentsSpeed += 0x1800;
    }
    if (self->contentsSpeed >= 0) {
        self->contentsSpeed = 0;
        ItemBox_GivePowerup();
        RSDK.SetSpriteAnimation(ItemBox->aniFrames, 5, &self->contentsAnimator, true, 0);
        self->state = ItemBox_State_ContentsDisappear;
    }
}

void ItemBox_State_ContentsDisappear(void)
{
    RSDK_THIS(ItemBox);
    if (LRZConvItem && self->lrzConvPhys) {
        LRZConvItem_HandleLRZConvPhys(self);
    }
    else {
        ItemBox_HandleFallingCollision();
    }
    RSDK.ProcessAnimation(&self->contentsAnimator);

    if (self->contentsAnimator.frameID == self->contentsAnimator.frameCount - 1) {
        RSDK.SetSpriteAnimation(-1, 0, &self->contentsAnimator, true, 0);
        self->state = ItemBox_State_Broken;
    }
}

void ItemBox_State_Normal(void)
{
    RSDK_THIS(ItemBox);
    self->contentsPos.x = self->position.x;

    if (self->direction == FLIP_NONE)
        self->contentsPos.y = self->position.y - 0x30000;
    else
        self->contentsPos.y = self->position.y + 0x30000;
    ItemBox_HandleObjectCollisions();
    ItemBox_CheckHit();
    RSDK.ProcessAnimation(&self->overlayAnimator);
#if RETRO_USE_PLUS
    if (self->type == ITEMBOX_STOCK) {
        RSDK.ProcessAnimation(&self->contentsAnimator);
        if (!API.CheckDLC(DLC_PLUS) && self->contentsAnimator.frameID >= 3)
            self->contentsAnimator.frameID = 0;
    }
#endif

    if (self->timer) {
        self->timer--;
    }
    else {
        RSDK.ProcessAnimation(&self->debrisAnimator);
        if (!self->debrisAnimator.frameID) {
            self->timer                        = RSDK.Rand(1, 15);
            self->debrisAnimator.frameDuration = RSDK.Rand(1, 32);
        }
    }
}
void ItemBox_State_Falling(void)
{
    RSDK_THIS(ItemBox);
    if (ItemBox_HandleFallingCollision())
        self->state = ItemBox_State_Normal;

    self->contentsPos.x = self->position.x;
    if (self->direction == FLIP_NONE)
        self->contentsPos.y = self->position.y - 0x30000;
    else
        self->contentsPos.y = self->position.y + 0x30000;

    ItemBox_CheckHit();
    RSDK.ProcessAnimation(&self->overlayAnimator);
#if RETRO_USE_PLUS
    if (self->type == ITEMBOX_STOCK) {
        RSDK.ProcessAnimation(&self->contentsAnimator);
        if (!API.CheckDLC(DLC_PLUS) && self->contentsAnimator.frameID >= 3)
            self->contentsAnimator.frameID = 0;
    }
#endif

    if (self->timer) {
        self->timer--;
    }
    else {
        RSDK.ProcessAnimation(&self->debrisAnimator);
        if (!self->debrisAnimator.frameID) {
            self->timer                        = RSDK.Rand(1, 15);
            self->debrisAnimator.frameDuration = RSDK.Rand(1, 32);
        }
    }
}
void ItemBox_State_Conveyor(void)
{
    RSDK_THIS(ItemBox);

    self->moveOffset    = LRZConvItem_HandleLRZConvPhys(self);
    self->contentsPos.x = self->position.x;
    if (self->direction == FLIP_NONE)
        self->contentsPos.y = self->position.y - 0x30000;
    else
        self->contentsPos.y = self->position.y + 0x30000;

    ItemBox_CheckHit();
    RSDK.ProcessAnimation(&self->overlayAnimator);
#if RETRO_USE_PLUS
    if (self->type == ITEMBOX_STOCK) {
        RSDK.ProcessAnimation(&self->contentsAnimator);
        if (!API.CheckDLC(DLC_PLUS) && self->contentsAnimator.frameID >= 3)
            self->contentsAnimator.frameID = 0;
    }
#endif

    if (self->timer) {
        self->timer--;
    }
    else {
        RSDK.ProcessAnimation(&self->debrisAnimator);
        if (!self->debrisAnimator.frameID) {
            self->timer                        = RSDK.Rand(1, 15);
            self->debrisAnimator.frameDuration = RSDK.Rand(1, 32);
        }
    }
}

void ItemBox_CheckHit(void)
{
    RSDK_THIS(ItemBox);
    foreach_active(Player, player)
    {
        if (self->planeFilter <= 0 || player->collisionPlane == (((uint8)self->planeFilter - 1) & 1)) {
#if RETRO_USE_PLUS
            if (player->characterID == ID_MIGHTY && player->jumpAbilityState > 1 && !self->parent) {
                if (RSDK.CheckObjectCollisionTouchCircle(player, 0x1000000, self, 0x100000)) {
                    if (self->position.y - 0x800000 < player->position.y && self->state != ItemBox_State_Falling) {
                        self->active = ACTIVE_NORMAL;
                        if (!self->lrzConvPhys)
                            self->state = ItemBox_State_Falling;
                        self->velocity.y = -0x20000;
                    }
                }
            }
#endif

            if (player->sidekick) {
                self->position.x -= self->moveOffset.x;
                self->position.y -= self->moveOffset.y;
                int32 px           = player->position.x;
                int32 py           = player->position.y;
                uint8 side         = Player_CheckCollisionBox(player, self, &ItemBox->hitbox);
                player->position.x = px;
                player->position.y = py;
                self->position.x += self->moveOffset.x;
                self->position.y += self->moveOffset.y;

                if (side == C_BOTTOM) {
                    self->active = ACTIVE_ALWAYS;
                    if (!self->lrzConvPhys)
                        self->state = ItemBox_State_Falling;
                    self->velocity.y = -0x20000;
                    if (!player->onGround)
                        player->velocity.y = 0x20000;
                }
                else if (side == C_TOP) {
                    player->position.x += self->moveOffset.x;
                    player->position.y += self->moveOffset.y;
                }

                if (Player_CheckCollisionBox(player, self, &ItemBox->hitbox) == C_BOTTOM) {
                    if (player->onGround) {
                        player->position.x = px;
                        player->position.y = py;
                    }
                }
            }
            else {
                int32 anim = player->animator.animationID;
                bool32 attacking =
                    anim == ANI_JUMP && (player->velocity.y >= 0 || player->onGround || self->direction || player->state == Ice_State_FrozenPlayer);
                switch (player->characterID) {
                    case ID_SONIC: attacking |= anim == ANI_DROPDASH; break;
                    case ID_KNUCKLES: attacking |= anim == ANI_FLY || anim == ANI_FLYLIFTTIRED; break;
#if RETRO_USE_PLUS
                    case ID_MIGHTY: attacking |= anim == ANI_DROPDASH || player->jumpAbilityState > 1; break;
#endif
                }

                if (!attacking) {
                    self->position.x -= self->moveOffset.x;
                    self->position.y -= self->moveOffset.y;
                    int32 px           = player->position.x;
                    int32 py           = player->position.y;
                    uint8 side         = Player_CheckCollisionBox(player, self, &ItemBox->hitbox);
                    player->position.x = px;
                    player->position.y = py;
                    self->position.x += self->moveOffset.x;
                    self->position.y += self->moveOffset.y;

                    if (side == C_BOTTOM) {
                        self->active = ACTIVE_ALWAYS;
                        if (!self->lrzConvPhys)
                            self->state = ItemBox_State_Falling;
                        self->velocity.y = -0x20000;
                        if (!player->onGround)
                            player->velocity.y = 0x20000;
                    }
                    else if (side == C_TOP) {
                        player->position.x += self->moveOffset.x;
                        player->position.y += self->moveOffset.y;
                    }

                    if (Player_CheckCollisionBox(player, self, &ItemBox->hitbox) == C_BOTTOM) {
                        if (player->onGround) {
                            player->position.x = px;
                            player->position.y = py;
                        }
                    }
                }
                else if (Player_CheckBadnikTouch(player, self, &ItemBox->hitbox)) {
                    ItemBox_Break(self, player);
                    foreach_break;
                }
            }
        }
    }
}
void ItemBox_GivePowerup(void)
{
    RSDK_THIS(ItemBox);
    EntityPlayer *player = (EntityPlayer *)self->storedEntity;

    switch (self->type) {
        case ITEMBOX_RING: Player_GiveRings(player, 10, true); break;

        case ITEMBOX_BLUESHIELD:
            player->shield = SHIELD_BLUE;
            Player_ApplyShieldEffect(player);
            RSDK.PlaySfx(Shield->sfxBlueShield, false, 255);
            break;

        case ITEMBOX_BUBBLESHIELD:
            player->shield = SHIELD_BUBBLE;
            Player_ApplyShieldEffect(player);
            RSDK.PlaySfx(Shield->sfxBubbleShield, false, 255);
            player->drownTimer = 0;
            Music_EndQueuedTrack(TRACK_DROWNING, false);
            break;

        case ITEMBOX_FIRESHIELD:
            player->shield = SHIELD_FIRE;
            Player_ApplyShieldEffect(player);
            RSDK.PlaySfx(Shield->sfxFireShield, false, 255);
            break;

        case ITEMBOX_LIGHTNINGSHIELD:
            player->shield = SHIELD_LIGHTNING;
            Player_ApplyShieldEffect(player);
            RSDK.PlaySfx(Shield->sfxLightningShield, false, 255);
            break;

        case ITEMBOX_INVINCIBLE:
            if (player->superState == SUPERSTATE_NONE) {
                EntityInvincibleStars *invincibleStars = RSDK_GET_ENTITY(Player->playerCount + RSDK.GetEntityID(player), InvincibleStars);
                RSDK.ResetEntityPtr(invincibleStars, InvincibleStars->objectID, player);
                player->invincibleTimer = 1260;
                Music_PlayQueuedTrack(TRACK_INVINCIBLE);
            }
            break;

        case ITEMBOX_SNEAKERS:
            player->speedShoesTimer = 1320;
            Player_UpdatePhysicsState(player);
            if (player->superState == SUPERSTATE_NONE) {
                Music_PlayQueuedTrack(TRACK_SNEAKERS);
                EntityImageTrail *powerup = RSDK_GET_ENTITY(2 * Player->playerCount + RSDK.GetEntityID(player), ImageTrail);
                RSDK.ResetEntityPtr(powerup, ImageTrail->objectID, player);
            }
            break;

        case ITEMBOX_1UP_SONIC:
        case ITEMBOX_1UP_TAILS:
        case ITEMBOX_1UP_KNUX:
#if RETRO_USE_PLUS
        case ITEMBOX_1UP_MIGHTY:
        case ITEMBOX_1UP_RAY:
#endif
            Player_GiveLife(player);
            break;

        case ITEMBOX_EGGMAN: Player_CheckHit(player, self); return;
        case ITEMBOX_HYPERRING:
            RSDK.PlaySfx(ItemBox->sfxHyperRing, false, 255);
            player->hyperRing = true;
            break;

        case ITEMBOX_SWAP:
#if RETRO_USE_PLUS
            if (globals->gameMode == MODE_ENCORE) {
                if (!globals->stock || player->animator.animationID == ANI_TRANSFORM) {
                    RSDK.PlaySfx(Player->sfxSwapFail, false, 255);
                }
                else {
                    int32 charID = player->characterID;
                    Player_ChangeCharacter(player, (uint8)globals->stock);
                    globals->stock >>= 8;

                    if (globals->stock) {
                        charID <<= 8;
                        if (globals->stock & 0xFF00)
                            charID <<= 8;
                    }
                    globals->stock |= charID;
                    EntityExplosion *explosion = CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_ENEMY), player->position.x, player->position.y);
                    explosion->drawOrder       = Zone->objectDrawHigh;
                    RSDK.PlaySfx(ItemBox->sfxPowerDown, false, 255);
                }
            }
            else if (globals->gameMode != MODE_COMPETITION) {
                RSDK.PlaySfx(Player->sfxSwapFail, false, 255);
            }
            else {
#endif
                Zone_StartTeleportAction();
                RSDK.PlaySfx(ItemBox->sfxTeleport, false, 255);
#if RETRO_USE_PLUS
            }
#endif
            break;

#if RETRO_USE_PLUS
        case ITEMBOX_RANDOM: {
            uint8 playerIDs[5]    = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
            uint8 newPlayerIDs[5] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
            if (player->animator.animationID == ANI_TRANSFORM) {
                RSDK.PlaySfx(Player->sfxSwapFail, false, 255);
            }
            else {
                EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
                EntityPlayer *player2 = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);

                int32 charID = -1;
                for (int32 i = player1->characterID; i > 0; ++charID) i >>= 1;
                playerIDs[0] = charID;

                charID = -1;
                for (int32 i = player2->characterID; i > 0; ++charID) i >>= 1;
                playerIDs[1] = charID;

                if (playerIDs[1] == 0xFF) {
                    RSDK.PlaySfx(Player->sfxSwapFail, false, 255);
                }
                else {
                    for (int32 i = 0; i < 3; ++i) {
                        if (globals->stock & (0xFF << (8 * i))) {
                            int32 characterID = (globals->stock >> (8 * i)) & 0xFF;

                            playerIDs[i + 2] = -1;
                            for (int32 c = characterID; c > 0; ++playerIDs[i + 2]) c >>= 1;
                        }
                    }

                    globals->stock = 0;

                    int32 tempStock = 0;
                    int32 p         = 0;
                    for (; p < 5;) {
                        bool32 inc = true;
                        if (playerIDs[p] == 0xFF)
                            break;

                        newPlayerIDs[p] = RSDK.Rand(0, 5);
                        if ((1 << newPlayerIDs[p]) & globals->characterFlags) {
                            while (true) {
                                if (!((1 << newPlayerIDs[p]) & tempStock)) {
                                    if (newPlayerIDs[p] != playerIDs[p]) {
                                        tempStock |= 1 << newPlayerIDs[p];
                                        break;
                                    }
                                    else if (p == 4 || playerIDs[p + 1] == 0xFF) {
                                        int32 slot         = RSDK.Rand(0, p);
                                        int32 id           = newPlayerIDs[slot];
                                        newPlayerIDs[slot] = newPlayerIDs[p];
                                        newPlayerIDs[p]    = id;
                                        tempStock |= 1 << newPlayerIDs[slot];
                                        tempStock |= 1 << newPlayerIDs[p];
                                        break;
                                    }
                                    else {
                                        newPlayerIDs[p] = RSDK.Rand(0, 5);
                                        if (!((1 << newPlayerIDs[p]) & globals->characterFlags)) {
                                            inc = false;
                                            break;
                                        }
                                    }
                                }
                                else {
                                    newPlayerIDs[p] = RSDK.Rand(0, 5);
                                    if (!((1 << newPlayerIDs[p]) & globals->characterFlags)) {
                                        inc = false;
                                        break;
                                    }
                                }
                            }
                            if (inc)
                                ++p;
                        }
                    }

                    for (int32 i = 0; i < p; ++i) {
                        switch (i) {
                            case 0: Player_ChangeCharacter(player1, 1 << newPlayerIDs[0]); break;
                            case 1: Player_ChangeCharacter(player2, 1 << newPlayerIDs[1]); break;
                            default:
                                globals->stock <<= 8;
                                globals->stock |= 1 << newPlayerIDs[i];
                                break;
                        }
                    }

                    EntityExplosion *explosion = CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_ENEMY), player1->position.x, player1->position.y);
                    explosion->drawOrder       = Zone->objectDrawHigh;

                    explosion            = CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_ENEMY), player2->position.x, player2->position.y);
                    explosion->drawOrder = Zone->objectDrawHigh;

                    RSDK.PlaySfx(ItemBox->sfxPowerDown, false, 255);
                }
            }
            break;
        }
#endif

        case ITEMBOX_SUPER:
            Player_GiveRings(player, 50, false);
            Player_CheckGoSuper(player, 0x7F);
            break;

#if RETRO_USE_PLUS
        case ITEMBOX_STOCK: {
            if (self->contentsAnimator.animationID == 7) {
                if (globals->gameMode == MODE_ENCORE) {
                    if (!((1 << self->contentsAnimator.frameID) & globals->characterFlags) && globals->characterFlags != 31
                        && !(globals->stock & 0xFF0000)) {
                        globals->characterFlags |= (1 << self->contentsAnimator.frameID);
                        EntityPlayer *player2 = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);
                        if (player2->objectID) {
                            int32 id = 0;
                            while ((globals->stock >> id) & 0xFF) id += 8;
                            globals->stock |= (1 << self->contentsAnimator.frameID << id);
                            HUD->stockFlashTimers[(id >> 3) + 1] = 120;
                        }
                        else {
                            player2->objectID   = Player->objectID;
                            Player->jumpInTimer = 0;
                            EntityDust *dust    = CREATE_ENTITY(Dust, intToVoid(1), player2->position.x, player2->position.y);

                            dust->visible         = 0;
                            dust->active          = ACTIVE_NEVER;
                            dust->isPermanent     = true;
                            dust->position.y      = (ScreenInfo->position.y - 128) << 16;
                            player2->playerID     = 1;
                            EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
                            if (player1->state == Player_State_Die || player1->state == Player_State_Drown) {
                                player2->state      = Player_State_EncoreRespawn;
                                player2->velocity.x = 0;
                                player2->velocity.y = 0;
                                player2->position.x = -0x200000;
                                player2->position.y = -0x200000;
                                break;
                            }
                            else {
                                Player_ChangeCharacter(player2, 1 << self->contentsAnimator.frameID);
                                player2->velocity.x = 0;
                                player2->velocity.y = 0;
                                player2->groundVel  = 0;
                                player2->position.x = -0x400000;
                                player2->position.y = -0x400000;
                                player2->angle      = 128;
                                if (player2->characterID == ID_TAILS) {
                                    player2->state = Player_State_FlyIn;
                                }
                                else {
                                    player2->state            = Player_State_JumpIn;
                                    player2->abilityValues[0] = ((ScreenInfo->position.y + ScreenInfo->height + 16) << 16) - player->position.y;
                                    player2->drawFX |= FX_SCALE;
                                    player2->scale.x = 1024;
                                    player2->scale.y = 1024;
                                    int32 spd        = player2->abilityValues[0] / -12;
                                    if (spd >= -0x68000 || spd > -0xE0000) {
                                        if (spd < -0x68000)
                                            player2->velocity.y = player2->abilityValues[0] / -12;
                                        else
                                            player2->velocity.y = -0x68000;
                                    }
                                    else {
                                        player2->velocity.y = -0xE0000;
                                    }
                                }
                                player2->abilityPtrs[0]   = dust;
                                player2->abilityValues[0] = 0;
                                player2->nextAirState     = StateMachine_None;
                                player2->nextGroundState  = StateMachine_None;
                                player2->stateInput       = Player_ProcessP2InputLag;
                                player2->tileCollisions   = false;
                                player2->interaction      = false;
                                player2->drawOrder        = Zone->playerDrawHigh;
                                player2->drownTimer       = 0;
                                player2->active           = ACTIVE_NORMAL;
                                player2->collisionPlane   = 0;
                                player2->collisionMode    = CMODE_FLOOR;
                                player2->collisionLayers  = Zone->fgLayers;
                                player2->controllerID     = CONT_P2;
                                player2->sidekick         = true;
                                player2->drawFX           = FX_FLIP | FX_ROTATE;
                                player2->visible          = true;
                                HUD->stockFlashTimers[0]  = 120;
                            }
                        }
                    }
                    RSDK.PlaySfx(ItemBox->sfxRevovery, false, 255);
                }
                else {
                    switch (self->contentsAnimator.frameID) {
                        case 0: Player_ChangeCharacter(player, ID_SONIC); break;
                        case 1: Player_ChangeCharacter(player, ID_TAILS); break;
                        case 2: Player_ChangeCharacter(player, ID_KNUCKLES); break;
                        case 3: Player_ChangeCharacter(player, ID_MIGHTY); break;
                        case 4: Player_ChangeCharacter(player, ID_RAY); break;
                        default: break;
                    }
                    EntityExplosion *explosion = CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_ENEMY), player->position.x, player->position.y);
                    explosion->drawOrder       = Zone->objectDrawHigh;
                    RSDK.PlaySfx(ItemBox->sfxPowerDown, false, 255);
                }
            }
            else {
                switch (self->contentsAnimator.frameID) {
                    case 1: self->type = ITEMBOX_BLUESHIELD; break;
                    case 2: self->type = ITEMBOX_BUBBLESHIELD; break;
                    case 3: self->type = ITEMBOX_FIRESHIELD; break;
                    case 4: self->type = ITEMBOX_LIGHTNINGSHIELD; break;
                    case 5: self->type = ITEMBOX_HYPERRING; break;
                    case 6: self->type = ITEMBOX_SWAP; break;
                    case 7: self->type = ITEMBOX_RANDOM; break;
                    default: self->type = ITEMBOX_RING; break;
                }

                player = (EntityPlayer *)self->parent;
                if ((uint32)self->type <= ITEMBOX_STOCK)
                    ItemBox_GivePowerup();
            }
            break;
        }

#endif
        default: break;
    }
}
void ItemBox_Break(EntityItemBox *itemBox, void *p)
{
    EntityPlayer *player = (EntityPlayer *)p;
    if (globals->gameMode == MODE_COMPETITION) {
        EntityCompetitionSession *session = (EntityCompetitionSession *)globals->competitionSession;
        ++session->items[RSDK.GetEntityID(player)];
    }
    RSDK.CreateEntity(TYPE_BLANK, NULL, itemBox->position.x, itemBox->position.y);
#if RETRO_USE_PLUS
    if (player->characterID == ID_MIGHTY && player->animator.animationID == ANI_DROPDASH)
        player->velocity.y -= 0x10000;
    else
#endif
        player->velocity.y = -(player->velocity.y + 2 * player->gravityStrength);
    itemBox->storedEntity  = (Entity *)player;
    itemBox->alpha         = 0x100;
    itemBox->contentsSpeed = -0x30000;
    itemBox->active        = ACTIVE_NORMAL;
    itemBox->velocity.y    = -0x20000;
    itemBox->isContents    = true;
    itemBox->state         = ItemBox_State_ContentsShown;
    RSDK.SetSpriteAnimation(ItemBox->aniFrames, 1, &itemBox->boxAnimator, true, 0);
    itemBox->boxAnimator.frameID = ItemBox->brokenFrame++;
    ItemBox->brokenFrame %= 3;
    RSDK.SetSpriteAnimation(-1, 0, &itemBox->overlayAnimator, true, 0);
    RSDK.SetSpriteAnimation(-1, 0, &itemBox->debrisAnimator, true, 0);

    EntityExplosion *explosion = CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_ITEMBOX), itemBox->position.x, itemBox->position.y - 0x100000);
    explosion->drawOrder       = Zone->objectDrawHigh;

    for (int32 d = 0; d < 6; ++d) {
        EntityDebris *debris =
            CREATE_ENTITY(Debris, NULL, itemBox->position.x + RSDK.Rand(-0x80000, 0x80000), itemBox->position.y + RSDK.Rand(-0x80000, 0x80000));
        debris->state      = Debris_State_Fall;
        debris->gravityStrength    = 0x4000;
        debris->velocity.x = RSDK.Rand(0, 0x20000);
        if (debris->position.x < itemBox->position.x)
            debris->velocity.x = -debris->velocity.x;
        debris->velocity.y = RSDK.Rand(-0x40000, -0x10000);
        debris->drawFX     = 1;
        debris->direction  = d & 3;
        debris->drawOrder  = Zone->objectDrawHigh;
        RSDK.SetSpriteAnimation(ItemBox->aniFrames, 6, &debris->animator, true, RSDK.Rand(0, 4));
    }

    RSDK.PlaySfx(ItemBox->sfxDestroy, false, 255);
    itemBox->active = ACTIVE_NORMAL;
    if (itemBox->type == ITEMBOX_RANDOM) {
#if RETRO_USE_PLUS
        if (globals->gameMode != MODE_ENCORE) {
#endif
            while (true) {
                itemBox->type = RSDK.Rand(0, 13);
                switch (itemBox->type) {
                    case ITEMBOX_1UP_SONIC:
                        if (globals->gameMode == MODE_TIMEATTACK)
                            continue;

                        switch (player->characterID) {
                            case ID_SONIC: itemBox->type = ITEMBOX_1UP_SONIC; break;
                            case ID_TAILS: itemBox->type = ITEMBOX_1UP_TAILS; break;
                            case ID_KNUCKLES: itemBox->type = ITEMBOX_1UP_KNUX; break;
#if RETRO_USE_PLUS
                            case ID_MIGHTY: itemBox->type = ITEMBOX_1UP_MIGHTY; break;
                            case ID_RAY: itemBox->type = ITEMBOX_1UP_RAY;
#endif
                                break;
                            default: break;
                        }
                        itemBox->contentsAnimator.frameID = itemBox->type;
                        break;

                    case ITEMBOX_1UP_TAILS:
                    case ITEMBOX_1UP_KNUX:
#if RETRO_USE_PLUS
                    case ITEMBOX_1UP_MIGHTY:
                    case ITEMBOX_1UP_RAY:
#endif
                        continue;

                    case ITEMBOX_SWAP:
                        if (globals->gameMode != MODE_COMPETITION)
                            continue;
                        itemBox->contentsAnimator.frameID = itemBox->type;
                        break;

                    default: itemBox->contentsAnimator.frameID = itemBox->type; break;
                }
                break;
            }
#if RETRO_USE_PLUS
        }
#endif
    }
}
bool32 ItemBox_HandleFallingCollision(void)
{
    RSDK_THIS(ItemBox);
    if (self->direction)
        return false;

    self->moveOffset.x = -self->position.x;
    self->moveOffset.y = -self->position.y;
    if (self->velocity.y)
#if RETRO_USE_PLUS
        self->parent = NULL;
#else
        self->groundVel = 0;
#endif
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;
    ItemBox_HandleObjectCollisions();

    if (self->velocity.y >= 0
        && (self->direction == FLIP_Y && self->boxAnimator.animationID == 1
                ? RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0, true)
                : RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x100000, true))) {
        self->velocity.y = 0;
        if (self->state != ItemBox_State_ContentsDisappear && self->state != ItemBox_State_ContentsShown)
            self->active = ACTIVE_BOUNDS;
        self->moveOffset.x += self->position.x;
        self->moveOffset.y += self->position.y;
        return true;
    }
    else {
        self->moveOffset.x += self->position.x;
        self->moveOffset.y += self->position.y;
        return false;
    }
}
bool32 ItemBox_HandlePlatformCollision(void *p)
{
    EntityPlatform *platform = (EntityPlatform *)p;
    RSDK_THIS(ItemBox);

    bool32 collided = false;
    if (platform->state != Platform_State_Collapse_Falling && platform->state != Platform_State_Collapse_CheckReset) {
        platform->position.x = platform->drawPos.x - platform->collisionOffset.x;
        platform->position.y = platform->drawPos.y - platform->collisionOffset.y;
        if (platform->collision) {
            if (platform->collision != 1) {
                if (platform->collision != 2 || !RSDK.CheckObjectCollisionTouchBox(platform, &platform->hitbox, self, &ItemBox->hitbox)) {
                    platform->position.x = platform->centerPos.x;
                    platform->position.y = platform->centerPos.y;
                    return false;
                }
                if (self->collisionLayers & Zone->moveID) {
                    TileLayer *move  = RSDK.GetSceneLayer(Zone->moveLayer);
                    move->position.x = -(platform->drawPos.x + platform->tileOrigin.x) >> 16;
                    move->position.y = -(platform->drawPos.y + platform->tileOrigin.y) >> 16;
                }
                if (self->state == ItemBox_State_Normal || self->velocity.y > 0x3800) {
                    platform->position.x = platform->centerPos.x;
                    platform->position.y = platform->centerPos.y;
                    return false;
                }
                else {
                }
                collided = true;
            }
            else {
                collided = RSDK.CheckObjectCollisionBox(platform, RSDK.GetHitbox(&platform->animator, 1), self, &ItemBox->hitbox, 1);
            }
        }
        else {
            collided = RSDK.CheckObjectCollisionPlatform(platform, RSDK.GetHitbox(&platform->animator, 0), self, &ItemBox->hitbox, 1);
        }

        if (!collided) {
            platform->position.x = platform->centerPos.x;
            platform->position.y = platform->centerPos.y;
            return false;
        }

#if RETRO_USE_PLUS
        self->parent = (Entity *)platform;
#else
        self->groundVel = RSDK.GetEntityID(platform);
#endif
        self->scale.x       = (self->position.x - platform->drawPos.x) & 0xFFFF0000;
        self->scale.y       = (self->position.y - platform->drawPos.y) & 0xFFFF0000;
        self->updateRange.x = platform->updateRange.x;
        self->updateRange.y = platform->updateRange.y;
        if (self->state == ItemBox_State_Falling)
            self->state = ItemBox_State_Normal;
        if (platform->state == Platform_State_Collapse && !platform->timer)
            platform->timer = 30;
        platform->stood      = true;
        self->velocity.y     = 0;
        platform->position.x = platform->centerPos.x;
        platform->position.y = platform->centerPos.y;
        return collided;
    }
    return false;
}
void ItemBox_HandleObjectCollisions(void)
{
    RSDK_THIS(ItemBox);

    bool32 platformCollided = false;

    if (Platform) {
#if RETRO_USE_PLUS
        if (self->parent) {
            EntityPlatform *platform = (EntityPlatform *)self->parent;
#else
        if (self->groundVel) {
            EntityPlatform *platform = RSDK_GET_ENTITY(self->groundVel, Platform);
#endif
            if (platform->objectID == Platform->objectID) {
                platform->stood    = true;
                self->position.x   = self->scale.x + platform->drawPos.x;
                self->position.y   = (self->scale.y + platform->drawPos.y) & 0xFFFF0000;
                self->moveOffset.x = platform->collisionOffset.x & 0xFFFF0000;
                self->moveOffset.y = platform->collisionOffset.y & 0xFFFF0000;
                self->contentsPos.x += platform->collisionOffset.x;
                self->contentsPos.y += platform->collisionOffset.y;
                self->velocity.y = 0;
                platformCollided             = true;
            }
        }
        else {
            foreach_active(Platform, platform)
            {
                if (ItemBox_HandlePlatformCollision(platform))
                    platformCollided = true;
            }
        }
    }

#if RETRO_USE_PLUS
    if (TilePlatform) {
        if (self->parent) {
            EntityTilePlatform *tilePlatform = (EntityTilePlatform *)self->parent;
            if (tilePlatform->objectID == TilePlatform->objectID) {
                platformCollided                = true;
                tilePlatform->stood = true;
                self->position.x    = self->scale.x + tilePlatform->drawPos.x;
                self->position.y    = (self->scale.y + tilePlatform->drawPos.y) & 0xFFFF0000;
                self->moveOffset.x  = tilePlatform->collisionOffset.x & 0xFFFF0000;
                self->moveOffset.y  = tilePlatform->collisionOffset.y & 0xFFFF0000;
                self->contentsPos.x += tilePlatform->collisionOffset.x;
                self->contentsPos.y += tilePlatform->collisionOffset.y;
                self->velocity.y = 0;
            }
        }
    }
#endif

    if (Crate) {
#if RETRO_USE_PLUS
        if (self->parent) {
            EntityCrate *crate = (EntityCrate *)self->parent;
#else
        if (self->groundVel) {
            EntityCrate *crate = RSDK_GET_ENTITY(self->groundVel, Crate);
#endif
            if (crate->objectID == Crate->objectID) {
                crate->stood       = true;
                self->position.x   = self->scale.x + crate->drawPos.x;
                self->position.y   = (self->scale.y + crate->drawPos.x) & 0xFFFF0000;
                self->moveOffset.x = crate->collisionOffset.x & 0xFFFF0000;
                self->moveOffset.y = crate->collisionOffset.y & 0xFFFF0000;
                self->contentsPos.x += crate->collisionOffset.x;
                self->contentsPos.y += crate->collisionOffset.y;
                self->velocity.y = 0;
            }
            else {
#if RETRO_USE_PLUS
                self->parent = NULL;
#else
                self->groundVel = 0;
#endif
            }
        }
        else {
            foreach_active(Crate, crate)
            {
                if (!crate->ignoreItemBox && ItemBox_HandlePlatformCollision((EntityPlatform *)crate))
                    platformCollided = true;
            }
        }
    }

    if (!platformCollided)
#if RETRO_USE_PLUS
        self->parent = NULL;
#else
        self->groundVel = 0;
#endif

    if (Ice) {
        foreach_active(Ice, ice)
        {
            if (ice->stateDraw == Ice_Draw_IceBlock) {
                int32 storeX = ice->position.x;
                int32 storeY = ice->position.y;
                ice->position.x -= ice->playerMoveOffset.x;
                ice->position.y -= ice->playerMoveOffset.y;

                if (RSDK.CheckObjectCollisionBox(ice, &ice->hitboxBlock, self, &ItemBox->hitbox, true)) {
                    self->position.x += ice->playerMoveOffset.x;
                    self->position.y += ice->playerMoveOffset.y;
                    self->position.y = self->position.y >> 0x10 << 0x10;
                    self->contentsPos.x += ice->playerMoveOffset.x;
                    self->contentsPos.y += ice->playerMoveOffset.y;
                    self->contentsPos.y = self->contentsPos.y >> 0x10 << 0x10;
                    self->moveOffset.x  = ice->playerMoveOffset.x;
                    self->moveOffset.y  = ice->playerMoveOffset.y;
                    self->velocity.y    = 0;
                }
                ice->position.x = storeX;
                ice->position.y = storeY;
            }
        }
    }

    foreach_active(Spikes, spikes)
    {
        int32 storeX = spikes->position.x;
        int32 storeY = spikes->position.y;
        spikes->position.x -= spikes->collisionOffset.x;
        spikes->position.y -= spikes->collisionOffset.y;
        if (RSDK.CheckObjectCollisionBox(spikes, &spikes->hitbox, self, &ItemBox->hitbox, true)) {
            self->position.x += spikes->collisionOffset.x;
            self->position.y += spikes->collisionOffset.y;
            self->position.y &= 0xFFFF0000;

            self->contentsPos.x += spikes->collisionOffset.x;
            self->contentsPos.y += spikes->collisionOffset.y;
            self->contentsPos.y &= 0xFFFF0000;

            self->moveOffset.x = spikes->collisionOffset.x;
            self->moveOffset.y = spikes->collisionOffset.y;
            self->velocity.y   = 0;
        }
        spikes->position.x = storeX;
        spikes->position.y = storeY;
    }

    foreach_active(ItemBox, itemBox)
    {
        if (itemBox != self) {
            if (self->state == ItemBox_State_Normal || self->state == ItemBox_State_Falling) {
                if (itemBox->state == ItemBox_State_Normal || itemBox->state == ItemBox_State_Falling) {
                    int32 storeX = itemBox->position.x;
                    int32 storeY = itemBox->position.y;
                    itemBox->position.x -= itemBox->moveOffset.x;
                    itemBox->position.y -= itemBox->moveOffset.y;
                    if (RSDK.CheckObjectCollisionBox(itemBox, &ItemBox->hitbox, self, &ItemBox->hitbox, true) == C_TOP) {
                        self->position.x += itemBox->moveOffset.x;
                        self->position.y += itemBox->moveOffset.y;
                        self->position.y = self->position.y >> 0x10 << 0x10;
                        self->contentsPos.x += itemBox->moveOffset.x;
                        self->contentsPos.y += itemBox->moveOffset.y;
                        self->contentsPos.y = self->contentsPos.y >> 0x10 << 0x10;

                        self->moveOffset.x = itemBox->moveOffset.x;
                        self->moveOffset.y = itemBox->moveOffset.y;
                        self->velocity.y   = 0;
                    }
                    itemBox->position.x = storeX;
                    itemBox->position.y = storeY;
                }
            }
        }
    }
}

#if RETRO_INCLUDE_EDITOR
void ItemBox_EditorDraw(void)
{
    RSDK_THIS(ItemBox);
    int dir = self->direction;
    self->direction *= FLIP_Y;

    self->contentsPos.x = self->position.x;
    if (self->direction == FLIP_NONE)
        self->contentsPos.y = self->position.y - 0x30000;
    else
        self->contentsPos.y = self->position.y + 0x30000;

    RSDK.DrawSprite(&self->boxAnimator, NULL, false);
    RSDK.DrawSprite(&self->contentsAnimator, &self->contentsPos, false);

    self->direction = dir;
}

void ItemBox_EditorLoad(void)
{
    ItemBox->aniFrames = RSDK.LoadSpriteAnimation("Global/ItemBox.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(ItemBox, type);
    RSDK_ENUM_VAR("Rings", ITEMBOX_RING);
    RSDK_ENUM_VAR("Blue Shield", ITEMBOX_BLUESHIELD);
    RSDK_ENUM_VAR("Bubble Shield", ITEMBOX_BUBBLESHIELD);
    RSDK_ENUM_VAR("Fire Shield", ITEMBOX_FIRESHIELD);
    RSDK_ENUM_VAR("Lightning Shield", ITEMBOX_LIGHTNINGSHIELD);
    RSDK_ENUM_VAR("Invincibility", ITEMBOX_INVINCIBLE);
    RSDK_ENUM_VAR("Speed Shoes", ITEMBOX_SNEAKERS);
    RSDK_ENUM_VAR("1UP (Sonic)", ITEMBOX_1UP_SONIC);
    RSDK_ENUM_VAR("1UP (Tails)", ITEMBOX_1UP_TAILS);
    RSDK_ENUM_VAR("1UP (Knux)", ITEMBOX_1UP_KNUX);
    RSDK_ENUM_VAR("Eggman", ITEMBOX_EGGMAN);
    RSDK_ENUM_VAR("Hyper Ring", ITEMBOX_HYPERRING);
    RSDK_ENUM_VAR("Swap", ITEMBOX_SWAP);
    RSDK_ENUM_VAR("Random", ITEMBOX_RANDOM);
    RSDK_ENUM_VAR("Super", ITEMBOX_SUPER);
#if RETRO_USE_PLUS
    RSDK_ENUM_VAR("1UP (Mighty)", ITEMBOX_1UP_MIGHTY);
    RSDK_ENUM_VAR("1UP (Ray)", ITEMBOX_1UP_RAY);
    RSDK_ENUM_VAR("Extra Stock", ITEMBOX_STOCK);
#endif

    RSDK_ACTIVE_VAR(ItemBox, planeFilter);
    RSDK_ENUM_VAR("No Filter", PLANEFILTER_NONE);
    RSDK_ENUM_VAR("Plane A", PLANEFILTER_A);
    RSDK_ENUM_VAR("Plane B", PLANEFILTER_B);

    RSDK_ACTIVE_VAR(ItemBox, direction);
    RSDK_ENUM_VAR("No Flip", FLIP_NONE);
    RSDK_ENUM_VAR("Flip Y", FLIP_X);
}
#endif

void ItemBox_Serialize(void)
{
    RSDK_EDITABLE_VAR(ItemBox, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(ItemBox, VAR_BOOL, isFalling);
    RSDK_EDITABLE_VAR(ItemBox, VAR_BOOL, hidden);
    RSDK_EDITABLE_VAR(ItemBox, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(ItemBox, VAR_ENUM, planeFilter);
    RSDK_EDITABLE_VAR(ItemBox, VAR_BOOL, lrzConvPhys);
}
