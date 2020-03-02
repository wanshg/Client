#include "Mesh.h"
//#include "../Render/Shader.h"
#include "Mathf.h"


void Mesh::enableVertexAttribs(unsigned int flags)
{
	glBindVertexArray(0);
	// hardcoded!
	for (int i = 0; i < MAX_ATTRIBUTES; i++) {
		unsigned int bit = 1 << i;
		bool enabled = flags & bit;
		bool enabledBefore = s_attributeFlags & bit;
		if (enabled != enabledBefore) {
			if (enabled)
				glEnableVertexAttribArray(i);
			else
				glDisableVertexAttribArray(i);
		}
	}
	s_attributeFlags = flags;
}

Mesh::Mesh(const char* _name) :
	name(_name)
	, _type(GL_TRIANGLES)
	, vertexSizeInFloat(0)
	, numIndex(0)
	, attribCount(0)
	, _vertexNumber(0)
	//, _sizePerVertex(0)
	//, _start(0)
	//, _count(0)
	, _initBuffer(false)
	//, _recreateVBOEventListener(nullptr)
{
	shader = new Shader("triangle.vertex", "triangle.frag");
	mesh_type = MESH_TYPE::STATIC;
}
Mesh::~Mesh()
{
	clear();
	glDeleteVertexArrays(1, &VAOId);
	glDeleteBuffers(1, &EBOId);
	for (auto id : VBOIdVec) {
		if (glIsBuffer(id))
			glDeleteBuffers(1, &id);
	}
}

bool Mesh::updateVertices()
{
	//return updateVertices((void*)&vertex[0], _indices.size(), 0);
	return false;
}

//------------------------------------------------------------------------------------------------
int Mesh::getVertexNumber() const
{
	return _vertexNumber;
}

void Mesh::setVerts(std::vector<Vector3> verts)
{
	_verts.clear();
	_vertexNumber = verts.size();
	for (int i = 0; i < _vertexNumber; i++)
	{
		_verts.push_back(verts[i].x);
		_verts.push_back(verts[i].y);
		_verts.push_back(verts[i].z);
	}
	//_verts = verts;
}
void Mesh::setNormals(std::vector<Vector3> normals)
{
	_normals.clear();
	for (int i = 0; i < normals.size(); i++)
	{
		_normals.push_back(normals[i].x);
		_normals.push_back(normals[i].y);
		_normals.push_back(normals[i].z);
	}
}
void Mesh::setUVs(int layer, std::vector<Vector2> texcoords)
{
	_texcoords.clear();
	if (layer != 0) return;
	for (int i = 0; i < texcoords.size(); i++)
	{
		_texcoords.push_back(texcoords[i].x);
		_texcoords.push_back(texcoords[i].y);
	}
}
void Mesh::setColors(std::vector<Color> colors)
{
	_colors.clear();
	for (int i = 0; i < colors.size(); i++)
	{
		_colors.push_back(colors[i].r);
		_colors.push_back(colors[i].g);
		_colors.push_back(colors[i].b);
		_colors.push_back(colors[i].a);
	}
}

