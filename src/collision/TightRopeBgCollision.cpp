#include <TRU/collision/TightRopeBgCollision.h>
#include <collision/ActorBgCollisionCheck.h>
#include <TRU/TRU.h>
#include <TRU/utility/NSMB2Funcs.h>

// I have to get the nsmb2 collision stuff functions out of the way first

namespace TRU {
    // Ripped right outta ida lmao
    bool TightRopeBgCollision::checkAreaActor(ActorBgCollisionCheckResult* check_result, sead::Vector2f* sensor_previous_pos, sead::Vector2f* sensor_current_pos, u8 direction, ActorBgCollisionCheck* check) {
        u64 v5; // d8
        u64 v6; // d9
        u64 v7; // d10
        u64 v8; // d11
        TightRopeBgCollision *CollisionOwnerFootPrev; // r0
        bool v12; // zf
        ActorBgCollisionCheck::Sensor* result; // r0
        BOOL v14; // r7
        sead::Vector3f *position; // r0
        BOOL v16; // r8
        int v17; // r4
        f32 v18; // s20
        f32 v19; // s18
        f32 v20; // s19
        BasicRideLine *sidesBuffer1; // r0
        const f32 *p_x; // r0
        f32 v23; // s0
        f32 v24; // s1
        BasicRideLine *v25; // r0
        const f32 *v26; // r0
        f32 v27; // s3
        f32 v28; // s4
        f32 v29; // s2
        f32 v30; // s3
        Actor *mpOwner; // r0
        f32 v32; // s1
        u32 hi; // r2
        sead::Vector2f p_mContactPoint; // [sp+4h] [bp-9Ch]
        f32 v35; // [sp+8h] [bp-98h] BYREF
        const f32 v36 = 0.0; // [sp+Ch] [bp-94h]
        ActorBgCollisionCheck::Sensor** a2; // [sp+10h] [bp-90h]
        //! sead::Vector2f v38; // [sp+18h] [bp-88h] BYREF
        //! u32 v39[2]; // [sp+20h] [bp-80h] BYREF
        sead::Segment2f v38; // [sp+18h] [bp-88h] BYREF
        //!
        sead::Vector2f p0; // [sp+28h] [bp-78h] BYREF
        sead::Vector2f a3; // [sp+30h] [bp-70h] BYREF
        Ride2Point v42; // [sp+38h] [bp-68h] BYREF
        u64 vn43; // [sp+4Ch] [bp-54h]
        u64 vn44; // [sp+54h] [bp-4Ch]
        u64 vn45; // [sp+5Ch] [bp-44h]
        u64 vn46; // [sp+64h] [bp-3Ch]
        TightRopeBgCollision *v43; // [sp+6Ch] [bp-34h]
        f32 *r1_0a; // [sp+70h] [bp-30h]
        sead::Vector2f *v45; // [sp+74h] [bp-2Ch]
        GLenum directiona; // [sp+78h] [bp-28h]
        bool result2;
        bool result3;
        bool result4;

        v43 = this;
        r1_0a = &sensor_previous_pos->x;
        v45 = sensor_previous_pos;
        directiona = direction;
        vn43 = v5;
        vn44 = v6;
        vn45 = v7;
        vn46 = v8;
        
     
        CollisionOwnerFootPrev = static_cast<TightRopeBgCollision*>(NSMB2_ActorBgCollisionCheck_getCollisionOwnerFootPrev(check));

        if (CollisionOwnerFootPrev == nullptr) {
            return false;
        }
        if (CollisionOwnerFootPrev == this) {
            return false;
        }

        p_mContactPoint = check_result->getContactPoint();
        if ( directiona == cDirType_Down ) {
            result = check->getSensor2(cDirType_Down);
            *a2 = result;
            if ( result != 0 ) {
                v14 = (check->getOutputPrev().isOnBit(0)) != 0;
                result2 = this->sub_2FD790(*r1_0a, sensor_current_pos->x);
                if ( result2 != 0 && (!v14 || (result3 = check->getOutputPrev().isOnBit(2)) != 0) ) {
                    position = this->mFollowArg.position;
                    v16 = false;
                    v17 = 0;
                    v18 = 3.4028e38;
                    v19 = position->x + this->mPosOffset.x;
                    v20 = position->y + this->mPosOffset.y;
                    do
                    {
                    sidesBuffer1 = this->mRideLineArray;
                    if ( v17 < 3 )
                    {
                        sidesBuffer1 += v17;
                    }
                    p_x = &sidesBuffer1->getSegment().getSegment().getPos0().x;
                    v23 = *p_x;
                    v24 = p_x[1];
                    v25 = this->mRideLineArray;
                    if ( v17 < 3 )
                    {
                        v25 += v17;
                    }
                    v26 = &v25->getSegment().getSegment().getPos1().x;
                    p0.x = v19 + v23;
                    p0.y = v20 + v24;
                    v27 = v26[1];
                    a3.x = v19 + *v26;
                    a3.y = v20 + v27;
                    v42.convertRide2Point(p0, a3);

                    if ( v14 )
                    {
                        //! TODO MAKE THIS SHI WORK BROSMAXXING IM CRINE
                        // Ride2Point::sub_205914(v42, a2, directiona, 1, 1);
                        v42.FUN_021b0044(*a2, directiona, 1, 1);
                    }
                    v38.setPos0(*sensor_current_pos);
                    v38.setPos1(sead::Vector2f(0.0f, 1.0f));
                    //! TODO MAKE THIS SHI WORK BROSMAXXING IM CRINE
                    sead::Vector2f bruh;
                    bruh = v38.getPos1();
                    if ( sub_49A0F4(&v42, &v35, v38, 0.0, bruh) != 0 )
                    {
                        v28 = v35 - sensor_current_pos->x;
                        v29 = v36 - sensor_current_pos->y;
                        v30 = 0.0;
                        if ( v14 )
                        {
                        v30 = -1.0;
                        }
                        if ( ((v28 * 0.0) + (v29 * 1.0)) > v30 || ((v28 * v28) + (v29 * v29)) == 0.0 )
                        {
                        mpOwner = check->getOwner();
                        v16 = true;
                        if ( static_cast<Actor*>(mpOwner)->getKind() == Actor::cActorKind_Player )
                        {
                            v16 = mpOwner->getSpeedVec().y <= 0.0;
                        }
                        v32 = ((*r1_0a - v35) * (*r1_0a - v35)) + ((r1_0a[1] - v36) * (r1_0a[1] - v36));
                        if ( v32 < v18 ) {
                                check_result->set_0(1);
                                check_result->setSolidOnTop(false);
                                check_result->set_2(1);
                                check_result->setHit(true);
                                hi = this->mBgCheckData >> sizeof(u32); // TODO ---- Fix this maybe
                                v18 = v32;
                                check_result->setBgCheckData((this->mBgCheckData & 0x00000000FFFFFFFF) + (hi << sizeof(u32))); //! What the fuck
                                check_result->setLayer(v17);
                                check_result->setAngle(0);
                                p_mContactPoint.x = v35;
                                check_result->getContactPoint().y = v36;
                                check_result->setOwner(this);
                            }
                        }
                    }
                    ++v17;
                    }
                    while ( v17 < 3 );
                    if ( v16 ) {
                        this->callCallbacks(check, static_cast<DirType>(directiona));
                        if ( ((this->mBgCheckData >> sizeof(u32)) & 0xF) == 0 ) {
                            return 0;
                        }
                    }
                    return v16;
                }
            }
        }
        else
        {
            if ( check->isHit(0xFF) != directiona || NSMB2_ActorBgCollisionCheck_getCollisionOwnerFoot(check) != this )
            {
            return 0;
            }
            check_result->set_0(1);
            check_result->setSolidOnTop(false);
            check_result->set_2(1);
            check_result->setBgCheckData(this->mBgCheckData);
            check_result->setOwner(this);
            check_result->setLayer(-1);
            check_result->setAngle(0);
            result4 = 1;
            p_mContactPoint.x = sensor_current_pos->x;
            check_result->getContactPoint().y = sensor_current_pos->y;
            check_result->setHit(false);

        }
        return result4;
    }

    void TightRopeBgCollision::execute() {

    }
}