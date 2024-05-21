#include "Enemy.h"

//Used to create the grunt enemies
Enemy::Enemy(const AG::Vector2<float>& spawnLocation, float speed)
{
	SetName("Enemy");
	SetTag("Enemy");
	SetLocation(spawnLocation.x, spawnLocation.y);


	m_EnemyTextureUV = sf::IntRect(148, 235, 24, 24);
	m_EnemyShapeRectangle.setSize(m_EnemySize);
	m_EnemyShapeRectangle.setTexture(&ResourceManager::GetTexture("Enemy"));
	m_EnemyShapeRectangle.setTextureRect(m_EnemyTextureUV);
	m_EnemyShapeRectangle.setOrigin(m_EnemySize / 2);
	m_EnemyShapeRectangle.setPosition(GetLocation());
	SetRectangleShape(m_EnemyShapeRectangle);



	AddComponent<BoxCollider>();
	AddComponent<AnimationComponent>();
	animationComponent = GetComponent<AnimationComponent>();

	m_MovementSpeed = speed;
}

//Update used to find the players location and move towards it
void Enemy::Update(float deltaTime, const AG::Vector2<float>& playerPosition)
{
	animationComponent->SetGruntAnimation(AnimationComponent::GruntStates::Moving);
	m_EnemyShapeRectangle.setTextureRect(animationComponent->GetCurrentFrame(animationComponent->GetGruntState(), deltaTime, animationComponent->GetGruntAnimationsMap()));
	SetRectangleShape(m_EnemyShapeRectangle);

	//Calculate direction vector towards the player
	AG::Vector2<float> direction = playerPosition - GetLocation();
	direction.Normalise();

	//Move the enemy towards the players position
	AG::Vector2<float> displacement = direction * m_MovementSpeed * deltaTime;
	Move(displacement);

	//GetComponent<BoxCollider>()->DrawOutlines(GetRectangleShape());
}

void Enemy::Move(AG::Vector2<float>& displacement)
{
	SetLocation(GetLocation().x + displacement.x, GetLocation().y + displacement.y);
	//GetRectangleShape().setPosition(GetLocation());
}

