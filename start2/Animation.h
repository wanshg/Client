#pragma once
#include <vector>
#include "AnimationClip.h"

enum AnimationCullingMode {
	AlwaysAnimate,
	CullUpdateTransforms,
	CullCompletely,
};

class Animation
{
public:
	Animation();
	~Animation();


	void addClip(AnimationClip* _clip);
	void removeClip(std::string& clip_name);
	
	AnimationClip* getClip(unsigned short index);
	AnimationClip* getClip(std::string& clip_name);

	void play();
	void pause();
	void stop();

	void setPlayAutomatically(bool autoplay);

	void play(std::string& clipName);

	void awake();
	void update();
private:
	//Avatar* avatar;
	std::vector<AnimationClip*> clips;

	bool _isPlayAutomatically;
	bool _ispause;
	AnimationClip* curClip;
	unsigned short curClipFrameCount;
	unsigned short curFrameIndex;
	//Transform* bones;


	//----------------------
	//BoundBox bound;
	AnimationCullingMode cullingMode;
};

