#pragma once
#include <GameObject.h>

//Enemy is being used to grunts (dumb idea should make a grunt class, but project it small enough to allow for this)
class Enemy : public GameObject
{

public:
	Enemy(const AG::Vector2<float>& spawnLocation, float speed);
	~Enemy() {}

	void Update(float delatTime) override {}
	void Update(float deltaTime, const AG::Vector2<float>& playerPosition) override;
	void Move(AG::Vector2<float>& displacement);

private:
	AG::Vector2<float> m_EnemySize = AG::Vector2<float>::one * 35;

	sf::RectangleShape m_EnemyShapeRectangle;
	AnimationComponent* animationComponent;

	sf::IntRect m_EnemyTextureUV;

	float m_MovementSpeed = 10.0f;


};
