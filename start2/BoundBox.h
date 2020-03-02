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

	/*** 获取绑定盒的中心点.*/
	Vector3 getCenter();

	/* 近面,  正 z 轴.
	* verts[0] : 前向 左顶
	* verts[1] : 前向 左底
	* verts[2] : 前向 右底
	* verts[3] : 前向 右顶
	*
	* 远面,  负 z 轴.
	* verts[4] : 后向 右顶
	* verts[5] : 后向 右底
	* verts[6] : 后向 左底
	* verts[7] : 后向 左顶
	*/
	void getCorners(Vector3 *dst) const;

	/**
	* 测试绑定盒是否在某指定绑定盒内.
	*/
	bool intersects(const BoundBox& aabb) const;

	/**
	* 检查点是否在里面.
	*/
	bool containPoint(const Vector3& point) const;

	/**
	* 合并绑定盒
	*/
	void merge(const BoundBox& box);

	/**
	* 通过指定最小最大值设置绑定盒.
	*/
	void set(const Vector3& min, const Vector3& max);

	/**
	* 重置最小最大值.
	*/
	void reset();

	bool isEmpty() const;

	/**
	* 从指定点更新最小最大值.
	*/
	void updateMinMax(const Vector3* point, size_t num);

	/**
	* 设置绑定盒矩阵.
	*/
	void transform(const Matrix4& mat);

public:
	Vector3 _min;
	Vector3 _max;
};

#endif
