# Mario Maker

As a fan of Mario Maker and a lover of coding, I took on the challenge of creating my own version of the game using C++. Unlike using a game engine, coding with C++ allowed me to have direct control over the game's code, resulting in a highly optimized and customized experience. My creation offers a unique twist on the classic Mario gameplay, delivering a personalized experience tailored to my vision. Building this from the ground up was a significant challenge, but it was worth it to bring my own take on the world of Mario to life. 

![img1](https://i.imgur.com/IP2vLyg.png)

The main architectural/design pattern used to build the game is the [ECS](https://en.wikipedia.org/wiki/Entity_component_system).

 # Libraries used

- [Raylib](https://github.com/raysan5/raylib) -  used as a framework to build the game
- [Gtest](https://github.com/google/googletest) - used for testing (is install when running the `cmake`)
- [tmxlite](https://github.com/fallahn/tmxlite) - for parsing the txm file used for the map
- [rededev/ECS](https://github.com/redxdev/ECS) - an header only entity component system library (already included)    