void Mesh::apply()
{
	MeshVertexAttrib attrib;
	attrib.size = 3;
	attrib.type = GL_FLOAT;
	attribs.clear();
	int temSize = 0;
	if (_verts.size())
	{
		attrib.vertexAttrib = VERT_ATTR_POSITION;
		attrib.attribSizeBytes = attrib.size * sizeof(float);
		attrib.inptr = &_verts[0];
		attrib.ptrSize = _verts.size() * sizeof(float);
		attribs.push_back(attrib);
		//temSize += attrib.size;
	}

	//bool hasnormal = false, hastex = false;
	if (_normals.size())
	{
		//hasnormal = true;
		attrib.vertexAttrib = VERT_ATTR_NORMAL;
		attrib.attribSizeBytes = attrib.size * sizeof(float);;
		attrib.inptr = &_normals[0];
		attrib.ptrSize = _normals.size() * sizeof(float);
		attribs.push_back(attrib);
		//temSize += attrib.size;
	}

	if (_texcoords.size())
	{
		//hastex = true;
		attrib.size = 2;
		attrib.vertexAttrib = VERT_ATTR_TEXCOORD;
		attrib.attribSizeBytes = attrib.size * sizeof(float);
		attrib.inptr = &_texcoords[0];
		attrib.ptrSize = _texcoords.size() * sizeof(float);
		attribs.push_back(attrib);
		//temSize += attrib.size;
	}

	if (_colors.size())
	{
		//hascolor = true;
		attrib.size = 4;
		attrib.vertexAttrib = VERT_ATTR_COLOR;
		attrib.attribSizeBytes = attrib.size * sizeof(float);
		attrib.inptr = &_colors[0];
		attrib.ptrSize = _colors.size() * sizeof(float);
		attribs.push_back(attrib);
		//temSize += attrib.size;
	}

	if (Draw_type != DRAW_EBO && _indices.size())
	{
		attrib.type = GL_UNSIGNED_SHORT;
		//hascolor = true;
		attrib.size = 3;
		attrib.vertexAttrib = VERT_ATTR_BLEND_INDEX;
		attrib.attribSizeBytes = attrib.size * sizeof(unsigned short);
		attrib.inptr = &_indices[0];
		attrib.ptrSize = _indices.size() * sizeof(unsigned short);
		attribs.push_back(attrib);
		//temSize += attrib.size;
	}

	/*switch (Draw_type)
	{
	case DRAW_TYPE::DRAW_1:
		updateVertData_moreVBO();
		break;
	case DRAW_TYPE::DRAW_2:
		updateVertData_seqBatchVBO();
		break;
	case DRAW_TYPE::DRAW_3:
		updateVertData_interleavedBatchVBO();
		break;
	case DRAW_TYPE::DRAW_4:
		updateVertData_glBufferSubData();
		break;
	case DRAW_TYPE::DRAW_5:
		updateVertData_glMapBuffer();
		break;
	case DRAW_TYPE::DRAW_6:
		updateVertData_glCopyBufferSubData();
		break;
	default:
		updateEBO();
		break;
	}*/
}
void Mesh::clear()
{
	_verts.clear();
	_normals.clear();
	_texcoords.clear();
	_colors.clear();
	_indices.clear();
}
//------------------------------------------------------------------------------------------------
void Mesh::setMeshType(MESH_TYPE type)
{
	mesh_type = type;
}

void Mesh::setDrawType(DRAW_TYPE type)
{
	Draw_type = type;
}

void Mesh::prepare() {
	prepareEBO();
}

void Mesh::doDraw() {

	if (!_initBuffer) {
		setDrawType(DRAW_EBO);
		apply();
		prepare();
		_initBuffer = true;
	}

	//if ((void*)&vertex[0])
	//{
		// 这里填写场景绘制代码
		glBindVertexArray(VAOId);
		shader->use();
		// 启用纹理单元 绑定纹理对象
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, textureId);
		glUniform1i(glGetUniformLocation(shader->programId, "tex"), 0); // 设置纹理单元为0号
		glUniform1f(glGetUniformLocation(shader->programId, "colorMixValue"), colorMixValue);
		if (Draw_type == DRAW_EBO) {
			glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_SHORT, 0);
		}
		else {
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		glBindVertexArray(0);
		glUseProgram(0);

		////_verts->use();
		//if (&_indices[0] != nullptr)
		//{
		//	GLenum type = GL_UNSIGNED_SHORT;
		//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vbo[1]);
		//	size_t ofet = 0 * sizeof(unsigned short);// 2;
		//	glDrawElements((GLenum)_type, _indices.size(), type, (GLvoid*)ofet);
		//}
		//else
		//	glDrawArrays((GLenum)_type, 0, _indices.size());

		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//}


}

void Mesh::rebuild()
{
	glDeleteVertexArrays(1,&VAOId);
	glDeleteBuffers(1, &EBOId);
	for (auto id : VBOIdVec) {
		if(glIsBuffer(id))
			glDeleteBuffers(1,&id);
	}
}

