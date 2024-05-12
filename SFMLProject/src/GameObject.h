#pragma once
#include <isComponent.h>
#include <ResourceManager.h>
#include <GameManager.h>
#include <PlayerComponent.h>
#include <BoxCollider.h>
#include <CircleCollider.h>
#include <AnimationComponent.h>

class GameObject : public Object
{

public:
	//Construtor
	GameObject();

	//Deconstructor
	~GameObject();

	//Copy Constructor
	GameObject(const GameObject&) = default;


	void Update(float deltaTime) {};
	virtual void Update(float deltaTime, const AG::Vector2<float>& playerPosition) {};



	inline void SetName(std::string newName) { m_Name = newName; }
	inline std::string GetName() { return m_Name; }

	void SetActive(bool newActive) {};
	inline bool GetActive() { return m_Active; }

	inline void SetTag(std::string newTag) { m_Tag = newTag; }
	std::string GetTag() const { return m_Tag; }


	inline bool CompareTag(std::string comp) { return m_Tag == comp; }


	//Sets and Gets RectangleShapes
	void SetRectangleShape(sf::RectangleShape &shape);
	sf::RectangleShape& GetRectangleShape();

	//Set and Gets CircleShapes
	void SetCircleShape(sf::CircleShape& shape);
	sf::CircleShape& GetCircleShape();

	//Sets and get locations
	void SetLocation(float x, float y);
	AG::Vector2<float>& GetLocation();

	AG::Vector2<float>& GetMoveDirection();

	void SetIsWall(bool isWall);
	bool GetIsWall();

	void SetIsPlayer(bool isPlayer);
	bool GetIsPlayer();

	void SetIsEnemy(bool isEnemy);
	bool GetIsEnemy();

	void SetIsShooting(bool isShooting);
	bool GetIsShooting();

	virtual void MarkForRemoval();
	virtual bool ShouldRemove();

private:
	std::string m_Name;
	bool m_Active;
	std::string m_Tag;
	bool m_IsWall = false;
	bool m_IsPlayer = false;
	bool m_IsEnemy = false;
	bool m_IsShooting = false;
	bool m_shouldRemove = false;


	AG::Vector2<float> m_location;
	AG::Vector2<float> m_MoveDirection;
	sf::RectangleShape m_SpriteShape;
	sf::CircleShape m_CircleShape;

	


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

		// Iterate over the components attached to the GameObject
		for (int i = 0; i < m_Components.size(); i++)
		{
			// Check if the component is not null
			if (m_Components[i])
			{
				// Attempt to cast to type T
				T* castComp = dynamic_cast<T*>(m_Components[i].get());
				// If the cast is successful and the pointer is not null, set returnComp and break
				if (castComp != nullptr)
				{
					returnComp = castComp;
					break;
				}
			}
		}

		// Returns the pointer to the component
		return returnComp;
	}

	//Adds a component to the GameObject
	template<class T> requires isComponent<T> T* AddComponent()
	{
		//Creates a new component
		std::unique_ptr<Component> newComp = std::make_unique<T>(this);

		//Move this newly created component into the vector
		m_Components.push_back(std::move(newComp));

		//std::cout << "Component attached: " << typeid(T).name() << " to " << GetName() << std::endl;

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


