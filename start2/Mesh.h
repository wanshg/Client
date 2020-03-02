#ifndef _MESH_2_H_
#define _MESH_2_H_

#include "MeshBase.h"
#include <vector>
#include "Vectors.h"
#include "Color.h"
#include "shader.h"
#include "BoundBox.h"
enum
{
	VERT_ATTR_POSITION,
	VERT_ATTR_COLOR,
	VERT_ATTR_TEXCOORD,
	VERT_ATTR_TEXCOORD1,
	VERT_ATTR_TEXCOORD2,
	VERT_ATTR_TEXCOORD3,
	VERT_ATTR_TEXCOORD4,
	VERT_ATTR_TEXCOORD5,
	VERT_ATTR_TEXCOORD6,
	VERT_ATTR_TEXCOORD7,
	VERT_ATTR_NORMAL,
	VERT_ATTR_BLEND_WEIGHT,
	VERT_ATTR_BLEND_INDEX,
	VERT_ATTR_MAX,
	VERT_ATTR_TEX_COORDS = VERT_ATTR_TEXCOORD,
};

enum DRAW_TYPE
{
	DRAW_1,
	DRAW_2,
	DRAW_3,
	DRAW_4,
	DRAW_5,
	DRAW_6,
	DRAW_EBO,
};
enum MESH_TYPE {
	STATIC,
	ANIMATION,
	PARTICLE,
};
//供固定网格使用
class Mesh
{
	struct t_weight {
		std::string boneName;
		std::vector<unsigned short> vertIds;
		std::vector<float> weights;
	};
public:
	std::vector <t_weight> weights;
	void setWeights(std::vector<t_weight> _weights) {
		weights.swap(_weights);
	}
	void setWeight(std::string& name, unsigned short vertId, float weights) {};
	float getWeight(std::string& name, unsigned short vertId) {
		for (auto w : weights) {
			if (w.boneName == name) {
				for (int i = 0; i < w.vertIds.size(); i++) {
					if (w.vertIds.at(i) == vertId) {
						return w.weights.at(i);
					}
				}
				return 0;
			}
		}
		return 0;
	};
	t_weight* getWeight(std::string& name) {
		for (auto w : weights) {
			if (w.boneName == name) {
				return &w;
			}
		}
		return nullptr;
	}
private:
	typedef std::vector<unsigned short> IndexArray;
	int vertexSizeInFloat;
	std::vector<IndexArray> subMeshIndices;
	std::vector<std::string> subMeshIds;
	int numIndex;
	std::vector<MeshVertexAttrib> attribs;
	int attribCount;
	bool _initBuffer;	//以后重调
public:

	friend class Animation;
	Mesh(const char* _name);
	virtual ~Mesh();

	/*void setOrignVerts(std::vector<Vector3> verts) {  };
	void setOrignNormals(std::vector<Vector3> normals) {  };
	void setOrignUVs(std::vector<Vector2> uvs) { };
	void setOrignColors(std::vector<Color> colors) {  };
	void setOrignIndexs(std::vector<vertex_index> indexs) {  };
	void updateOrignData() {};*/


	void setVerts(std::vector<Vector3> verts);
	void setNormals(std::vector<Vector3> normals);
	void setUVs(int layer, std::vector<Vector2> texcoords);
	void setColors(std::vector<Color> colors);
	//void setIndexs(std::vector<Color> indices);
	void setIndexs(std::vector<unsigned short> indices) {
		_indices = indices;
		//setCount(indices.size());
	};

	void draw();

	//int getSizePerVertex() const;
	int getVertexNumber() const;
	//bool updateVertices();
	//bool updateVertices(const void* verts, int count, int begin);

	//int getSize() const{ return _sizePerVertex * _vertexNumber; };
	void clear();
	void initBuffer();

	bool updateVertices();
	//bool updateVertices(const void* verts, int count, int begin);

	const char* name;
public:
	//std::vector<float> vertex;

	std::vector<float> _verts;
	std::vector<float> _normals;
	std::vector<unsigned short> _indices;
	std::vector<float> _texcoords;
	std::vector<float> _colors;

	int _type;
protected:

	//void initVAO();
	//void initVBO();
	void enableVertexAttribs(unsigned int flags = true);

	//bool enableVAO;
	//GLuint _vao;
	//GLuint _vbo[2];	//顶点
	//GLuint vbo;
	//int _sizePerVertex;
	int _vertexNumber;

private:
	MESH_TYPE mesh_type;
	GLuint VAOId;
	GLuint EBOId;
	GLuint VBOIdVec[13];

	Shader* shader;
	GLfloat colorMixValue = 0.5;
	DRAW_TYPE Draw_type;
	BoundBox* boundBox;

	void prepareEBO();


	void updateEBO();
	void setMeshType(MESH_TYPE type);
	void setDrawType(DRAW_TYPE type);
	void rebuild();
public:
	void apply();
	void prepare();
	void doDraw();

	bool setVertex(const unsigned int index, const Vector3 pos);
	bool setVertex(const unsigned int index, const float x, const float y, const float z);
	Vector3 getVertex(const unsigned int index);
	void addVertex(Vector3 pos);
	void addVertex(float x,float y,float z);
	void deleteVertex(const unsigned int index);
	void refreshAllVertex();

	bool setVertexColor(const unsigned int index, const Color col);
	bool setVertexColor(const unsigned int index, const float r, const float g, const float b,const float a = 1.0f);
	Color getVertexColor(const unsigned int index);
	void addVertexColor(const Color col);
	void addVertexColor(float r, float g, float b, float a=1.0f);

	bool setVertexUV(const unsigned int index, const Vector2 uv);
	bool setVertexUV(const unsigned int index, const float u, const float v);
	Vector2 getVertexUV(const unsigned int index);
	void addVertexUV(const Vector2 uv);
	void addVertexUV(float u, float v);

	bool setVertexNormal(const unsigned int index, const Vector3 nor);
	bool setVertexNormal(const unsigned int index, const float x, const float y, const float z);
	Vector3 getVertexNormal(const unsigned int index);
	void addVertexNormal(Vector3 nor);
	void addVertexNormal(float x, float y, float z);

	bool setTriangle(const unsigned int index, const int p1, const int p2, const int p3);
	Vector3 getTriangle(const unsigned int index);
	void addTriangle(const int p1, const int p2, const int p3);
	void deleteTriangle(const unsigned int index);
	void refreshAllTriangle();

	BoundBox* CalcBound();
};
#endif
