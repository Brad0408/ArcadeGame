#pragma once
#include <Object.h>


class ResourceManager : public Object
{

public:
	//Get Texture and sounds path
	static const std::string& GetTexturePath(const std::string& TextureName);
	//static const std::string& GetSoundBufferPath(const std::string& SoundName);

	//Get Texture and sounds
	static sf::Texture& GetTexture(const std::string& TextureName);
	//static sf::SoundBuffer& GetSoundBuffer(const std::string& SoundName);

	//Clears map 'Textures'
	static void ClearTextureMap();   
	//static void ClearSoundBufferMap();

	// Play music
	//static void PlayMusic(sf::Music& music);

	// Play sound
	//static void PlaySound(sf::SoundBuffer& buffer);

private:
	//A Map that has a string matched to a file path
	static const std::unordered_map<std::string, std::string> TexturePaths;
	//static const std::unordered_map<std::string, std::string> SoundBufferPaths;


	//A map that has a string matched to a texture
	static std::unordered_map<std::string, sf::Texture> Textures;
	//static std::unordered_map<std::string, sf::SoundBuffer> SoundBuffers;
	
};
