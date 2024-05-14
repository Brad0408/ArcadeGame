#include "ResourceManager.h"

std::unordered_map<std::string, sf::Texture> ResourceManager::Textures;
std::unordered_map<std::string, sf::SoundBuffer> ResourceManager::SoundBuffers;

std::list<sf::Sound> ResourceManager::Sounds;

 
//Set the key and matching value to the key with the file path
const std::unordered_map<std::string, std::string> ResourceManager::TexturePaths = 
{
	{"Player", "Textures/robotronsprites.png"},
	{"Enemy", "Textures/robotronsprites.png"},
	{"Mario", "Textures/mario.jpg"},
};


const std::unordered_map<std::string, std::string> ResourceManager::SoundBufferPaths =
{
	{"Shoot", "Audio/Shoot.wav"},
	{"Respawn", "Audio/respawn.mp3"},
};







//Returns the texture path by passing in the matching key as a string
const std::string& ResourceManager::GetTexturePath(const std::string& TextureName) 
{
	//Check if the texture name exists in the map
	if (TexturePaths.find(TextureName) != TexturePaths.end()) 
	{
		//std::cout << "Texture path was found of key: " << TextureName << std::endl;
		return TexturePaths.at(TextureName);
	}
	else
	{
		//If the passed in string value doesnt match any keys within the map do this
		std::cout<< "Texture path name not found: " << TextureName << ", created Default Texture " << std::endl;

		static const std::string defaultPath = "Textures/robotronsprites.jpg";
		return defaultPath;
	}
}


sf::Texture& ResourceManager::GetTexture(const std::string& TextureName)
{
	// Check if the texture is in the map
	auto it = Textures.find(TextureName);
	if (it == Textures.end())
	{
		// Texture not found, load the default texture
		sf::Texture texture;
		if (!texture.loadFromFile(GetTexturePath(TextureName)))
		{
			// Default texture could not be loaded
			std::cout << "Default texture not found: " << TextureName << std::endl;
			// Load the default texture
			if (!texture.loadFromFile("Textures/robotronsprites.jpg"))
			{
				// Failed to load the default texture, print an error message
				std::cout << "Failed to load default texture: robotronsprites.jpg" << std::endl;
			}
		}

		// Insert the default texture into the map
		Textures.emplace(TextureName, std::move(texture));

		// Return the newly inserted texture
		return Textures.at(TextureName);
	}

	// Texture found in the map, return it
	return it->second;
}






const std::string& ResourceManager::GetSoundBufferPath(const std::string& SoundName)
{
	// Check if the sound name exists in the map
	if (SoundBufferPaths.find(SoundName) != SoundBufferPaths.end())
	{
		// Sound path found
		return SoundBufferPaths.at(SoundName);
	}
	else
	{
		// Sound path not found
		static const std::string empty = "";
		std::cout << "Sound buffer path not found: " << SoundName << std::endl;
		return empty;
	}
}



sf::SoundBuffer& ResourceManager::GetSoundBuffer(const std::string& SoundName)
{
	// Check if the sound buffer is in the map
	if (SoundBuffers.find(SoundName) == SoundBuffers.end())
	{
		// Sound buffer not found, load it from file
		sf::SoundBuffer buffer;
		if (!buffer.loadFromFile(GetSoundBufferPath(SoundName)))
		{
			// Error loading sound buffer
			std::cout << "Sound buffer not found: " << SoundName << std::endl;
		}
		else
		{
			// Move the loaded sound buffer into the map
			SoundBuffers.emplace(SoundName, std::move(buffer));
		}
	}

	// Return the sound buffer
	return SoundBuffers.at(SoundName);
}







void ResourceManager::PlaySound(const std::string& name)
{
	// Check if the sound buffer exists
	if (SoundBuffers.find(name) == SoundBuffers.end())
	{
		// Load the sound buffer if it hasn't been loaded yet
		GetSoundBuffer(name);
	}

	// Try to find an existing sound that is not currently playing
	for (auto& sound : Sounds)
	{
		if (sound.getStatus() != sf::Sound::Playing)
		{
			// Reuse the existing sound
			sound.setBuffer(SoundBuffers[name]);
			sound.setVolume(1); // Reset volume if needed
			sound.play();
			return;
		}
	}

	// If no available sound was found, create a new one
	sf::Sound newSound;
	newSound.setBuffer(SoundBuffers[name]);
	newSound.setVolume(100.0f); // Set volume
	newSound.play();
	Sounds.push_back(std::move(newSound));
}

void ResourceManager::GetListSoundsNames()
{
	int index = 0;
	for (const auto& sound : Sounds)
	{
		std::cout << "Sound " << index << ": ";
		if (sound.getStatus() == sf::Sound::Playing)
		{
			std::cout << "Playing";
		}
		else if (sound.getStatus() == sf::Sound::Paused)
		{
			std::cout << "Paused";
		}
		else if (sound.getStatus() == sf::Sound::Stopped)
		{
			std::cout << "Stopped";
		}
		std::cout << std::endl;
		++index;
	}
}




//Stops memory error when the program is terminated
void ResourceManager::ClearTextureMap()
{
	Textures.clear();
}

void ResourceManager::ClearSoundBufferMap()
{
	SoundBuffers.clear();
}

void ResourceManager::PlayMusic(sf::Music& music)
{
	music.play();
}
