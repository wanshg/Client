#include "AceParse.h"
#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <vector>
#include "Matrix.h"
#include <sstream>
#include "Vectors.h"
#include "AnimationClip.h"

#include <map>

struct vertex_index
{
	int v_idx, vt_idx, vn_idx;
	vertex_index() {};
	vertex_index(int idx) : v_idx(idx), vt_idx(idx), vn_idx(idx) {};
	vertex_index(int vidx, int vtidx, int vnidx) : v_idx(vidx), vt_idx(vtidx), vn_idx(vnidx) {};
};
// for std::map
static inline bool operator<(const vertex_index& a, const vertex_index& b)
{
	if (a.v_idx != b.v_idx) return (a.v_idx < b.v_idx);
	if (a.vn_idx != b.vn_idx) return (a.vn_idx < b.vn_idx);
	if (a.vt_idx != b.vt_idx) return (a.vt_idx < b.vt_idx);
	return false;
}
struct Triangle_vertex {
	vertex_index i0;
	vertex_index i1;
	vertex_index i2;
};

t_Node* AceParse::findFromNodes(std::string& _name) {
	for (auto& _node : nodes) {
		if (_name == _node.name)
			return &_node;
	}
	return nullptr;
};

static std::vector<Vector3> verts;
static std::vector<Vector2> uvs;
static std::vector<Vector3> normals;
static std::vector<Vector3> indexs;
static std::vector<Vector3> fuvs;
static std::vector<std::string> tokens;
static std::vector <t_weight> weights;


static size_t updateVertex(std::map<vertex_index, size_t>& vertexCache, std::vector<Vector3>& positions, std::vector<Vector3>& normals, std::vector<Vector2>& texcoords, std::vector<t_weight>& weights,
	const std::vector<Vector3>& in_positions, const std::vector<Vector3>& in_normals, const std::vector<Vector2>& in_texcoords, std::vector<t_weight> &in_weights, const vertex_index& i)
{
	const auto it = vertexCache.find(i);

	if (it != vertexCache.end())
	{
		// 查找缓存
		return it->second;
	}

	assert(in_positions.size() > (i.v_idx));

	positions.push_back(in_positions[i.v_idx]);

	if (i.vn_idx >= 0)
	{
		normals.push_back(in_normals[i.vn_idx]);
	}

	if (i.vt_idx >= 0)
	{
		texcoords.push_back(in_texcoords[i.vt_idx]);
	}

	unsigned short idx = positions.size() - 1;
	vertexCache[i] = idx;


	for (auto& tweight : in_weights) {
		for (int k = 0; k < tweight.vertIds.size(); k++) {
			if (idx != i.v_idx && tweight.vertIds.at(k) == i.v_idx) {
				tweight.vertIds.push_back(idx);
				tweight.weights.push_back(tweight.weights.at(k));
				break;
			}
		}
	}

	return idx;
}

static bool exportFaceGroupToShape(t_mesh& shape, const std::vector<Vector3> &in_positions,
	const std::vector<Vector3> &in_normals, const std::vector<Vector2> &in_texcoords, std::vector<t_weight> &in_weights, const std::vector<Triangle_vertex>& faceGroup)
{
	if (faceGroup.empty())
	{
		return false;
	}
	std::map<vertex_index, size_t> vertexCache;
	std::vector<Vector3>& positions = shape.verts;
	std::vector<Vector3>& normals = shape.normals;
	std::vector<Vector2>& texcoords = shape.uvs;
	std::vector<unsigned short>& indices = shape.indexs;
	std::vector<t_weight>& mweights = shape.weights;

	// 展平顶点与索引
	for (size_t i = 0; i < faceGroup.size(); i++)
	{
		const Triangle_vertex face = faceGroup[i];

		vertex_index i0 = face.i0;
		vertex_index i1 = face.i1;
		vertex_index i2 = face.i2;


		unsigned short v0 = updateVertex(vertexCache, positions, normals, texcoords, mweights, in_positions, in_normals, in_texcoords, in_weights, i0);
		unsigned short v1 = updateVertex(vertexCache, positions, normals, texcoords, mweights, in_positions, in_normals, in_texcoords, in_weights, i1);
		unsigned short v2 = updateVertex(vertexCache, positions, normals, texcoords, mweights, in_positions, in_normals, in_texcoords, in_weights, i2);

		indices.push_back(v0);
		indices.push_back(v1);
		indices.push_back(v2);
	}
	mweights.swap(in_weights);
	return true;

}

