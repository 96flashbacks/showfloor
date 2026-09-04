#ifndef OBJECT_FIELDS_H
#define OBJECT_FIELDS_H

/**
 * The array [0x88, 0x1C8) in struct Object consists of fields that can vary by
 * object type. These macros provide access to these fields.
 */

#ifdef OBJECT_FIELDS_INDEX_DIRECTLY
#define OBJECT_FIELD_U32(index)           index
#define OBJECT_FIELD_S32(index)           index
#define OBJECT_FIELD_S16(index, subIndex) index
#define OBJECT_FIELD_F32(index)           index
#define OBJECT_FIELD_S16P(index)          index
#define OBJECT_FIELD_S32P(index)          index
#define OBJECT_FIELD_ANIMS(index)         index
#define OBJECT_FIELD_WAYPOINT(index)      index
#define OBJECT_FIELD_CHAIN_SEGMENT(index) index
#define OBJECT_FIELD_OBJ(index)           index
#define OBJECT_FIELD_SURFACE(index)       index
#define OBJECT_FIELD_VPTR(index)          index
#define OBJECT_FIELD_CVPTR(index)         index
#else
#define OBJECT_FIELD_U32(index)           rawData.asU32[index]
#define OBJECT_FIELD_S32(index)           rawData.asS32[index]
#define OBJECT_FIELD_S16(index, subIndex) rawData.asS16[index][subIndex]
#define OBJECT_FIELD_F32(index)           rawData.asF32[index]
#if !IS_64_BIT
#define OBJECT_FIELD_S16P(index)          rawData.asS16P[index]
#define OBJECT_FIELD_S32P(index)          rawData.asS32P[index]
#define OBJECT_FIELD_ANIMS(index)         rawData.asAnims[index]
#define OBJECT_FIELD_WAYPOINT(index)      rawData.asWaypoint[index]
#define OBJECT_FIELD_CHAIN_SEGMENT(index) rawData.asChainSegment[index]
#define OBJECT_FIELD_OBJ(index)           rawData.asObject[index]
#define OBJECT_FIELD_SURFACE(index)       rawData.asSurface[index]
#define OBJECT_FIELD_VPTR(index)          rawData.asVoidPtr[index]
#define OBJECT_FIELD_CVPTR(index)         rawData.asConstVoidPtr[index]
#else
#define OBJECT_FIELD_S16P(index)          ptrData.asS16P[index]
#define OBJECT_FIELD_S32P(index)          ptrData.asS32P[index]
#define OBJECT_FIELD_ANIMS(index)         ptrData.asAnims[index]
#define OBJECT_FIELD_WAYPOINT(index)      ptrData.asWaypoint[index]
#define OBJECT_FIELD_CHAIN_SEGMENT(index) ptrData.asChainSegment[index]
#define OBJECT_FIELD_OBJ(index)           ptrData.asObject[index]
#define OBJECT_FIELD_SURFACE(index)       ptrData.asSurface[index]
#define OBJECT_FIELD_VPTR(index)          ptrData.asVoidPtr[index]
#define OBJECT_FIELD_CVPTR(index)         ptrData.asConstVoidPtr[index]
#endif
#endif

