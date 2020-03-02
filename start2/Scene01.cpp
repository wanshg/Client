#include "Scene01.h"



Scene01::Scene01()
{
}


Scene01::~Scene01()
{
}

static Mesh* bone = nullptr;

Mesh* Bone()
{
	if (bone == nullptr) {
		bone = new Mesh("bone");

		std::vector<Vector3> _vert;
		std::vector<Vector3> _normal;
		std::vector<Vector2> _uv;
		std::vector<unsigned short> _finalIndex;

		float back_size = 0.2f;
		float front_size = 0.2f;

		/*_vert.push_back(Vector3(-back_size, -back_size, 0));
		_vert.push_back(Vector3(back_size, -back_size, 0));
		_vert.push_back(Vector3(-back_size, back_size, 0));
		_vert.push_back(Vector3(back_size, back_size, 0));
		_vert.push_back(Vector3(-front_size, -front_size, 1));
		_vert.push_back(Vector3(front_size, -front_size, 1));
		_vert.push_back(Vector3(-front_size, front_size, 1));
		_vert.push_back(Vector3(front_size, front_size, 1));*/

		_vert.push_back(Vector3(-back_size, 0, -back_size));
		_vert.push_back(Vector3(back_size, 0, -back_size));
		_vert.push_back(Vector3(-back_size, 0, back_size));
		_vert.push_back(Vector3(back_size, 0, back_size));

		_vert.push_back(Vector3(-front_size, 1, -front_size));
		_vert.push_back(Vector3(front_size, 1, -front_size));
		_vert.push_back(Vector3(-front_size, 1, front_size));
		_vert.push_back(Vector3(front_size, 1, front_size));



		for (int i = 0; i < 8; i++) {
			_normal.push_back(Vector3(0, 0, 0));
		}

		for (int i = 0; i < 8; i++) {
			_uv.push_back(Vector2(0, 0));
		}

		//back
		_finalIndex.push_back(0);
		_finalIndex.push_back(1);
		_finalIndex.push_back(2);

		_finalIndex.push_back(2);
		_finalIndex.push_back(1);
		_finalIndex.push_back(3);

		//front
		_finalIndex.push_back(4);
		_finalIndex.push_back(5);
		_finalIndex.push_back(6);

		_finalIndex.push_back(6);
		_finalIndex.push_back(5);
		_finalIndex.push_back(7);

		//0 2 4 6     1 3 5 7

		//left
		_finalIndex.push_back(0);
		_finalIndex.push_back(2);
		_finalIndex.push_back(4);

		_finalIndex.push_back(4);
		_finalIndex.push_back(2);
		_finalIndex.push_back(6);

		//right
		_finalIndex.push_back(1);
		_finalIndex.push_back(3);
		_finalIndex.push_back(5);

		_finalIndex.push_back(5);
		_finalIndex.push_back(3);
		_finalIndex.push_back(7);

		//0 1 4 5   2 3 6 7
		//up
		_finalIndex.push_back(0);
		_finalIndex.push_back(1);
		_finalIndex.push_back(4);

		_finalIndex.push_back(4);
		_finalIndex.push_back(1);
		_finalIndex.push_back(5);

		//down
		_finalIndex.push_back(2);
		_finalIndex.push_back(3);
		_finalIndex.push_back(6);

		_finalIndex.push_back(6);
		_finalIndex.push_back(3);
		_finalIndex.push_back(7);


		bone->setVerts(_vert);
		bone->setNormals(_normal);
		bone->setUVs(0, _uv);

		bone->setIndexs(_finalIndex);
		bone->initBuffer();
		bone->updateVertices();
	}

	return bone;
}
void InstanceNode(std::vector<t_Node>& _nodes, GameObject* parent, GameObject* _root) {

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

				Mesh* mesh = MeshCache::create();
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
				ren->setMaterial(mat);

			}
			else if (k.type == t_NodeType::BONE) {


				if (bone == nullptr) return;
				Material* mat = new Material();

				Image* img = new Image();
				bool initImg = img->CreateImage("Resources/img/dog.jpg");
				Texture* tex = new Texture();
				tex->initWithImage(img);
				mat->setMainTex(tex);

				MeshRender* ren = child->AddComponent<MeshRender>();
				ren->setMesh(bone);
				ren->setMaterial(mat);
			}
			else if (k.type == t_NodeType::SKINMESH) {
				Mesh* mesh = MeshCache::create();
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

				SkinMeshRender* ren = child->AddComponent<SkinMeshRender>();
				ren->setMesh(mesh);
				ren->setMaterial(mat);
				ren.setRoot(_root);
			}

			InstanceNode(_nodes, child,_root);
		}
	}
}
GameObject* createGameObject(AnimationClip* clip, std::vector<t_Node>& _nodes, std::string& filename) {
	GameObject* root = new GameObject(filename.c_str());

	if (clip) {
		Animation* ani = root->AddComponent<Animation>();
		ani->addClip(clip);
	}

	InstanceNode(_nodes, root, root);
	return  root;
}