//------------------------------------------------------------------------------------------------

//// 顶点属性每个独立的VBO
//void Mesh::prepareVertData_moreVBO()
//{
//	//// 单个属性独立
//	//GLfloat vertPos[] = {
//	//	-0.5f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f,  // 顶点位置
//	//};
//	//GLfloat vertColor[] = {
//	//	1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,   // 颜色
//	//};
//	//GLfloat vertTextCoord[] = {
//	//	0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f     // 纹理坐标
//	//};
//
//	// 创建缓存对象
//	//GLuint VBOId[3];
//	// Step1: 创建并绑定VAO对象
//	glGenVertexArrays(1, &VAOId);
//	glBindVertexArray(VAOId);
//
//	VBOIdVec.clear();
//	int i = 0;
//	for (auto& element : attribs)
//	{
//		GLuint VboId;
//		glGenBuffers(1, &VboId);
//		glBindBuffer(GL_ARRAY_BUFFER, VboId);
//
//		glBufferData(GL_ARRAY_BUFFER, element.ptrSize, element.inptr, GL_STATIC_DRAW);
//
//		glVertexAttribPointer(element.vertexAttrib, element.size, GL_FLOAT, GL_FALSE, 0, NULL);
//		glEnableVertexAttribArray(element.vertexAttrib);
//		VBOIdVec.push_back(VboId);
//		i++;
//	};
//
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glBindVertexArray(0);
//}
//// 顶点属性单个连续的整体VBO
//void Mesh::prepareVertData_seqBatchVBO()
//{
//	// 单个属性连续作为整体
//	GLfloat vertices[] = {
//		-0.5f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f,  // 顶点位置
//		1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,   // 颜色
//		0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f     // 纹理坐标
//	};
//	// 创建缓存对象
//	GLuint VBOId;
//	// Step1: 创建并绑定VAO对象
//	glGenVertexArrays(1, &VAOId);
//	glBindVertexArray(VAOId);
//	// Step2: 创建并绑定VBO 对象 传送数据
//	glGenBuffers(1, &VBOId);
//	glBindBuffer(GL_ARRAY_BUFFER, VBOId);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//	// Step3: 指定解析方式  并启用顶点属性
//	// 顶点位置属性
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0); // 紧密排列 stride也可以设置为 3 * sizeof(GL_FLOAT)
//	glEnableVertexAttribArray(0);
//	// 顶点颜色属性
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(9 * sizeof(GL_FLOAT)));
//	glEnableVertexAttribArray(1);
//	// 顶点纹理坐标
//	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(18 * sizeof(GL_FLOAT)));
//	glEnableVertexAttribArray(2);
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glBindVertexArray(0);
//
//	VBOIdVec.push_back(VBOId);
//}
//
//// 顶点属性之间交错的整体VBO
//void Mesh::prepareVertData_interleavedBatchVBO()
//{
//	// 交错指定顶点属性数据
//	GLfloat vertices[] = {
//		// 顶点位置 颜色 纹理纹理
//		-0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // 0
//		0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // 1
//		0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  // 2
//	};
//	// 创建缓存对象
//	GLuint VBOId;
//	// Step1: 创建并绑定VAO对象
//	glGenVertexArrays(1, &VAOId);
//	glBindVertexArray(VAOId);
//	// Step2: 创建并绑定VBO 对象 传送数据
//	glGenBuffers(1, &VBOId);
//	glBindBuffer(GL_ARRAY_BUFFER, VBOId);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//	// Step3: 指定解析方式  并启用顶点属性
//	// 顶点位置属性
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)0);
//	glEnableVertexAttribArray(0);
//	// 顶点颜色属性
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
//		8 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GL_FLOAT)));
//	glEnableVertexAttribArray(1);
//	// 顶点纹理坐标
//	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
//		8 * sizeof(GL_FLOAT), (GLvoid*)(6 * sizeof(GL_FLOAT)));
//	glEnableVertexAttribArray(2);
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glBindVertexArray(0);
//
//	VBOIdVec.push_back(VBOId);
//}
//// 使用glBufferSubData完成VBO
//void Mesh::prepareVertData_glBufferSubData()
//{
//	// 单个属性独立
//	GLfloat vertPos[] = {
//		-0.5f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f,  // 顶点位置
//	};
//	GLfloat vertColor[] = {
//		1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,   // 颜色
//	};
//	GLfloat vertTextCoord[] = {
//		0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f     // 纹理坐标
//	};
//	// 创建缓存对象
//	GLuint VBOId;
//	// Step1: 创建并绑定VAO对象
//	glGenVertexArrays(1, &VAOId);
//	glBindVertexArray(VAOId);
//	// Step2: 创建并绑定VBO 对象 预分配空间
//	glGenBuffers(1, &VBOId);
//	glBindBuffer(GL_ARRAY_BUFFER, VBOId);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertPos) + sizeof(vertColor) + sizeof(vertTextCoord),
//		NULL, GL_STATIC_DRAW);  // 预分配空间
//	// Step3: 填充具体的数据
//	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertPos), vertPos);
//	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertPos), sizeof(vertColor), vertColor);
//	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertPos) + sizeof(vertColor), sizeof(vertTextCoord), vertTextCoord);
//	// Step4: 指定解析方式  并启用顶点属性
//	// 顶点位置属性
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0); // 紧密排列 stride也可以设置为 3 * sizeof(GL_FLOAT)
//	glEnableVertexAttribArray(0);
//	// 顶点颜色属性
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(9 * sizeof(GL_FLOAT)));
//	glEnableVertexAttribArray(1);
//	// 顶点纹理坐标
//	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(18 * sizeof(GL_FLOAT)));
//	glEnableVertexAttribArray(2);
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glBindVertexArray(0);
//
//	VBOIdVec.push_back(VBOId);
//}
//
//// 使用glMapBuffer
//void Mesh::prepareVertData_glMapBuffer()
//{
//	// 交错指定顶点属性数据
//	GLfloat vertices[] = {
//		// 顶点位置 颜色 纹理纹理
//		-0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // 0
//		0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // 1
//		0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  // 2
//	};
//	// 创建缓存对象
//	GLuint VBOId;
//	// Step1: 创建并绑定VAO对象
//	glGenVertexArrays(1, &VAOId);
//	glBindVertexArray(VAOId);
//	// Step2: 创建并绑定VBO 对象
//	glGenBuffers(1, &VBOId);
//	glBindBuffer(GL_ARRAY_BUFFER, VBOId);
//	// Step3: 映射数据
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), NULL, GL_STATIC_DRAW); // 预分配空间
//	void* ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
//	if (!ptr)
//	{
//		std::cerr << " map buffer failed!" << std::endl;
//		return;
//	}
//	memcpy(ptr, vertices, sizeof(vertices));  // 可以操作这个指针指向的内存空间
//	glUnmapBuffer(GL_ARRAY_BUFFER);
//	// Step4: 指定解析方式  并启用顶点属性
//	// 顶点位置属性
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)0);
//	glEnableVertexAttribArray(0);
//	// 顶点颜色属性
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
//		8 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GL_FLOAT)));
//	glEnableVertexAttribArray(1);
//	// 顶点纹理坐标
//	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
//		8 * sizeof(GL_FLOAT), (GLvoid*)(6 * sizeof(GL_FLOAT)));
//	glEnableVertexAttribArray(2);
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glBindVertexArray(0);
//	
//	VBOIdVec.push_back(VBOId);
//}
//
//// 使用glCopyBufferSubData 
//void Mesh::prepareVertData_glCopyBufferSubData()
//{
//	// 交错指定顶点属性数据
//	GLfloat vertices[] = {
//		// 顶点位置 颜色 纹理纹理
//		-0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // 0
//		0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // 1
//		0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  // 2
//	};
//	// 创建缓存对象
//	GLuint VBOId;
//	// Step1: 创建并绑定VAO对象
//	glGenVertexArrays(1, &VAOId);
//	glBindVertexArray(VAOId);
//	// Step2: 创建并绑定VBO 对象
//	glGenBuffers(1, &VBOId);
//	glBindBuffer(GL_ARRAY_BUFFER, VBOId);
//	GLuint TempBufferId;
//	glGenBuffers(1, &TempBufferId);
//	glBindBuffer(GL_COPY_WRITE_BUFFER, TempBufferId);
//	// Step3: 映射数据到GL_COPY_WRITE_BUFFER
//	glBufferData(GL_COPY_WRITE_BUFFER, sizeof(vertices), NULL, GL_STATIC_DRAW); // 预分配空间
//	void* ptr = glMapBuffer(GL_COPY_WRITE_BUFFER, GL_WRITE_ONLY);
//	if (!ptr)
//	{
//		std::cerr << " map buffer failed!" << std::endl;
//		return;
//	}
//	memcpy(ptr, vertices, sizeof(vertices));  // 可以操作这个指针指向的内存空间
//	glUnmapBuffer(GL_COPY_WRITE_BUFFER);
//	// Step4: 将数据拷贝到VBO
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), NULL, GL_STATIC_DRAW); // 注意拷贝前预分配空间
//	glCopyBufferSubData(GL_COPY_WRITE_BUFFER, GL_ARRAY_BUFFER, 0, 0, sizeof(vertices));
//	glDeleteBuffers(1, &TempBufferId);
//	// Step5: 指定解析方式  并启用顶点属性
//	// 顶点位置属性
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)0);
//	glEnableVertexAttribArray(0);
//	// 顶点颜色属性
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
//		8 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GL_FLOAT)));
//	glEnableVertexAttribArray(1);
//	// 顶点纹理坐标
//	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
//		8 * sizeof(GL_FLOAT), (GLvoid*)(6 * sizeof(GL_FLOAT)));
//	glEnableVertexAttribArray(2);
//
//	glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glBindVertexArray(0);
//
//	VBOIdVec.push_back(VBOId);
//}

