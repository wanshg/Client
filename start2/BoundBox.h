#ifndef _BoundBox_H_
#define _BoundBox_H_

#include "Vectors.h"
#include "Matrix.h"

class BoundBox
{

public:
	BoundBox();
	BoundBox(const Vector3& min, const Vector3& max);
	BoundBox(const BoundBox& box);

	/*** ��ȡ�󶨺е����ĵ�.*/
	Vector3 getCenter();

	/* ����,  �� z ��.
	* verts[0] : ǰ�� ��
	* verts[1] : ǰ�� ���
	* verts[2] : ǰ�� �ҵ�
	* verts[3] : ǰ�� �Ҷ�
	*
	* Զ��,  �� z ��.
	* verts[4] : ���� �Ҷ�
	* verts[5] : ���� �ҵ�
	* verts[6] : ���� ���
	* verts[7] : ���� ��
	*/
	void getCorners(Vector3 *dst) const;

	/**
	* ���԰󶨺��Ƿ���ĳָ���󶨺���.
	*/
	bool intersects(const BoundBox& aabb) const;

	/**
	* �����Ƿ�������.
	*/
	bool containPoint(const Vector3& point) const;

	/**
	* �ϲ��󶨺�
	*/
	void merge(const BoundBox& box);

	/**
	* ͨ��ָ����С���ֵ���ð󶨺�.
	*/
	void set(const Vector3& min, const Vector3& max);

	/**
	* ������С���ֵ.
	*/
	void reset();

	bool isEmpty() const;

	/**
	* ��ָ���������С���ֵ.
	*/
	void updateMinMax(const Vector3* point, size_t num);

	/**
	* ���ð󶨺о���.
	*/
	void transform(const Matrix4& mat);

public:
	Vector3 _min;
	Vector3 _max;
};

#endif
