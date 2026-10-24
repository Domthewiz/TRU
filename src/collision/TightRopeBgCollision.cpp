#include <TRU/collision/TightRopeBgCollision.h>
#include <collision/ActorBgCollisionCheck.h>
#include <TRU/TRU.h>
#include <TRU/utility/NSMB2Funcs.h>

// This was scrapped.
// I have to get the nsmb2 collision stuff functions out of the way first
//! Sorry for my absolutely garbage code

namespace TRU {
    
    bool TightRopeBgCollision::checkAreaActor(ActorBgCollisionCheckResult* check_result, sead::Vector2f* sensor_previous_pos, sead::Vector2f* sensor_current_pos, u8 direction, ActorBgCollisionCheck* check) {
    
        // Check if the the previous foot collision owner is null, and if it's this TightRopeBgCollision
        BgCollision* collisionOwnerFootPrevious = NSMB2_ActorBgCollisionCheck_getCollisionOwnerFootPrev(check);
        if (collisionOwnerFootPrevious == nullptr) {
            return false;
        }
        if (collisionOwnerFootPrevious != this) {
            return false;
        }
        
        // Get the contact point and work sensor
        sead::Vector2f* contactPoint;
        contactPoint = &check_result->getContactPoint();
        ActorBgCollisionCheck::Sensor* workSensor = check->getSensor2(cDirType_Down);
        
        // If the direction is not down, or if the sensor is invalid
        if (direction != cDirType_Down || workSensor == nullptr) {
            if ( check->isHit(0xFF) != direction || NSMB2_ActorBgCollisionCheck_getCollisionOwnerFoot(check) != this ) {
                return false;
            }
            check_result->set_0(1);
            check_result->setSolidOnTop(false);
            check_result->set_2(1);
            check_result->setBgCheckData(this->mBgCheckData);
            check_result->setOwner(this);
            check_result->setLayer(-1);
            check_result->setAngle(0);
            contactPoint->x = sensor_current_pos->x;
            check_result->getContactPoint().y = sensor_current_pos->y;
            check_result->setHit(false);
            return true;
        }

        // Setting all the variables
        bool result;
        bool previousFlagSetBit0 = check->getOutputPrev().isOnBit(0);
        bool v14 = sub_2FD790(sensor_previous_pos, sensor_current_pos);
        static constexpr u32 cFloatMaxSafeguardRepresentation = 0x7F7FFFFF;
        f32 floatMax = std::bit_cast<f32>(cFloatMaxSafeguardRepresentation);
        sead::Vector3f followArgPosition;
        sead::Vector2f* previousSensorPosition;
        sead::Vector2f* currentSensorPosition;
        sead::Segment2f calcSegment;
        sead::Vector2f calcVectorOut;
        bool isHit = false;

        if ( v14 != false && (!previousFlagSetBit0 || (result = (check->getOutputPrev().isOnBit(2))) != false)){

            f32 offsetPosX = mFollowArg.position->x + mPosOffset.x;
            f32 offsetPosY = mFollowArg.position->y + mPosOffset.y;

            for (u32 i = 0; i < 3; i++) {
                sead::Vector2f p0 { 
                    offsetPosX + mRideLineArray[i].getSegment().getSegment().getPos0().x,
                    offsetPosY + mRideLineArray[i].getSegment().getSegment().getPos0().y
                };
                sead::Vector2f p1 { 
                    offsetPosX + mRideLineArray[i].getSegment().getSegment().getPos1().x,
                    offsetPosY + mRideLineArray[i].getSegment().getSegment().getPos1().y
                };

                Segment segment(p0, p1);
                if (previousFlagSetBit0) {
                    segment.FUN_021b0044(workSensor, direction, 1, 1);
                }
                
                calcSegment.setPos0(*sensor_current_pos);
                calcSegment.setPos1(sead::Vector2f(0.0f, 1.0f));

                //! TODO: Make this better, sub_49A0F4 is pretty jankily implemented
                if ( sub_49A0F4(&segment, &calcVectorOut, calcSegment, 0.0, calcSegment.getPos1())) {
                    f32 outputXDist = calcVectorOut.x - sensor_current_pos->x;
                    f32 outputYDist = calcVectorOut.y - sensor_current_pos->y;
                    f32 v28 = (previousFlagSetBit0) ? -1.0f : 0.0f;

                    if ( ((outputXDist * 0.0) + (outputYDist * 1.0)) > v28 || ((outputXDist * outputXDist) + (outputYDist * outputYDist)) == 0.0 ) {
                        Actor* mpOwner = check->getOwner();
                        isHit = true;

                        // Change Speed for players only
                        if ( mpOwner->getKind() == Actor::cActorKind_Player ) {
                            isHit = mpOwner->getSpeedVec().y <= 0.0;
                        }
                        // Do some math
                        f32 v30 = ((previousSensorPosition->x - calcVectorOut.x) * (previousSensorPosition->x - calcVectorOut.x))
                                + ((previousSensorPosition->y - calcVectorOut.y) * (previousSensorPosition->y - calcVectorOut.y));
                        if ( v30 < floatMax ) {
                            check_result->set_0(1);
                            check_result->setSolidOnTop(false);
                            check_result->set_2(1);
                            check_result->setHit(true);
                            // It gets the high bytes of this 
                            u32 hi = this->mBgCheckData >> sizeof(u32); // TODO ---- Fix this maybe
                            floatMax = v30;
                            check_result->setBgCheckData((this->mBgCheckData & 0x00000000FFFFFFFF) + (hi << sizeof(u32))); //! What the fuck
                            check_result->setLayer(i);
                            check_result->setAngle(0);
                            contactPoint->x = calcVectorOut.x;
                            check_result->getContactPoint().y = calcVectorOut.y;
                            check_result->setOwner(this);
                        }
                    }
                }
            }

            result = isHit;
            if (isHit) {
                check->set_14d8(check_result->get_0());
                callCallbacks(check, direction);
                callBgUnitCallbacks(check, direction);
                
                if (!check_result->get_0() || check->calcSensorCollision(this, direction)) {
                    result = false;
                }
            }
        }

        // const ActorBgCollisionCheck::Sensor* sensor = check->ActorBgCollisionCheck::getSensor(direction);
        return result;
    }

