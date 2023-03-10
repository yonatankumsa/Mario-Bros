//
// Created by paolo on 09/01/21.
//
#pragma once

#include <cstdio>
#include <raylib.h>
#include "renderers/TextureRenderer.h"
#include <map>
#include <utility>
#include <iostream>
#include <unordered_set>


ECS_TYPE_IMPLEMENTATION;

struct AABBComponent {

    AABBComponent(Rectangle box) : collisionBox_(box) {}

    AABBComponent operator+(const Vector2 &offset)
    {
        return AABBComponent(Rectangle{
                collisionBox_.x + offset.x,
                collisionBox_.y + offset.y,
                collisionBox_.width,
                collisionBox_.height
        });
    }

    [[ nodiscard ]] float right() const { return collisionBox_.x + collisionBox_.width; }

    float left() const { return collisionBox_.x; }

    float top() const { return collisionBox_.y; }

    float bottom() const { return collisionBox_.y + collisionBox_.height; }

    float getCenterX() const { return collisionBox_.x + collisionBox_.width / 2.0f; }

    float getCenterY() const { return collisionBox_.y + collisionBox_.height / 2.0f; }

    void setBottom(float value) {
        collisionBox_.y = value - collisionBox_.height;
    }

    void setTop(float value) {
        collisionBox_.y = value;
    }

    void setLeft(float value) {
        collisionBox_.x = value;
    }

    void setRight(float value) {
        collisionBox_.x = value - collisionBox_.width;
    }

    void setWidth(float value) {
        if (value >= 0) {
            collisionBox_.width = value;
        }
    }

    void setHeight(float value) {
        if (value >= 0) {
            collisionBox_.height = value;
        }
    }

    Rectangle collisionBox_;
};

struct KineticComponent {

    KineticComponent() = default;

    KineticComponent(
            float speedX,
            float speedY,
            float accX = 0.0,
            float accY = 0.0)
    : speedX_(speedX), speedY_(speedY), accX_(accX), accY_(accY) {}

    float speedX_ = 0;
    float speedY_ = 0;
    float accX_ = 0;
    float accY_ = 0;
};

struct WalkComponent {

    WalkComponent() = default;

    WalkComponent(float speed) : speed(speed) {}

    float speed = -0.6f;
};

struct BrickComponent {};

struct SquareBrick {};

struct PipeComponent {};

struct QuestionBlockComponent {

    QuestionBlockComponent() = default;

    bool coin = false;
    bool superMarioMushroom = false;
    bool megaMushroom = false;
    bool flameMushroom = false;
    bool oneUpMushroom = false;
};

struct GroundComponent {};

enum PlayerState {
    STANDING,
    JUMPING,
    RUNNING,
    DRIFTING,
    DUCKING,
    INVINCIBLE,
    SHOOTING,
    SIT
};

struct PlayerComponent {

    PlayerComponent()
    {
        current_state_ = STANDING;
    };

    PlayerState current_state_;
    bool sprint = false;
    bool canShoot = true;
    int left = 0;
    int right = 0;
};

struct DeadComponent {};

struct MarioComponent {};

struct LuigiComponent {};

struct SuperComponent {};

struct MegaComponent {};

struct SuperFlameComponent {};

struct FireBulletComponent {

    FireBulletComponent(float upperBound) : upperBound_(upperBound) {}

    void setUpperBound(float newUpperBound) {
        if (newUpperBound >= 0) {
            upperBound_ = newUpperBound;
        }
    }

    float upperBound_;
};

struct GravityComponent {};

struct SolidComponent {};

struct FrozenComponent {};

struct BottomCollisionComponent {};

struct TopCollisionComponent {};

struct RightCollisionComponent {};

struct LeftCollisionComponent {};

struct BounceComponent {

    int getHeight() {
        frames--;
        return height[frames];
    }

    [[nodiscard]] bool finished() const {
        return frames == 0;
    }

    void reset() {
        frames = 13;
    }

public:
    bool hit = false;

private:
    int frames = 13;
    int height[13] = {-2, 1, 3, 2, 1, 1, 1, 0, 0, -1, -1, -4, -1};;
};

struct BreakableComponent {};

struct BouncingComponent {};

struct GrowComponent {

    GrowComponent(int frames, bool up) : frames_(frames), up_(up), n_(frames) {}

