#ifndef _SKINMESH_RENDER_H_
#define _SKINMESH_RENDER_H_


#include "Mesh.h"
#include "GameObject.h"

class SkinMeshRender : public Component
{
public:
	struct t_bone {
		GameObject* bone;
		Matrix4 BoneRelativeRootMatrixInv_Init;
		Matrix4 ObjRelativeBoneMatrix_Init;//��ʼʱ��������ڸù�������
	};
public:
	SkinMeshRender();
	~SkinMeshRender();
	void Awake() override;
	void Start() override;
	void Update() override;

	void setMesh(Mesh* _mesh);
	Mesh* getMesh() { return mesh; };

	void setMaterial(Material* _mat);
	Material* getMaterial() { return mat; };

	void setRoot(GameObject* _root);


	void Init();
	void Calum();


private:
	GameObject* root;
	Mesh* mesh;
	Mesh* sharedmesh;
	Material* mat;
	PrimitiveRender* render;

	std::vector<t_bone> Bones;
	bool inited;
};
#endif