    bool TightRopeBgCollision::sub_2FD790(sead::Vector2f* previous_pos, sead::Vector2f* current_pos) {
        for (u32 i = 0; i < 3; i++) {
            sead::Vector2f currentP0 { 
                mFollowArg.position->x + mPosOffset.x + mRideLineArray[i].getSegment().getSegment().getPos0().x,
                mFollowArg.position->y + mPosOffset.y + mRideLineArray[i].getSegment().getSegment().getPos0().y
            };
            sead::Vector2f currentP1 { 
                mFollowArg.position->x + mPosOffset.x + mRideLineArray[i].getSegment().getSegment().getPos1().x,
                mFollowArg.position->y + mPosOffset.y + mRideLineArray[i].getSegment().getSegment().getPos1().y
            };
            Segment currentSegment(currentP0, currentP1);

            f32 currentSegmentXDist = currentSegment.getSegment().getPos1().x - currentSegment.getSegment().getPos0().x;
            f32 currentSegmentYDist = currentSegment.getSegment().getPos1().y - currentSegment.getSegment().getPos0().y;
            f32 negativeCurrentSegmentYDist2 = currentSegmentYDist;
            f32 currentSegmentNegativeHypotenuse;

            if (currentSegmentXDist < -0.0001 || currentSegmentXDist > 0.0001) {
                if (currentSegmentXDist >= 0.0) {
                    if (currentSegmentYDist < 0.0) {
                        negativeCurrentSegmentYDist2 = -currentSegmentYDist;
                        if (currentSegmentXDist < -currentSegmentYDist) {
                            currentSegmentNegativeHypotenuse = currentSegmentXDist / negativeCurrentSegmentYDist2;
                            sead::Mathf::atanIdx(currentSegmentNegativeHypotenuse);
                            goto LABEL_22;
                        }
                    } else if (currentSegmentXDist < currentSegmentYDist) {
                        currentSegmentNegativeHypotenuse = currentSegmentXDist / negativeCurrentSegmentYDist2;
                        sead::Mathf::atanIdx(currentSegmentNegativeHypotenuse);
                        goto LABEL_22;
                    }
                    currentSegmentNegativeHypotenuse = negativeCurrentSegmentYDist2 / currentSegmentXDist;
                    sead::Mathf::atanIdx(currentSegmentNegativeHypotenuse);
                    goto LABEL_22;
                }

                if ( currentSegmentYDist < 0.0 ) {
                    if (currentSegmentXDist <= currentSegmentYDist) {
                        currentSegmentNegativeHypotenuse = negativeCurrentSegmentYDist2 / currentSegmentXDist;
                        sead::Mathf::atanIdx(currentSegmentNegativeHypotenuse);
                        goto LABEL_22;
                    }
                } else {
                    currentSegmentXDist = -currentSegmentXDist;
                    if (currentSegmentXDist >= currentSegmentYDist) {
                        currentSegmentNegativeHypotenuse = negativeCurrentSegmentYDist2 / currentSegmentXDist;
                        sead::Mathf::atanIdx(currentSegmentNegativeHypotenuse);
                        goto LABEL_22;
                    }
                }
                currentSegmentNegativeHypotenuse = currentSegmentXDist / negativeCurrentSegmentYDist2;
                sead::Mathf::atanIdx(currentSegmentNegativeHypotenuse);
            }
            LABEL_22:
            if ( (-(currentSegmentYDist * (current_pos->x - currentSegment.getSegment().getPos0().x))
                 - (currentSegmentXDist * (current_pos->y - currentSegment.getSegment().getPos0().y))) <= 0.0 ) {
                sead::Vector2f previousP0 { 
                    mFollowArg.position_prev->x + mPosOffset.x + mRideLinePrevArray[i].getSegment().getSegment().getPos0().x,
                    mFollowArg.position_prev->y + mPosOffset.y + mRideLinePrevArray[i].getSegment().getSegment().getPos0().y
                };
                sead::Vector2f previousP1 { 
                    mFollowArg.position_prev->x + mPosOffset.x + mRideLinePrevArray[i].getSegment().getSegment().getPos1().x,
                    mFollowArg.position_prev->y + mPosOffset.y + mRideLinePrevArray[i].getSegment().getSegment().getPos1().y
                };
                Segment previousSegment(previousP0, previousP1);

                if ( (-((previousSegment.getSegment().getPos1().y - previousSegment.getSegment().getPos0().y) * (previous_pos->x - previousSegment.getSegment().getPos0().x))
                    - ((previousSegment.getSegment().getPos1().x - previousSegment.getSegment().getPos0().x) * (previous_pos->y - previousSegment.getSegment().getPos0().y))) >= 0.0 ) {
                    return true;
                }
            }
        }
        return false;
    }

