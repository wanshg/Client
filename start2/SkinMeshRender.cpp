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

	for (auto w : mesh->clusters) {

		GameObject* boneObj = findObj(w.boneName, root);
		if (boneObj) {
			if (boneObj) {
				t_Clusterbone tbone;
				tbone.bone = boneObj;
				Pose.push_back(tbone);
			}
		}
	}


	Matrix4 rootInv = root->transform->WorldToLocalMatrix();
	for (auto& bone : Pose) {
		if (!bone.bone) continue;
		//bone.BoneRelativeRootMatrix_Init = rootInv * bone.bone->transform->GetWroldMatrix();
		bone.BoneRelativeRootMatrixInv_Init = (rootInv * bone.bone->transform->GetWroldMatrix()).inverse();
		bone.ObjRelativeBoneMatrix_Init = bone.bone->transform->WorldToLocalMatrix() * transform->GetWroldMatrix();


		bone.lClusterGlobalInitPosition = rootInv * bone.bone->transform->GetWroldMatrix();
		//bone.lReferenceGlobalInitPosition = rootInv * bone.bone->transform->GetWroldMatrix();
		
	}

	lReferenceGlobalInitPosition = rootInv * transform->GetWroldMatrix();

	inited = true;
}

void SkinMeshRender::Calum() {
	if (!root) return;
	Matrix4 rootInv = root->transform->WorldToLocalMatrix();
	for (auto& bone : Pose) {
		if (!bone.bone) continue;

		Matrix4 spaceMatrix = rootInv * bone.bone->transform->GetWroldMatrix();
		Matrix4 deltaMatrix = bone.BoneRelativeRootMatrixInv_Init * spaceMatrix;

		for (auto& w : mesh->clusters) {
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

void SkinMeshRender::Calum2(std::unordered_map<int, Matrix4> skeletonDelateMats)
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

void SkinMeshRender::ComputeClusterDeformation(t_Cluster* lCluster, Matrix4& pVertexTransformMatrix) {
	if (!root) return;

	Matrix4 rootInv = root->transform->WorldToLocalMatrix();
	/*for (auto& bone : Bones) {
		if (!bone.bone) continue;

		Matrix4 spaceMatrix = rootInv * bone.bone->transform->GetWroldMatrix();
		Matrix4 deltaMatrix = bone.BoneRelativeRootMatrixInv_Init * spaceMatrix;

		for (auto& w : mesh->weights) {
			if (bone.bone->name == w.boneName) {
				for (int j = 0; j < w.vertIds.size(); j++) {
					pVertexTransformMatrix = bone.ObjRelativeBoneMatrix_Init.inverse() * (bone.ObjRelativeBoneMatrix_Init * deltaMatrix);
				}
				break;
			}
		}
	}*/


	//-------------------------------------------------------------------------------------------------------------------------------------------------
	for (auto& bone : Pose) {
		if (!bone.bone) continue;

		if (bone.bone->name == lCluster.name)
		{
			Matrix4 lClusterGlobalCurrentPosition = rootInv * bone.bone->transform->GetWroldMatrix();
			

			//1
			//Matrix4 lReferenceGlobalCurrentPosition = rootInv * transform->GetWroldMatrix();
			//// Compute the initial position of the link relative to the reference.
			//Matrix4 lClusterRelativeInitPosition = bone.lClusterGlobalInitPosition.inverse() * lReferenceGlobalInitPosition;
			//// Compute the current position of the link relative to the reference.
			//Matrix4 lClusterRelativeCurrentPositionInverse = lReferenceGlobalCurrentPosition.inverse() * lClusterGlobalCurrentPosition;
			//// Compute the shift of the link relative to the reference.
			//pVertexTransformMatrix = lClusterRelativeCurrentPositionInverse * lClusterRelativeInitPosition;


			//2
			Matrix4 deltaMatrix = bone.lClusterGlobalInitPosition.inverse() * lClusterGlobalCurrentPosition;
			Matrix4 lReferenceGlobalCurrentPosition = bone.lClusterGlobalInitPosition * (deltaMatrix * bone.ObjRelativeBoneMatrix_Init);
			pVertexTransformMatrix = lReferenceGlobalInitPosition.inverse() * lReferenceGlobalCurrentPosition;
			//pVertexTransformMatrix = bone.ObjRelativeBoneMatrix_Init.inverse() * (deltaMatrix * bone.ObjRelativeBoneMatrix_Init);


			break;
		}
	}

	//-------------------------------------------------------------------------------------------------------------------------------------------------
	//// pCluster->GetTransformMatrix(lReferenceGlobalInitPosition);
	//Matrix4 lReferenceGlobalInitPosition;
	//Matrix4 lReferenceGlobalCurrentPosition = pGlobalPosition;
	//Matrix4 lReferenceGeometry = GetGeometry(pMesh->GetNode());
	//lReferenceGlobalInitPosition *= lReferenceGeometry;

	//// Get the link initial global position and the link current global position.
	//Matrix4 lClusterGlobalInitPosition;
	//pCluster->GetTransformLinkMatrix(lClusterGlobalInitPosition);
	//Matrix4 lClusterGlobalCurrentPosition = GetGlobalPosition(pCluster->GetLink(), pTime, pPose);

	//// Compute the initial position of the link relative to the reference.
	//Matrix4 lClusterRelativeInitPosition = lClusterGlobalInitPosition.Inverse() * lReferenceGlobalInitPosition;

	//// Compute the current position of the link relative to the reference.
	//Matrix4 lClusterRelativeCurrentPositionInverse = lReferenceGlobalCurrentPosition.Inverse() * lClusterGlobalCurrentPosition;

	//// Compute the shift of the link relative to the reference.
	//pVertexTransformMatrix = lClusterRelativeCurrentPositionInverse * lClusterRelativeInitPosition;

}

void SkinMeshRender::ComputeLinearDeformation(){

	int lVertexCount = mesh->getVertexNumber();
	Matrix4* lClusterDeformation = new Matrix4[lVertexCount];
	memset(lClusterDeformation, 0, lVertexCount * sizeof(Matrix4));

	double* lClusterWeight = new double[lVertexCount];
	memset(lClusterWeight, 0, lVertexCount * sizeof(double));

	//for (int lClusterIndex = 0; lClusterIndex < lClusterCount; ++lClusterIndex)
	for (t_Cluster* lCluster: mesh->clusters)
	{
		Matrix4 lVertexTransformMatrix;
		ComputeClusterDeformation(lCluster, lVertexTransformMatrix);

		int lVertexIndexCount = lCluster->GetControlPointIndicesCount();
		for (int k = 0; k < lVertexIndexCount; ++k)
		{
			int lIndex = lCluster->GetControlPointIndices()[k];

			if (lIndex >= lVertexCount)
				continue;

			double lWeight = lCluster->GetControlPointWeights()[k];

			//if (lWeight == 0.0)
				//continue;

			Matrix4 lInfluence = lVertexTransformMatrix;
			MatrixScale(lInfluence, lWeight);

			//if (lClusterMode == FbxCluster::eAdditive)
			//{
			//	MatrixAddToDiagonal(lInfluence, 1.0 - lWeight);
			//	lClusterDeformation[lIndex] = lInfluence * lClusterDeformation[lIndex];
			//	lClusterWeight[lIndex] = 1.0;
			//}
			//else // lLinkMode == FbxCluster::eNormalize || lLinkMode == FbxCluster::eTotalOne
			//{
				MatrixAdd(lClusterDeformation[lIndex], lInfluence);
				lClusterWeight[lIndex] += lWeight;
			//}
		}//For each vertex			
	}//lClusterCount


	//for (int i = 0; i < lVertexCount; i++)
	//{
	//	Vector3 lSrcVertex = pVertexArray[i];
	//	Vector3& lDstVertex = pVertexArray[i];
	//	double lWeight = lClusterWeight[i];

	//	if (lWeight != 0.0)
	//	{
	//		lDstVertex = lClusterDeformation[i] * (lSrcVertex);
	//		//if (lClusterMode == FbxCluster::eNormalize)
	//		//{
	//		//	lDstVertex /= lWeight;
	//		//}
	//		//else if (lClusterMode == FbxCluster::eTotalOne)
	//		//{
	//			lSrcVertex *= (1.0 - lWeight);
	//			lDstVertex += lSrcVertex;
	//		//}
	//	}
	//}

	std::vector<float>& vertexs = mesh->_verts;
	for (int i = 0; i < lVertexCount; i++)
	{
		int vertex_index = i * 3;
		float& sDstVertex_x = vertexs[vertex_index];
		float& sDstVertex_y = vertexs[vertex_index + 1];
		float& sDstVertex_z = vertexs[vertex_index + 2];

		float sSrcVertex_x = sDstVertex_x;
		float sSrcVertex_y = sDstVertex_y;
		float sSrcVertex_z = sDstVertex_z;


		double lWeight = lClusterWeight[i];

		if (lWeight != 0.0)
		{
			//lDstVertex = lClusterDeformation[i] * (lSrcVertex);
			Matrix4& m = lClusterDeformation[i];
			sDstVertex_x = m[0] * sSrcVertex_x + m[4] * sSrcVertex_y + m[8] * sSrcVertex_z + m[12];
			sDstVertex_y = m[1] * sSrcVertex_x + m[5] * sSrcVertex_y + m[9] * sSrcVertex_z + m[13];
			sDstVertex_z = m[2] * sSrcVertex_x + m[6] * sSrcVertex_y + m[10] * sSrcVertex_z + m[14];


			//if (lClusterMode == FbxCluster::eNormalize)
			//{
			//	lDstVertex /= lWeight;
			//}
			//else if (lClusterMode == FbxCluster::eTotalOne)
			//{
			//lSrcVertex *= (1.0 - lWeight);
			//lDstVertex += lSrcVertex;


			float diffWeight = (1.0 - lWeight);
			sSrcVertex_x *= diffWeight;
			sSrcVertex_y *= diffWeight;
			sSrcVertex_z *= diffWeight;

			sDstVertex_x += sSrcVertex_x;
			sDstVertex_y += sSrcVertex_y;
			sDstVertex_z += sSrcVertex_z;

			//}
		}
	}


	delete[] lClusterDeformation;
	delete[] lClusterWeight;


	//mesh->setVerts(_vert);
	mesh->refreshAllVertex();
}