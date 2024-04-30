#pragma once
#include <Component.h>
#include <GameObject.h>

class Bullet;

class PlayerComponent : public Component
{

public:
	PlayerComponent(GameObject* owner);
	~PlayerComponent();


	void Move(float deltaTime);

	void CreateFiringPoint();
	void CalculateFiringPointRotation(sf::RenderWindow &window);
	AG::Vector2<float> &GetFirePointLocation();

	void Shooting(sf::RenderWindow& window);

private:
	AG::Vector2<float> m_PlayerSize = AG::Vector2<float>::one * 50;
	AG::Vector2<float> m_FiringPointSize = AG::Vector2<float>::one * 10;

	sf::RectangleShape m_PlayerShapeRectangle;
	sf::RectangleShape m_FiringPoint;

	sf::IntRect m_PlayerTextureUV;

	float m_MovementSpeed = 37.0f;
	float m_orbitRadius = 50.0f;
	float m_angle;

	std::vector<Bullet> m_Bullets;


};
