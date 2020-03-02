#pragma once
#include "Avatar.h"
class Animator
{
public:
	Animator();
	~Animator();

	void setAvatar(Avatar* _avatar);
private:
	Avatar* avatar;
};

