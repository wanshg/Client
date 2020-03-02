#include "Object.h"

Object::Object():
_refreceCount(0)
, _autoRelease(false)
{

}
Object::~Object()
{

}

void Object::retain()
{
	_refreceCount++;
}
void Object::release()
{
	_refreceCount--;
	if (_autoRelease){
		if (_refreceCount <=0)
			delete(this);
	}
}

void Object::setAutoRelease(bool value)
{
	_autoRelease = value;
}
void Object::Unload(bool value)
{
	if (value)
		delete(this);
	else if (_refreceCount <= 0)
		delete(this);
}

