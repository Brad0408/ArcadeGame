#include "BoxCollider.h"

BoxCollider::BoxCollider(GameObject* owner) : ColliderComponent (owner)
{
}


//Just checks collision between two objects by checking if their shapes intersect
bool BoxCollider::CheckCollision(GameObject* objectOne, GameObject* objectTwo)
{
	sf::RectangleShape rectOne = objectOne->GetRectangleShape();
	sf::RectangleShape rectTwo = objectTwo->GetRectangleShape();

	if (rectOne.getGlobalBounds().intersects(rectTwo.getGlobalBounds()))
	{
		//std::cout << "Collision has occured" << std::endl;
		return true;
	}

	return false;
}


//More inteserection checks - Wall collision will push the object in the opposite direction they are moving to replicate a wall
bool BoxCollider::WallCollision(GameObject* objectOne, GameObject* objectTwo)
{
	sf::FloatRect playerBounds = objectOne->GetRectangleShape().getGlobalBounds();
	sf::FloatRect wallBounds = objectTwo->GetRectangleShape().getGlobalBounds();
	sf::FloatRect intersection;

	if (playerBounds.intersects(wallBounds, intersection))
	{
		float overlapX = intersection.width;
		float overlapY = intersection.height;

		//Get the direction the object is going in
		AG::Vector2<float> moveDirection = objectOne->GetMoveDirection();

		//Calculate displacement needed to move the object out of the wall
		float displacementX = 0.0f;
		float displacementY = 0.0f;

		if (moveDirection.x > 0)
		{
			displacementX = overlapX;
		}
		else if (moveDirection.x < 0)
		{
			displacementX = -overlapX;
		}

		if (moveDirection.y > 0)
		{
			displacementY = overlapY;
		}
		else if (moveDirection.y < 0)
		{
			displacementY = -overlapY;
		}

		//Apply the displacement to move the object out of the wall
		AG::Vector2<float> currentPosition = objectOne->GetLocation();
		currentPosition.x -= displacementX;
		currentPosition.y -= displacementY;

		//Set the objects new position based on the wall pushing it the oppsosite way
		objectOne->SetLocation(currentPosition.x, currentPosition.y);

		return true; 

	}

	return false;
}

void BoxCollider::DrawOutlines(sf::RectangleShape& shape)
{
	shape.setOutlineThickness(3);
	shape.setOutlineColor(sf::Color(0, 0, 255));
}

