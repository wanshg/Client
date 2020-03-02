#ifndef _BEZIER_CURVES_H_
#define _BEZIER_CURVES_H_
#include <list>
#include "Vectors.h"


enum BezierSideControlType
{
	Free,
	Line,
	Constant,
	//Weighted,
};

enum BezierSide
{
	Left,
	Right,
};


class BezierPoint
{
public:
	BezierPoint();
	~BezierPoint();
	
	void setLeftTangent(float x, float y);
	void setRightTangent(float x, float y);
	Vector2 getLeftTangent();
	Vector2 getRightTangent();
private:
	Vector2 leftTangent;
	Vector2 rightTangent;
public:
	float curve_delta = 0.0f;
	Vector2 point;
	BezierSideControlType leftTangentType;
	BezierSideControlType rightTangentType;
};

class BezierCurve {

public:
	BezierCurve();
	~BezierCurve();
private:
	
	std::list<BezierPoint*> _listpoints;

	Vector2 GetBezierPoint(BezierPoint* pt1, BezierPoint* pt2, float t);

	//t range 0-1
public:
	float Evaluate(float t);

	void Insert(BezierPoint* point);


	void Delete(int index);

	//void DrawLine();

	//void DebugList();
};

#endif