#pragma once

#include <GLEW/glew.h>
static const int MAX_ATTRIBUTES = 16;
static unsigned int s_attributeFlags = 0;  // 32 attributes max

struct vertex_index
{
	int v_idx, vt_idx, vn_idx;
	vertex_index() {};
	vertex_index(int idx) : v_idx(idx), vt_idx(idx), vn_idx(idx) {};
	vertex_index(int vidx, int vtidx, int vnidx) : v_idx(vidx), vt_idx(vtidx), vn_idx(vnidx) {};

};
/**Õ¯∏Ò∂•µ„ Ù–‘*/
struct MeshVertexAttrib
{
	//attribute size
	GLint size;
	//GL_FLOAT
	GLenum type;
	int  vertexAttrib;
	int attribSizeBytes;
	bool _normalize;
	int deltaSizePos;
	void *inptr;
	int ptrSize;
};

class MeshBase
{
public:
	MeshBase();
	~MeshBase();
};