// 0x088 (0x00), the first field, is object-specific and defined below the common fields.
/* Common fields */
#define /*0x08C*/ oFlags                      OBJECT_FIELD_U32(0x01)
#define /*0x090*/ oDialogResponse             OBJECT_FIELD_S16(0x02, 0)
#define /*0x092*/ oDialogState                OBJECT_FIELD_S16(0x02, 1)
#define /*0x094*/ oUnk94                      OBJECT_FIELD_U32(0x03)
// 0x98 unused/removed.
#define /*0x09C*/ oIntangibleTimer            OBJECT_FIELD_S32(0x05)
#define /*0x0A0*/ O_POS_INDEX                 0x06
#define /*0x0A0*/ oPosX                       OBJECT_FIELD_F32(O_POS_INDEX + 0)
#define /*0x0A4*/ oPosY                       OBJECT_FIELD_F32(O_POS_INDEX + 1)
#define /*0x0A8*/ oPosZ                       OBJECT_FIELD_F32(O_POS_INDEX + 2)
#define /*0x0AC*/ oVelX                       OBJECT_FIELD_F32(0x09)
#define /*0x0B0*/ oVelY                       OBJECT_FIELD_F32(0x0A)
#define /*0x0B4*/ oVelZ                       OBJECT_FIELD_F32(0x0B)
#define /*0x0B8*/ oForwardVel                 OBJECT_FIELD_F32(0x0C)
#define /*0x0B8*/ oForwardVelS32              OBJECT_FIELD_S32(0x0C)
#define /*0x0BC*/ oLeftVel                    OBJECT_FIELD_F32(0x0D)
#define /*0x0C0*/ oUpVel                      OBJECT_FIELD_F32(0x0E)
#define /*0x0C4*/ O_MOVE_ANGLE_INDEX          0x0F
#define /*0x0C4*/ O_MOVE_ANGLE_PITCH_INDEX    (O_MOVE_ANGLE_INDEX + 0)
#define /*0x0C4*/ O_MOVE_ANGLE_YAW_INDEX      (O_MOVE_ANGLE_INDEX + 1)
#define /*0x0C4*/ O_MOVE_ANGLE_ROLL_INDEX     (O_MOVE_ANGLE_INDEX + 2)
#define /*0x0C4*/ oMoveAnglePitch             OBJECT_FIELD_S32(O_MOVE_ANGLE_PITCH_INDEX)
#define /*0x0C8*/ oMoveAngleYaw               OBJECT_FIELD_S32(O_MOVE_ANGLE_YAW_INDEX)
#define /*0x0CC*/ oMoveAngleRoll              OBJECT_FIELD_S32(O_MOVE_ANGLE_ROLL_INDEX)
#define /*0x0D0*/ O_FACE_ANGLE_INDEX          0x12
#define /*0x0D0*/ O_FACE_ANGLE_PITCH_INDEX    (O_FACE_ANGLE_INDEX + 0)
#define /*0x0D0*/ O_FACE_ANGLE_YAW_INDEX      (O_FACE_ANGLE_INDEX + 1)
#define /*0x0D0*/ O_FACE_ANGLE_ROLL_INDEX     (O_FACE_ANGLE_INDEX + 2)
#define /*0x0D0*/ oFaceAnglePitch             OBJECT_FIELD_S32(O_FACE_ANGLE_PITCH_INDEX)
#define /*0x0D4*/ oFaceAngleYaw               OBJECT_FIELD_S32(O_FACE_ANGLE_YAW_INDEX)
#define /*0x0D8*/ oFaceAngleRoll              OBJECT_FIELD_S32(O_FACE_ANGLE_ROLL_INDEX)
#define /*0x0DC*/ oGraphYOffset               OBJECT_FIELD_F32(0x15)
#define /*0x0E0*/ oActiveParticleFlags        OBJECT_FIELD_U32(0x16)
#define /*0x0E4*/ oGravity                    OBJECT_FIELD_F32(0x17)
#define /*0x0E8*/ oFloorHeight                OBJECT_FIELD_F32(0x18)
#define /*0x0EC*/ oMoveFlags                  OBJECT_FIELD_U32(0x19)
#define /*0x0F0*/ oAnimState                  OBJECT_FIELD_S32(0x1A)
// 0x0F4-0x110 (0x1B-0x22) are object specific and defined below the common fields.
#define /*0x114*/ oAngleVelPitch              OBJECT_FIELD_S32(0x23)
#define /*0x118*/ oAngleVelYaw                OBJECT_FIELD_S32(0x24)
#define /*0x11C*/ oAngleVelRoll               OBJECT_FIELD_S32(0x25)
#define /*0x120*/ oAnimations                 OBJECT_FIELD_ANIMS(0x26)
#define /*0x124*/ oHeldState                  OBJECT_FIELD_U32(0x27)
#define /*0x128*/ oWallHitboxRadius           OBJECT_FIELD_F32(0x28)
#define /*0x12C*/ oDragStrength               OBJECT_FIELD_F32(0x29)
#define /*0x130*/ oInteractType               OBJECT_FIELD_U32(0x2A)
#define /*0x134*/ oInteractStatus             OBJECT_FIELD_S32(0x2B)
#define /*0x138*/ O_PARENT_RELATIVE_POS_INDEX 0x2C
#define /*0x138*/ oParentRelativePosX         OBJECT_FIELD_F32(O_PARENT_RELATIVE_POS_INDEX + 0)
#define /*0x13C*/ oParentRelativePosY         OBJECT_FIELD_F32(O_PARENT_RELATIVE_POS_INDEX + 1)
#define /*0x140*/ oParentRelativePosZ         OBJECT_FIELD_F32(O_PARENT_RELATIVE_POS_INDEX + 2)
#define /*0x144*/ oBhvParams2ndByte           OBJECT_FIELD_S32(0x2F)
// 0x148 unused, possibly a third param byte.
#define /*0x14C*/ oAction                     OBJECT_FIELD_S32(0x31)
#define /*0x150*/ oSubAction                  OBJECT_FIELD_S32(0x32)
#define /*0x154*/ oTimer                      OBJECT_FIELD_S32(0x33)
#define /*0x158*/ oBounciness                 OBJECT_FIELD_F32(0x34)
#define /*0x15C*/ oDistanceToMario            OBJECT_FIELD_F32(0x35)
#define /*0x160*/ oAngleToMario               OBJECT_FIELD_S32(0x36)
#define /*0x164*/ oHomeX                      OBJECT_FIELD_F32(0x37)
#define /*0x168*/ oHomeY                      OBJECT_FIELD_F32(0x38)
#define /*0x16C*/ oHomeZ                      OBJECT_FIELD_F32(0x39)
#define /*0x170*/ oFriction                   OBJECT_FIELD_F32(0x3A)
#define /*0x174*/ oBuoyancy                   OBJECT_FIELD_F32(0x3B)
#define /*0x178*/ oSoundStateID               OBJECT_FIELD_S32(0x3C)
#define /*0x17C*/ oOpacity                    OBJECT_FIELD_S32(0x3D)
#define /*0x180*/ oDamageOrCoinValue          OBJECT_FIELD_S32(0x3E)
#define /*0x184*/ oHealth                     OBJECT_FIELD_S32(0x3F)
#define /*0x188*/ oBhvParams                  OBJECT_FIELD_S32(0x40)
#define /*0x18C*/ oPrevAction                 OBJECT_FIELD_S32(0x41)
#define /*0x190*/ oInteractionSubtype         OBJECT_FIELD_U32(0x42)
#define /*0x194*/ oCollisionDistance          OBJECT_FIELD_F32(0x43)
#define /*0x198*/ oNumLootCoins               OBJECT_FIELD_S32(0x44)
#define /*0x19C*/ oDrawingDistance            OBJECT_FIELD_F32(0x45)
#define /*0x1A0*/ oRoom                       OBJECT_FIELD_S32(0x46)
// 0x1A4 is unused, possibly related to 0x1A8 in removed macro purposes.
#define /*0x1A8*/ oUnusedBhvParams            OBJECT_FIELD_U32(0x48)
// 0x1AC-0x1B2 (0x48-0x4A) are object specific and defined below the common fields.
#define /*0x1B4*/ oWallAngle                  OBJECT_FIELD_S32(0x4B)
#define /*0x1B8*/ oFloorType                  OBJECT_FIELD_S16(0x4C, 0)
#define /*0x1BA*/ oFloorRoom                  OBJECT_FIELD_S16(0x4C, 1)
#define /*0x1BC*/ oAngleToHome                OBJECT_FIELD_S32(0x4D)
#define /*0x1C0*/ oFloor                      OBJECT_FIELD_SURFACE(0x4E)
#define /*0x1C4*/ oDeathSound                 OBJECT_FIELD_S32(0x4F)

