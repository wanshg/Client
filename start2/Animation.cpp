#include "Animation.h"

Animation::Animation() {
	cullingMode = AnimationCullingMode::AlwaysAnimate;
	_ispause = true;
	curClip = nullptr;
	curFrameIndex = 0;
	curClipFrameCount = 0;
}

Animation::~Animation() {
	for (auto p : clips) {
		if (p) delete p;
	}
	clips.clear();
}


void Animation::addClip(AnimationClip* _clip) {
	clips.push_back(_clip);
	_clip->retain();
	if (clips.size() == 1) {

		curClip = clips.front();
		curClipFrameCount = curClip->getFrameCount();
		curFrameIndex = 0;

		if (_isPlayAutomatically)
			play();
	}
}
void Animation::removeClip(std::string& clip_name) {
	//if (curClip != nullptr) {curClip = nullptr;}

	std::vector<AnimationClip*>::iterator it;
	for (it = clips.begin(); it != clips.end(); it++) {
		if ((*it)->name == clip_name) {
			break;
		}
	}

	if (it != clips.end()) {
		(*it)->release();
		clips.erase(it);
		delete (*it);
	}

}

AnimationClip* Animation::getClip(unsigned short index) {
	if (index > clips.size()) return nullptr;
	
}

AnimationClip* Animation::getClip(std::string& clip_name) {
	for (auto clip : clips) {
		if (clip->name == clip_name) {
			return clip;
		}
	}
}

void Animation::play() {
	_ispause = false;
	if (curFrameIndex == curClipFrameCount) curFrameIndex = 0;
}

void Animation::play(std::string& clipName) {
	for (auto clip : clips) {
		if (clip->name == clipName) {
			curClip = clip;
			curClipFrameCount = curClip->getFrameCount();
			curFrameIndex = 0;
			break;
		}
	}
}

void Animation::pause() {
	_ispause = true;
}

void Animation::stop() {
	_ispause = true;
	curFrameIndex = 0;
}

void Animation::setPlayAutomatically(bool autoplay) {
	_isPlayAutomatically = autoplay;
}

void Animation::awake() {
	if (clips.size() > 0) {
		
		curClip = clips.front();
		curClipFrameCount = curClip->getFrameCount();
		curFrameIndex = 0;

		if (_isPlayAutomatically) {
			play();
		}	
	}
}

void Animation::update() {
	if (_ispause || clips.size() == 0 || curClip==nullptr) return;

	curFrameIndex++;

	if (curFrameIndex > curClipFrameCount) {
		if (curClip->getWrapMode() == AnimationClip::ClipWrapMode::Once) {
			curFrameIndex = curClipFrameCount;
			_ispause = true;
			return;
		}
		else {
			curFrameIndex = 0;
		}
	}

	switch (cullingMode) {
		case AnimationCullingMode::AlwaysAnimate:
			//curClip->Sample(gameObject,curFrameIndex);
			break;
		case AnimationCullingMode::CullCompletely:

			break;
		case AnimationCullingMode::CullUpdateTransforms:

			break;
	}

}