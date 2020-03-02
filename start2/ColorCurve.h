#ifndef _COLOR_CURVES_H_
#define _COLOR_CURVES_H_
#include "Color.h"
#include "Mathf.h"
#include <list>



class ColorPoint {

public:
	ColorPoint() { x_delta = 0;  rgba = Color(1, 1, 1, 1);};
	~ColorPoint();

private:
	float x_delta; //[0-1]
	Color rgba;

	//[0-1]
public:
	void setColor(float r, float g, float b, float a)
	{
		r = Mathf::Clamp01(r);
		g = Mathf::Clamp01(g);
		b = Mathf::Clamp01(b);
		a = Mathf::Clamp01(a);
		rgba.r = r;
		rgba.g = g;
		rgba.b = b;
		rgba.a = a;
	}

	Color getColor(){return rgba;}

	void setPos(float x){x = Mathf::Clamp01(x);x_delta = x;}

	float getPos(){return x_delta;}
};

class ColorCurve
{
public:
	ColorCurve();
	~ColorCurve();

private:
	std::list<ColorPoint*> _listpoints;
	Color GetColor(ColorPoint* pt1, ColorPoint* pt2, float t);

public:
	//t range 0-1
	Color Evaluate(float t);

	void Insert(ColorPoint* point);

	void Delete(int index);

	/*public void DebugList()
	{
		for (int i = 0; i < _listpoints.Count; i++)
		{
			Debug.Log(_listpoints[i].getColor());
		}
	}*/
};

#endif