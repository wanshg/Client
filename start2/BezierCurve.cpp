//#include "BezierCurve.h"
//#include "Mathf.h"
//
//
//bool cmp(BezierPoint x, BezierPoint y)
//{
//	if (x.point.x > y.point.x)
//		return true;
//	//return 1;
//
////return -1;
//	return false;
//}
//
//BezierPoint::BezierPoint() {
//	curve_delta = 0.0f;
//	BezierSideControlType leftTangentType = BezierSideControlType::Free;
//	BezierSideControlType rightTangentType = BezierSideControlType::Free;
//	leftTangent = Vector2(0.3f, 0.3f);
//	rightTangent = Vector2(0.3f, 0.3f);
//}
//BezierPoint::~BezierPoint() {
//
//}
//void BezierPoint::setLeftTangent(float x, float y) {
//	leftTangent.x = x;
//	leftTangent.y = y;
//}
//
//void BezierPoint::setRightTangent(float x, float y)
//{
//	rightTangent.x = x;
//	rightTangent.y = y;
//}
//
//Vector2 BezierPoint::getLeftTangent() {
//	return leftTangent;
//}
//
//Vector2 BezierPoint::getRightTangent()
//{
//	return rightTangent;
//}
//
//
//
//BezierCurve::BezierCurve() {
//
//}
//
//BezierCurve::~BezierCurve() {
//	for (auto p : _listpoints) {
//		if (p) delete p;
//	}
//
//	_listpoints.clear();
//}
//
//Vector2 BezierCurve::GetBezierPoint(BezierPoint* pt1, BezierPoint* pt2, float t)
//{
//	Vector2 P = Vector2(0,0);
//	if (pt1->rightTangentType == BezierSideControlType::Constant || pt2->leftTangentType == BezierSideControlType::Constant)
//	{
//		P.x = (1 - t) * pt1->point.x + t * pt2->point.x;
//		P.y = (t > 0.999f) ? pt2->point.y : pt1->point.y;
//	}
//	else if (pt1->rightTangentType == BezierSideControlType::Line && pt2->leftTangentType == BezierSideControlType::Line)
//	{
//		P.x = (1 - t) * pt1->point.x + t * pt2->point.x;
//		P.y = (1 - t) * pt1->point.y + t * pt2->point.y;
//	}
//	else if (pt1->rightTangentType == BezierSideControlType::Line || pt2->leftTangentType == BezierSideControlType::Line)
//	{
//		float pt1CtrRight_x = 0;
//		float pt1CtrRight_y = 0;
//
//		float pt2CtrLeft_x = 0;
//		float pt2CtrLeft_y = 0;
//
//		if (pt1->rightTangentType == BezierSideControlType::Line)
//		{
//			pt1CtrRight_x = pt1->point.x + (pt2->point.x - pt1->point.x) * pt1->getRightTangent().x;
//			pt1CtrRight_y = pt1->point.y + (pt2->point.y - pt1->point.y) * pt1->getRightTangent().x;
//		}
//		else {
//			pt1CtrRight_x = pt1->point.x + (pt2->point.x - pt1->point.x) * pt1->getRightTangent().x;
//			pt1CtrRight_y = pt1->point.y + (pt2->point.y - pt1->point.y) * pt1->getRightTangent().y;
//		}
//
//		if (pt2->leftTangentType == BezierSideControlType::Line)
//		{
//			pt2CtrLeft_x = pt2->point.x - (pt2->point.x - pt1->point.x) * pt2->getLeftTangent().x;
//			pt2CtrLeft_y = pt2->point.y - (pt2->point.y - pt1->point.y) * pt2->getLeftTangent().x;
//		}
//		else {
//			pt2CtrLeft_x = pt2->point.x - (pt2->point.x - pt1->point.x) * pt2->getLeftTangent().x;
//			pt2CtrLeft_y = pt2->point.y + (pt2->point.y - pt1->point.y) * pt2->getLeftTangent().y;
//		}
//
//		P.x = Mathf::Pow((1.0f - t), 3) * pt1->point.x + 3 * t * Mathf::Pow((1.0f - t), 2) * pt1CtrRight_x + 3 * (1.0f - t) * Mathf::Pow(t, 2) * pt2CtrLeft_x + Mathf::Pow(t, 3) * pt2->point.x;
//		P.y = Mathf::Pow((1.0f - t), 3) * pt1->point.y + 3 * t * Mathf::Pow((1.0f - t), 2) * pt1CtrRight_y + 3 * (1.0f - t) * Mathf::Pow(t, 2) * pt2CtrLeft_y + Mathf::Pow(t, 3) * pt2->point.y;
//	}
//	else {
//		float pt1CtrRight_x = pt1->point.x + (pt2->point.x - pt1->point.x) * pt1->getRightTangent().x;
//		float pt1CtrRight_y = pt1->point.y + (pt2->point.y - pt1->point.y) * pt1->getRightTangent().y;
//
//		float pt2CtrLeft_x = pt2->point.x - (pt2->point.x - pt1->point.x) * pt2->getLeftTangent().x;
//		float pt2CtrLeft_y = pt2->point.y + (pt2->point.y - pt1->point.y) * pt2->getLeftTangent().y;
//
//		P.x = Mathf::Pow((1.0f - t), 3) * pt1->point.x + 3 * t * Mathf::Pow((1.0f - t), 2) * pt1CtrRight_x + 3 * (1.0f - t) * Mathf::Pow(t, 2) * pt2CtrLeft_x + Mathf::Pow(t, 3) * pt2->point.x;
//		P.y = Mathf::Pow((1.0f - t), 3) * pt1->point.y + 3 * t * Mathf::Pow((1.0f - t), 2) * pt1CtrRight_y + 3 * (1.0f - t) * Mathf::Pow(t, 2) * pt2CtrLeft_y + Mathf::Pow(t, 3) * pt2->point.y;
//	}
//
//	return P;
//}
//
//float BezierCurve::Evaluate(float t) {
//	int count = _listpoints.size();
//	if (count < 2) return 0;
//
//
//	float ct = Mathf::Clamp01(t);
//	float prev_t = 0;
//	float next_t = 1;
//	int i = 0;
//	for (i = 0; i < count - 1; i++) {
//		/*prev_t = _listpoints[i].curve_delta;
//		next_t = _listpoints[i + 1].curve_delta;*/
//
//		prev_t = ((BezierPoint*) *(_listpoints.begin+i))->curve_delta;
//		next_t = ((BezierPoint*) *(_listpoints.begin + i+1))->curve_delta;
//
//		if (prev_t <= ct && next_t >= ct) break;
//	}
//
//	float new_t = (ct - prev_t) / (next_t - prev_t);
//	//Vector2 v = GetBezierPoint(_listpoints[i], _listpoints[i + 1], new_t);
//	Vector2 v = GetBezierPoint((BezierPoint*) *(_listpoints.begin + i), (BezierPoint*) *(_listpoints.begin + i + 1), new_t);
//	return v.y;
//}
//
//void BezierCurve::Insert(BezierPoint* point) {
//	_listpoints.push_back(point);
//	_listpoints.sort(cmp);
//
//	int count = _listpoints.size();
//	if (_listpoints.size() == 2) {
//		_listpoints.front()->curve_delta = 0.0f;
//		_listpoints.back()->curve_delta = 1.0f;
//	}
//	else if (_listpoints.size() > 2)
//	{
//		float start = _listpoints.front()->point.x;
//		float end = _listpoints.back()->point.x;
//		
//		for (int k = 0; k < count; k++) {
//			BezierPoint* it = (BezierPoint*)*(_listpoints.begin + k);
//			it->curve_delta = (it->point.x - start) / (end - start);
//		}
//	}
//}
//
//
//void BezierCurve::Delete(int index)
//{
//	if (index < 0 || index >= _listpoints.size()) return;
//	//_listpoints.RemoveAt(index);
//	BezierPoint* it = **(_listpoints.begin + index);
//	delete it;
//	_listpoints.erase(_listpoints.begin + index);
//}
//
////void BezierCurve::DrawLine() {
////	DebugList();
////	for (int i = 0; i < _listpoints.size() - 1; i++)
////	{
////		BezierPoint pt1 = _listpoints[i];
////		BezierPoint pt2 = _listpoints[i + 1];
////		for (int k = 0; k <= 20; k++)
////		{
////			GameObject o = GameObject.CreatePrimitive(PrimitiveType.Sphere);
////			Vector2 v = GetBezierPoint(pt1, pt2, k / (20.0f));
////			o.transform.position = v;
////		}
////	}
////}
//
////void BezierCurve::DebugList() {
////	for (int i = 0; i < _listpoints.Count; i++) {
////		Debug.Log(_listpoints[i].point);
////	}
////}