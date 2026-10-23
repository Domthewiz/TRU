#pragma once

#include <actor/Profile.h>
#include <graphics/AnimModel.h>
#include <collision/ActorPolylineBgCollision.h>
#include <actor/ActorState.h>

namespace TRU {

    class TightRope : public ActorMultiState {
        SEAD_RTTI_OVERRIDE(TightRope, ActorMultiState);

    public:
        enum VisibilityFlags : u32 {
            cVisibilityFlags_Visible = 0,
            cVisibilityFlags_HideLeft,
            cVisibilityFlags_HideRight,
        };

    public:

    public:
        static Profile* sProfile;
        static const ActorCreateInfo cCreateInfo;

    public:
        TightRope(const ActorCreateParam& param);
        ~TightRope() override = default;
        
    public:
        Result create() override;
        bool execute() override;
        bool draw() override;

    private:
        void calcMdl_();
        
    protected:
        u8 _FEC[4];
        ActorPolylineBgCollision<2> mEndCol;
        // TightRopeBgCol mTightRopeCol;
        sead::Vector2f _1600[2];
        u8 _1610[8];                        // unk
        // dG3d::Allocator mAllocator;
        sead::Vector2f mSlope[2];
        sead::Vector3f mEndPositions[2];
        u8 _1648[8];                        // unk
        sead::Vector2f _1650[15];
        u8 _16C8[64];                       // unk
        u8 _1708[15][4];                    // ctor 12806C
        u8 _1744[15][4];                    // ctor 12806C
        // sead::TextureCtrGR mTexture;
        u8 _2A0C[4];                        // unk
        u64 mSwitchFlags;
        VisibilityFlags mVisibilityFlags;
        u8 _2A1C[4];                        // unk
    };

}