    explicit GrowComponent(int frames) : frames_(frames), n_(frames) {}

    GrowComponent() {
        n_ = frames_;
    };

    [[nodiscard]] bool finished() {
        frames_--;
        return frames_ <= 0;
    }

    void wait() {
        waitCounter_++;
        if (waitCounter_ >= n_) {
            waitCounter_ = 0;
            frames_ = n_;
            up_ = !up_;
        }
    }

    bool isGoingUp() { return up_; }

private:
    bool up_ = true;
    int frames_ = 64;
    int n_;
    int waitCounter_ = 0;
};

namespace Collectible {
    enum CollectibleType {
        NONE,
        SUPER_MARIO_MUSHROOM,
        MEGA_MUSHROOM,
        FLAME_MUSHROOM,
        ONE_UP_MUSHROOM
    };
}

struct CollectibleComponent {

    CollectibleComponent(Collectible::CollectibleType type)
    : type(type) {}

    Collectible::CollectibleType type;
};

struct AnimationComponent {
    explicit AnimationComponent(
                std::vector<TextureId> textures,
                int duration,
                bool flipH = false,
                bool flipV = false,
                bool loop = true
    ) : textures{std::move(textures)},
        duration{duration},
        counter{duration},
        flipH{flipH},
        flipV{flipV},
        loop{loop} {}

    int duration = 0;
    int counter = 0;
    bool flipH = false;
    bool flipV = false;
    bool loop = true;
    int8_t currentTexture = 0;
    std::vector<TextureId> textures;
};

struct TileTexture_
{
    Texture2D texture;
    int8_t id;
};
typedef TileTexture_* TileTexture;

struct TileComponent {};

namespace Object {
    enum Type {
        FINAL_FLAG_POLE,
        FINAL_FLAG
    };
}

struct ObjectComponent {

    ObjectComponent(Object::Type type) : type(type) {}

    Object::Type type;
};

struct WinnerFlagComponent {};

struct TextureComponent {
    explicit TextureComponent(TextureId textureId) : textureId_(textureId) {};

    ~TextureComponent()  = default;

    TextureId textureId_;

    bool flipH = false;
    bool flipV = false;

    float offSetX = 0;
    float offSetY = 0;
    int w = 0;
    int h = 0;
};

struct RotationComponent {
public:
    RotationComponent() = default;

    void increment() {
        rotation_++;
    }

    int rotation_ = 0;
};

struct LeadCameraComponent {};

enum Command {
    NONE_COMMAND,
    JUMP,
    MOVE_LEFT,
    MOVE_RIGHT,
    DUCK,
    SPRINT,
    SPECIAL,
    SHOOT
};

struct CommandComponent {

    CommandComponent(std::map<Command, int> commandsTable)
    : commandsKeyTable_(std::move(commandsTable))
    {
        createKeyCommandTable();
    }

    void setNullCommand() { currentCommand_ = NONE_COMMAND; }

    void setCurrentCommand(int key) {
        auto it = keyCommandTable_.find(key);
        if (it != keyCommandTable_.end()) {
            currentCommand_ = it->second;
            return;
        }
        currentCommand_ = NONE_COMMAND;
    }

    void createKeyCommandTable() {
        for (std::map<Command, int>::iterator i = commandsKeyTable_.begin(); i != commandsKeyTable_.end(); ++i)
            keyCommandTable_[i->second] = i->first;
    }

    void unsetCurrentCommand(int key) {
        auto it = keyCommandTable_.find(key);
        if (it != keyCommandTable_.end()) {
            if (currentCommand_ == it->second) {
                currentCommand_ = NONE_COMMAND;
            }
        }
    }

    void setNewKey(Command command, int newKey) {
        auto it = commandsKeyTable_.find(command);
        if (it != commandsKeyTable_.end()) {
            it->second = newKey;
        }

        createKeyCommandTable();
    }

    std::map<Command, int> commandsKeyTable_;
    std::map<int, Command> keyCommandTable_;
    Command currentCommand_;
};

ECS_DEFINE_TYPE(CommandComponent);

struct CameraComponent {