/* Pathed (see obj_follow_path) */
#define /*0x0FC*/ oPathedStartWaypoint     OBJECT_FIELD_WAYPOINT(0x1D)
#define /*0x100*/ oPathedPrevWaypoint      OBJECT_FIELD_WAYPOINT(0x1E)
#define /*0x104*/ oPathedPrevWaypointFlags OBJECT_FIELD_S32(0x1F)
#define /*0x108*/ oPathedTargetPitch       OBJECT_FIELD_S32(0x20)
#define /*0x10C*/ oPathedTargetYaw         OBJECT_FIELD_S32(0x21)

/* Special Object Macro */
#define /*0x108*/ oMacroUnk108 OBJECT_FIELD_F32(0x20)
#define /*0x10C*/ oMacroUnk10C OBJECT_FIELD_F32(0x21)
#define /*0x110*/ oMacroUnk110 OBJECT_FIELD_F32(0x22)

/* Mario */
#define /*0x0F4*/ oMarioParticleFlags    OBJECT_FIELD_S32(0x1B)
#define /*0x108*/ oMarioPoleUnk108       OBJECT_FIELD_S32(0x20)
#define /*0x108*/ oMarioReadingSignDYaw  OBJECT_FIELD_S32(0x20)
#define /*0x10C*/ oMarioPoleYawVel       OBJECT_FIELD_S32(0x21)
#define /*0x10C*/ oMarioCannonObjectYaw  OBJECT_FIELD_S32(0x21)
#define /*0x10C*/ oMarioTornadoYawVel    OBJECT_FIELD_S32(0x21)
#define /*0x10C*/ oMarioReadingSignDPosX OBJECT_FIELD_F32(0x21)
#define /*0x110*/ oMarioPolePos          OBJECT_FIELD_F32(0x22)
#define /*0x110*/ oMarioCannonInputYaw   OBJECT_FIELD_S32(0x22)
#define /*0x110*/ oMarioTornadoPosY      OBJECT_FIELD_F32(0x22)
#define /*0x110*/ oMarioReadingSignDPosZ OBJECT_FIELD_F32(0x22)
#define /*0x110*/ oMarioWhirlpoolPosY    OBJECT_FIELD_F32(0x22)
#define /*0x110*/ oMarioBurnTimer        OBJECT_FIELD_S32(0x22)
#define /*0x110*/ oMarioLongJumpIsSlow   OBJECT_FIELD_S32(0x22)
#define /*0x110*/ oMarioSteepJumpYaw     OBJECT_FIELD_S32(0x22)
#define /*0x110*/ oMarioWalkingPitch     OBJECT_FIELD_S32(0x22)

