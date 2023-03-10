//
// Created by paolo on 10/01/21.
//

#ifndef MARIO_MAKER_MAPRENDERER_H
#define MARIO_MAKER_MAPRENDERER_H
#include <raylib.h>
#include "Map.h"
#include "ECS.h"
#include "Renderer.h"

class MapRenderer : public Renderer {
public:

    MapRenderer(Map *map, const char* filepath);

    void render(ECS::World* world, float delta);

    virtual ~MapRenderer();

    void renderBackground(ECS::World *world);

private:

    void drawGraphicsLayer(unsigned int** mapToRender, ECS::World* world, bool graphics);

    void renderTexture(TextureId textureId, int x, int y);

    void renderOtherEntities(ECS::World *pWorld, float d);

private:
    Map map_;
};


#endif //MARIO_MAKER_MAPRENDERER_H