//-----------------------------------------------------------------------------------------
//void AceParse::getRelativePath(std::string& s, GameObject* currentObj, GameObject* relativeParent) {
//	//std::string s;
//	if (currentObj->GetParent() && currentObj != relativeParent) {
//		getRelativePath(s, currentObj->GetParent()->gameObject, relativeParent);
//		if (s.empty())
//			s = currentObj->name;
//		else
//			s = s + "/" + currentObj->name;
//		//return s;
//	}
//	printf("");
//	//return s;
//}
//std::string AceParse::getRelativePath(GameObject* currentObj, GameObject* relativeParent) {
//	std::string s;
//	if (currentObj == nullptr) return s;
//	s = "";
//	getRelativePath(s, currentObj, relativeParent);
//	/*if (currentObj->parent && currentObj != relativeParent) {
//		 s = currentObj->parent->getRelativePath(currentObj->parent, relativeParent);
//		 if (s.empty())
//			 s = currentObj->name;
//		 else
//			s = s +"/" + currentObj->name;
//		return s;
//	}*/
//	return s;
//}

void AceParse::getRelativePath(std::string& s, t_Node& _node) {

	if (!_node.parentname.empty() && _node.parentname != "NULL" && _node.parentname != fileName) {
		s = _node.parentname + "/" + s;

		for (t_Node& k : nodes) {
			if (k.name == _node.parentname)
				getRelativePath(s, k);
		}
	}
}
std::string AceParse::getRelativePath(t_Node& _node) {
	std::string s;
	s = _node.name;
	getRelativePath(s, _node);
	return s;
}


std::string AceParse::getData(const std::string& filename, bool forString)
{
	if (filename.empty())
	{
		return std::string("");
	}
	std::string ret("");
	//FileData ret;
	unsigned char* buffer = nullptr;
	size_t size = 0;
	size_t readsize;
	const char* mode = nullptr;
	if (forString)
		mode = "rt";
	else
		mode = "rb";

	do
	{
		// Read the file from hardware
		//std::string fullPath = FileUtil::getInstance()->fullPathForFilename(filename);
		std::string fullPath = filename;
		FILE *fp = fopen(fullPath.c_str(), mode);
		if (!fp) break;
		fseek(fp, 0, SEEK_END);
		size = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		if (forString)
		{
			buffer = (unsigned char*)malloc(sizeof(unsigned char)* (size + 1));
			buffer[size] = '\0';
		}
		else
		{
			buffer = (unsigned char*)malloc(sizeof(unsigned char)* size);
		}

		readsize = fread(buffer, sizeof(unsigned char), size, fp);
		fclose(fp);

		if (forString && readsize < size)
		{
			buffer[readsize] = '\0';
		}
	} while (0);

	if (nullptr == buffer || 0 == readsize)
	{
		std::string msg = "Get data from file(";
		msg.append(filename).append(") failed!");
	}
	else
	{
		if ((buffer == nullptr || readsize == 0))
			return std::string("");

		ret = std::string((const char*)buffer);

		//ret.fastSet(buffer, readsize);
	}


	return ret;
}

bool AceParse::readLine(std::string& str, std::string& line) {
	int end = str.find("\n", 0);
	if (end == -1)return false;
	line = str.substr(0, end);
	str.erase(0, end + 1);
	return true;
}