    //TODO!
    Segment* TightRopeBgCollision::vf84() {
        // if (mBcListFoot.front() == nullptr) {

        // }
        // sead::Vector2f currentP0 { 
        //     mFollowArg.position->x + mPosOffset.x + mRideLineArray[1].getSegment().getSegment().getPos0().x,
        //     mFollowArg.position->y + mPosOffset.y + mRideLineArray[1].getSegment().getSegment().getPos0().y
        // };
        // sead::Vector2f currentP1 { 
        //     mFollowArg.position->x + mPosOffset.x + mRideLineArray[1].getSegment().getSegment().getPos1().x,
        //     mFollowArg.position->y + mPosOffset.y + mRideLineArray[1].getSegment().getSegment().getPos1().y
        // };
        // Segment currentSegment(currentP0, currentP1);
        return &mRideLineArray[1].getSegment();
    }

    // bool TightRopeBgCollision::checkAreaActor(ActorBgCollisionCheckResult* check_result, sead::Vector2f* sensor_previous_pos, sead::Vector2f* sensor_current_pos, u8 direction, ActorBgCollisionCheck* check) {
    //     u64 v5; // d8
    //     u64 v6; // d9
    //     u64 v7; // d10
    //     u64 v8; // d11
    //     TightRopeBgCollision *CollisionOwnerFootPrev; // r0
    //     bool v12; // zf
    //     ActorBgCollisionCheck::Sensor* result; // r0
    //     BOOL v14; // r7
    //     sead::Vector3f *position; // r0
    //     BOOL v16; // r8
    //     int v17; // r4
    //     f32 v18; // s20
    //     f32 v19; // s18
    //     f32 v20; // s19
    //     BasicRideLine *sidesBuffer1; // r0
    //     const f32 *p_x; // r0
    //     f32 v23; // s0
    //     f32 v24; // s1
    //     BasicRideLine *v25; // r0
    //     const f32 *v26; // r0
    //     f32 v27; // s3
    //     f32 v28; // s4
    //     f32 v29; // s2
    //     f32 v30; // s3
    //     Actor *mpOwner; // r0
    //     f32 v32; // s1
    //     u32 hi; // r2
    //     sead::Vector2f p_mContactPoint; // [sp+4h] [bp-9Ch]
    //     f32 v35; // [sp+8h] [bp-98h] BYREF
    //     const f32 v36 = 0.0; // [sp+Ch] [bp-94h]
    //     ActorBgCollisionCheck::Sensor** a2; // [sp+10h] [bp-90h]
    //     //! sead::Vector2f v38; // [sp+18h] [bp-88h] BYREF
    //     //! u32 v39[2]; // [sp+20h] [bp-80h] BYREF
    //     sead::Segment2f v38; // [sp+18h] [bp-88h] BYREF
    //     //!
    //     sead::Vector2f p0; // [sp+28h] [bp-78h] BYREF
    //     sead::Vector2f a3; // [sp+30h] [bp-70h] BYREF
    //     Ride2Point v42; // [sp+38h] [bp-68h] BYREF
    //     u64 vn43; // [sp+4Ch] [bp-54h]
    //     u64 vn44; // [sp+54h] [bp-4Ch]
    //     u64 vn45; // [sp+5Ch] [bp-44h]
    //     u64 vn46; // [sp+64h] [bp-3Ch]
    //     TightRopeBgCollision *v43; // [sp+6Ch] [bp-34h]
    //     f32 *r1_0a; // [sp+70h] [bp-30h]
    //     sead::Vector2f *v45; // [sp+74h] [bp-2Ch]
    //     GLenum directiona; // [sp+78h] [bp-28h]
    //     bool result2;
    //     bool result3;
    //     bool result4;

