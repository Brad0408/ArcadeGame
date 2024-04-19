#pragma once
#include <Object.h>

class GameObject;

//Inherit from object
class Component : public Object
{
public:
	//Constructor, initalizes _GameObject with pointer
	Component(GameObject* owner) : _GameObject(owner) {}

	//Deconstructor
	virtual ~Component() {}

	//Returns _GameObject pointer - provides access to the object associated with this component
	GameObject* GetGameObject() { return _GameObject; }

	//Responds to changes in the active state of the owner object
	virtual void OwnerActiveChange(bool newActive) {}

	//Start - Called once when the component is initialized or activated.
	virtual void Start();

	//Update - Called every frame
	virtual void Update(float deltaTime);

	//Fixed Update - Called at a fixed interval with a fixed time step
	virtual void FixedUpdate(float fixedDeltaTime);

	//Enable - Called when the component is enabled (activated).
	virtual void OnEnable();

	//Disable - Called when the component is disabled or deactivated.
	virtual void OnDisable();

protected:
	//Pointer to GameObject that owns the component
	GameObject* _GameObject;

};
