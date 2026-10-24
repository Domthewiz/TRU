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
            //! TODO: This lmao
            virtual Segment* vf84();
            
            void init(ActorBase* owner, u32 param_3);
            void sub_2FD434(const sead::Vector3f& param_2, u32 param_3);

            //! These vectors should probably be ( const sead::Vector2f& )
            bool sub_2FD790(sead::Vector2f* previous_pos, sead::Vector2f* current_pos);

        public:
            struct VectorStructInfo {
                f32 _0;
                f32 _4;
                f32 _8;
                f32 _c;
                f32 _10;
                f32 _14;
                f32 _18;
                u16 _1c;
                u8  _1e;
                u8  _1f;
            };
            
            struct IdkBroStruct {
                u32 _0;
                u32 _4;
                u32 _8;
                u32 _c;
                u32 _10;
                ActorBase* mPlayer;
                u32 _18;
                u32 _1c;
                u32 _20;
            };
            
        protected: // should be protected
            u32 _238[16];                  // unk
            sead::Vector2f mUnkVectors[4];
             f32 mRunningTarget;                   ///< unk
            u32 _29C;
            u32 _2A0;
            f32 _2A4;
            u32 _2A8;
            ActorBase* mPlayer;
            u32 _2B0;
            u32 _2B4;
            u32 _2B8;
            u8 _2BC;
            u8 _2BD;
            u8 _2BE[2];                    // unk (probably padding)
    };
}