    //     v43 = this;
    //     r1_0a = &sensor_previous_pos->x;
    //     v45 = sensor_previous_pos;
    //     directiona = direction;
    //     vn43 = v5;
    //     vn44 = v6;
    //     vn45 = v7;
    //     vn46 = v8;
        
     
    //     CollisionOwnerFootPrev = static_cast<TightRopeBgCollision*>(NSMB2_ActorBgCollisionCheck_getCollisionOwnerFootPrev(check));

    //     if (CollisionOwnerFootPrev == nullptr) {
    //         return false;
    //     }
    //     if (CollisionOwnerFootPrev == this) {
    //         return false;
    //     }

    //     p_mContactPoint = check_result->getContactPoint();
    //     if ( directiona == cDirType_Down ) {
    //         result = check->getSensor2(cDirType_Down);
    //         *a2 = result;
    //         if ( result != 0 ) {
    //             v14 = (check->getOutputPrev().isOnBit(0)) != 0;
    //             result2 = this->sub_2FD790(*r1_0a, sensor_current_pos->x);
    //             if ( result2 != 0 && (!v14 || (result3 = check->getOutputPrev().isOnBit(2)) != 0) ) {
    //                 position = this->mFollowArg.position;
    //                 v16 = false;
    //                 v17 = 0;
    //                 v18 = 3.4028e38;
    //                 v19 = position->x + this->mPosOffset.x;
    //                 v20 = position->y + this->mPosOffset.y;
    //                 do
    //                 {
    //                 sidesBuffer1 = this->mRideLineArray;
    //                 if ( v17 < 3 )
    //                 {
    //                     sidesBuffer1 += v17;
    //                 }
    //                 p_x = &sidesBuffer1->getSegment().getSegment().getPos0().x;
    //                 v23 = *p_x;
    //                 v24 = p_x[1];
    //                 v25 = this->mRideLineArray;
    //                 if ( v17 < 3 )
    //                 {
    //                     v25 += v17;
    //                 }
    //                 v26 = &v25->getSegment().getSegment().getPos1().x;
    //                 p0.x = v19 + v23;
    //                 p0.y = v20 + v24;
    //                 v27 = v26[1];
    //                 a3.x = v19 + *v26;
    //                 a3.y = v20 + v27;
    //                 v42.convertRide2Point(p0, a3);