void Scene01::Init()
{
	//Shader* ss = new Shader();
	//delete ss;

	GameObject* camera = new GameObject();
	Camera* cam = camera->AddComponent<Camera>();
	camera->AddComponent<Test>();

	addChild(camera->transform);
	setMainCamera(cam);
	camera->transform->SetPostion(0, 0, -50);
	//camera->transform->SetEuler(30, 0, 0);
	//useKsy(_skyBox,camera->transform->GetWroldMatrix());

	//Shader* a = new Shader("ww","bww");

	//camera->transform->SetEuler(90, 0, 0);
	//Matrix4 m = camera->transform->GetWroldMatrix();
	//Vector3 v = camera->transform->GetWorldPostion();
	//Vector3 v2 = camera->transform->GetLocalPostion();

	/*std::string imagepath = "Resources/img/cube.jpg";
	Image* img = new Image();
	img->CreateImage(imagepath);
	Texture* tex = new Texture();
	tex->initWithImage(img);*/

	Bone();

	AceParse* ace = new AceParse();
	ace->parse("Resources/models/oneBoneAni.ace");
	ace->nodes;

	GameObject* obj = createGameObject(ace->clip, ace->nodes, ace->fileName);
	addChild(obj->transform);
	printf("");


	//GameObject* aceobj = Resources::LoadObj("Resources/box.obj", "Resources/img/floor.jpg");
	//aceobj->transform->SetPostion(0, 0, -5);
	//objobj->AddComponent<Test>();
	////obj1->transform->SetEuler(0, 60, 0);
	//addChild(aceobj->transform);

	//GameObject* objobj = Resources::LoadObj("Resources/box.obj", "Resources/img/floor.jpg");
	//objobj->transform->SetPostion(0, 0, 5);
	////objobj->AddComponent<Test>();
	//////obj1->transform->SetEuler(0, 60, 0);
	//addChild(objobj->transform);

	//Vector3 vp = objobj->transform->GetWorldPostion();
	//Camera::Main->SphereInFrustum(vp.x, vp.y, vp.z,1);


	GameObject* p = Primitives::createCube();
	//p->transform->SetPostion(0, 0, 5);
	//p->transform->SetScale(10, 10, 10);
	//p->transform->SetEuler(0, 0, 30);
	//p->AddComponent<Test>();
	addChild(p->transform);





	//GameObject* p = Primitives::createFrustumPlane();
	//addChild(p->transform);

	//Matrix4 fm2 = cam->getCameraFrustumMatrix(50, 1080, 720, Camera::FitType::ALL);
	////Matrix4 fm = p->transform->GetWroldMatrix() * fm2;
	//p->transform->SetWorldMatrix(fm2);


}

void Scene01::SceneUpdate()
{
	//printf("sss");
}
