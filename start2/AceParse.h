#pragma once
//#include <vector>
//#include "Vectors.h"

enum t_NodeType {
	NONE,
	MESH,
	SKINMESH,
	BONE,
	ROOT,
};

struct t_weight {
	std::string boneName;
	std::vector<unsigned short> vertIds;
	std::vector<float> weights;
};

struct t_mesh {
	std::vector<Vector3> verts;
	std::vector<Vector2> uvs;
	std::vector<Vector3> normals;
	std::vector<unsigned short> indexs;
	std::vector <t_weight> weights;
};

struct t_Node {
	t_Node() { parentname = "NULL"; }
	t_Node(const char* _name, const char* _parentName) { name = _name; parentname = _parentName; }
	t_Node(std::string& _name, std::string& _parentName) { name = _name; parentname = _parentName; }
	std::string name;
	std::string parentname;
	Matrix4 m;

	t_NodeType type;
	t_mesh mesh;
	//GameObject* obj;
};


class AceParse
{

public:
	AceParse();
	~AceParse();

	void parse(const char* path);

	/*static std::vector<Vector3> verts;
	static std::vector<Vector2> uvs;
	static std::vector<Vector3> normals;
	static std::vector<Vector3> indexs;*/

	//std::vector<t_mesh> meshs;
	//GameObject* root = nullptr;
	//GameObject* testObj;
	t_Node node_root;
	std::vector<t_Node> nodes;

	GameObject* createGameObject();
	std::string fileName;

	int NumKeyBons;
	int frameRate;
	int TimeRangeStart;
	int TimeRangeEnd;
	AnimationClip* clip;

private:
	t_Node * findFromNodes(std::string& _name);
	void InstanceNode(std::vector<t_Node>& _nodes, GameObject* parent);

	/*static size_t updateVertex(std::map<vertex_index, size_t>& vertexCache, std::vector<Vector3>& positions, std::vector<Vector3>& normals,
		std::vector<Vector2>& texcoords, const std::vector<Vector3>& in_positions, const std::vector<Vector3>& in_normals, const std::vector<Vector2>& in_texcoords, const vertex_index& i);
	static bool exportFaceGroupToShape(const std::vector<Vector3> &in_positions,
		const std::vector<Vector3> &in_normals, const std::vector<Vector2> &in_texcoords, const std::vector<Triangle_vertex>& faceGroup);
*/
//-----------------------------------------------------------------------------------------
	void getRelativePath(std::string& s, GameObject* currentObj, GameObject* relativeParent = nullptr);

	std::string getRelativePath(GameObject* currentObj, GameObject* relativeParent = nullptr);

	void getRelativePath(std::string& s, t_Node& _node);
	std::string getRelativePath(t_Node& _node);


	std::string getData(const std::string& filename, bool forString = true);

	bool readLine(std::string& str, std::string& line);

	bool split(const std::string& s, char delimiter, std::vector<std::string>& _tokens);

	bool removeBrackets(std::string& lineContent);
	void handleVertex(std::string& content, int count);

	void handleUv(std::string& content, int count);

	void handleFuv(std::string& content, int count);

	void handleNormal(std::string& content, int count);

	void handleFace(std::string& content, int count);

	void handleSkin(std::string& content, int count);

	void parseNode(std::string& content, int count);

	void handleChildStruct(std::string& content, t_Node* node);

	void prossesBoneStruct(std::string& content, int count);

	void prossesKey(std::string& content, int frameindex);

	void prossesAnimation(std::string& content, int count);
};