    //                 if ( v14 )
    //                 {
    //                     //! TODO MAKE THIS SHI WORK BROSMAXXING IM CRINE
    //                     // Ride2Point::sub_205914(v42, a2, directiona, 1, 1);
    //                     v42.FUN_021b0044(*a2, directiona, 1, 1);
    //                 }
    //                 v38.setPos0(*sensor_current_pos);
    //                 v38.setPos1(sead::Vector2f(0.0f, 1.0f));
    //                 //! TODO MAKE THIS SHI WORK BROSMAXXING IM CRINE
    //                 sead::Vector2f bruh;
    //                 bruh = v38.getPos1();
    //                 if ( sub_49A0F4(&v42, &v35, v38, 0.0, bruh) != 0 )
    //                 {
    //                     v28 = v35 - sensor_current_pos->x;
    //                     v29 = v36 - sensor_current_pos->y;
    //                     v30 = 0.0;
    //                     if ( v14 )
    //                     {
    //                     v30 = -1.0;
    //                     }
    //                     if ( ((v28 * 0.0) + (v29 * 1.0)) > v30 || ((v28 * v28) + (v29 * v29)) == 0.0 )
    //                     {
    //                     mpOwner = check->getOwner();
    //                     v16 = true;
    //                     if ( static_cast<Actor*>(mpOwner)->getKind() == Actor::cActorKind_Player )
    //                     {
    //                         v16 = mpOwner->getSpeedVec().y <= 0.0;
    //                     }
    //                     v32 = ((*r1_0a - v35) * (*r1_0a - v35)) + ((r1_0a[1] - v36) * (r1_0a[1] - v36));
    //                     if ( v32 < v18 ) {
    //                             check_result->set_0(1);
    //                             check_result->setSolidOnTop(false);
    //                             check_result->set_2(1);
    //                             check_result->setHit(true);
    //                             hi = this->mBgCheckData >> sizeof(u32); // TODO ---- Fix this maybe
    //                             v18 = v32;
    //                             check_result->setBgCheckData((this->mBgCheckData & 0x00000000FFFFFFFF) + (hi << sizeof(u32))); //! What the fuck
    //                             check_result->setLayer(v17);
    //                             check_result->setAngle(0);
    //                             p_mContactPoint.x = v35;
    //                             check_result->getContactPoint().y = v36;
    //                             check_result->setOwner(this);
    //                         }
    //                     }
    //                 }
    //                 ++v17;
    //                 }
    //                 while ( v17 < 3 );
    //                 if ( v16 ) {
    //                     this->callCallbacks(check, static_cast<DirType>(directiona));
    //                     if ( ((this->mBgCheckData >> sizeof(u32)) & 0xF) == 0 ) {
    //                         return 0;
    //                     }
    //                 }
    //                 return v16;
    //             }
    //         }
    //     }
    //     else
    //     {
    //         if ( check->isHit(0xFF) != directiona || NSMB2_ActorBgCollisionCheck_getCollisionOwnerFoot(check) != this )
    //         {
    //         return 0;
    //         }
    //         check_result->set_0(1);
    //         check_result->setSolidOnTop(false);
    //         check_result->set_2(1);
    //         check_result->setBgCheckData(this->mBgCheckData);
    //         check_result->setOwner(this);
    //         check_result->setLayer(-1);
    //         check_result->setAngle(0);
    //         result4 = 1;
    //         p_mContactPoint.x = sensor_current_pos->x;
    //         check_result->getContactPoint().y = sensor_current_pos->y;
    //         check_result->setHit(false);

    //     }
    //     return result4;
    // }

    // void TightRopeBgCollision::execute() {
    //     Actor* v72[26];

    //     reset();
    //     _298[5] = 0;
    //     v72[0] = 0;
    //     v72[1] = 0;

    //     for (BcList::Node* i = mBcListFoot.front(); i != nullptr; i = i->next) {
    //         ActorBgCollisionCheck* check = i->obj;
    //         if (check != nullptr) {
    //             continue;
    //         }
    //         if (check->getOwner()->getKind() == Actor::cActorKind_Player) {
    //             continue; 
    //         }
    //         v72[_298[5]] = check->getOwner();
    //         _298[5]++;
    //         if (_298[5] == 1) {
    //             break;
    //         }
    //     }

    //     sead::Vector2f v68[2];
    //     if ( _298[5] <= 0 ) {
    //         _298[8] = 0;

    //         if (_2BC) {
    //             _2BC = 0;
    //             _2BD = 0;
    //         }
    //         smoothStepTowards(&this->mUnkVectors[1].x, *this->_298 - 0.1f, 0.125f, 1.0f, 0.01f);
    //         smoothStepTowards(&this->mUnkVectors[2].x, *this->_298 + 0.1f, 0.125f, 1.0f, 0.01f);
    //         this->mUnkVectors[1].y = sub_40B46C(this->_238,  0, *&this->_298[4]) + *&this->_298[3];
    //         this->mUnkVectors[2].y = sub_40B46C(&this->_238[8],  0, *&this->_298[4]) + *&this->_298[3];
    //     }
        
    // }
}