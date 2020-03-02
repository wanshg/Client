#include "AnimationClip.h"



//bool cmp(AnimationSubClipPoint x, AnimationSubClipPoint y)
//{
//	if (x.getFramePos() > y.getFramePos())
//		return true;
//	//return 1;
//
////return -1;
//	return false;
//}

AnimationSubClipPoint::AnimationSubClipPoint(unsigned short _frameIndex, Matrix4 _m) {
	this->frameIndex = _frameIndex;
	this->m = _m;
}

void AnimationSubClipPoint::setFramePos(unsigned short _framePos) {
	frameIndex = _framePos;
}

void AnimationSubClipPoint::updateMatrix(Matrix4 _m) {
	m = _m;
}



//---------------------------------------------------------------------------------

AnimationSubClip::AnimationSubClip() {

}
AnimationSubClip::~AnimationSubClip() {
	for (auto p : _listFramePoint) {
		if (p) delete p;
	}

	_listFramePoint.clear();

	for (auto p : _childSubClip) {
		if (p) delete p;
	}
	_childSubClip.clear();
}

//²åÖµÔËËã
const Matrix4& AnimationSubClip::getFrameMatrix(AnimationSubClipPoint* m1, AnimationSubClipPoint* m2 , float t) {
	Matrix4 m;
	return m;
}

void AnimationSubClip::insertPoint(AnimationSubClipPoint* point) {
	//_listFramePoint.push_back(point);
	//_listFramePoint.sort(cmp);

	int index = 1;
	for (auto x : _listFramePoint) {
		if (point->getFramePos() < x->getFramePos())
			break;
		index++;
	}

	_listFramePoint.insert(_listFramePoint.begin(), index, point);
	printf("");
}

void AnimationSubClip::removePoint(unsigned short frame) {
	//AnimationSubClipPoint* it = **(_listFramePoint.begin + frame);

	int index = -1;
	bool hasKey = false;
	for (auto x : _listFramePoint) {
		index++;
		if (x->getFramePos() == frame) {
			hasKey = true;
			delete x;
			break;
		}
	}

	//AnimationSubClipPoint* it = _listFramePoint.at(frame);
	//delete it;
	if(hasKey)
		_listFramePoint.erase(_listFramePoint.begin() + index);
}

AnimationSubClip* AnimationSubClip::findChildIfNotThenAdd(std::string _name) {
	short tIndex = _name.find("/");
	if (tIndex < 0) {
		for (auto child : _childSubClip) {
			if (child->name == _name) {
				return child;
			}
		}

		AnimationSubClip* finded = new AnimationSubClip();
		finded->name = _name.substr(0, tIndex);
		_childSubClip.push_back(finded);
		return finded;
	}
	else {
		std::string tName = _name.substr(0, tIndex);
		AnimationSubClip* finded = nullptr;
		for (auto child : _childSubClip) {
			if (child->name == tName) {
				finded = child;
				break;
			}
		}

		std::string s = _name.substr(tIndex+1, _name.size() - 1);
		if (finded) {
			return finded->findChildIfNotThenAdd(s);
		}
		else {
			AnimationSubClip* subClip = new AnimationSubClip();
			subClip->name = _name.substr(0, tIndex);
			_childSubClip.push_back(subClip);
			return subClip->findChildIfNotThenAdd(s);
		}
	}
}

AnimationSubClip* AnimationSubClip::findChild(std::string _name) {
	short tIndex = _name.find("/");
	if (tIndex < 0) {
		for (auto child : _childSubClip) {
			if (child->name == _name) {
				return child;
			}
		}
	}
	else {
		std::string tName = _name.substr(0, tIndex);
		for (auto child : _childSubClip) {
			if (child->name == tName) {
				std::string s = _name.substr(tIndex, _name.size() - 1);
				return child->findChild(s);
			}
		}
	}

	return nullptr;
}

void AnimationSubClip::addChildSubClip(std::string _name) {
	short tIndex = _name.find("/");
	if (tIndex < 0) {
		AnimationSubClip* finded = nullptr;
		for (auto child : _childSubClip) {
			if (child->name == _name) {
				finded = child;
				break;
			}
		}
		if (!finded) {
			finded = new AnimationSubClip();
			finded->name = _name.substr(0, tIndex);
			_childSubClip.push_back(finded);
		}
	}
	else {
		AnimationSubClip* subClip = new AnimationSubClip();
		subClip->name = _name.substr(0, tIndex);
		std::string s = _name.substr(tIndex, _name.size() - 1);
		_childSubClip.push_back(subClip);

		subClip->addChildSubClip(s);
	}
}