bool AceParse::split(const std::string& s, char delimiter, std::vector<std::string>& _tokens)
{
	bool has = false;
	//std::vector<std::string> tokens;
	_tokens.clear();
	std::string token;
	std::istringstream tokenStream(s);
	while (std::getline(tokenStream, token, delimiter))
	{
		has = true;
		_tokens.push_back(token);
	}
	return has;
}

bool AceParse::removeBrackets(std::string& lineContent) {
	size_t tStart = lineContent.find('[');
	size_t tEnd = lineContent.rfind(']');
	if (tStart >= 0 && tEnd > 0 && tStart < tEnd) {
		lineContent = lineContent.substr(tStart + 1, tEnd - tStart - 1);
		return true;
	}

	tStart = lineContent.find('#(');
	tEnd = lineContent.rfind(')');
	if (tStart >= 0 && tEnd > 0 && tStart < tEnd) {
		lineContent = lineContent.substr(tStart + 1, tEnd - tStart - 1);
		return true;
	}
	return false;

}

void AceParse::handleVertex(std::string& content, int count) {
	bool hasLine = false;
	std::string str;
	for (int i = 0; i < count; i++) {
		hasLine = readLine(content, str);
		if (hasLine) {
			removeBrackets(str);
			bool ss = split(str, ',', tokens);
			float x = atof(tokens.at(0).c_str());
			float y = atof(tokens.at(1).c_str());
			float z = atof(tokens.at(2).c_str());
			Vector3 v(x, y, z);
			verts.push_back(v);
		}
	}
	printf("v");
}

void AceParse::handleUv(std::string& content, int count) {
	bool hasLine = false;
	std::string str;
	for (int i = 0; i < count; i++) {
		hasLine = readLine(content, str);
		if (hasLine) {
			removeBrackets(str);
			bool ss = split(str, ',', tokens);
			float x = atof(tokens.at(0).c_str());
			float y = atof(tokens.at(1).c_str());
			Vector2 uv(x, y);
			uvs.push_back(uv);
		}
	}
	printf("uvs");
}

void AceParse::handleFuv(std::string& content, int count) {
	bool hasLine = false;
	std::string str;
	for (int i = 0; i < count; i++) {
		hasLine = readLine(content, str);
		if (hasLine) {
			removeBrackets(str);
			bool ss = split(str, ',', tokens);
			float x = atof(tokens.at(0).c_str()) - 1;
			float y = atof(tokens.at(1).c_str()) - 1;
			float z = atof(tokens.at(2).c_str()) - 1;
			Vector3 uv(x, y, z);
			fuvs.push_back(uv);

		}
	}
}

void AceParse::handleNormal(std::string& content, int count) {
	bool hasLine = false;
	std::string str;
	for (int i = 0; i < count; i++) {
		hasLine = readLine(content, str);
		if (hasLine) {
			removeBrackets(str);
			bool ss = split(str, ',', tokens);
			float x = atof(tokens.at(0).c_str());
			float y = atof(tokens.at(1).c_str());
			float z = atof(tokens.at(2).c_str());
			Vector3 vn(x, y, z);
			normals.push_back(vn);
		}
	}
	printf("vn");
}

void AceParse::handleFace(std::string& content, int count) {
	bool hasLine = false;
	std::string str;
	for (int i = 0; i < count; i++) {
		hasLine = readLine(content, str);
		if (hasLine) {
			removeBrackets(str);
			bool ss = split(str, ',', tokens);
			unsigned short x = atoi(tokens.at(0).c_str());
			unsigned short y = atoi(tokens.at(1).c_str());
			unsigned short z = atoi(tokens.at(2).c_str());
			Vector3 index(x - 1, y - 1, z - 1);
			indexs.push_back(index);
		}
	}
	printf("indexs");
}

