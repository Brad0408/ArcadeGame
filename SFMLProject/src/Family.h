#pragma once
#include <GameObject.h>



class Family : public GameObject
{



public:
	Family(const AG::Vector2<float>& spawnLocation);
	~Family() {};

	void Update(float deltaTime) override;
	void Move(float deltaTime);

	void ReverseDirection();

private:
	AG::Vector2<float> m_FamilySize = AG::Vector2<float>::one * 35;

	sf::RectangleShape m_FamilyRectangle;
	AnimationComponent* animationComponent;

	sf::IntRect m_FamilyTextureUV;

	float m_MovementSpeed = 10.0f;


	sf::Clock directionChangeClock;
	AG::Vector2<float> direction;

};
