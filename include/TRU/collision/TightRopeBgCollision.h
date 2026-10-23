#pragma once

#include <collision/ActorPolylineBgCollision.h>
#include <collision/Ride2Point.h>

typedef u32 GLenum;

namespace TRU {

    class TightRopeBgCollision : public ActorPolylineBgCollision<3> {
            SEAD_RTTI_OVERRIDE_INLINE(TightRopeBgCollision, ActorPolylineBgCollision);

        public:
            TightRopeBgCollision(const ActorCreateParam& param);
            ~TightRopeBgCollision() override = default;
            
            void execute() override;

            bool checkAreaActor(ActorBgCollisionCheckResult*, sead::Vector2f* sensor_previous_pos, sead::Vector2f* sensor_current_pos, u8 direction, ActorBgCollisionCheck* check) override;

            void sub_2FD3F8(ActorBase* owner, u32 param_3);
            void sub_2FD434(const sead::Vector3f& param_2, u32 param_3);
            bool sub_2FD790(f32 r1_0, f32 r2_0);

            // virtual Ride2Point* vf84();

        public: // should be protected
            u8 _238[64];                   // unk
            sead::Vector2f mUnkVectors[4];
            u8 _298[36];                   // unk
            u8 _2BC;
            u8 _2BD;
            u8 _2BE[2];                    // unk (probably padding)
    };
}