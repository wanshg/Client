//#include "ColorCurve.h"
//
//
//bool cmp(ColorPoint x, ColorPoint y)
//{
//	if (x.getPos() > y.getPos())
//		return true;
//	//return 1;
//
////return -1;
//	return false;
//}
//
//ColorCurve::ColorCurve() {
//	ColorPoint* p_start = new ColorPoint();
//	ColorPoint* p_end = new ColorPoint();
//	p_end->setPos(1.0f);
//	_listpoints.push_back(p_start);
//	_listpoints.push_back(p_end);
//}
//
//ColorCurve::~ColorCurve() {
//	for (auto p : _listpoints) {
//		if (p) delete p;
//	}
//	_listpoints.clear();
//}
//Color ColorCurve::GetColor(ColorPoint* pt1, ColorPoint* pt2, float t)
//{
//	Color P1 = pt1->getColor();
//	Color P2 = pt2->getColor();
//
//	float r = (1 - t) * P1.r + t * P2.r;
//	float g = (1 - t) * P1.g + t * P2.g;
//	float b = (1 - t) * P1.b + t * P2.b;
//	float a = (1 - t) * P1.a + t * P2.a;
//
//	return Color(r, g, b, a);
//}
////t range 0-1
//Color ColorCurve::Evaluate(float t)
//{
//	int count = _listpoints.size();
//	if (count < 1) return Color::WHITE;
//	if (count == 1) return ((ColorPoint*) *_listpoints.begin())->getColor();
//
//	/*for (auto it = _listpoints.begin(); it != _listpoints.end(); it++) {
//		ColorPoint* p = *it;
//	}*/
//
//	float ct = Mathf::Clamp01(t);
//	float prev_t = 0;
//	float next_t = 1;
//	int i = 0;
//	for (i = 0; i < count - 1; i++)
//	{
//
//		/*prev_t = _listpoints[i].getPos();
//		next_t = _listpoints[i + 1].getPos();*/
//
//		prev_t =((ColorPoint*) *(_listpoints.begin+i))->getPos();
//		next_t = ((ColorPoint*) *(_listpoints.begin + i+1))->getPos();
//	
//		if (prev_t <= ct && next_t >= ct) break;
//	}
//
//	float new_t = (ct - prev_t) / (next_t - prev_t);
//
//	//Color v = GetColor(_listpoints[i], _listpoints[i + 1], new_t);
//	Color v = GetColor((ColorPoint*) *(_listpoints.begin + i), (ColorPoint*) *(_listpoints.begin + i + 1), new_t);
//	return v;
//}
//
//void ColorCurve::Insert(ColorPoint* point)
//{
//	_listpoints.push_back(point);
//	_listpoints.sort(cmp);
//}
//
//void ColorCurve::Delete(int index)
//{
//	if (index < 0 || index >= _listpoints.size()) return;
//	//_listpoints.RemoveAt(index);
//	ColorPoint* it = **(_listpoints.begin + index);
//	delete it;
//	_listpoints.erase(_listpoints.begin + index);
//}
//
///*void ColorCurve::DebugList()
//{
//for (int i = 0; i < _listpoints.Count; i++)
//{
//Debug::Log(_listpoints[i].getColor());
//}
//}*/