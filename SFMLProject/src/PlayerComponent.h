#pragma once
#include <Component.h>
#include <GameObject.h>

class Bullet;
class AnimationComponent;

//Essentially a player class but done as a component instead
class PlayerComponent : public Component
{

public:
	PlayerComponent(GameObject* owner);
	~PlayerComponent() {}


	void Move(float deltaTime);
	void Update(float deltaTime) override;

	void CreateFiringPoint();
	void CalculateFiringPointRotation(sf::RenderWindow &window);
	const AG::Vector2<float> &GetFirePointLocation();
	AG::Vector2<float> &GetMousePosition();
	AG::Vector2<float> CalculateDirection();

	void Shooting();

private:
	AG::Vector2<float> m_PlayerSize = AG::Vector2<float>::one * 35;
	AG::Vector2<float> m_FiringPointSize = AG::Vector2<float>::one * 8;
	AG::Vector2<float> m_FiringPointLocation = AG::Vector2<float>::one * 10;
	AG::Vector2<float> m_MousePosition;


	sf::RectangleShape m_PlayerShapeRectangle;
	sf::RectangleShape m_FiringPoint;

	sf::IntRect m_PlayerTextureUV;
	AnimationComponent* animationComponent;

	float m_MovementSpeed = 37.0f;
	float m_orbitRadius = 50.0f;
	float m_angle;
	float m_timeSinceLastShot = 0.0f;

};