/* Bird Chirp Chirp */
#define /*0x0F4*/ oBirdChirpChirpUnkF4 OBJECT_FIELD_S32(0x1B)

/* Bob-omb Explosion Bubble */
#define /*0x0FC*/ oBobombExpBubGfxScaleFacX OBJECT_FIELD_S32(0x1D)
#define /*0x100*/ oBobombExpBubGfxScaleFacY OBJECT_FIELD_S32(0x1E)
#define /*0x104*/ oBobombExpBubGfxExpRateX  OBJECT_FIELD_S32(0x1F)
#define /*0x108*/ oBobombExpBubGfxExpRateY  OBJECT_FIELD_S32(0x20)

/* Bomp (Small) */
#define /*0x100*/ oSmallBompInitX OBJECT_FIELD_F32(0x1E)

/* Boo */
#define /*0x088*/ oBooDeathStatus           OBJECT_FIELD_S32(0x00)
#define /*0x0F4*/ oBooTargetOpacity         OBJECT_FIELD_S32(0x1B)
#define /*0x0F8*/ oBooBaseScale             OBJECT_FIELD_F32(0x1C)
#define /*0x0FC*/ oBooOscillationTimer      OBJECT_FIELD_S32(0x1D)
#define /*0x100*/ oBooMoveYawDuringHit      OBJECT_FIELD_S32(0x1E)
#define /*0x104*/ oBooMoveYawBeforeHit      OBJECT_FIELD_F32(0x1F)
// Removed 'oBooParentBigBoo' since it likely didn't exist at this point
// 'oBooNegatedAggressiveness', 'oBooInitialMoveYaw' and 'oBooTurningSpeed' are missing on pathminiteresa.p

// 'obake_flag' define from 'pathcoin.p'
#define obake_flag  OBJECT_FIELD_S32(0x22)

// Removed 'oBigBooNumMinionBoosKilled' since it likely didn't exist at this point

/* Haunted Bookshelf */
#define /*0x088*/ oHauntedBookshelfShouldOpen OBJECT_FIELD_S32(0x00)

/* Bouncing FireBall */
#define /*0x0F4*/ oBouncingFireBallUnkF4 OBJECT_FIELD_S32(0x1B)

