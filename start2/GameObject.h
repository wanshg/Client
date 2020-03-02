#pragma once
#include <string>
class GameObject {

public:
	GameObject(const char* _name) { name = _name; };
	~GameObject() {
		for (auto child : children) {
			delete child;
			child = nullptr;
		}
		//children.clear();
	};

	GameObject* clone() const { 
		GameObject* obj =new GameObject(*this);
		obj->children.clear();
		for (auto child : children) {
			obj->addChild(child->clone());
		}


		//GameObject obj(name.c_str());
		//memcpy(&obj, this, sizeof(this));
		return obj;
	}

	void addChild(GameObject* obj) {
		children.push_back(obj);
	}
	std::vector<GameObject*> children;
	std::string name;
	GameObject* parent;
};