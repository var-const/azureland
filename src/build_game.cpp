#include "stdafx.h"

#include "build_game.h"

#include "app.h"
#include "camera.h"
#include "game_objects.h"
#include "game_scene.h"
#include "projectile.h"
#include "weapons/cri_forcefield.h"

#include <cinder/ImageIo.h>
#include <cinder/Rand.h>
#include <cinder/Vector.h>
#include <cinder/app/App.h>

#include <memory>
#include <string>

using ci::Vec2f;
using ci::gl::Texture;
using std::string;
using std::vector;

#ifndef CHEATS
//#define CHEATS
#endif

void CreateObstacle(GameScene& Scene, const float LeftUpperTileX,
    const float LeftUpperTileY, const float TileWidth, const float TileHeight)
{
    static const float TileSize = 51.f;
    const Vec2f Size = Vec2f(TileWidth, TileHeight) * TileSize;
    const Vec2f Pos = Vec2f(LeftUpperTileX, LeftUpperTileY) * TileSize;
    Scene.AddObject(
        std::unique_ptr<GameObject>(new Obstacle(Size, Pos + Size / 2.f)));
}

void BuildGame(App& App)
{
    using ci::Vec2f;

    std::unique_ptr<GameScene> Scene = new GameScene(App, 1280 * 3, 1024 * 3);

    Scene->AddObject(std::unique_ptr<GameObject>(
        new Obstacle(Vec2f(1280 * 3, 50), Vec2f(1280.f * 3.f / 2.f, 25.f))));
    Scene->AddObject(std::unique_ptr<GameObject>(new Obstacle(
        Vec2f(1280 * 3, 50), Vec2f(1280.f * 3.f / 2.f, 1024.f * 3.f - 25.f))));
    Scene->AddObject(std::unique_ptr<GameObject>(
        new Obstacle(Vec2f(50, 1024 * 3), Vec2f(25.f, 1024.f * 3.f / 2.f))));
    Scene->AddObject(std::unique_ptr<GameObject>(new Obstacle(
        Vec2f(50, 1024 * 3), Vec2f(1280.f * 3.f - 25.f, 1024.f * 3.f / 2.f))));

    // Centurion
    CreateObstacle(*Scene, 15.627f, 16.875f, 11.408f, 12.656f);
    // Pipes
    CreateObstacle(*Scene, 41.355f, 0.238f, 1.485f, 27.510f);
    // Bridge 1
    CreateObstacle(*Scene, 26.560f, 27.748f, 17.409f, 1.010f);
    // Bridge 2
    CreateObstacle(*Scene, 16.043f, 37.314f, 27.926f, 0.891f);
    // Police robot
    CreateObstacle(*Scene, 66.013f, 38.206f, 14.023f, 8.853f);
    // Shaft
    CreateObstacle(*Scene, 16.043f, 37.908f, 5.229f, 7.368f);

    // Ship 1
    CreateObstacle(*Scene, 42.954f, 0.105f, 6.039f, 7.869f);
    // Ship 2
    CreateObstacle(*Scene, 49.046f, 0.f, 6.196f, 10.092f);
    // Ship 3
    CreateObstacle(*Scene, 55.268f, 0.f, 4.209f, 9.856f);
    // Ship 4
    CreateObstacle(*Scene, 59.529f, 0.f, 4.183f, 8.680f);
    // Ship 5
    CreateObstacle(*Scene, 63.712f, 0.f, 4.157f, 6.954f);
    // Ship 6
    CreateObstacle(*Scene, 67.895f, 0.f, 3.320f, 5.595f);
    // Ship 7
    CreateObstacle(*Scene, 71.294f, 0.f, 3.556f, 4.052f);

    // Stairway
    CreateObstacle(*Scene, 43.969f, 34.878f, 10.042f, 4.278f);
    CreateObstacle(*Scene, 58.405f, 34.797f, 21.752f, 3.791f);
    CreateObstacle(*Scene, 49.359f, 43.268f, 4.758f, 4.654f);
    CreateObstacle(*Scene, 58.118f, 43.059f, 4.654f, 5.020f);

    // Pomps
    CreateObstacle(*Scene, 36.549f, 0.745f, 4.745f, 3.216f);
    CreateObstacle(*Scene, 36.471f, 5.647f, 4.824f, 3.725f);
    CreateObstacle(*Scene, 36.471f, 10.902f, 4.824f, 3.725f);
    CreateObstacle(*Scene, 36.471f, 15.765f, 4.824f, 3.725f);
    CreateObstacle(*Scene, 36.471f, 20.863f, 4.824f, 3.725f);

    // Crates near ship
    CreateObstacle(*Scene, 60.941f, 20.980f, 7.843f, 4.039f);
    CreateObstacle(*Scene, 61.176f, 28.157f, 7.529f, 3.961f);

    // Far crates
    CreateObstacle(*Scene, 26.039f, 38.078f, 7.725f, 3.882f);
    CreateObstacle(*Scene, 34.039f, 38.118f, 7.804f, 3.882f);

    Scene->AddObject(CreatePlayer(App, *Scene));

    CreateEnemies(*Scene, *Player);

    const int ProjectileTexture =
        Scene->GetCamera().RegisterTexture("energy.png");
    Projectile::TextureDescr = ProjectileTexture;
    const int FieldTexture = Scene->GetCamera().RegisterTexture("field.png");
    Forcefield::TextureDescr = FieldTexture;
    const int HealthTexture = Scene->GetCamera().RegisterTexture("health.png");
    HealthPickup::TextureDescr = HealthTexture;

    App.SetScene(std::move(Scene));
}