void AceParse::handleSkin(std::string& content, int count) {
	bool hasLine = false;
	std::string str;
	int endp = -1;
	std::string valuestr;
	int value = 0;
	for (int i = 0; i < count; i++) {
		hasLine = readLine(content, str);
		endp = str.find(' ');
		valuestr = str.substr(0, endp);

		t_weight weight;
		weight.boneName = valuestr;

		hasLine = readLine(content, str);
		if (str.find("vertIds:") != -1)
		{
			endp = str.find(":");
			valuestr = str.substr(endp + 1, str.size() - endp);
			removeBrackets(valuestr);
			split(valuestr, ',', tokens);
			for (auto k : tokens) {
				weight.vertIds.push_back(atoi(k.c_str()) - 1);
			}
		}

		hasLine = readLine(content, str);
		if (str.find("weights:") != -1) {

			endp = str.find(":");
			valuestr = str.substr(endp + 1, str.size() - endp);
			removeBrackets(valuestr);
			split(valuestr, ',', tokens);
			for (auto k : tokens) {
				weight.weights.push_back(atof(k.c_str()));
			}
		}
		weights.push_back(weight);
		//mesh.weights.push_back(weight);
	}
}

void AceParse::parseNode(std::string& content, int count) {
	std::string str;
	bool hasLine = false;
	int endp = -1;
	std::string valuestr;
	int value = 0;
	for (int i = 0; i < count; i++) {

		if (hasLine == false)
			hasLine = readLine(content, str);

		t_Node child;
		child.type = t_NodeType::MESH;

		while (hasLine) {
			if (str.find("fuv:") != -1)
			{
				endp = str.find(":");
				valuestr = str.substr(endp + 1, str.size() - endp);
				value = std::stoi(valuestr);

				handleFuv(content, value);
			}
			else if (str.find("uv:") != -1) {

				endp = str.find(":");
				valuestr = str.substr(endp + 1, str.size() - endp);
				value = std::stoi(valuestr);

				handleUv(content, value);
			}
			else if (str.find("v:") != -1) {

				endp = str.find(":");
				valuestr = str.substr(endp + 1, str.size() - endp);
				value = std::stoi(valuestr);

				handleVertex(content, value);
			}
			else if (str.find("vn:") != -1) {

				endp = str.find(":");
				valuestr = str.substr(endp + 1, str.size() - endp);
				value = std::stoi(valuestr);

				handleNormal(content, value);
			}
			else if (str.find("f:") != -1) {
				endp = str.find(":");
				valuestr = str.substr(endp + 1, str.size() - endp);
				value = std::stoi(valuestr);

				handleFace(content, value);
			}
			else if (str.find("Skin:") != -1) {
				endp = str.find(":");
				valuestr = str.substr(endp + 1, str.size() - endp);
				value = std::stoi(valuestr);

				t_Node& t_child = nodes.at(nodes.size() - 1);
				t_child.type = t_NodeType::SKINMESH;
				//child.type = t_NodeType::SKINMESH;
				handleSkin(content, value);
			}
			else if (str.find("endNode:") != -1) {
				hasLine = false;
				continue;
			}
			else if (str.find("Node:") != -1) {
				endp = str.find(":");
				valuestr = str.substr(endp + 1, str.size() - endp);
				child.name = valuestr; child.parentname = node_root.name;


				readLine(content, str);
				removeBrackets(str);
				split(str, ',', tokens);
				if (tokens.size() == 16) {
					for (int i = 0; i < 16; i++) {
						child.m[i] = atof(tokens.at(i).c_str());
					}
				}
				nodes.push_back(child);
			}

			hasLine = readLine(content, str);
		}

		std::vector<Triangle_vertex> triangles;

		for (int i = 0; i < indexs.size(); i++) {
			Triangle_vertex tri;
			tri.i0.v_idx = indexs.at(i).x;
			tri.i0.vn_idx = tri.i0.v_idx;
			tri.i0.vt_idx = fuvs.at(i).x;

			tri.i1.v_idx = indexs.at(i).y;
			tri.i1.vn_idx = tri.i1.v_idx;
			tri.i1.vt_idx = fuvs.at(i).y;

			tri.i2.v_idx = indexs.at(i).z;
			tri.i2.vn_idx = tri.i2.v_idx;
			tri.i2.vt_idx = fuvs.at(i).z;
			triangles.push_back(tri);
		}
		t_Node& t_child = nodes.at(nodes.size() - 1);
		exportFaceGroupToShape(t_child.mesh, verts, normals, uvs, weights, triangles);


		verts.clear();
		uvs.clear();
		normals.clear();
		indexs.clear();
		fuvs.clear();
		weights.clear();
	}
}