/* Bowser */
#define /*0x088*/ oBowserCamAct          OBJECT_FIELD_S32(0x00)
#define /*0x0F4*/ oBowserStatus          OBJECT_FIELD_S32(0x1B)
#define /*0x0F8*/ oBowserTimer           OBJECT_FIELD_S32(0x1C)
#define /*0x0FC*/ oBowserDistToCenter    OBJECT_FIELD_F32(0x1D)
#define /*0x106*/ oBowserBitSJustJump    OBJECT_FIELD_S16(0x1F, 1)
#define /*0x108*/ oBowserRandSplitFloor  OBJECT_FIELD_S16(0x20, 0)
#define /*0x10A*/ oBowserHeldAnglePitch  OBJECT_FIELD_S16(0x20, 1)
#define /*0x10D*/ oBowserHeldAngleVelYaw OBJECT_FIELD_S16(0x21, 0)
#define /*0x10E*/ oBowserGrabbedStatus   OBJECT_FIELD_S16(0x21, 1)
#define /*0x110*/ oBowserIsReacting      OBJECT_FIELD_S16(0x22, 0)
#define /*0x112*/ oBowserAngleToCenter   OBJECT_FIELD_S16(0x22, 1)
#define /*0x1AC*/ oBowserTargetOpacity   OBJECT_FIELD_S16(0x49, 0)
#define /*0x1AE*/ oBowserEyesTimer       OBJECT_FIELD_S16(0x49, 1)
#define /*0x1B0*/ oBowserEyesShut        OBJECT_FIELD_S16(0x4A, 0)
#define /*0x1B2*/ oBowserRainbowLight    OBJECT_FIELD_S16(0x4A, 1)

/* Black Smoke Bowser */
#define /*0x0F4*/ oBlackSmokeBowserUnkF4 OBJECT_FIELD_F32(0x1B)

/* Bowser Puzzle */
#define /*0x0F4*/ oBowserPuzzleCompletionFlags OBJECT_FIELD_S32(0x1B)

/* Bowser Puzzle Piece */
#define /*0x0FC*/ oBowserPuzzlePieceOffsetX                  OBJECT_FIELD_F32(0x1D)
#define /*0x100*/ oBowserPuzzlePieceOffsetY                  OBJECT_FIELD_F32(0x1E)
#define /*0x104*/ oBowserPuzzlePieceOffsetZ                  OBJECT_FIELD_F32(0x1F)
#define /*0x108*/ oBowserPuzzlePieceContinuePerformingAction OBJECT_FIELD_S32(0x20)
#define /*0x10C*/ oBowserPuzzlePieceActionList               OBJECT_FIELD_VPTR(0x21)
#define /*0x110*/ oBowserPuzzlePieceNextAction               OBJECT_FIELD_VPTR(0x22)

/* Bullet Bill */
#define /*0x0F8*/ oBulletBillInitialMoveYaw OBJECT_FIELD_S32(0x1C)

/* Bully (all variants) */
#define /*0x0F4*/ oBullySubtype                   OBJECT_FIELD_S32(0x1B)
#define /*0x0F8*/ oBullyPrevX                     OBJECT_FIELD_F32(0x1C)
#define /*0x0FC*/ oBullyPrevY                     OBJECT_FIELD_F32(0x1D)
#define /*0x100*/ oBullyPrevZ                     OBJECT_FIELD_F32(0x1E)
#define /*0x104*/ oBullyKBTimerAndMinionKOCounter OBJECT_FIELD_S32(0x1F)
#define /*0x108*/ oBullyMarioCollisionAngle       OBJECT_FIELD_S32(0x20)

/* Triplet Butterfly */
#define /*0x0F4*/ oTripletButterflyScale             OBJECT_FIELD_F32(0x1B)
#define /*0x0F8*/ oTripletButterflySpeed             OBJECT_FIELD_F32(0x1C)
#define /*0x0FC*/ oTripletButterflyBaseYaw           OBJECT_FIELD_F32(0x1D)
#define /*0x100*/ oTripletButterflyTargetPitch       OBJECT_FIELD_S32(0x1E)
#define /*0x104*/ oTripletButterflyTargetYaw         OBJECT_FIELD_S32(0x1F)

/* Cannon */
#define /*0x0F4*/ oCannonUnkF4  OBJECT_FIELD_S32(0x1B)
#define /*0x0F8*/ oCannonUnkF8  OBJECT_FIELD_S32(0x1C)
#define /*0x10C*/ oCannonUnk10C OBJECT_FIELD_S32(0x21)

