#include "GameObject.h"


GameObject::GameObject()
{
}

template<class T> requires isComponent<T>
T* GameObject::GetComponent()
{
	T* returnComp = nullptr;

	//Iterate over the componenets attached to the GameObject
	for (int i = 0; i < m_Components.size(); i++)
	{
		//Cast to type T and set returnComp will point to the component of type T
		returnComp = static_cast<T*>(m_Components[i].get());
		if (returnComp != nullptr)
		{
			//Break if T has been found
			break;
		}
	}

	//Returns the pointer to the component
	return returnComp;
}

template<class T> requires isComponent<T>
T* GameObject::AddComponent()
{
	//Creates a new component
	std::unique_ptr<Component> newComp = std::make_unique<T>(this);

	//Move this newly created component into the vector
	m_Components.push_back(std::move(newComp));

	//Returns a pointer to the newly added component
	return static_cast<T*>(m_Components[m_Components.size() - 1].get());
}

template<class T> requires isComponent<T>
bool GameObject::RemoveComponent(T* comp)
{
	//Search for the component in the m_Components vector
	auto it = std::find_if(m_Components.begin(), m_Components.end(), [comp](const std::unique_ptr<Component>& ptr) 
	{
		return dynamic_cast<T*>(ptr.get()) == comp;
	});

	//If the component is found, remove it
	if (it != m_Components.end()) 
	{
		m_Components.erase(it);
		return true;
	}

	//Component not found or not removed
	return false; 


}