void Mesh::prepareEBO() {
	/*vertex.clear();
	auto vertexNum = _verts.size() / 3;
	for (auto i = 0; i < vertexNum; i++)
	{
		vertex.push_back(_verts[i * 3]);
		vertex.push_back(_verts[i * 3 + 1]);
		vertex.push_back(_verts[i * 3 + 2]);

		if (_normals.size())
		{
			vertex.push_back(_normals[i * 3]);
			vertex.push_back(_normals[i * 3 + 1]);
			vertex.push_back(_normals[i * 3 + 2]);
		}

		if (_texcoords.size())
		{
			vertex.push_back(_texcoords[i * 2]);
			vertex.push_back(_texcoords[i * 2 + 1]);
		}

		if (_colors.size())
		{
			vertex.push_back(_colors[i * 4]);
			vertex.push_back(_colors[i * 4+1]);
			vertex.push_back(_colors[i * 4+2]);
			vertex.push_back(_colors[i * 4+3]);
		}
	}

	int k = vertex.size() * sizeof(float);*/
	int d = _indices.size() * sizeof(unsigned short);

	// 创建缓存对象
	//GLuint VAOId;
	GLuint VBOId;
	// Step1: 创建并绑定VAO对象
	glGenVertexArrays(1, &VAOId);
	glBindVertexArray(VAOId);

	// Step2: 创建并绑定VBO 对象 传送数据
	//VBOIdVec.clear();
	int i = 0;
	for (auto& element : attribs)
	{
		GLuint VboId;
		glGenBuffers(1, &VboId);
		glBindBuffer(GL_ARRAY_BUFFER, VboId);

		glBufferData(GL_ARRAY_BUFFER, element.ptrSize, element.inptr, GL_STATIC_DRAW);

		glVertexAttribPointer(element.vertexAttrib, element.size, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(element.vertexAttrib);

		VBOIdVec[element.vertexAttrib] = VboId;
		//VBOIdVec.push_back(VboId);
		i++;
	};

	// Step3: 创建并绑定EBO 对象 传送数据
	glGenBuffers(1, &EBOId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, d, (void*)&_indices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	

	//clear();
}

//------------------------------------------------------------------------------------------------
//update

void Mesh::updateEBO() {

	//int d = _indices.size() * sizeof(unsigned short);
	//glBindVertexArray(VAOId);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOId);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, d, (void*)&_indices[0], GL_DYNAMIC_DRAW);

	////glBindBuffer(GL_ARRAY_BUFFER, EBOId);
	////glBufferSubData(GL_ARRAY_BUFFER, 0, d, (void*)&_indices[0]);

	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);
}

//------------------------------------------------------------------------------------------------

bool Mesh::setVertex(const unsigned int index, const Vector3 pos) {
	return setVertex(index,pos.x,pos.y,pos.z);
}
bool Mesh::setVertex(const unsigned int index, const float x, const float y, const float z) {
	if (index<0 || (index * 3)>= _verts.size()) return false;

	int startIndex = index * 3;
	_verts[startIndex] = x;
	_verts[startIndex + 1] = y;
	_verts[startIndex + 2] = z;

	glBindBuffer(GL_ARRAY_BUFFER, VBOIdVec[VERT_ATTR_POSITION]);
	glBufferSubData(GL_ARRAY_BUFFER, startIndex * sizeof(float), 3* sizeof(float), &_verts[startIndex]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return true;
}
Vector3 Mesh::getVertex(const unsigned int index)
{
	Vector3 v = Vector3(0,0,0);
	if (index<0 || (index * 3)>= _verts.size()) return v;
	int startIndex = index * 3;
	v.x = _verts[startIndex];
	v.y = _verts[startIndex + 1];
	v.z = _verts[startIndex + 2];
	return v;
}
void Mesh::addVertex(Vector3 pos) {
	addVertex(pos.x,pos.y,pos.z);
}
void Mesh::addVertex(float x, float y, float z) {
	_verts.push_back(x);
	_verts.push_back(y);
	_verts.push_back(z);

	glBindBuffer(GL_ARRAY_BUFFER, VBOIdVec[VERT_ATTR_POSITION]);
	glBufferData(GL_ARRAY_BUFFER, _verts.size() * sizeof(float), &_verts[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void Mesh::deleteVertex(const unsigned int index)
{
	if (index<0 || (index * 3 + 2)> _verts.size()) return;

	int startIndex = index * 3;
	_verts.erase(_verts.begin() + startIndex, _verts.begin() + startIndex + 3);

	
	int tri_num = _indices.size() / 3;
	for (int i = (tri_num-1); i >= 0;i--) {
		startIndex = i * 3;
		if (_indices[startIndex] == index || _indices[startIndex + 1] == index || _indices[startIndex + 2] == index) {
			_indices.erase(_indices.begin() + startIndex, _indices.begin() + startIndex + 3);
		}
	}
	
	glBindBuffer(GL_ARRAY_BUFFER, VBOIdVec[VERT_ATTR_POSITION]);
	glBufferData(GL_ARRAY_BUFFER, _verts.size() * sizeof(float), &_verts[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, EBOId);
	if (_indices.size() < 3) {
		glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW);
	}
	else {
		glBufferData(GL_ARRAY_BUFFER, _indices.size() * sizeof(unsigned short), (void*)&_indices[0], GL_DYNAMIC_DRAW);
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}
void Mesh::refreshAllVertex() {
	if (_verts.size() < 9) return;
	glBindBuffer(GL_ARRAY_BUFFER, VBOIdVec[VERT_ATTR_POSITION]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, _verts.size() * sizeof(float), &_verts[0]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

bool Mesh::setVertexColor(const unsigned int index, const Color col)
{
	return setVertexColor(index, col.r, col.g, col.b, col.a);
}
bool Mesh::setVertexColor(const unsigned int index, const float r, const float g, const float b, const float a)
{
	if (index<0 || (index * 4)>= _colors.size()) return false;

	int startIndex = index * 4;
	_colors[startIndex] = r;
	_colors[startIndex + 1] = g;
	_colors[startIndex + 2] = b;
	_colors[startIndex + 3] = a;

	glBindBuffer(GL_ARRAY_BUFFER, VBOIdVec[VERT_ATTR_COLOR]);
	glBufferSubData(GL_ARRAY_BUFFER, startIndex * sizeof(float), 4 * sizeof(float), &_colors[startIndex]);
	//glBufferSubData(GL_ARRAY_BUFFER, 0, _verts.size() * sizeof(float), &_verts[0]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return true;
}
Color Mesh::getVertexColor(const unsigned int index)
{
	Color v = Color(1.0f, 1.0f, 1.0f,1.0f);
	if (index<0 || (index * 3)>= _colors.size()) return v;
	int startIndex = index * 4;
	v.r = _colors[startIndex];
	v.g = _colors[startIndex + 1];
	v.b = _colors[startIndex + 2];
	v.a = _colors[startIndex + 3];
	return v;
}
void Mesh::addVertexColor(const Color col)
{
	addVertexColor(col.r,col.g,col.b,col.a);
}
void Mesh::addVertexColor(float r, float g, float b, float a)
{
	_colors.push_back(r);
	_colors.push_back(g);
	_colors.push_back(b);
	_colors.push_back(a);

	glBindBuffer(GL_ARRAY_BUFFER, VBOIdVec[VERT_ATTR_COLOR]);
	glBufferData(GL_ARRAY_BUFFER, _colors.size() * sizeof(float), &_colors[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


bool Mesh::setVertexUV(const unsigned int index, const Vector2 uv)
{
	return setVertexUV(index, uv.x,uv.y);
}
bool Mesh::setVertexUV(const unsigned int index, const float u, const float v)
{
	if (index<0 || (index * 4)>= _texcoords.size()) return false;

	int startIndex = index * 2;
	_texcoords[startIndex] = u;
	_texcoords[startIndex + 1] = v;

	glBindBuffer(GL_ARRAY_BUFFER, VBOIdVec[VERT_ATTR_TEXCOORD]);
	glBufferSubData(GL_ARRAY_BUFFER, startIndex * sizeof(float), 2 * sizeof(float), &_texcoords[startIndex]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return true;
}
Vector2 Mesh::getVertexUV(const unsigned int index)
{
	Vector2 v = Vector2(0.0f, 0.0f);
	if (index<0 || (index * 2)>= _texcoords.size()) return v;
	int startIndex = index * 2;
	v.x = _texcoords[startIndex];
	v.y = _texcoords[startIndex + 1];
	return v;
}
void Mesh::addVertexUV(const Vector2 uv)
{
	addVertexUV(uv.x,uv.y);
}
void Mesh::addVertexUV(float u, float v)
{
	_texcoords.push_back(u);
	_texcoords.push_back(v);
}

bool Mesh::setVertexNormal(const unsigned int index, const Vector3 nor)
{
	return setVertexNormal(index, nor.x, nor.y, nor.z);
}
bool Mesh::setVertexNormal(const unsigned int index, const float x, const float y, const float z)
{
	if (index<0 || (index * 3)>= _normals.size()) return false;

	int startIndex = index * 3;
	_normals[startIndex] = x;
	_normals[startIndex + 1] = y;
	_normals[startIndex + 2] = z;

	glBindBuffer(GL_ARRAY_BUFFER, VBOIdVec[VERT_ATTR_NORMAL]);
	glBufferSubData(GL_ARRAY_BUFFER, startIndex * sizeof(float), 3 * sizeof(float), &_normals[startIndex]);
	//glBufferSubData(GL_ARRAY_BUFFER, 0, _verts.size() * sizeof(float), &_verts[0]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return true;
}
Vector3 Mesh::getVertexNormal(const unsigned int index)
{
	Vector3 v = Vector3(0, 0, 0);
	if (index<0 || (index * 3)>= _normals.size()) return v;
	int startIndex = index * 3;
	v.x = _normals[startIndex];
	v.y = _normals[startIndex + 1];
	v.z = _normals[startIndex + 2];
	return v;
}
void Mesh::addVertexNormal(Vector3 nor)
{
	addVertexNormal(nor.x, nor.y, nor.z);
}
void Mesh::addVertexNormal(float x, float y, float z)
{
	_normals.push_back(x);
	_normals.push_back(y);
	_normals.push_back(z);
}

bool Mesh::setTriangle(const unsigned int index, const int p1, const int p2, const int p3)
{
	if (index<0 || (index * 3)>= _indices.size()) return false;

	int startIndex = index * 3;
	_indices[startIndex] = p1;
	_indices[startIndex + 1] = p2;
	_indices[startIndex + 2] = p3;

	glBindBuffer(GL_ARRAY_BUFFER, EBOId);
	glBufferSubData(GL_ARRAY_BUFFER, startIndex * sizeof(float), 3 * sizeof(float), &_indices[startIndex]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return true;
}
Vector3 Mesh::getTriangle(const unsigned int index)
{
	Vector3 v = Vector3(0, 0, 0);
	if (index<0 || (index * 3)>= _indices.size()) return v;
	int startIndex = index * 3;
	v.x = _indices[startIndex];
	v.y = _indices[startIndex + 1];
	v.z = _indices[startIndex + 2];
	return v;
}
void Mesh::addTriangle(const int p1, const int p2, const int p3)
{
	_indices.push_back(p1);
	_indices.push_back(p2);
	_indices.push_back(p3);

	glBindBuffer(GL_ARRAY_BUFFER, EBOId);
	glBufferData(GL_ARRAY_BUFFER, _indices.size() * sizeof(unsigned short), (void*)&_indices[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void Mesh::deleteTriangle(const unsigned int index)
{
	if (index<0 || (index * 3 + 2)> _indices.size()) return;
	int startIndex = index * 3;
	_indices.erase(_indices.begin()+startIndex, _indices.begin()+startIndex + 3);

	glBindBuffer(GL_ARRAY_BUFFER, EBOId);
	if (_indices.size() < 3) {
		glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW);
	}
	else {
		glBufferData(GL_ARRAY_BUFFER, _indices.size() * sizeof(unsigned short), (void*)&_indices[0], GL_DYNAMIC_DRAW);
	}
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void Mesh::refreshAllTriangle() {
	glBindBuffer(GL_ARRAY_BUFFER, EBOId);
	if (_indices.size() < 3) {
		glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW);
	}
	else {
		glBufferData(GL_ARRAY_BUFFER, _indices.size() * sizeof(unsigned short), (void*)&_indices[0], GL_DYNAMIC_DRAW);
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

BoundBox* Mesh::CalcBound()
{
	if (boundBox == nullptr)
	{
		boundBox = new BoundBox();
	}
	if (_verts.size() < 6) return boundBox;

	Vector3 min = Vector3(_verts[0], _verts[1], _verts[2]);
	Vector3 max = Vector3(_verts[0], _verts[1], _verts[2]);
	//float min_x = _verts[0]; float min_y = _verts[1]; float min_z = _verts[2];
	//float max_x = _verts[0]; float max_y = _verts[1]; float max_z = _verts[2];

	int vertNum = _verts.size() / 3;
	for (int i = 1; i < vertNum; i++) {
		int startIndex = i * 3;
		// 计算最新最小点.
		min.x = Mathf::Min(min.x, _verts[startIndex]);
		min.y = Mathf::Min(min.y, _verts[startIndex+1]);
		min.z = Mathf::Min(min.z, _verts[startIndex+2]);

		// 计算最新最大点.
		max.x = Mathf::Max(max.x, _verts[startIndex]);
		max.y = Mathf::Max(max.y, _verts[startIndex + 1]);
		max.z = Mathf::Max(max.z, _verts[startIndex + 2]);
	}
	boundBox->set(min, max);

	return boundBox;
}