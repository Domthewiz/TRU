#include <collision/ActorBgCollisionCheck.h>
#include <collision/Ride2Point.h>
#include <TRU/utility/NSMB2Funcs.h>

//* X-------------------X
//* | START NSMB2 Funcs |
//* X-------------------X

//? BgCollision *__fastcall ActorBgCollisionCheck::getCollisionOwnerFootPrev(ActorBgCollisionCheck *this)
//? NSMB2 Address: 0x0049B104
BgCollision* NSMB2_ActorBgCollisionCheck_getCollisionOwnerFootPrev(ActorBgCollisionCheck* this_) {
    int collisionCat;
    if ( (this_->getOutputPrev().isOnBit(ActorBgCollisionCheck::Output::Bit::cBit_OnGround)) != 0
        && (collisionCat = this_->getBgCheckResultIdxPrev()[cDirType_Down]) < cBgCollisionCat_Num )
    {
        return this_->getBgCheckResultPrev()[cDirType_Down][collisionCat].getOwner();
    }
    else
    {
        return nullptr;
    }
}

//? BgCollision *__fastcall ActorBgCollisionCheck::getCollisionOwnerFoot(ActorBgCollisionCheck *this)
//? NSMB2 Address: 0x0049AB34
BgCollision* NSMB2_ActorBgCollisionCheck_getCollisionOwnerFoot(ActorBgCollisionCheck* this_) {
    int collisionCat;
    if ( (this_->getOutput().isOnBit(ActorBgCollisionCheck::Output::Bit::cBit_OnGround)) != 0
        && (collisionCat = this_->getBgCheckResultIdx()[cDirType_Down]) < cBgCollisionCat_Num )
    {
        return this_->getBgCheckResult()[cDirType_Down][collisionCat].getOwner();
    }
    else
    {
        return nullptr;
    }
}

//? float __fastcall sub_55FFC4(sead::Vector2f *sensor_pos, Ride2Point *ride_2_point, float *a3, float *pointer)
//? NSMB2 Address: 0x0055FFC4
f32 sub_55FFC4(sead::Segment2f* sensor_pos, Ride2Point* ride_2_point, sead::Vector2f* a3, float* pointer) {
    float rideX; // s11
    float rideY; // s12
    float currentSensorX; // s13
    float currentSensorY; // s14
    sead::Vector2f *prevSensorPos; // r0
    float rideDistanceX; // s7
    float rideDistanceY; // s5
    float previousSensorX; // s15
    float previousSensorY; // s16
    float sensorMinusRideX; // s2
    float rideHypotenuseSquared; // s1
    float sensorDistanceSquared; // s9
    float v16; // s8
    float v17; // s4
    float v18; // s0
    float v19; // s1
    float v20; // s5

    rideX = ride_2_point->getSegment().getSegment().getPos0().x;
    rideY = ride_2_point->getSegment().getSegment().getPos0().y;
    currentSensorX = sensor_pos->getPos0().x;
    currentSensorY = sensor_pos->getPos0().y;
    prevSensorPos->x = sensor_pos->getPos1().x;
    prevSensorPos->y = sensor_pos->getPos1().y;
    rideDistanceX = ride_2_point->getSegment().getSegment().getPos1().x - ride_2_point->getSegment().getSegment().getPos0().x;
    rideDistanceY = ride_2_point->getSegment().getSegment().getPos1().y - rideY;
    previousSensorX = prevSensorPos->x;
    previousSensorY = prevSensorPos->y;
    sensorMinusRideX = currentSensorX - ride_2_point->getSegment().getSegment().getPos0().x;
    rideHypotenuseSquared = (rideDistanceX * rideDistanceX) + (rideDistanceY * rideDistanceY);
    sensorDistanceSquared = (prevSensorPos->x * rideDistanceX) + (previousSensorY * rideDistanceY);
    v16 = (prevSensorPos->x * sensorMinusRideX) + (previousSensorY * (currentSensorY - rideY));
    v17 = -(sensorDistanceSquared * sensorDistanceSquared) - rideHypotenuseSquared;
    if ( rideHypotenuseSquared < -0.00000011921 )
    {
        if ( (v17 < -0.00000011921 || v17 > 0.00000011921)
          && (v18 = (-(sensorDistanceSquared * v16) - (1.0 * ((rideDistanceX * sensorMinusRideX) + (rideDistanceY * (currentSensorY - rideY)))))
          / v17) >= 0.0 )
        {
          if ( v18 > 1.0 )
          {
              v18 = 1.0;
          }
        }
        else
        {
            v18 = 0.0;
        }
        v19 = (v16 - (sensorDistanceSquared * v18)) / 1.0;
    }
    else
    {
        v18 = 0.0;
        v19 = -v16 / 1.0;
    }
    v20 = rideDistanceY * v18;
    //! Omg bro this is the end of me
    if ( a3->x != 0 )
    {
        a3->x = v19;
    }
    if ( pointer != nullptr )
    {
        *pointer = v18;
    }
    return ((((previousSensorX * v19) + currentSensorX) - ((rideDistanceX * v18) + rideX))
          * (((previousSensorX * v19) + currentSensorX) - ((rideDistanceX * v18) + rideX)))
          + ((((previousSensorY * v19) + currentSensorY) - (v20 + rideY)) * (((previousSensorY * v19) + currentSensorY) - (v20 + rideY)));
}

