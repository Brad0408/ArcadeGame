#include "Electrode.h"

Electrode::Electrode(const AG::Vector2<float>& spawnLocation, float speed) : Enemy(spawnLocation, speed)
{
	SetName("Electrode");
	SetTag("Electrode");
	SetLocation(spawnLocation.x, spawnLocation.y);


	m_ElectrodeTextureUV = sf::IntRect(118, 234, 24, 24);
	m_ElectrodeShape.setSize(m_ElectrodeSize);
	m_ElectrodeShape.setTexture(&ResourceManager::GetTexture("Enemy"));
	m_ElectrodeShape.setTextureRect(m_ElectrodeTextureUV);
	m_ElectrodeShape.setOrigin(m_ElectrodeSize / 2);
	m_ElectrodeShape.setPosition(GetLocation());
	SetRectangleShape(m_ElectrodeShape);



	AddComponent<BoxCollider>();
	AddComponent<AnimationComponent>();
	animationComponent = GetComponent<AnimationComponent>();

	GetComponent<BoxCollider>()->DrawOutlines(GetRectangleShape());


}

void Electrode::Update(float deltaTime)
{
	animationComponent->SetElectrodeAnimation(AnimationComponent::ElectrodeStates::Idle);
	m_ElectrodeShape.setTextureRect(animationComponent->GetCurrentFrame(animationComponent->GetElectrodeState(), deltaTime, animationComponent->GetElectrodeAnimationsMap()));
	SetRectangleShape(m_ElectrodeShape);
}