/* Cheep Cheep */
#define /*0x0F4*/ oCheepCheepUnkF4  OBJECT_FIELD_F32(0x1B)
#define /*0x0F8*/ oCheepCheepUnkF8  OBJECT_FIELD_F32(0x1C)
#define /*0x0FC*/ oCheepCheepUnkFC  OBJECT_FIELD_F32(0x1D)
#define /*0x104*/ oCheepCheepUnk104 OBJECT_FIELD_F32(0x1F)
#define /*0x108*/ oCheepCheepUnk108 OBJECT_FIELD_F32(0x20)

/* Coin */
#define /*0x0F4*/ oCoinCollectedFlags       OBJECT_FIELD_S32(0x1B)
#define /*0x0F8*/ oCoinOnGround             OBJECT_FIELD_S32(0x1C)
#define /*0x110*/ oCoinBaseVelY             OBJECT_FIELD_F32(0x22)

/* Door */
#define /*0x088*/ oDoorUnk88  OBJECT_FIELD_S32(0x00)
#define /*0x0F8*/ oDoorUnkF8  OBJECT_FIELD_S32(0x1C)
#define /*0x0FC*/ oDoorUnkFC  OBJECT_FIELD_S32(0x1D)
#define /*0x100*/ oDoorUnk100 OBJECT_FIELD_S32(0x1E)

/* Fire Spitter */
#define /*0x0F4*/ oFireSpitterScaleVel OBJECT_FIELD_F32(0x1B)

/* Blue Fish */
#define /*0x0F4*/ oBlueFishRandomVel   OBJECT_FIELD_F32(0x1B)
#define /*0x0F8*/ oBlueFishRandomTime  OBJECT_FIELD_S32(0x1C)
#define /*0x100*/ oBlueFishRandomAngle OBJECT_FIELD_F32(0x1E)

/* Fish Group */
#define /*0x0F4*/ oFishWaterLevel     OBJECT_FIELD_F32(0x1B)
#define /*0x0F8*/ oFishGoalY          OBJECT_FIELD_F32(0x1C)
#define /*0x0FC*/ oFishHeightOffset   OBJECT_FIELD_F32(0x1D)
#define /*0x100*/ oFishYawVel         OBJECT_FIELD_S32(0x1E)
#define /*0x104*/ oFishRoamDistance   OBJECT_FIELD_F32(0x1F)
#define /*0x108*/ oFishGoalVel        OBJECT_FIELD_F32(0x20)
#define /*0x10C*/ oFishDepthDistance  OBJECT_FIELD_F32(0x21)
#define /*0x110*/ oFishActiveDistance OBJECT_FIELD_F32(0x22)

/* Flame */
#define /*0x0F4*/ oFlameScale            OBJECT_FIELD_F32(0x1B)
#define /*0x0F8*/ oFlameSpeedTimerOffset OBJECT_FIELD_S32(0x1C)
#define /*0x0FC*/ oFlameUnusedRand       OBJECT_FIELD_F32(0x1D)
#define /*0x100*/ oFlameBowser           OBJECT_FIELD_OBJ(0x1E)

/* Blue Flame */
#define /*0x0F8*/ oBlueFlameNextScale OBJECT_FIELD_F32(0x1C)

/* Small Piranha Flame */
#define /*0x0F4*/ oSmallPiranhaFlameStartSpeed     OBJECT_FIELD_F32(0x1B)
#define /*0x0F8*/ oSmallPiranhaFlameEndSpeed       OBJECT_FIELD_F32(0x1C)
#define /*0x0FC*/ oSmallPiranhaFlameModel          OBJECT_FIELD_S32(0x1D)
#define /*0x100*/ oSmallPiranhaFlameNextFlameTimer OBJECT_FIELD_S32(0x1E)
#define /*0x104*/ oSmallPiranhaFlameSpeed          OBJECT_FIELD_F32(0x1F)

/* Moving Flame */
#define /*0x0F4*/ oMovingFlameTimer OBJECT_FIELD_S32(0x1B)

/* Flamethrower Flame */
#define /*0x110*/ oFlameThowerFlameUnk110 OBJECT_FIELD_S32(0x22)

