#pragma once
#include <Object.h>

class GameObject;

class Component : public Object
{
public:
	//Constructor, initalizes _GameObject with pointer
	Component(GameObject* owner) : _GameObject(owner) {}

	virtual ~Component() {}

	//Returns _GameObject pointer - provides access to the object associated with this component
	GameObject* GetGameObject() { return _GameObject; }

	virtual void Update(float deltaTime);

	virtual void FixedUpdate(float fixedDeltaTime);

protected:
	//Pointer to GameObject that owns the component
	GameObject* _GameObject;

};
