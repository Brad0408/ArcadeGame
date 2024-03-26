#pragma once
#include <isComponent.h>
#include <Transform.h>


class GameObject : public Object
{

public:
	//Construtor
	GameObject();

	//Copy Constructor
	GameObject(const GameObject&) = default;

	//Deconstructor
	virtual ~GameObject() {}

	//Define the GameObjects transform
	Transform* transform;

	

	//Set the Name of the GameObject
	inline void SetName(std::string newName) { m_Name = newName; }

	//Get the Name of the GameObject
	inline std::string GetName() { return m_Name; }

	//Set if the GameObject is active
	void SetActive(bool newActive);

	//Get the curret active state of the GameObject
	inline bool GetActive() { return m_Active; }

	//Set the tag of the GameObject
	inline void SetTag(std::string newTag) { m_Tag = newTag; }

	//Check if a passed in tag value is the same as the set tag value of the GameObject
	inline bool CompareTag(std::string comp) { return m_Tag == comp; }

	//Template functions that only 'component' can be used with them
	//Gets the component
	template<class T> requires isComponent<T> T* GetComponent();

	//Adds a component to the GameObject
	template<class T> requires isComponent<T> T* AddComponent();

	//Removes a component from the GameObject
	template<class T> requires isComponent<T> bool RemoveComponent(T* comp);

private:
	//Basic attributes for the GameObject
	std::string m_Name;
	bool m_Active;
	std::string m_Tag;

	//1 pointer, if gameObject is gone, so does the component, all memory is wiped
	std::vector<std::unique_ptr<Component>> m_Components;

public:
	//Compares two GameObject instances, compares the uuids returns true if two GameObjects are equal
	inline bool operator==(const GameObject& other) { return this->uuid == other.uuid; }

	//Compares two GameObject instances, return true if they dont match
	inline bool operator!=(const GameObject& other) { return !(*this == other); }



};