//? bool __fastcall sub_49A0F4(Ride2Point *ride2point, float *a2, float *sensor_pos_current, float a4, int a5)
//? NSMB2 Address: 0x0049A0F4
bool sub_49A0F4(Ride2Point *ride2point, float *a2, sead::Segment2f& sensor_pos_current, float a4, sead::Vector2f& a5) {
    float v8; // s4
    float v9; // s5
    float x; // s6
    f32 v11; // s2
    float y; // s1
    f32 v13; // s0
    sead::Vector2f* v15; // [sp+0h] [bp-18h] BYREF

    v15 = &a5;
    f32 v67777 = sub_55FFC4(&sensor_pos_current, ride2point, v15, nullptr);
    // if ( *(u32*)&v67777 >= 0x38D1B717 ) // evil floating point bit level hacking
    if ( v67777 >= 0.0001 )
    {
      return false;
    }
    v8 = sensor_pos_current.getPos0().x + (sensor_pos_current.getPos1().x * v15->x);
    v9 = sensor_pos_current.getPos0().y + (sensor_pos_current.getPos1().y * v15->x);
    if ( ride2point->getSegment().getSegment().getPos0().x < ride2point->getSegment().getSegment().getPos1().x )
    {
      x = ride2point->getSegment().getSegment().getPos0().x;
    }
    else
    {
      x = ride2point->getSegment().getSegment().getPos1().x;
      v11 = ride2point->getSegment().getSegment().getPos0().x;
    }
    if ( ride2point->getSegment().getSegment().getPos0().y >= ride2point->getSegment().getSegment().getPos1().y )
    {
      y = ride2point->getSegment().getSegment().getPos1().y;
    }
    else
    {
      y = ride2point->getSegment().getSegment().getPos0().y;
      v13 = ride2point->getSegment().getSegment().getPos1().y;
    }
    if ( x > v8 || y > v9 )
    {
      return false;
    }
    if ( a2 != nullptr )
    {
      *a2 = v8;
      a2[1] = v9;
    }
    return true;
}


// //? sub_205914
// //? NSMB2 Address: 0x0020592C
// u32 Ride2Point_sub_205914(Ride2Point *this_, ActorBgCollisionCheck::Sensor* a2, int a3, int a4, int a5) {
//   u32 result; // r0
//   float v10; // s2
//   float v11; // s1
//   const sead::Vector2f *p_mP1; // r9
//   float v13; // s5
//   float v14; // s4
//   float v15; // s1
//   float v16; // s2
//   s32 v17; // r5
//   float v18; // s16
//   float v19; // s18
//   float v20; // s17
//   float v21; // s19
//   float v22; // s2
//   float v23; // s1
//   float v24; // s3
//   unsigned int v25; // r0
//   int v26; // s0
//   float v27; // s0
//   f32 v28; // s4
//   sead::Vector2f *v29; // r6
//   float v30; // s2
//   float v31; // s1
//   f32 v32; // s0
//   sead::Vector2f v33; // [sp+0h] [bp-40h] BYREF
//   f32 sinOut; // [sp+8h] [bp-38h] BYREF
//   float cosOut[13]; // [sp+Ch] [bp-34h] BYREF

