//#pragma once
//#include "Matrix.h"
//#include <unordered_map>
//
//class AnimationFrame
//{
//public:
//	AnimationFrame();
//	~AnimationFrame();
//
//	Matrix4[] matrices;
//
//	Quaternion rootMotionDeltaPositionQ;
//
//	Quaternion rootMotionDeltaRotation;
//
//	float rootMotionDeltaPositionL;
//
//public:
//	Matrix4 RootMotionInv(int rootBoneIndex)
//	{
//		if (!rootMotionInvInit)
//		{
//			rootMotionInv = matrices[rootBoneIndex].inverse;
//			rootMotionInvInit = true;
//		}
//		return rootMotionInv;
//	}
//
//private:
//	bool rootMotionInvInit = false;
//	Matrix4 rootMotionInv;
//
//};
//
