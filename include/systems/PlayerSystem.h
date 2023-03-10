//
// Created by paolo on 14/01/21.
//

#ifndef MARIO_MAKER_PLAYERSYSTEM_H
#define MARIO_MAKER_PLAYERSYSTEM_H
#include "ECS.h"
#include "Events.h"
#include <raylib.h>

using namespace ECS;

class PlayerSystem :
        public EntitySystem,
        public EventSubscriber<EnemyCollisionEvent>,
        public EventSubscriber<PLayerCollectableCollisionEvent>{
public:
    PlayerSystem();

    void configure(World* world) override;

    void unconfigure(World* world) override;

    void tick(World* world, float delta) override;

    void setAnimation(Entity* playerEntity, PlayerState state);

    void receive(World* world, const EnemyCollisionEvent& enemyCollisionEvent) override;

    void receive(World* world, const PLayerCollectableCollisionEvent& pLayerCollectableCollisionEvent) override;
private:

    void eatMushroom(Entity *pEntity, Collectible::CollectibleType type);

    void handleFrozenTransform(Entity* entity);

    void setRightAnimation(Entity* player);

    TextureId getRightTransitionAnimation(Entity* entity, Collectible::CollectibleType mushroom, bool isMario);

    bool isSuperTexture(TextureId textureId);

    bool isFlameTexture(TextureId textureId);

    bool isMegaTexture(TextureId textureId);

    void movePlayer(Entity* player, ComponentHandle<PlayerComponent> playerComponent, Command command);

    void shrink(Entity *player);

    void createFireBullet(World *world, Entity *entity);
};


#endif //MARIO_MAKER_PLAYERSYSTEM_H
