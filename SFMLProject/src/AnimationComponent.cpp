#include "AnimationComponent.h"



AnimationComponent::AnimationComponent(GameObject* owner) : Component(owner)
{
	m_playerAnimations =
	{
		{PlayerStates::Down, {sf::IntRect(342, 164, 24, 24), sf::IntRect(367, 164, 24, 24), sf::IntRect(394, 164, 24, 24)}},
		{PlayerStates::Up, {sf::IntRect(420, 164, 24, 24), sf::IntRect(446, 164, 24, 24), sf::IntRect(472, 164, 24, 24)}},   
		{PlayerStates::Left, {sf::IntRect(185, 164, 24, 24), sf::IntRect(210, 164, 24, 24)}},                            
		{PlayerStates::Right, {sf::IntRect(262, 164, 24, 24), sf::IntRect(288, 164, 24, 24)}},
		{PlayerStates::Idle, {sf::IntRect(342, 164, 24, 24)}}
	};

	m_gruntAnimations =
	{
		{GruntStates::Moving, {sf::IntRect(148, 235, 24, 24), sf::IntRect(178, 235, 24, 27), sf::IntRect(208, 235, 24, 27)}}
	};
}

AnimationComponent::~AnimationComponent()
{
}


void AnimationComponent::SetPlayerAnimation(PlayerStates state)
{
	m_currentPlayerState = state;
}

AnimationComponent::PlayerStates AnimationComponent::GetPlayerState()
{
	return m_currentPlayerState;
}

std::unordered_map<AnimationComponent::PlayerStates, std::vector<sf::IntRect>>& AnimationComponent::GetPlayerAnimationsMap()
{
	return m_playerAnimations;
}

void AnimationComponent::SetGruntAnimation(GruntStates state)
{
	m_currentGruntState = state;
}

AnimationComponent::GruntStates AnimationComponent::GetGruntState()
{
	return m_currentGruntState;
}

std::unordered_map<AnimationComponent::GruntStates, std::vector<sf::IntRect>>& AnimationComponent::GetGruntAnimationsMap()
{
	return m_gruntAnimations;
}

