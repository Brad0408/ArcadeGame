#include "Enemy.h"

Enemy::Enemy(const AG::Vector2<float>& spawnLocation)
{
	SetName("Enemy");
	SetTag("Enemy");
	SetIsEnemy(true);

	SetLocation(spawnLocation.x, spawnLocation.y);


	//Cookie cutter part of sprite sheet (0,0 = Coordinates, 24, 24 = Size of rectangle)
	m_EnemyTextureUV = sf::IntRect(148, 235, 24, 24);

	m_EnemyShapeRectangle.setSize(m_EnemySize);



	//Set texture to be the whole sprite sheet
	m_EnemyShapeRectangle.setTexture(&ResourceManager::GetTexture("Player"));


	//Set the texture to the cookie cutter section of the sprite sheet
	m_EnemyShapeRectangle.setTextureRect(m_EnemyTextureUV);


	//Set Rect to the middle / position
	m_EnemyShapeRectangle.setOrigin(m_EnemySize / 2);
	m_EnemyShapeRectangle.setPosition(GetLocation());


	//_GameObject->DrawOutlines(PlayerShapeRectangle);


	//Actually set it
	SetRectangleShape(m_EnemyShapeRectangle);

	AddComponent<BoxCollider>()->DrawOutlines(GetRectangleShape());
}


Enemy::~Enemy()
{
}
