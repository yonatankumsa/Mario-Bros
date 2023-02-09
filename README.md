# Mario Maker

A clone of mario maker made with no game engine, just with C++.

![img1](https://i.imgur.com/IP2vLyg.png)

The main architectural/design pattern used to build the game is the [ECS](https://en.wikipedia.org/wiki/Entity_component_system).

 # Libraries used

- [Raylib](https://github.com/raysan5/raylib) -  used as a framework to build the game
- [Gtest](https://github.com/google/googletest) - used for testing (is install when running the `cmake`)
- [tmxlite](https://github.com/fallahn/tmxlite) - for parsing the txm file used for the map
- [rededev/ECS](https://github.com/redxdev/ECS) - an header only entity component system library (already included)    

