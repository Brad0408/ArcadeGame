#pragma once
#include <Object.h>


class ResourceManager : public Object
{

public:
	//Get Texture and sounds path
	static const std::string& GetTexturePath(const std::string& TextureName);

	//Get Texture and sounds
	static sf::Texture& GetTexture(const std::string& TextureName);

	//Clears map 'Textures'
	static void ClearTextureMap(); 



	// Play sound
	static void PlaySound(const std::string& key);
	static void StopSounds();

	static void PlayMusic(const std::string& key);
	static void StopMusic();


	static void CreateSoundBuffers();
	static void CreateMusicTracks();

private:
	static const std::unordered_map<std::string, std::string> TexturePaths;
	static std::unordered_map<std::string, sf::Texture> Textures;

	static const std::unordered_map<std::string, std::string> SoundBufferPaths;
	static std::unordered_map<std::string, sf::SoundBuffer> soundBuffers;
	static std::vector<std::shared_ptr<sf::Sound>> activeSounds;

	static const std::unordered_map<std::string, std::string> MusicPaths;
	static std::unordered_map<std::string, std::shared_ptr<sf::Music>> musicTracks;
	static std::shared_ptr<sf::Music> currentMusic;

};
