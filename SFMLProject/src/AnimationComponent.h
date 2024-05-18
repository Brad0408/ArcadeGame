#pragma once
#include <Component.h>


class AnimationComponent : public Component
{

public:
	enum class PlayerStates
	{
		Down,
		Up,
		Left,
		Right,
		Idle
	};

	enum class FamilyStates
	{
		Down,
		Up,
		Left,
		Right,
		Idle
	};

	enum class GruntStates 
	{
		Moving
	};

	AnimationComponent(GameObject* owner);
	~AnimationComponent() {};



	//Set and get animation states and UV maps
	void SetPlayerAnimation(PlayerStates state);
	PlayerStates GetPlayerState() { return m_currentPlayerState; }
	std::unordered_map<PlayerStates, std::vector<sf::IntRect>> &GetPlayerAnimationsMap();

	void SetGruntAnimation(GruntStates state);
	GruntStates GetGruntState() { return m_currentGruntState; }
	std::unordered_map<GruntStates, std::vector<sf::IntRect>> &GetGruntAnimationsMap();

	void SetFamilyAnimation(FamilyStates state);
	FamilyStates GetFamilyState() { return m_currentFamilyState; }
	std::unordered_map<FamilyStates, std::vector<sf::IntRect>> &GetFamilyAnimationsMap();


private:
	//Maps to store UV values
	std::unordered_map<PlayerStates, std::vector<sf::IntRect>> m_playerAnimations;
	std::unordered_map<GruntStates, std::vector<sf::IntRect>> m_gruntAnimations;  
	std::unordered_map<FamilyStates, std::vector<sf::IntRect>> m_familyAnimations;  

	//Enums of states
	PlayerStates m_currentPlayerState;
	GruntStates m_currentGruntState;
	FamilyStates m_currentFamilyState;

	float m_frameDuration = 0.5f;
	float m_elapsedTime = 0.0f;

public:
	//Gets the current frame based of its Enum state and the map of sf::IntRect associated with it
	template<typename Enum, typename Map> sf::IntRect GetCurrentFrame(Enum state, float deltaTime, const Map& animations)
	{
		//Retrieve the animation frames for the specified object state
		auto it = animations.find(state);
		if (it != animations.end())
		{
			//Get the animation frames for the objects state
			const std::vector<sf::IntRect>& frames = it->second;

			//Check if there are any frames for the player state
			if (!frames.empty())
			{
				//Get current frame and the max frame
				static int currentFrameIndex = 0;
				int numFrames = frames.size();


				//Update the frame based of deltaTime updates
				float timePerFrame = m_frameDuration;
				m_elapsedTime += deltaTime;
				currentFrameIndex = static_cast<int>(m_elapsedTime / timePerFrame) % numFrames;

				//Return the current frame
				return frames[currentFrameIndex];
			}
		}
		return sf::IntRect();
	}
};


