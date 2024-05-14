#include "ResourceManager.h"

std::unordered_map<std::string, sf::Texture> ResourceManager::Textures;
std::unordered_map<std::string, sf::SoundBuffer> ResourceManager::SoundBuffers;

 
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
		std::cout << "Sound buffer path not found" << std::endl;
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

void ResourceManager::PlaySound(sf::SoundBuffer& buffer)
{
	//std::cout << "playerosinbdhuavdlbsaidiasjasd" << std::endl;

	// Create an sf::Sound instance
	sf::Sound sound;

	// Set the sound buffer
	sound.setBuffer(buffer);

	// Play the sound
	sound.play();
}
