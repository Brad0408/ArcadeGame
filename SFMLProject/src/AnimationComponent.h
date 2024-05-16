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
	~AnimationComponent();



	// Set the current animation state
	void SetPlayerAnimation(PlayerStates state);
	PlayerStates GetPlayerState();
	std::unordered_map<PlayerStates, std::vector<sf::IntRect>> &GetPlayerAnimationsMap();

	void SetGruntAnimation(GruntStates state);
	GruntStates GetGruntState();
	std::unordered_map<GruntStates, std::vector<sf::IntRect>> &GetGruntAnimationsMap();

	void SetFamilyAnimation(FamilyStates state);
	FamilyStates GetFamilyState();
	std::unordered_map<FamilyStates, std::vector<sf::IntRect>> &GetFamilyAnimationsMap();


private:
	//Maps to store UV values
	std::unordered_map<PlayerStates, std::vector<sf::IntRect>> m_playerAnimations;
	std::unordered_map<GruntStates, std::vector<sf::IntRect>> m_gruntAnimations;  
	std::unordered_map<FamilyStates, std::vector<sf::IntRect>> m_familyAnimations;  


	PlayerStates m_currentPlayerState;
	GruntStates m_currentGruntState;
	FamilyStates m_currentFamilyState;

	float m_frameDuration = 0.5f;
	float m_elapsedTime = 0.0f;

public:
	template<typename Enum, typename Map> sf::IntRect GetCurrentFrame(Enum state, float deltaTime, const Map& animations)
	{
		// Retrieve the animation frames for the specified player state
		auto it = animations.find(state);
		if (it != animations.end())
		{
			// Get the animation frames for the player state
			const std::vector<sf::IntRect>& frames = it->second;

			// Check if there are any frames for the player state
			if (!frames.empty())
			{
				// Use a static variable to keep track of the current frame index
				static int currentFrameIndex = 0;

				// Get the total number of frames
				int numFrames = frames.size();

				// Calculate the time per frame (assuming constant frame rate)
				float timePerFrame = m_frameDuration;

				// Update the elapsed time
				m_elapsedTime += deltaTime;

				// Calculate the current frame index based on elapsed time
				currentFrameIndex = static_cast<int>(m_elapsedTime / timePerFrame) % numFrames;

				// Return the current frame
				return frames[currentFrameIndex];
			}
		}

		// Return an empty rectangle if the state is not found or there are no frames
		return sf::IntRect();
	}
};


