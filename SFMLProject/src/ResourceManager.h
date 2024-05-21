#pragma once
#include <Object.h>


class ResourceManager : public Object
{

public:
	static const std::string& GetTexturePath(const std::string& TextureName);
	static sf::Texture& GetTexture(const std::string& TextureName);
	static void ClearTextureMap(); 


	static void PlaySound(const std::string& key);
	static void StopSounds();

	static void PlayMusic(const std::string& key);
	static void StopMusic();
	static void AudioCleanup();

	static void CreateSoundBuffers();
	static void CreateMusicTracks();

private:
	//Maps of paths to file paths and maps with a key matching that stored file path
	static const std::unordered_map<std::string, std::string> TexturePaths;
	static std::unordered_map<std::string, sf::Texture> Textures;

	static const std::unordered_map<std::string, std::string> SoundBufferPaths;
	static std::unordered_map<std::string, sf::SoundBuffer> soundBuffers;
	static std::vector<std::shared_ptr<sf::Sound>> activeSounds;

	static const std::unordered_map<std::string, std::string> MusicPaths;
	static std::unordered_map<std::string, std::shared_ptr<sf::Music>> musicTracks;
	static std::shared_ptr<sf::Music> currentMusic;

};