//   result = a4 | a5;
//   if ( (a4 | a5) == 0 )
//   {
//     return result;
//   }
//   v10 = this_->getSegment().getSegment().getPos0().x - this_->getSegment().getSegment().getPos0().x;
//   v11 = this_->getSegment().getSegment().getPos1().y - this_->getSegment().getSegment().getPos0().y;
//   p_mP1 = &this_->getSegment().getSegment().getPos1(); //! Maybe its not address
//   if ( v10 >= -0.0001 )
//   {
//     v13 = this_->getSegment().getSegment().getPos1().x - this_->getSegment().getSegment().getPos0().x;
//     if ( v10 <= 0.0001 )
//     {
//       if ( v11 < 0.0 )
//       {
//         result = -1073741824;
//       }
//       else
//       {
//         result = 0x40000000;
//       }
//       goto LABEL_29;
//     }
//   }
//   if ( v11 >= -0.0001 )
//   {
//     v14 = this_->getSegment().getSegment().getPos1().y - this_->getSegment().getSegment().getPos0().y;
//     if ( v10 < 0.0 )
//     {
//       result = 0x80000000;
//       goto LABEL_29;
//     }
//   }
//   else if ( v10 != 0.0 )
//   {
//     if ( v10 < 0.0 )
//     {
//       if ( v11 < 0.0 )
//       {
//         if ( v10 > v11 )
//         {
//           result = -1073741824 - sead::MathCalcCommon<float>::atanIdx( v10 / v11);
//         }
//         else
//         {
//           result = sead::MathCalcCommon<float>::atanIdx( v11 / v10) + 0x80000000;
//         }
//       }
//       else
//       {
//         v16 = -v10;
//         if ( v16 < v11 )
//         {
//           result = sead::MathCalcCommon<float>::atanIdx( v16 / v11) + 0x40000000;
//         }
//         else
//         {
//           result = 0x80000000 - sead::MathCalcCommon<float>::atanIdx( v11 / v16);
//         }
//       }
//     }
//     else if ( v11 < 0.0 )
//     {
//       v15 = -v11;
//       if ( v10 < v15 )
//       {
//         result = sead::MathCalcCommon<float>::atanIdx( v10 / v15) - 0x40000000;
//       }
//       else
//       {
//         result = -sead::MathCalcCommon<float>::atanIdx( v15 / v10);
//       }
//     }
//     else if ( v10 < v11 )
//     {
//       result = 0x40000000 - sead::MathCalcCommon<float>::atanIdx( v10 / v11);
//     }
//     else
//     {
//       result = sead::MathCalcCommon<float>::atanIdx( v11 / v10);
//     }
// LABEL_29:
//     v17 = result;
//     switch ( a3 )
//     {
//       case 0:
//         v17 = result - 0x40000000;
//         if ( (result - 0x40000000) < 0 )
//         {
//           if ( result == 0xC0000000 ) // NOT SURE
//           {
//             return result;
//           }
//           result = 0x40000000 - result;
//         }
//         else
//         {
//           result -= 0x40000000;
//         }
//         if ( result > 0x20000000 )
//         {
//           return result;
//         }
//         a2->getDistFromCenterVert(&v33);
//         v19 = v33.y - a2->p2;
//         v18 = v33.x - a2->center_offset;
//         v21 = v18;
//         v20 = v33.y - a2->p1;
//         goto LABEL_38;
//       case 1:
//         v17 = result + 0x40000000;
//         if ( (result + 0x40000000) < 0 )
//         {
//           if ( result == 0x40000000 )
//           {
//             return result;
//           }
//           result = -v17;
//         }
//         else
//         {
//           result += 0x40000000;
//         }
//         if ( result > 0x20000000 )
//         {
//           return result;
//         }
//         a2->getDistFromCenterVert(&v33);
//         v19 = v33.y - a2->p1;
//         v18 = v33.x - a2->center_offset;
//         v21 = v18;
//         v20 = v33.y - a2->p2;
//         goto LABEL_38;
//       case 2:
//         v17 = result + 0x80000000;
//         if ( (result + 0x80000000) < 0 )
//         {
//           if ( result == 0 )
//           {
//             return result;
//           }
//           result = -v17;
//         }
//         else
//         {
//           result += 0x80000000;
//         }
//         if ( result > 0x20000000 )
//         {
//           return result;
//         }
//         a2->getDistFromCenterHoriz(&v33);
//         v18 = v33.x - a2->p1;
//         v19 = v33.y - a2->center_offset;
//         v20 = v19;
//         v21 = v33.x - a2->p2;
//         goto LABEL_38;
//       default:
//         break;
//     }
//     if ( (result & 0x80000000) != 0 )
//     {
//       if ( result == 0x80000000 )
//       {
//         return result;
//       }
//       result = -result;
//     }
//     if ( result <= 0x20000000 )
//     {
//       a2->getDistFromCenterHoriz(&v33);
//       v18 = v33.x - a2->p2;
//       v19 = v33.y - a2->center_offset;
//       v20 = v19;
//       v21 = v33.x - a2->p1;
// LABEL_38:
//       sead::MathCalcCommon<float>::sinCosIdx(&sinOut, cosOut, v17);
//       v22 = sinOut;
//       v23 = cosOut[0];
//       v24 = -sinOut;
//       v25 = (v17 * 2.0);
//       v26 = v25 & 0xFFFFFF;
//       //! TODO: Unfuck these
//     //   result = &sead::MathCalcCommon<float>::cSinCosTbl[(v25 >> sizeof(u16))];
//     //   v27 = *(result + 8) + (*(result + 12) * (v26 * 0.000000059605));
//       if ( a4 != 0 )
//       {
//         v28 = this_->getSegment().getSegment().getPos0().y + (((sinOut * v18) + (cosOut[0] * v19)) * v27);
//         // this_->getSegment().getSegment().getPos0().x = (this_->getSegment().getSegment().getPos0().x + (((cosOut[0] * v18) + (v24 * v19)) * v27));
//         // this_->getSegment().getSegment().getPos0().y = v28;
//       }
//       if ( a5 != 0 )
//       {
//         // v29 = &this_->getSegment().getSegment().getPos1();
//         v30 = (v22 * v21) + (v23 * v20);
//         v31 = ((v23 * v21) + (v24 * v20)) * v27;
//         v32 = p_mP1->y + (v30 * v27);
//         v29->x = p_mP1->x + v31;
//         v29->y = v32;
//       }
//     }
//     return result;
//   }
//   result = 0;
//   goto LABEL_29;
// }

//* X-----------------X
//* | END NSMB2 Funcs |
//* X-----------------X
