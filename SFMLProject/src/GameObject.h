#pragma once
#include <isComponent.h>
#include <ResourceManager.h>
#include <PlayerComponent.h>
#include <EnemyComponent.h>
#include <BoxCollider.h>

class GameObject : public Object
{

public:
	//Construtor
	GameObject();

	//Deconstructor
	~GameObject();

	//Copy Constructor
	GameObject(const GameObject&) = default;


	void Update(float deltaTime);
	void Render(sf::RenderWindow& window);


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


	//Sets and Get textures
	void SetTexture(sf::Texture *texture);
	const sf::Texture* GetTexture();

	//Set and get TextureUVs
	void SetTextureRect(sf::IntRect &textureRect);
	sf::IntRect GetTextureRect();

	//Sets and Gets RectangleShapes
	void SetRectangleShape(sf::RectangleShape &shape);
	sf::RectangleShape& GetRectangleShape();

	//Sets and get locations
	void SetLocation(float x, float y);
	AG::Vector2<float>& GetLocation();

	//Draw an outline around the gameObject
	void DrawOutlines(sf::RectangleShape &shape);

	bool HasBoxCollider(GameObject* GameObject);
	AG::Vector2<float>& GetMoveDirection();

	void SetIsWall(bool isWall);
	bool GetIsWall();

	void SetIsShooting(bool isShooting);
	bool GetIsShooting();

	void Destroy();

private:
	//Basic attributes for the GameObject
	std::string m_Name;
	bool m_Active;
	std::string m_Tag;
	bool m_IsWall = false;
	bool m_IsShooting = false;

	AG::Vector2<float> m_location;
	AG::Vector2<float> m_MoveDirection;
	sf::RectangleShape m_SpriteShape;

	


	//1 pointer, if gameObject is gone, so does the component, all memory is wiped
	std::vector<std::unique_ptr<Component>> m_Components;

public:
	//Compares two GameObject instances, compares the uuids returns true if two GameObjects are equal
	inline bool operator==(const GameObject& other) { return this->uuid == other.uuid; }

	//Compares two GameObject instances, return true if they dont match
	inline bool operator!=(const GameObject& other) { return !(*this == other); }



	//TEMPLATED FUNCTIONS NEED IMPLEMENTATION WITHIN THE HEADER FILE, WHERE ITS INSTANSTIATED OR THE COMPILER WILL HATE YOU


	//Template functions that only 'component' can be used with them
	//Gets the component
	template<class T> requires isComponent<T> T* GetComponent()
	{
		T* returnComp = nullptr;

		//Iterate over the componenets attached to the GameObject
		for (int i = 0; i < m_Components.size(); i++)
		{
			//Cast to type T and set returnComp will point to the component of type T
			returnComp = dynamic_cast<T*>(m_Components[i].get());
			if (returnComp != nullptr)
			{
				//Break if T has been found
				break;
			}
		}

		//Returns the pointer to the component
		return returnComp;
	}

	//Adds a component to the GameObject
	template<class T> requires isComponent<T> T* AddComponent()
	{
		//Creates a new component
		std::unique_ptr<Component> newComp = std::make_unique<T>(this);

		//Move this newly created component into the vector
		m_Components.push_back(std::move(newComp));

		std::cout << "Component attached: " << typeid(T).name() << " to " << GetName() << std::endl;

		//Returns a pointer to the newly added component
		return static_cast<T*>(m_Components[m_Components.size() - 1].get());
	}

	//Removes a component from the GameObject
	template<class T> requires isComponent<T> bool RemoveComponent(T* comp)
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

};


