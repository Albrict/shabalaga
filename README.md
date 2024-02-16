# Shabalaga
Shabalaga is a simple top down shooter in space. Game is playable, but lacks some features and game balance.
The goal of the project was to implement a game in data-oriented way using ECS system.
I may add some new features, but currently I'm done with this project.
Created by using [raylib](https://github.com/raysan5/raylib), [raylib-aseprite](https://github.com/RobLoach/raylib-aseprite), [raygui](https://github.com/raysan5/raygui) and [entt](https://github.com/skypjack/entt).

To summarize what I learned and achieved:
- How to use ECS system via entt library
- How to design a program in data-oriented way
- How to use sound
- How to work with ImGUI UI
- Implemented settings menu
- Implemented fade effect
- Implemented more complex collision system with callbacks
- Implemented sprite animation system with key-frames callbacks
- Implemented hitbox system
- Implemented simple store system
  
## How to compile
### Linux
To compile this project on Linux you need simply clone this repo:

`git clone https://github.com/Albrict/shabalaga.git`

Then

`cd shabalaga && mkdir build`

And then compile it(You can compile it in DEBUG mode, but be aware of that this project uses sanitazers, so compilation will take some time in DEBUG mode)

`cd build && cmake .. -DCMAKE_BUILD_TYPE=RELEASE`

After that copy **assets** directory into build direcory and also create **saves** direcory

`cp -rf ../assets . && mkdir saves`

### Windows
I didn't compile this project on Windows, but I think steps are the same(clone, compile, create some directories). But I have Windows build compiled by MingW that works perfectly fine even on Linux using Wine.

## Game demonstration:
Footage is compressed, so fps is low, but game have stable 60 fps.

https://github.com/Albrict/shabalaga/assets/65279613/b25cd92c-413f-4cca-9a39-cdc59987b5e6

## Credits
Thanks to FoozleCC for space shooter assets:
https://foozlecc.itch.io/

Thanks to Screaming Brain Studios for some nice backgrounds:
https://screamingbrainstudios.itch.io/

Thanks to Juhani Junkala for UI and game sounds:
https://opengameart.org/content/512-sound-effects-8-bit-style

Thanks to BDragon1727 for some explosions and UI sprites:
https://bdragon1727.itch.io/
