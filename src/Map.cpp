//
// Created by paolo on 08/01/21.
//

#include <cmath>
#include <iostream>
#include <Constants.h>
#include "../include/Map.h"

Map::Map(std::string filename)
:name(filename)
{
    loaded_ = false;
    width_ = 0;
    height_ = 0;
}

Map::~Map() {
    for (int i = 0; i < width_; i++)
    {
        delete[] graphicsLayer_[i];
        delete[] backgroundLayer_[i];
    }
}

void Map::loadMap(ECS::World* world) {
    tmx::Map map;
    if(map.load(name))
    {
        // Get Map sizes
        loadMapBasicInfo(map.getTileCount());

        // Load properties
        loadProperties(map.getProperties());

        // Load layers
        std::set<unsigned int> usedTiles = loadLayers(map.getLayers(), world);

        // Load tileset
        loadMapTiles(const_cast<std::vector<tmx::Tileset> &>(map.getTilesets()), usedTiles);
    } else {
        throw "Cannot load map from " + name;
    }

    this->loaded_ = true;
}

void Map::loadMapBasicInfo(const tmx::Vector2u &orientation) {
    width_ = orientation.x;
    height_ = orientation.y;

    graphicsLayer_ = new unsigned int*[width_];
    backgroundLayer_ = new unsigned int*[width_];
    for (int i = 0; i < width_; i++)
    {
        graphicsLayer_[i] = new unsigned int[height_];
        backgroundLayer_[i] = new unsigned int[height_];
    }
}

void Map::loadProperties(const std::vector<tmx::Property> properties) {
    Vector2 marioSpawn;
    for (const tmx::Property& property : properties)
    {
        int value = std::stoi(property.getStringValue());
        if (property.getName() == "spawn_mario_x") {
            marioSpawn.x = value;

        } else if (property.getName() == "spawn_mario_y") {
            marioSpawn.y = value;
        }
    }

    spawnPositionP1_ = marioSpawn;
    spawnPostionP2_ = Vector2{marioSpawn.x - 2, marioSpawn.y};
}

std::set<unsigned int> Map::loadLayers(const std::vector<tmx::Layer::Ptr>& layers, ECS::World* world) {
    std::set<unsigned int> usedTilesSet;

    for (const auto& layer : layers)
    {
        if(layer->getType() == tmx::Layer::Type::Object)
        {
            std::string layerName = layer->getName();
            const auto& objectLayer = layer->getLayerAs<tmx::ObjectGroup>();
            const auto& objects = objectLayer.getObjects();

            for (const auto& object : objects)
            {
                tmx::FloatRect AABB = object.getAABB();
                ECS::Entity* ent = world->create();
                // Adjust coordinates
                float x = std::round((AABB.left * 2) / 32) * 32;
                float y = std::round((AABB.top * 2) / 32) * 32;
                float width, height;
                if (std::round(AABB.width) <= TILE_SIZE + 2) {
                    width = GAME_TILE_SIZE;
                } else {
                    int n = (int) std::round(AABB.width / 16);
                    width = GAME_TILE_SIZE * n;
                }

                if (std::round(AABB.width) <= TILE_SIZE + 2) {
                    height = GAME_TILE_SIZE;
                } else {
                    int n = (int) std::round(AABB.height / 16);
                    height = GAME_TILE_SIZE * n;
                }

                ent->assign<AABBComponent>(Rectangle{x , y, width, height});
                ent->assign<SolidComponent>();
                ent->assign<TileComponent>();
                if (layerName == "pipes") ent->assign<PipeComponent>();
                else if (layerName == "bricks") ent->assign<BrickComponent>();
                else if (layerName == "ground") ent->assign<GroundComponent>();
                else if (layerName == "coins") ent->assign<CoinBoxComponent>();
            }
        }
        else if(layer->getType() == tmx::Layer::Type::Tile)
        {
            unsigned int** mapToLoad;
            if (layer->getName() == "background")
            {
                mapToLoad = backgroundLayer_;
            } else if (layer->getName() == "graphics")
            {
                mapToLoad = graphicsLayer_;
            }

            const auto& tileLayer = layer->getLayerAs<tmx::TileLayer>();
            const auto & tiles = tileLayer.getTiles();
            for (int j = 0; j < height_; j++)
            {
                for (int i = 0; i < width_; i++)
                {
                    unsigned int value = tiles.at(i + j * width_).ID;
                    mapToLoad[i][j] = value;
                    usedTilesSet.insert(value);
                }
            }
        }
    }

    return usedTilesSet;
}

Texture2D Map::getTexture(const std::string& path, tmx::Vector2u tilePosition, tmx::Vector2u tileSize) {
    Image image = LoadImage(path.c_str());
    ImageCrop(&image, (Rectangle){
        static_cast<float>(tilePosition.x - 16.f),
        static_cast<float>(tilePosition.y),
        static_cast<float>(tileSize.x),
        static_cast<float>(tileSize.y)});
    ImageResize(&image, 32, 32);

    Texture2D texture2D = LoadTextureFromImage(image);
    UnloadImage(image);

    return texture2D;
}

void Map::loadMapTiles(std::vector<tmx::Tileset> &tileset, const std::set<unsigned int>& usedTiles) {
    const auto& tiles = tileset.at(0).getTiles();
    for(const auto& tile : tiles)
    {
        if (usedTiles.count(tile.ID))
        {
            Texture2D texture2D = getTexture(tile.imagePath, tile.imagePosition, tile.imageSize);
            TileTexture pTileTexture = static_cast<TileTexture>(malloc(sizeof(TileTexture)));
            pTileTexture->texture = texture2D;
            pTileTexture->id = tile.ID;
            mapTextureTable_.insert(std::make_pair(tile.ID, pTileTexture));
        }
    }
}

Texture2D Map::getTexture(unsigned int id)
{
    auto it = mapTextureTable_.find(id);
    if (it != mapTextureTable_.end()){
        return it->second->texture;
    } else {
        throw "Texture not found";
    }
}

void Map::unloadTextures()
{
    for (const auto &p : mapTextureTable_)
    {
        UnloadTexture(p.second->texture);
    }
}

int Map::getHeight() const {
    return height_;
}

int Map::getWidth() const {
    return width_;
}

const Vector2 &Map::getSpawnPositionP1() const {
    return spawnPositionP1_;
}

const Vector2 &Map::getSpawnPositionP2() const {
    return spawnPostionP2_;
}

unsigned int **Map::getGraphicsLayer() const {
    return graphicsLayer_;
}

unsigned int **Map::getBackgroundLayer() const {
    return backgroundLayer_;
}

const std::map<unsigned int, TileTexture> &Map::getTextureTable() const {
    return mapTextureTable_;
}

int Map::getPixelHeight() const {
    return height_ * 32;
}

int Map::getPixelWidth() const {
    return width_ * 32;
}