void AnimationSubClip::removeChildSubClip(std::string _name) {
	short tIndex = _name.find("/");
	if (tIndex < 0) {
		for (auto child : _childSubClip) {
			if (child->name == _name) {
				delete child;
				break;
			}
		}
	}
	else {
		for (auto child : _childSubClip) {
			std::string s = _name.substr(tIndex, _name.size() - 1);
			child->removeChildSubClip(s);
		}
	}
}

void AnimationSubClip::Sameple(GameObject* _rootObj, unsigned short _frame) {
	/*Sample(_frame);
	_rootObj->transform->setLocalMatrix(curMatrix);

	for (auto child : _childSubClip) {
		for (auto objChild : _rootObj->children) {
			if (child->name == objChild.name) {
				child->Sameple(objChild,_frame);
			}
		}
	}*/
}

void AnimationSubClip::Sample(const unsigned short _frame)
{
	int count = _listFramePoint.size();
	if (_frame >= _listFramePoint.back()->getFramePos())
	{
		_listFramePoint.back()->getMatrix();
		return;
	} 

	unsigned short prev_t = 0;
	unsigned short next_t = 1;
	int i = 0;
	for (i = 0; i < count - 1; i++)
	{
		//prev_t = (*_listFramePoint.begin() + i)->getFramePos();
		prev_t = _listFramePoint.at(i)->getFramePos();
		if (prev_t == _frame) {
			curMatrix = _listFramePoint.back()->getMatrix();
			return;
		}
		//next_t = (*(_listFramePoint.begin + i) + 1)->getFramePos();
		next_t = _listFramePoint.at(i + 1)->getFramePos();
		if (next_t == _frame) {
			curMatrix = _listFramePoint.back()->getMatrix();
			return;
		}

		if (prev_t <= _frame && next_t >= _frame) break;
	}

	float new_t = ((float)(_frame - prev_t)) / (next_t - prev_t);

	//curMatrix = getFrameMatrix((AnimationSubClipPoint*) *(_listFramePoint.begin + i), (AnimationSubClipPoint*) *(_listFramePoint.begin + i + 1), new_t);
	curMatrix = getFrameMatrix(_listFramePoint.at(i), _listFramePoint.at(i + 1), new_t);
}




//---------------------------------------------------------------------------------

AnimationClip* AnimationClip::createClip(GameObject* obj) {
	AnimationClip* c = new AnimationClip(obj->name.c_str());
	return c;
}

AnimationClip::AnimationClip(const char* _name)
{
	setAutoRelease(true);
	name = _name;
	length = 1.0f;
	fps = 30;
	wrapMode = ClipWrapMode::Once;
	rootClip = new AnimationSubClip();
	rootClip->name = "root";
}


AnimationClip::~AnimationClip()
{
	if (rootClip) 
		delete rootClip;
}

void AnimationClip::Sameple(GameObject* _rootObj, unsigned short _frame)
{
	rootClip->Sameple(_rootObj,_frame);
}

void AnimationClip::setFps(unsigned short _fps) {
	fps = _fps;
}

void AnimationClip::addPosPoint(std::string& childName, unsigned short _frame, Vector3 _pos) {

}

void AnimationClip::addScalePoint(std::string& childName, unsigned short _frame, Vector3 _scale) {

}

void AnimationClip::addRotationPoint(std::string& childName, unsigned short _frame, Vector3 _rotation) {

}

void AnimationClip::addPoint(std::string& childName, unsigned short _frame, Vector3 _pos, Vector3 _scale, Vector3 _rotation) {

}

void AnimationClip::addPoint(std::string& childName, unsigned short _frame, Matrix4 _m) {
	if (childName.empty() || childName== "root") {

		AnimationSubClipPoint* p = new AnimationSubClipPoint(_frame, _m);
		rootClip->insertPoint(p);

		return;
	}

	auto it = rootClip->findChildIfNotThenAdd(childName);
	AnimationSubClipPoint* p = new AnimationSubClipPoint(_frame, _m);
	it->insertPoint(p);

	if (totalFrames < _frame) {
		totalFrames = _frame;
	}
}

void AnimationClip::removePoint(std::string& childName, unsigned short _frame) {

}