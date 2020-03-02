#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "Object.h"
#include "AnimationFramePoint.h"
#include "Matrix.h"
#include "GameObject.h"

//key point
class AnimationSubClipPoint {
public:
	AnimationSubClipPoint(unsigned short _frameIndex, Matrix4 _m=Matrix4::IDENTITY);
	~AnimationSubClipPoint() {};

	void setFramePos(unsigned short _framePos);
	void updateMatrix(Matrix4 _m);

	const unsigned short getFramePos() { return frameIndex; };
	const Matrix4& getMatrix() { return m; }
private:
	unsigned short frameIndex;
	Matrix4 m;
};

//single object
class AnimationSubClip {
public:
	AnimationSubClip();
	~AnimationSubClip();

	
	void insertPoint(AnimationSubClipPoint* point);
	void removePoint(unsigned short frame);

	void Sample(const unsigned short _frame);

	const unsigned short getKeyCount() {return _listFramePoint.size();};
	//AnimationSubClipPoint* getKeyPoint(unsigned short _index) { return **(_listFramePoint.begin+ _index); };
	AnimationSubClipPoint* getKeyPoint(unsigned short _index) { return _listFramePoint.at(_index); };

	const Matrix4 getCurMatrix() { return curMatrix; };

	AnimationSubClip* findChildIfNotThenAdd(std::string _name);
	AnimationSubClip* findChild(std::string _name);
	void addChildSubClip(std::string _name);
	void removeChildSubClip(std::string _name);

	void Sameple(GameObject* _rootObj, unsigned short _frame);
public:
	std::string name;
	
private:
	const Matrix4& getFrameMatrix(AnimationSubClipPoint* m1, AnimationSubClipPoint* m2 , float t);

	Matrix4 curMatrix;
	std::vector<AnimationSubClipPoint*> _listFramePoint;
	std::list<AnimationSubClip*> _childSubClip;
	//std::unordered_map<std::string, AnimationSubClip*> _childSubClip;
};


class AnimationClip :public Object
{
public:
	enum ClipWrapMode
	{
		Once,
		Loop
	};

public:
	AnimationClip(const char* _name);
	~AnimationClip();

	static AnimationClip* createClip(GameObject* obj);

	void setFps(unsigned short _fps);

	void addPoint(std::string& childName, unsigned short _frame, Matrix4 _m);
	void removePoint(std::string& childName, unsigned short _frame);

	void addPosPoint(std::string& childName, unsigned short _frame, Vector3 _pos);
	void addScalePoint(std::string& childName, unsigned short _frame, Vector3 _scale);
	void addRotationPoint(std::string& childName, unsigned short _frame, Vector3 _rotation);

	void addPoint(std::string& childName, unsigned short _frame, Vector3 _pos, Vector3 _scale, Vector3 _rotation);

	unsigned short getFrameCount() { return totalFrames; };
	ClipWrapMode getWrapMode() { return wrapMode; };

public:
	std::string name;
private:
	void Sameple(GameObject* _rootObj, unsigned short _frame);
	GameObject* rootObj;

	//unsigned short curFrameIndex = 0;
	unsigned short totalFrames = 0;
	float length;
	unsigned short fps;
	ClipWrapMode wrapMode;

	AnimationSubClip* rootClip;
	//std::unordered_map<std::string, AnimationSubClip*> _animationSubClips;
	//std::unordered_map<std::string, Matrix4> _animationFrameMatrix;
	//std::vector<SkinFrame*> frames;
	std::vector<void*> events;
};