/* Flamethrower */
#define /*0x110*/ oFlameThowerUnk110 OBJECT_FIELD_S32(0x22)

/* Hoot */
#define /*0x0F4*/ oHootAvailability     OBJECT_FIELD_S32(0x1B)
#define /*0x110*/ oHootMarioReleaseTime OBJECT_FIELD_S32(0x22)

/* Horizontal Movement */
#define /*0x0F4*/ oHorizontalMovementUnkF4  OBJECT_FIELD_S32(0x1B)
#define /*0x0F8*/ oHorizontalMovementUnkF8  OBJECT_FIELD_S32(0x1C)
#define /*0x100*/ oHorizontalMovementUnk100 OBJECT_FIELD_F32(0x1E)
#define /*0x104*/ oHorizontalMovementUnk104 OBJECT_FIELD_S32(0x1F)
#define /*0x108*/ oHorizontalMovementUnk108 OBJECT_FIELD_F32(0x20)

/* Mr. I */
#define /*0x0F4*/ oMrIUnkF4  OBJECT_FIELD_S32(0x1B)
#define /*0x0FC*/ oMrIUnkFC  OBJECT_FIELD_S32(0x1D)
#define /*0x100*/ oMrIUnk100 OBJECT_FIELD_S32(0x1E)
#define /*0x104*/ oMrIUnk104 OBJECT_FIELD_S32(0x1F)
#define /*0x108*/ oMrIUnk108 OBJECT_FIELD_S32(0x20)
#define /*0x10C*/ oMrIScale  OBJECT_FIELD_F32(0x21)
#define /*0x110*/ oMrIUnk110 OBJECT_FIELD_S32(0x22)

/* Motos */
#define oMotosHoldStatus OBJECT_FIELD_S32(0x00)

/* Small Penguin */
#define /*0x088*/ oSmallPenguinUnk88  OBJECT_FIELD_S32(0x00)
#define /*0x100*/ oSmallPenguinUnk100 OBJECT_FIELD_S32(0x1E) // angle?
#define /*0x104*/ oSmallPenguinUnk104 OBJECT_FIELD_F32(0x1F)
#define /*0x108*/ oSmallPenguinUnk108 OBJECT_FIELD_F32(0x20)
#define /*0x110*/ oSmallPenguinUnk110 OBJECT_FIELD_S32(0x22)

/* Piranha Plant */
#define /*0x0F4*/ oPiranhaPlantSleepMusicState OBJECT_FIELD_S32(0x1B)
#define /*0x0F8*/ oPiranhaPlantScale           OBJECT_FIELD_F32(0x1C)

/* WF Platform */
#define /*0x10C*/ oPlatformUnk10C OBJECT_FIELD_F32(0x21)
#define /*0x110*/ oPlatformUnk110 OBJECT_FIELD_F32(0x22)

/* Platform Spawner */
#define /*0x0F4*/ oPlatformSpawnerUnkF4  OBJECT_FIELD_S32(0x1B)
#define /*0x0F8*/ oPlatformSpawnerUnkF8  OBJECT_FIELD_S32(0x1C)
#define /*0x0FC*/ oPlatformSpawnerUnkFC  OBJECT_FIELD_S32(0x1D)
#define /*0x100*/ oPlatformSpawnerUnk100 OBJECT_FIELD_F32(0x1E)
#define /*0x104*/ oPlatformSpawnerUnk104 OBJECT_FIELD_F32(0x1F)
#define /*0x108*/ oPlatformSpawnerUnk108 OBJECT_FIELD_F32(0x20)

/* LLL Rotating Hex Flame */
#define /*0x0F4*/ oLLLRotatingHexFlameUnkF4 OBJECT_FIELD_F32(0x1B)
#define /*0x0F8*/ oLLLRotatingHexFlameUnkF8 OBJECT_FIELD_F32(0x1C)
#define /*0x0FC*/ oLLLRotatingHexFlameUnkFC OBJECT_FIELD_F32(0x1D)

/* WF Sliding Brick Platform */
#define /*0x0F4*/ oWFSlidBrickPtfmMovVel OBJECT_FIELD_F32(0x1B)

/* Smoke */
#define /*0x0F4*/ oSmokeTimer OBJECT_FIELD_S32(0x1B)

/* Sound Effect */
#define /*0x0F4*/ oSoundEffectUnkF4 OBJECT_FIELD_S32(0x1B)

