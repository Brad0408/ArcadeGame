#include "ResourceManager.h"

std::unordered_map<std::string, sf::Texture> ResourceManager::Textures;

std::unordered_map<std::string, sf::SoundBuffer> ResourceManager::soundBuffers;
std::vector<std::shared_ptr<sf::Sound>> ResourceManager::activeSounds;

std::unordered_map<std::string, std::shared_ptr<sf::Music>> ResourceManager::musicTracks;
std::shared_ptr<sf::Music> ResourceManager::currentMusic;
 
//Set the key and matching value to the key with the file path
const std::unordered_map<std::string, std::string> ResourceManager::TexturePaths = 
{
	{"Player", "Textures/robotronsprites.png"},
	{"Enemy", "Textures/robotronsprites.png"},
	{"Mario", "Textures/mario.jpg"},
};


const std::unordered_map<std::string, std::string> ResourceManager::SoundBufferPaths =
{
	{"Shoot", "Audio/Sounds/Shoot.wav"},
	{"Respawn", "Audio/Sounds/respawn.mp3"},
	{"LivesUp", "Audio/Sounds/liveup.wav"},
	{"GameStart", "Audio/Sounds/gameStart.mp3"},
	{"WaveUp", "Audio/Sounds/WaveUp.wav"},
	{"FamilySaved", "Audio/Sounds/familysaved.wav"},
};


const std::unordered_map<std::string, std::string> ResourceManager::MusicPaths =
{
	{"MainMenu", "Audio/Music/MainMenu.mp3"},
	{"Gameplay", "Audio/Music/gameplay.mp3"},
	{"GameOver", "Audio/Music/gameover.mp3"},

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






void ResourceManager::CreateSoundBuffers()
{
	for (const auto& pair : SoundBufferPaths)
	{
		const std::string& key = pair.first;
		const std::string& path = pair.second;

		sf::SoundBuffer buffer;
		if (buffer.loadFromFile(path)) 
		{
			soundBuffers[key] = std::move(buffer);  // Store the buffer in the map
			std::cout << "Loaded and stored sound buffer for key: " << key << std::endl;
		}
		else 
		{
			std::cerr << "Failed to load sound file: " << path << std::endl;
		}
	}
}

void ResourceManager::CreateMusicTracks()
{
	for (const auto& pair : MusicPaths) {
		const std::string& key = pair.first;
		const std::string& path = pair.second;

		auto music = std::make_shared<sf::Music>();
		if (music->openFromFile(path)) {
			musicTracks[key] = music;  // Store the music in the map
			std::cout << "Loaded and stored music for key: " << key << std::endl;
		}
		else {
			std::cerr << "Failed to load music file: " << path << std::endl;
		}
	}
}



void ResourceManager::PlaySound(const std::string& key)
{
	auto it = soundBuffers.find(key);
	if (it != soundBuffers.end())
	{
		auto sound = std::make_shared<sf::Sound>();
		sound->setBuffer(it->second);// Set the buffer to the sound
		sound->setVolume(15);
		//std::cout << "Setting buffer for sound with key: " << key << std::endl;
		sound->play();
		activeSounds.push_back(sound); // Add the sound to the active sounds

		// Remove finished sounds
		int initialSize = activeSounds.size();
		activeSounds.erase(std::remove_if(activeSounds.begin(), activeSounds.end(), [](const std::shared_ptr<sf::Sound>& s)
		{ 
			return s->getStatus() == sf::Sound::Stopped; 
		}), activeSounds.end());


		if (activeSounds.size() < initialSize)
		{
			//std::cout << "Removed finished sounds. Active sounds count: " << activeSounds.size() << std::endl;
		}
	}
	else {
		std::cout << "Sound buffer not found for key: " << key << std::endl;
	}
}

void ResourceManager::StopSounds()
{
	// Stop and clear all active sounds
	for (auto& sound : activeSounds)
	{
		sound->stop();
	}
	activeSounds.clear();
}

void ResourceManager::PlayMusic(const std::string& key)
{
	// Stop currently playing music if there is any
	if (currentMusic && currentMusic->getStatus() == sf::Music::Playing) {
		currentMusic->stop();
	}

	// Find and play the new music
	auto it = musicTracks.find(key);
	if (it != musicTracks.end()) 
	{
		currentMusic = it->second;
		std::cout << "Playing music with key: " << key << std::endl;
		currentMusic->setVolume(5);
		currentMusic->setLoop(true);
		currentMusic->play();
	}
	else {
		std::cout << "Music not found for key: " << key << std::endl;
	}
}

void ResourceManager::StopMusic()
{
	// Stop the currently playing music
	if (currentMusic && currentMusic->getStatus() == sf::Music::Playing)
	{
		currentMusic->stop();
		currentMusic = nullptr;
	}
}












//Stops memory error when the program is terminated
void ResourceManager::ClearTextureMap()
{
	Textures.clear();
}
