# Robotron 2084 Recreation

For code go to (SFMLProject -> src) or (https://tinyurl.com/3r4nevhp)

[GameManager.cpp](https://github.com/Brad0408/ArcadeGame/blob/main/SFMLProject/src/GameManager.cpp) is a large part of this project

Recreating the arcade game "Robotron 2084" (https://youtu.be/l800GL6NQPY?t=205) through the SFML framework

This project is aiming to recreating the game but through a modern day ECS structure

Also using C++ 20 and its 'concepts' to create the components that go into my ECS structure and learnig different pointer types to deal with memory managment


## Playable Build

Playable Build Here - https://1drv.ms/u/s!AibVxcIqqiJcit5eReGAsPVRnEHH0g?e=tztyKl

Once downloaded extract from zip and run "SFMLProject" to play the game.


## Cloning + Compile

1) Clone of the main branch
2) Double click the .bat file to generate project files
3) Open the projec with the .sln file in the root directory

Now there are additional steps to get the audio working
1) Go to SFMLProject -> vendor -> SFML-2.6.0 -> bin
2) Copy the 'openal32' file from this location
3) Go back to the root and go to bin -> Debug-windows-x86_64 or Release (depending on what mode you have it on in the project) -> SFMLProject -> Copy the 'openal32' file next to the executable
4) The game will now run in visual studio


## Gameplay

<ins> Here is some gameplay below ! </ins>

[![Robotron](https://i.ytimg.com/vi/adypIm5LvcQ/maxresdefault.jpg?sqp=-oaymwEmCIAKENAF8quKqQMa8AEB-AH-CYAC0AWKAgwIABABGH8gEygTMA8=&amp;rs=AOn4CLBGf-b7ap22g0jiwROJa1cqly0JDQ)](https://youtu.be/adypIm5LvcQ)
