#pragma once
#include <Vector2.h>
#include <Component.h>


class Transform : public Component
{

public:
	Transform();


	~Transform();


	AG::Vector2<float> GameObjectPosition;
	// GameObjectRotation; - Of type quaternion, would have to make a quat header or smthing 
	AG::Vector2<float> GameObjectScale;


	AG::Vector2<float> GetActorLocation() { return GameObjectPosition; }
	void SetActorLocation(AG::Vector2<float>& newLocation);;


	// GetActorRotation() { return GameObjectRotation; }
	AG::Vector2<float> GetActorScale() { return GameObjectScale; }



};
