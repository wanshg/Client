#include "SkinMeshRender.h"

SkinMeshRender::SkinMeshRender() :
	inited(false)
	,mesh(nullptr)
	, mat(nullptr) {
	render = new PrimitiveRender();
}

SkinMeshRender::~SkinMeshRender() {
	if (render)
		delete(render);
	if (mesh)
		delete mesh;
}

void SkinMeshRender::Awake()
{
	if (!inited)
		Init();
}

void SkinMeshRender::Start()
{

}

void SkinMeshRender::Update()
{
	Calum();
	render->init(mat, mesh, transform->GetWroldMatrix());
	RenderCenter::getRender()->AddRender(render);
}

void SkinMeshRender::setMesh(Mesh* _mesh)
{
	sharedmesh = _mesh;
	mesh = new Mesh(*_mesh);
}
void SkinMeshRender::setMaterial(Material* _mat)
{
	mat = _mat;
}
void SkinMeshRender::setRoot(GameObject* _root) {
	root = _root;
}


GameObject* findObj(std::string& objName, GameObject* startObj) {
	
	for (auto& child : startObj->children) {
		if (child->name == objName)
		{
			return child;
		}
		else {
			GameObject* obj = findObj(objName, child);
			if (obj) return obj;
		}
	}
	return nullptr;
}

void SkinMeshRender::Init() {
	if (!root) return;

	for (auto w : mesh->weights) {

		GameObject* boneObj = findObj(w.boneName, root);
		if (boneObj) {
			if (boneObj) {
				t_bone tbone;
				tbone.bone = boneObj;
				Bones.push_back(tbone);
			}
		}
	}


	Matrix4 rootInv = root->transform->WorldToLocalMatrix();
	for (auto& bone : Bones) {
		if (!bone.bone) continue;
		//bone.BoneRelativeRootMatrix_Init = rootInv * bone.bone->transform->GetWroldMatrix();
		bone.BoneRelativeRootMatrixInv_Init = (rootInv * bone.bone->transform->GetWroldMatrix()).inverse();
		bone.ObjRelativeBoneMatrix_Init = bone.bone->transform->WorldToLocalMatrix() * transform->GetWroldMatrix();
	}

	inited = true;
}

void SkinMeshRender::Calum() {
	if (!root) return;
	Matrix4 rootInv = root->transform->WorldToLocalMatrix();
	for (auto& bone : Bones) {
		if (!bone.bone) continue;

		Matrix4 spaceMatrix = rootInv * bone.bone->transform->GetWroldMatrix();
		Matrix4 deltaMatrix = bone.BoneRelativeRootMatrixInv_Init * spaceMatrix;

		for (auto& w : mesh->weights) {
			if (bone.bone->name == w.boneName) {
				for (int j = 0; j < w.vertIds.size(); j++) {
					//转换为物体坐标
					Matrix4 deltaMatrix = bone.ObjRelativeBoneMatrix_Init.inverse() * (bone.ObjRelativeBoneMatrix_Init * deltaMatrix);


					Vector3 v = sharedmesh->getVertex(w.vertIds.at(j));
					//Vector3 vertRelativeBonePos = bone.ObjRelativeBoneMatrix_Init * deltaMatrix * v;

					//Matrix4 objToRootMatrix = rootInv * transorm->GetWorldMatrix();

					Vector3 v2 = deltaMatrix * v;


					float weight = w.weights.at(j);
					v = v2 * weight + v * (1 - weight);
				}
				
				break;
			}
		}

		//mesh->setVerts()
	}
}


void MeshFilter::Evaluate(std::unordered_map<int, Matrix4> skeletonDelateMats)
{
	if (ctrlPointSkeletonList.size() <= 0) return;

	size_t lVertexCount = ctrlPointSkeletonList.size();
	Matrix4* lClusterDeformation = new Matrix4[lVertexCount];
	memset(lClusterDeformation, 0, lVertexCount * sizeof(Matrix4));
	//为每个顶点分配簇权重
	double* lClusterWeight = new double[lVertexCount];
	memset(lClusterWeight, 0, lVertexCount * sizeof(double));

	for (auto it = ctrlPointSkeletonList.begin(); it != ctrlPointSkeletonList.end(); it++)
	{
		//orignVerts[it->first]
		//int lIndex = lCluster->GetControlPointIndices()[k];

		//if (lIndex >= lVertexCount)
			//continue;

		VertexSkeletonList sleletons = it->second;
		int lIndex = it->first;

		for (auto itWight = sleletons.skeletonWeights.begin(); itWight != sleletons.skeletonWeights.end(); itWight++)
		{
			float lWeight = itWight->second;

			if (lWeight == 0.0)
				continue;

			Matrix4 lVertexTransformMatrix = skeletonDelateMats[itWight->first];

			Matrix4 lInfluence = lVertexTransformMatrix;
			MatrixScale(lInfluence, lWeight);

			//if (lClusterMode == FbxCluster::eAdditive)
			//{
				//MatrixAddToDiagonal(lInfluence, 1.0 - lWeight);
				//lClusterDeformation[lIndex] = lInfluence * lClusterDeformation[lIndex];
				//lClusterWeight[lIndex] = 1.0;
			//}
			//else // lLinkMode == FbxCluster::eNormalize || lLinkMode == FbxCluster::eTotalOne
			//{
				
				MatrixAdd(lClusterDeformation[lIndex], lInfluence);
				lClusterWeight[lIndex] += lWeight;
			//}
		}

		//----------------------------------------------------------------
		//顶点偏移
		Vector3 lSrcVertex = orignVerts[lIndex];
		Vector3& lDstVertex = orignVerts[lIndex];
		//double lWeight = lClusterWeight[i];
		double lWeight = 1.0;
		if (lWeight != 0.0)
		{
			Vector4 v1 = Vector4(lSrcVertex.x, lSrcVertex.y, lSrcVertex.z, 1);
			Vector4 v2 = lClusterDeformation[lIndex] * v1;
			lDstVertex = Vector3(v2.x,v2.y,v2.z);
			printf("tt");
			//if (lClusterMode == FbxCluster::eNormalize)
			//{
			//lDstVertex /= lWeight;
			//}
			/*else if (lClusterMode == FbxCluster::eTotalOne)
			{
			lSrcVertex *= (1.0 - lWeight);
			lDstVertex += lSrcVertex;
			}*/
		}
	}

	delete[] lClusterDeformation;
	delete[] lClusterWeight;

	updateOrignData();
}