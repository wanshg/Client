#ifndef _SKINMESH_RENDER_H_
#define _SKINMESH_RENDER_H_


#include "Mesh.h"
#include "GameObject.h"

class SkinMeshRender : public Component
{
public:
	struct t_Clusterbone {
		GameObject* bone;

		Matrix4 lClusterGlobalInitPosition;
		//Matrix4 lReferenceGlobalInitPosition;

		//Matrix4 BoneRelativeRootMatrixInv_Init;
		Matrix4 ObjRelativeBoneMatrix_Init;//初始时对象相对于该骨骼矩阵
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
	void Calum2();
	void ComputeClusterDeformation(t_Cluster* lCluster, Matrix4& pVertexTransformMatrix);
	void ComputeLinearDeformation();

private:
	GameObject* root;
	Mesh* mesh;
	Mesh* sharedmesh;
	Material* mat;
	PrimitiveRender* render;

	std::vector<t_Clusterbone> Pose;
	bool inited;

	Matrix4 lReferenceGlobalInitPosition;
};
#endif