void AceParse::handleChildStruct(std::string& content, t_Node* node) {
	//std::string str;
	int endp = -1;
	std::string valuestr;

	endp = content.find('[');
	if (endp == -1) {
		valuestr = content.substr(0, content.size());
		t_Node temChild(valuestr, node->name);
		temChild.type = t_NodeType::BONE;
		nodes.push_back(temChild);

		return;
	}
	else {
		valuestr = content.substr(0, endp);
	}

	t_Node temChild(valuestr, node->name);
	temChild.type = t_NodeType::BONE;

	nodes.push_back(temChild);


	removeBrackets(content);
	split(content, ',', tokens);//注意出错
	for (auto strr : tokens) {
		handleChildStruct(strr, &temChild);
	}
}

void AceParse::prossesBoneStruct(std::string& content, int count) {
	std::string str;
	bool hasLine = false;
	int endp = -1;
	std::string valuestr;
	int value = 0;
	for (int i = 0; i < count; i++) {

		hasLine = readLine(content, str);

		handleChildStruct(str, &node_root);
	}
}

void AceParse::prossesKey(std::string& content, int frameindex) {
	std::string str;
	bool hasLine = readLine(content, str);
	int index = -1;
	int index2 = -1;
	while (hasLine) {
		if (str.find("endkey:") != -1) {
			hasLine = false;
			continue;
		}
		else {
			if ((index = str.find('#')) != -1) {
				std::string boneName = str.substr(0, index - 1);

				index = str.find("(") + 1;
				index2 = str.find(")");

				std::string matrix = str.substr(index, index2 - index);
				bool arr = split(matrix, ',', tokens);
				if (tokens.size() == 16) {
					Matrix4 m;
					for (int i = 0; i < 16; i++) {
						m[i] = atof(tokens.at(i).c_str());
					}

					/*m[0] = atof(tokens.at(0).c_str());
					m[1] = atof(tokens.at(4).c_str());
					m[2] = atof(tokens.at(8).c_str());
					m[3] = atof(tokens.at(3).c_str());
					m[4] = atof(tokens.at(1).c_str());
					m[5] = atof(tokens.at(5).c_str());
					m[6] = atof(tokens.at(9).c_str());
					m[7] = atof(tokens.at(7).c_str());
					m[8] = atof(tokens.at(2).c_str());
					m[9] = atof(tokens.at(6).c_str());
					m[10] = atof(tokens.at(10).c_str());
					m[11] = atof(tokens.at(11).c_str());
					m[12] = atof(tokens.at(12).c_str());
					m[13] = atof(tokens.at(13).c_str());
					m[14] = atof(tokens.at(14).c_str());
					m[15] = atof(tokens.at(15).c_str());*/


					t_Node* _node = findFromNodes(boneName);
					if (_node) {
						boneName = getRelativePath(*_node);
						//boneName = getRelativePath(_node->obj);
					}
					if (frameindex == 0) {
						_node->m = m;
					}

					clip->addPoint(boneName, frameindex, m);
					printf("www");

				}
			}
		}

		hasLine = readLine(content, str);
	}
}

