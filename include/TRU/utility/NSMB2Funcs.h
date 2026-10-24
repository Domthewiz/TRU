#pragma once

#include <collision/Ride2Point.h>
#include <collision/ActorBgCollisionCheck.h>

//? BgCollision *__fastcall ActorBgCollisionCheck::getCollisionOwnerFootPrev(ActorBgCollisionCheck *this)
//? NSMB2 Address: 0x0049B104
BgCollision* NSMB2_ActorBgCollisionCheck_getCollisionOwnerFootPrev(ActorBgCollisionCheck* this_);

//? BgCollision *__fastcall ActorBgCollisionCheck::getCollisionOwnerFoot(ActorBgCollisionCheck *this)
//? NSMB2 Address: 0x0049AB34
BgCollision* NSMB2_ActorBgCollisionCheck_getCollisionOwnerFoot(ActorBgCollisionCheck* this_);

//? sub_205914
//? NSMB2 Address: 0x0020592C
u32 Ride2Point_sub_205914(Ride2Point *this_, ActorBgCollisionCheck::Sensor* a2, int a3, int a4, int a5);

//? float __fastcall sub_55FFC4(sead::Vector2f *sensor_pos, Ride2Point *ride_2_point, float *a3, float *pointer)
//? NSMB2 Address: 0x0055FFC4
f32 sub_55FFC4(sead::Segment2f* sensor_pos, Segment* ride_2_point, f32* a3, float* pointer);

//? bool __fastcall sub_49A0F4(Ride2Point *ride2point, float *a2, float *sensor_pos_current, float a4, int a5)
//? NSMB2 Address: 0x0049A0F4
bool sub_49A0F4(Segment *ride2point, sead::Vector2f* vec_out, sead::Segment2f& sensor_pos_current, float a4, const sead::Vector2f& a5);

//? f32 __fastcall smoothStepTowards(f32 *value, f32 target, f32 scaleFactor, f32 maxStep, f32 minStep)
//? NSMB2 Address: 0x0040A550
f32 smoothStepTowards(f32 *value, f32 target, f32 scaleFactor, f32 maxStep, f32 minStep);