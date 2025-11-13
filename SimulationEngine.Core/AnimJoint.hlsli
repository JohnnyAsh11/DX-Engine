#ifndef __ANIMJOINT_H_
#define __ANIMJOINT_H_

#define MAX_JOINT_COUNT 100

// MUST FOLLOW 16 BYTE RULE.
struct Joint
{
	// - -
    float4x4 InvBindPose;
	// - -
    int id;
    int parentIndex;
    float2 padding;
	// - -
};

#endif //__ANIMJOINT_H_