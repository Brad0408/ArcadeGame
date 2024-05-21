#pragma once
#include <Enemy.h>

//Stationary enemy
class Electrode : public Enemy
{

public:
	Electrode(const AG::Vector2<float>& spawnLocation, float speed);
	~Electrode() {}

	void Update(float deltaTime) override;


private:
	AG::Vector2<float> m_ElectrodeSize = AG::Vector2<float>::one * 35;

	sf::RectangleShape m_ElectrodeShape;
	AnimationComponent* animationComponent;

	sf::IntRect m_ElectrodeTextureUV;
};
