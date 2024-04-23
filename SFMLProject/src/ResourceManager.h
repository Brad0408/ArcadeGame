#pragma once
#include <Object.h>


class ResourceManager : public Object
{

public:
	//Get Texture path
	static const std::string& GetTexturePath(const std::string& TextureName);

	//Get Texture
	static sf::Texture& GetTexture(const std::string& TextureName);

	//Clears map 'Textures'
	static void ClearTextureMap();

private:
	//A Map that has a string matched to a file path
	static const std::unordered_map<std::string, std::string> TexturePaths;

	//A map that has a string matched to a texture
	static std::unordered_map<std::string, sf::Texture> Textures;

};
