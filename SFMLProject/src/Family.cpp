#include "Family.h"

Family::Family(const AG::Vector2<float>& spawnLocation)
{
	SetName("Family");
	SetTag("Family");
	SetLocation(spawnLocation.x, spawnLocation.y);

	m_FamilyTextureUV = sf::IntRect(355, 0, 24, 28);
	m_FamilyRectangle.setSize(m_FamilySize);
	m_FamilyRectangle.setTexture(&ResourceManager::GetTexture("Enemy"));
	m_FamilyRectangle.setTextureRect(m_FamilyTextureUV);
	m_FamilyRectangle.setOrigin(m_FamilySize / 2);
	m_FamilyRectangle.setPosition(GetLocation());
	SetRectangleShape(m_FamilyRectangle);




	AddComponent<BoxCollider>();
	AddComponent<AnimationComponent>();
	animationComponent = GetComponent<AnimationComponent>();

	//Random number to set the random direction the family memeber will go in
	int randDirection = std::rand() % 4;
	switch (randDirection)
	{
	case 0: 
		direction = AG::Vector2<float>(-1.0f, 0.0f); 
		break; //Left

	case 1:
		direction = AG::Vector2<float>(1.0f, 0.0f); 
		break;  //Right

	case 2:
		direction = AG::Vector2<float>(0.0f, -1.0f);
		break; //Up

	case 3: 
		direction = AG::Vector2<float>(0.0f, 1.0f); 
		break;  //Down
	}

}

void Family::Update(float deltaTime)
{
	//Using sf::Clock to change the familys direction every 1.5 seconds
	if (directionChangeClock.getElapsedTime().asSeconds() >= 1.5f) 
	{
		int randDirection = std::rand() % 4;
		switch (randDirection)
		{
		case 0:
			direction = AG::Vector2<float>(-1.0f, 0.0f);
			break; //Left

		case 1:
			direction = AG::Vector2<float>(1.0f, 0.0f);
			break;  //Right

		case 2:
			direction = AG::Vector2<float>(0.0f, -1.0f);
			break; //Up

		case 3:
			direction = AG::Vector2<float>(0.0f, 1.0f);
			break;  //Down
		}

		directionChangeClock.restart();
	}

	Move(deltaTime);

	//GetComponent<BoxCollider>()->DrawOutlines(GetRectangleShape());
}

void Family::Move(float deltaTime) 
{
	GetLocation() += direction * m_MovementSpeed * deltaTime;


	for (GameObject* wall : GameManager::GetWalls())
	{
		BoxCollider* wallCollider = wall->GetComponent<BoxCollider>();
		BoxCollider* familyCollider = GetComponent<BoxCollider>();

		if (wallCollider && familyCollider && wallCollider->CheckCollision(wall, this))
		{
			BoxCollider::WallCollision(this, wall);

			//ReverseDirection();

			break;
		}
	}







	//Set the animation based on the direction
	if (direction.x < 0)
	{
		animationComponent->SetFamilyAnimation(AnimationComponent::FamilyStates::Left);
	}
	else if (direction.x > 0)
	{
		animationComponent->SetFamilyAnimation(AnimationComponent::FamilyStates::Right);
	}
	else if (direction.y < 0) 
	{
		animationComponent->SetFamilyAnimation(AnimationComponent::FamilyStates::Up);
	}
	else if (direction.y > 0)
	{
		animationComponent->SetFamilyAnimation(AnimationComponent::FamilyStates::Down);
	}

	m_FamilyRectangle.setTextureRect(animationComponent->GetCurrentFrame(animationComponent->GetFamilyState(), deltaTime, animationComponent->GetFamilyAnimationsMap()));
	SetRectangleShape(m_FamilyRectangle);

	direction.Normalise();
	GetMoveDirection() = direction;

	GetRectangleShape().setPosition(GetLocation());
}

//Not working - the direction tries to just reset itself on the clock so the logic is wrong
void Family::ReverseDirection()
{
	direction.x = -direction.x;
    direction.y = -direction.y;
}
