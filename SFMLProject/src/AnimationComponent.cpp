#include "AnimationComponent.h"


//Storing Enum state to a vector of IntRect values on the whole sprite sheet
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

	
	m_familyAnimations =
	{
		{FamilyStates::Down, {sf::IntRect(355, 0, 24, 28), sf::IntRect(382, 0, 24, 29), sf::IntRect(408, 0, 24, 29)}},
		{FamilyStates::Up, {sf::IntRect(434, 0, 24, 28), sf::IntRect(460, 0, 24, 29), sf::IntRect(486, 0, 24, 29)}},
		{FamilyStates::Left, {sf::IntRect(200, 0, 24, 28), sf::IntRect(225, 0, 24, 28), sf::IntRect(252, 0, 24, 28)}},
		{FamilyStates::Right, {sf::IntRect(278, 0, 24, 28), sf::IntRect(304, 0, 24, 28), sf::IntRect(331, 0, 24, 28)}},
		{FamilyStates::Idle, {sf::IntRect(355, 0, 24, 28)}}
	};
}

///////Player////////


void AnimationComponent::SetPlayerAnimation(PlayerStates state)
{
	m_currentPlayerState = state;
}

std::unordered_map<AnimationComponent::PlayerStates, std::vector<sf::IntRect>>& AnimationComponent::GetPlayerAnimationsMap()
{
	return m_playerAnimations;
}


///////Grunt////////


void AnimationComponent::SetGruntAnimation(GruntStates state)
{
	m_currentGruntState = state;
}


std::unordered_map<AnimationComponent::GruntStates, std::vector<sf::IntRect>>& AnimationComponent::GetGruntAnimationsMap()
{
	return m_gruntAnimations;
}


///////Family////////


void AnimationComponent::SetFamilyAnimation(FamilyStates state)
{
	m_currentFamilyState = state;
}


std::unordered_map<AnimationComponent::FamilyStates, std::vector<sf::IntRect>>& AnimationComponent::GetFamilyAnimationsMap()
{
	return m_familyAnimations;
}

