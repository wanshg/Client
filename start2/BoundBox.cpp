#include "BoundBox.h"
#include "Mathf.h"

BoundBox::BoundBox()
{
	reset();
}

BoundBox::BoundBox(const Vector3& min, const Vector3& max)
{
	set(min, max);
}

BoundBox::BoundBox(const BoundBox& box)
{
	set(box._min, box._max);
}

Vector3 BoundBox::getCenter()
{
	Vector3 center;
	center.x = 0.5f*(_min.x + _max.x);
	center.y = 0.5f*(_min.y + _max.y);
	center.z = 0.5f*(_min.z + _max.z);

	return center;
}

void BoundBox::getCorners(Vector3 *dst) const
{
	assert(dst);

	// ����.
	// ��-��-ǰ.
	dst[0].set(_min.x, _max.y, _max.z);
	// ��-��-ǰ.
	dst[1].set(_min.x, _min.y, _max.z);
	// ��-��-ǰ.
	dst[2].set(_max.x, _min.y, _max.z);
	// ��-��-ǰ.
	dst[3].set(_max.x, _max.y, _max.z);

	// Զ��.
	// ��-��-��.
	dst[4].set(_max.x, _max.y, _min.z);
	// ��-��-��.
	dst[5].set(_max.x, _min.y, _min.z);
	// ��-��-��.
	dst[6].set(_min.x, _min.y, _min.z);
	// ��-��-��.
	dst[7].set(_min.x, _max.y, _min.z);
}

bool BoundBox::intersects(const BoundBox& aabb) const
{
	return ((_min.x >= aabb._min.x && _min.x <= aabb._max.x) || (aabb._min.x >= _min.x && aabb._min.x <= _max.x)) &&
		((_min.y >= aabb._min.y && _min.y <= aabb._max.y) || (aabb._min.y >= _min.y && aabb._min.y <= _max.y)) &&
		((_min.z >= aabb._min.z && _min.z <= aabb._max.z) || (aabb._min.z >= _min.z && aabb._min.z <= _max.z));
}

bool BoundBox::containPoint(const Vector3& point) const
{
	if (point.x < _min.x) return false;
	if (point.y < _min.y) return false;
	if (point.z < _min.z) return false;
	if (point.x > _max.x) return false;
	if (point.y > _max.y) return false;
	if (point.z > _max.z) return false;
	return true;
}

void BoundBox::merge(const BoundBox& box)
{
	// ����������С��.
	_min.x = Mathf::Min(_min.x, box._min.x);
	_min.y = Mathf::Min(_min.y, box._min.y);
	_min.z = Mathf::Min(_min.z, box._min.z);

	// ������������.
	_max.x = Mathf::Max(_max.x, box._max.x);
	_max.y = Mathf::Max(_max.y, box._max.y);
	_max.z = Mathf::Max(_max.z, box._max.z);
}

void BoundBox::set(const Vector3& min, const Vector3& max)
{
	this->_min = min;
	this->_max = max;
}

void BoundBox::reset()
{
	_min.set(99999.0f, 99999.0f, 99999.0f);
	_max.set(-99999.0f, -99999.0f, -99999.0f);
}

bool BoundBox::isEmpty() const
{
	return _min.x > _max.x || _min.y > _max.y || _min.z > _max.z;
}

void BoundBox::updateMinMax(const Vector3* point, size_t num)
{
	for (size_t i = 0; i < num; i++)
	{
		// �����.
		if (point[i].x < _min.x)
			_min.x = point[i].x;

		// ��׵�.
		if (point[i].y < _min.y)
			_min.y = point[i].y;

		// ��Զ��.
		if (point[i].z < _min.z)
			_min.z = point[i].z;

		// ���ҵ�.
		if (point[i].x > _max.x)
			_max.x = point[i].x;

		// ��ߵ�.
		if (point[i].y > _max.y)
			_max.y = point[i].y;

		// �����.
		if (point[i].z > _max.z)
			_max.z = point[i].z;
	}
}

void BoundBox::transform(const Matrix4& mat)
{
	//Vector3 corners[8];
	//// ����
	//// ��-��-ǰ.
	//corners[0].set(_min.x, _max.y, _max.z);
	//// ��-��-ǰ.
	//corners[1].set(_min.x, _min.y, _max.z);
	//// ��-��-ǰ.
	//corners[2].set(_max.x, _min.y, _max.z);
	//// ��-��-ǰ.
	//corners[3].set(_max.x, _max.y, _max.z);

	//// Զ��
	//// ��-��-��.
	//corners[4].set(_max.x, _max.y, _min.z);
	//// ��-��-��.
	//corners[5].set(_max.x, _min.y, _min.z);
	//// ��-��-��.
	//corners[6].set(_min.x, _min.y, _min.z);
	//// ��-��-��.
	//corners[7].set(_min.x, _max.y, _min.z);

	//// �ƶ��ǵ�, ��·�����¼�����С����.
	//for (int i = 0; i < 8; i++)
	//	mat.transformPoint(&corners[i]);

	//reset();

	//updateMinMax(corners, 8);
}

