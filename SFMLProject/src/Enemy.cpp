#include "Enemy.h"

Enemy::Enemy(const AG::Vector2<float>& spawnLocation)
{
	SetName("Enemy");
	SetTag("Enemy");
	SetIsEnemy(true);
	SetIsWall(false);

	SetLocation(spawnLocation.x, spawnLocation.y);


	//Cookie cutter part of sprite sheet (0,0 = Coordinates, 24, 24 = Size of rectangle)
	m_EnemyTextureUV = sf::IntRect(148, 235, 24, 24);

	m_EnemyShapeRectangle.setSize(m_EnemySize);



	//Set texture to be the whole sprite sheet
	m_EnemyShapeRectangle.setTexture(&ResourceManager::GetTexture("Enemy"));


	//Set the texture to the cookie cutter section of the sprite sheet
	m_EnemyShapeRectangle.setTextureRect(m_EnemyTextureUV);


	//Set Rect to the middle / position
	m_EnemyShapeRectangle.setOrigin(m_EnemySize / 2);
	m_EnemyShapeRectangle.setPosition(GetLocation());


	//_GameObject->DrawOutlines(PlayerShapeRectangle);


	//Actually set it
	SetRectangleShape(m_EnemyShapeRectangle);

	AddComponent<BoxCollider>();
	AddComponent<AnimationComponent>();
	animationComponent = GetComponent<AnimationComponent>();

	// Initialize the random number generator
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(10.0f, 15.5f);

	// Assign a random movement speed to the enemy
	m_MovementSpeed = dis(gen);

}


Enemy::~Enemy()
{
}


void Enemy::Update(float deltaTime, const AG::Vector2<float>& playerPosition)
{
	animationComponent->SetGruntAnimation(AnimationComponent::GruntStates::Moving);
	m_EnemyShapeRectangle.setTextureRect(animationComponent->GetCurrentFrame(animationComponent->GetGruntState(), deltaTime, animationComponent->GetGruntAnimationsMap()));
	SetRectangleShape(m_EnemyShapeRectangle);

	// Calculate direction vector towards the player
	AG::Vector2<float> direction = playerPosition - GetLocation();
	direction.Normalise(); // Normalize the direction vector to get a unit vector

	// Move the enemy towards the player's position
	AG::Vector2<float> displacement = direction * m_MovementSpeed * deltaTime;
	Move(displacement);

	GetComponent<BoxCollider>()->DrawOutlines(GetRectangleShape());
}

void Enemy::Move(AG::Vector2<float>& displacement)
{
	SetLocation(GetLocation().x + displacement.x, GetLocation().y + displacement.y);
	GetRectangleShape().setPosition(GetLocation());
}

