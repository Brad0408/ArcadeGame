#include "Family.h"

Family::Family(const AG::Vector2<float>& spawnLocation)
{
	SetName("Family");
	SetTag("Family");
	SetLocation(spawnLocation.x, spawnLocation.y);


	m_FamilyTextureUV = sf::IntRect(148, 235, 24, 24);
	m_FamilyRectangle.setSize(m_FamilySize);
	m_FamilyRectangle.setTexture(&ResourceManager::GetTexture("Enemy"));
	m_FamilyRectangle.setTextureRect(m_FamilyTextureUV);
	m_FamilyRectangle.setOrigin(m_FamilySize / 2);
	m_FamilyRectangle.setPosition(GetLocation());
	SetRectangleShape(m_FamilyRectangle);




	AddComponent<BoxCollider>();
	AddComponent<AnimationComponent>();
	animationComponent = GetComponent<AnimationComponent>();


}


void Family::Update(float deltaTime)
{
}


void Family::Move()
{
}
