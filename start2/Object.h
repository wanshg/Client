#ifndef OBJECT_H
#define OBJECT_H
class Object
{
public:
	Object();
	virtual ~Object();

public:
	void retain();
	void release();

	void setAutoRelease(bool value);
	void Unload(bool value);
private:
	int _refreceCount;
	bool _autoRelease;
};
#endif