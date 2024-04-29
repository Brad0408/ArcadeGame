#pragma once
#include <GameObject.h>

class Bullet : public GameObject
{

public:

	Bullet();

	~Bullet();



private:
	AG::Vector2<float> m_velocity;


};