void AceParse::prossesAnimation(std::string& content, int count) {
	std::string str;
	std::string valuestr;
	bool hasLine = readLine(content, str);
	int endp = -1;
	int value = 0;
	while (hasLine) {
		if (str.find("TimeRange:") != -1) {

			endp = str.find(":");
			valuestr = str.substr(endp + 1, str.size() - endp);
			bool arr = split(valuestr, ' ', tokens);

			TimeRangeStart = atoi(tokens.at(0).c_str());
			TimeRangeEnd = atoi(tokens.at(1).c_str());
			printf("");
		}
		else if (str.find("NumKeyBons:") != -1) {
			endp = str.find(":");
			valuestr = str.substr(endp + 1, str.size() - endp);
			NumKeyBons = std::stoi(valuestr);
			printf("");
		}
		else if (str.find("Key:") != -1) {
			endp = str.find(":");
			valuestr = str.substr(endp + 1, str.size() - endp);
			value = std::stoi(valuestr);

			prossesKey(content, value);
		}
		else if (str.find("frameEnd:") != -1) {
			hasLine = false;
			continue;
		}

		hasLine = readLine(content, str);
	}
}



AceParse::AceParse()
{

	NumKeyBons = 0;
	frameRate = 0;
	TimeRangeStart = 0;
	TimeRangeEnd = 0;
	//AnimationClip* clip;

	node_root = t_Node("root", "NULL");
}

AceParse::~AceParse()
{
}

GameObject* AceParse::createGameObject() {
	GameObject* root = new GameObject(fileName.c_str());
	InstanceNode(nodes, root);
	return  root;
}

void AceParse::InstanceNode(std::vector<t_Node>& _nodes, GameObject* parent) {

	for (auto& k : _nodes) {
		if (k.parentname == parent->name) {
			GameObject* child = new GameObject(k.name.c_str());
			parent->AddChild(child);

			//t_Node* _node = findFromNodes(k.name);
			//if (_node) {
			//	_node->obj = child;
			//}
			//k.obj = child;
			child->transform->SetLocalMatrix(k.m);
			if (k.type == t_NodeType::MESH) {
				//child->AddComponent<MeshRender>();

				/*Mesh* mesh = MeshCache::create();
				mesh->setVerts(k.mesh.verts);
				mesh->setNormals(k.mesh.normals);
				mesh->setUVs(0, k.mesh.uvs);
				mesh->setIndexs(k.mesh.indexs);
				if (mesh == nullptr) return;
				Material* mat = new Material();
				Image* img = new Image();
				bool initImg = img->CreateImage("Resources/img/dog.jpg");
				Texture* tex = new Texture();
				tex->initWithImage(img);
				mat->setMainTex(tex);
				MeshRender* ren = child->AddComponent<MeshRender>();
				ren->setMesh(mesh);
				ren->setMaterial(mat);*/

			}
			else if (k.type == t_NodeType::SKINMESH) {
				//child->AddComponent<SkinMeshRender>();
			}

			InstanceNode(_nodes, child);
		}
	}
}

void AceParse::parse(const char* path) {
	clip = new AnimationClip("test");
	int endp = -1;

	fileName = path;
	int startP = fileName.rfind("/");
	startP = startP < 0 ? 0 : startP + 1;
	endp = fileName.rfind(".ace");
	fileName = fileName.substr(startP, endp - startP);

	node_root.name = fileName;


	nodes.push_back(node_root);

	std::string fileContent = getData(path);
	std::string str;
	bool hasLine = readLine(fileContent, str);
	endp = -1;
	std::string valuestr;
	int value = 0;
	while (hasLine) {
		if (str.find("geomCount:") != -1) {
			endp = str.find(":");
			valuestr = str.substr(endp + 1, str.size() - endp);
			value = std::stoi(valuestr);
			parseNode(fileContent, value);
		}
		else if (str.find("Bones:") != -1) {
			endp = str.find(":");
			valuestr = str.substr(endp + 1, str.size() - endp);
			value = std::stoi(valuestr);
			prossesBoneStruct(fileContent, value);

		}
		else if (str.find("FrameRate:") != -1) {
			endp = str.find(":");
			valuestr = str.substr(endp + 1, str.size() - endp);
			frameRate = std::stoi(valuestr);

			prossesAnimation(fileContent, frameRate);

		}

		hasLine = readLine(fileContent, str);
	}




	//AnimationClip* ani = clip;


	//std::string s = getRelativePath(testObj);

	printf("");

	//nodes.clear();
}