std::unique_ptr<Player> CreatePlayer(App& App, GameScene& Scene)
{
    using ci::app::getWindowSize;

    // @FIXME hard coded values
    const Movable::SizeT Size = Movable::SizeT(85.f, 85.f);
    const Movable::PosT Pos = Movable::PosT(440.f,
        // getWindowSize().y - 30.f);
        1500.f);
// 90.f);
// const Movable::PosT Pos = Movable::PosT(getWindowSize().x / 2.f,
//    getWindowSize().y - 30.f);
#ifdef CHEATS
    const int PlayerHealth = 1000000;
#else
    const int PlayerHealth = 100;
// const int PlayerHealth = 10000;
#endif

    std::unique_ptr<Player> Player = new Player(Size, Pos, PlayerHealth, App);
#ifdef CHEATS
    Player->SetSpeed(1800.f);
#else
    // Player->SetSpeed(130.f);
    Player->SetSpeed(180.f);
#endif
    const int TextureDescr = Scene.GetCamera().RegisterTexture("player.png");
    Player->SetTextureDescriptor(TextureDescr);
    // Player->SetVelocity(Movable::VelT(0.f, -300.f));
    App.AddInputListener(*Player);

    return Player;
}

void CreateEnemies(GameScene& Scene, Player& Player)
{
    vector<int> Textures;
    Textures.push_back(Scene.GetCamera().RegisterTexture("enemy.png"));
    Textures.push_back(Scene.GetCamera().RegisterTexture("enemy2.png"));

    // Bottom
    SpawnEnemies(Scene, Player, 600, Vec2f(60.f, 2500.f), 70, 5.f, Textures);
    // Top
    SpawnEnemies(Scene, Player, 230, Vec2f(60.f, 60.f), 30, 5.f, Textures);
    // Start screen 1
    SpawnEnemies(Scene, Player, 50, Vec2f(60.f, 920.f), 10, 5.f, Textures);
    // Start screen 2
    SpawnEnemies(Scene, Player, 50, Vec2f(60.f, 1830.f), 10, 5.f, Textures);
    // Bridge
    SpawnEnemies(Scene, Player, 20, Vec2f(1530.f, 1530.f), 10, 5.f, Textures);
    // Near the ship
    SpawnEnemies(Scene, Player, 50, Vec2f(2240.f, 610.f), 25, 5.f, Textures);
}

void SpawnEnemies(GameScene& Scene, Player& Player, const int Count,
    const Vec2f From, const int MaxRowLength, const float Dispersion,
    const vector<int>& Textures)
{
    using ci::randBool;
    using ci::randFloat;
    using ci::randInt;

    assert(Count >= 0);
    assert(MaxRowLength > 0);
    assert(Dispersion > 0.f);
    assert(!Textures.empty());

    // @FIXME hard coded values

    const Movable::SizeT Size = Movable::SizeT(60.f, 60.f);
    const Movable::VelT VelocityBase = Movable::VelT(200.f, 200.f);
    Movable::PosT CurPos = From;

    int CurRow = 0;
    for (int i = 0; i != Count; ++i) {
        // const float Speed = randInt(10) < 9 ? 100 : 200;
        const float Speed = 50;
        // const float Speed = 10;
        std::unique_ptr<Enemy> const Enemy = new Enemy(Player, Size, CurPos);
        Enemy->SetTextureDescriptor(Textures[randInt() % Textures.size()]);
        Enemy->SetSpeed(Speed);
        Scene.AddObject(std::move(Enemy));

        CurPos.x += Size.x + randFloat(1.f, Dispersion);
        if (CurRow++ == MaxRowLength) {
            CurRow = 0;
            CurPos.x = From.x;
            CurPos.y += Size.y + randFloat(1.f, Dispersion);
        }
    }
}