    CameraComponent(Vector2 target, Vector2 offset, float rotation, float zoom)
    {
        Camera2D camera2D;
        camera2D.target = target;
        camera2D.offset = offset;
        camera2D.rotation = rotation;
        camera2D.zoom = zoom;
        camera = camera2D;
    }

    Camera2D camera;
};

struct IdsMapComponent {

    explicit IdsMapComponent(int width, int height)
    :width_(width), height_(height)
    {
        idsMatrix_ = std::vector<int>(width * height);
        std::fill(idsMatrix_.begin(), idsMatrix_.end(), -1);;
    }

    int get(int x, int y) {
        if (x < 0 || y < 0) return -1;
        if (x >= width_ || y >= height_) return -1;
        return idsMatrix_.at(x + y * width_);
    }

    void set(size_t entId, int x, int y) {
        if (x < width_ && y < height_) {
            idsMatrix_.at(x + y * width_) =  entId;
        }
    }

    void clear() {
        for (int i = 0; i < width_ * height_; i++) {
            idsMatrix_.at(i) = -1;
        }
    }

    ~IdsMapComponent() {}

public:
    unsigned int width_;
    unsigned int height_;
private:
    std::vector<int> idsMatrix_;
};

struct SpacialHashMapComponent {

    explicit SpacialHashMapComponent(unsigned int width, unsigned int height)
    : width_(width), height_(height) {
        clear();
    }

    std::unordered_set<int> get(int x, int y) {
        if (x < 0 || y < 0) return {-1};
        if (x >= width_ || y >= height_) return {-1};
        return spacialHashmap_.at(x + y * width_);
    }

    void set(unsigned int x,unsigned int y, int entId) {
        if (x < width_ && y < height_) {
            if (entId != -1) {
                spacialHashmap_.at(x + y * width_).insert(entId);
                spacialHashmap_.at(x + y * width_).erase(-1);
            } else {
                std::unordered_set<int> arr;
                arr.insert(-1);
                spacialHashmap_.insert({x + y * width_ , arr});
            }
        }
    }

    void clear() {
        for (int i = 0; i < width_ * height_; i++) {
            std::unordered_set<int> arr;
            arr.insert(-1);
            spacialHashmap_.insert({i , arr});
        }
    }

public:
    unsigned int width_;
    unsigned int height_;
    std::map<int, std::unordered_set<int>> spacialHashmap_;
};

struct StaticEntitiesMapComponent {};

struct KineticEntitiesMapComponent {};

struct TimerComponent {

    TimerComponent(std::function<void(void)> callback, int time)
    : time(time), callback(std::move(callback)) {}

    int time;
    std::function<void(void)> callback;

};

namespace Enemy {
    enum Type {
        GOOMBA,
        KOOPA_TROOPA,
        RED_KOOPA_TROOPA,
        GREEN_TURTLE_SHELL,
        RED_TURTLE_SHELL,
        PIRANHA_PLANT,
        TARTOSSO
    };

    enum TartossoState {
        LIVE,
        DEAD,
        TRANSFORMING
    };
}

struct EnemyComponent {

    EnemyComponent(Enemy::Type type) : type_(type) {}

    Enemy::Type type_;
};

struct TartossoComponent {

    TartossoComponent() = default;

    TartossoComponent(Enemy::TartossoState tartossoState) : tartossoState(tartossoState) {}

    [[nodiscard]] bool dead() {
        counter++;
        if (counter > n) {
            counter = 0;
            tartossoState = Enemy::TRANSFORMING;
            return false;
        }
        return true;
    }

    [[nodiscard]] bool isTransforming() {
        counter++;
        if (counter > n / 9) {
            counter = 0;
            tartossoState = Enemy::LIVE;
            return false;
        }
        return true;
    }

    float prevVelocity = 0.0f;
    Enemy::TartossoState tartossoState = Enemy::LIVE;
    int counter = 0;
private:
    int n = 360;
};

struct TurtleShellComponent {

    TurtleShellComponent() = default;

    TurtleShellComponent(bool isMoving) : isMoving_(isMoving) {}

    bool isMoving_ = false;
};

struct DestroyDelayedComponent {

    DestroyDelayedComponent(int timer) : timer_(timer) {
        if (timer <= 0) this->timer_ = 10;
    }

    bool shouldDestroy() {
        timer_--;
        return timer_ < 0;
    }

private:
    int timer_;
};