/* Celebration Star */
#define /*0x0F4*/ oCelebStarUnkF4              OBJECT_FIELD_S32(0x1B)
#define /*0x108*/ oCelebStarDiameterOfRotation OBJECT_FIELD_S32(0x20)

/* Sushi Shark */
#define /*0x0F4*/ oSushiSharkUnkF4 OBJECT_FIELD_S32(0x1B) // angle?

/* Thwomp */
#define /*0x0F4*/ oThwompRandomTimer OBJECT_FIELD_S32(0x1B)

/* Tilting Platform */
#define /*0x0F4*/ oTiltingPyramidNormalX         OBJECT_FIELD_F32(0x1B)
#define /*0x0F8*/ oTiltingPyramidNormalY         OBJECT_FIELD_F32(0x1C)
#define /*0x0FC*/ oTiltingPyramidNormalZ         OBJECT_FIELD_F32(0x1D)
#define /*0x10C*/ oTiltingPyramidMarioOnPlatform OBJECT_FIELD_S32(0x21)

/* Beta Trampoline */
#define /*0x110*/ oBetaTrampolineMarioOnTrampoline OBJECT_FIELD_S32(0x22)

/* Tree Snow or Leaf */
#define /*0x0F4*/ oTreeSnowOrLeafUnkF4 OBJECT_FIELD_S32(0x1B)
#define /*0x0F8*/ oTreeSnowOrLeafUnkF8 OBJECT_FIELD_S32(0x1C)
#define /*0x0FC*/ oTreeSnowOrLeafUnkFC OBJECT_FIELD_S32(0x1D)

/* Tumbling Bridge */
#define /*0x0F4*/ oTumblingBridgeUnkF4 OBJECT_FIELD_S32(0x1B)

/* Water Objects */
#define /*0x0F4*/ oWaterObjUnkF4  OBJECT_FIELD_S32(0x1B)
#define /*0x0F8*/ oWaterObjUnkF8  OBJECT_FIELD_S32(0x1C)
#define /*0x0FC*/ oWaterObjUnkFC  OBJECT_FIELD_S32(0x1D)
#define /*0x100*/ oWaterObjUnk100 OBJECT_FIELD_S32(0x1E)

/* Water Ring (both variants) */
#define /*0x0F4*/ oWaterRingScalePhaseX      OBJECT_FIELD_S32(0x1B)
#define /*0x0F8*/ oWaterRingScalePhaseY      OBJECT_FIELD_S32(0x1C)
#define /*0x0FC*/ oWaterRingScalePhaseZ      OBJECT_FIELD_S32(0x1D)
#define /*0x100*/ oWaterRingNormalX          OBJECT_FIELD_F32(0x1E)
#define /*0x104*/ oWaterRingNormalY          OBJECT_FIELD_F32(0x1F)
#define /*0x108*/ oWaterRingNormalZ          OBJECT_FIELD_F32(0x20)
#define /*0x10C*/ oWaterRingMarioDistInFront OBJECT_FIELD_F32(0x21)
#define /*0x110*/ oWaterRingIndex            OBJECT_FIELD_S32(0x22)
#define /*0x1AC*/ oWaterRingAvgScale         OBJECT_FIELD_F32(0x49)

/* Water Ring Spawner (Jet Stream Ring Spawner and Manta Ray) */
#define /*0x1AC*/ oWaterRingSpawnerRingsCollected OBJECT_FIELD_S32(0x49)

/* Water Ring Manager (Jet Stream Ring Spawner and Manta Ray Ring Manager) */
#define /*0x0F4*/ oWaterRingMgrNextRingIndex     OBJECT_FIELD_S32(0x1B)
#define /*0x0F8*/ oWaterRingMgrLastRingCollected OBJECT_FIELD_S32(0x1C)

/* Wave Trail */
#define /*0x0F8*/ oWaveTrailSize OBJECT_FIELD_F32(0x1C)

/* Whomp */
#define /*0x0F8*/ oWhompShakeVal OBJECT_FIELD_S32(0x1C)

/* LLL Wood Piece */
#define /*0x0F4*/ oLLLWoodPieceOscillationTimer OBJECT_FIELD_S32(0x1B)

#endif // OBJECT_FIELDS_H
