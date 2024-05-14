#pragma once
#include <Object.h>


class ResourceManager : public Object
{

public:
	//Get Texture and sounds path
	static const std::string& GetTexturePath(const std::string& TextureName);
	static const std::string& GetSoundBufferPath(const std::string& SoundName);

	//Get Texture and sounds
	static sf::Texture& GetTexture(const std::string& TextureName);
	static sf::SoundBuffer& GetSoundBuffer(const std::string& SoundName);

	//Clears map 'Textures'
	static void ClearTextureMap();   
	static void ClearSoundBufferMap();

	// Play music
	static void PlayMusic(sf::Music& music);

	// Play sound
	static void PlaySound(const std::string& name);

	static void GetListSoundsNames();



private:
	static const std::unordered_map<std::string, std::string> TexturePaths;
	static std::unordered_map<std::string, sf::Texture> Textures;

	static const std::unordered_map<std::string, std::string> SoundBufferPaths;
	static std::unordered_map<std::string, sf::SoundBuffer> SoundBuffers;
	static std::list<sf::Sound> Sounds;
	

};
