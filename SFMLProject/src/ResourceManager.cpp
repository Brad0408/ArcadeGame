#include "ResourceManager.h"


//Set the key and matching value to the key with the file path
const std::unordered_map<std::string, std::string> ResourceManager::TexturePaths = 
{
	{"Player", "Textures/robotronsprites.jpg"},
	{"Enemy", "Textures/robotronsprites.jpg"},
	{"Mario", "Textures/mario.jpg"},
};

std::unordered_map<std::string, sf::Texture> ResourceManager::Textures;





//Returns the texture path by passing in the matching key as a string
const std::string& ResourceManager::GetTexturePath(const std::string& TextureName) 
{
	//Check if the texture name exists in the map
	if (TexturePaths.find(TextureName) != TexturePaths.end()) 
	{
		std::cout << "Texture path was found of key: " << TextureName << std::endl;
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
	//Check if the texture is on the map
	if (Textures.find(TextureName) == Textures.end())
	{
		//Load the texture based of the file path
		sf::Texture texture;
		if (!texture.loadFromFile(GetTexturePath(TextureName)))
		{
			std::cout << "Texture not found: " << TextureName << std::endl;
		}
		else
		{
			//Move the created texture onto the "Textures" Map
			Textures.emplace(TextureName, std::move(texture));

			//std::cout << "Added texture: Key = " << TextureName << ", Value = " << Textures.at(TextureName).getNativeHandle() << std::endl;

		}
	}
	

	//If the texture already existed just return it
	return Textures.at(TextureName);
}


//Stops memory error when the program is terminated
void ResourceManager::ClearTextureMap()
{
	Textures.clear();
}
