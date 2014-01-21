#include "cri_stdafx.h"

#include "cri_build_game.h"

#include "cri_app.h"
#include "cri_camera.h"
#include "cri_game_objects.h"
#include "cri_game_scene.h"

#include <cinder/ImageIo.h>
#include <cinder/Rand.h>
#include <cinder/Vector.h>
#include <cinder/app/App.h>

#include <string>

using ci::Vec2f;
using ci::gl::Texture;
using std::string;
using std::vector;

#ifndef CHEATS
//#define CHEATS
#endif

void BuildGame( CRIApp& App )
{
    using ci::Vec2f;

    CRIGameScene* const Scene = new CRIGameScene(App, 1280 * 3, 1024 * 3);
    Scene->AddObject( *new CRIObstacle(Vec2f(1280 * 3, 50), Vec2f(1280.f * 3.f / 2.f, 25.f)) );
    Scene->AddObject( *new CRIObstacle(Vec2f(1280 * 3, 50), Vec2f(1280.f * 3.f / 2.f, 1024.f * 3.f - 25.f)) );
    Scene->AddObject( *new CRIObstacle(Vec2f(50, 1024 * 3), Vec2f(25.f, 1024.f * 3.f / 2.f)) );
    Scene->AddObject( *new CRIObstacle(Vec2f(50, 1024 * 3), Vec2f(1280.f * 3.f - 25.f, 1024.f * 3.f / 2.f)) );
    CRIPlayer* const Player = CreatePlayer(App, *Scene);
    Scene->AddObject(*Player);
    CreateEnemies(*Scene, *Player);
    
    App.SetScene(Scene);
}

CRIPlayer* CreatePlayer( CRIApp& App, CRIGameScene& Scene )
{
    using ci::app::getWindowSize;

    // @FIXME hard coded values
    const CRIMovable::SizeT Size = CRIMovable::SizeT(85.f, 85.f);
    const CRIMovable::PosT Pos = CRIMovable::PosT(440.f,
        //getWindowSize().y - 30.f);
        1500.f);
        //90.f);
    //const CRIMovable::PosT Pos = CRIMovable::PosT(getWindowSize().x / 2.f,
    //    getWindowSize().y - 30.f);
#ifdef CHEATS
    const int PlayerHealth = 10000;
#else
    //const int PlayerHealth = 100;
    const int PlayerHealth = 10000;
#endif

    CRIPlayer* const Player = new CRIPlayer(Size, Pos, PlayerHealth, App);
#ifdef CHEATS
    Player->SetSpeed(1800.f);
#else
    //Player->SetSpeed(130.f);
    Player->SetSpeed(180.f);
#endif
    const int TextureDescr = Scene.GetCamera().RegisterTexture("player.png");
    Player->SetTextureDescriptor(TextureDescr);
    //Player->SetVelocity(CRIMovable::VelT(0.f, -300.f));
    App.AddInputListener(*Player);

    return Player;
}

void CreateEnemies( CRIGameScene& Scene, CRIPlayer& Player )
{
    vector<int> Textures;
    Textures.push_back(Scene.GetCamera().RegisterTexture("enemy.png"));
    Textures.push_back(Scene.GetCamera().RegisterTexture("enemy2.png"));

	SpawnEnemies(Scene, Player, 200, Vec2f(300.f, 1900.f), 10, 5.f, Textures);
    SpawnEnemies(Scene, Player, 200, Vec2f(2000.f, 200.f), 10, 5.f, Textures);

//    SpawnEnemies(Scene, Player, 100, Vec2f(120.f, 90.f), 10, 5.f);
//    SpawnEnemies(Scene, Player, 100, Vec2f(360.f, 200.f), 10, 15.f);
//    SpawnEnemies(Scene, Player, 100, Vec2f(1700.f, 1400.f), 10, 15.f);
//    SpawnEnemies(Scene, Player, 100, Vec2f(2500.f, 2000.f), 10, 5.f);
//
//    SpawnEnemies(Scene, Player, 50, Vec2f(200.f, 1000.f), 10, 55.f);
//    SpawnEnemies(Scene, Player, 50, Vec2f(2500.f, 600.f), 10, 55.f);
//    SpawnEnemies(Scene, Player, 50, Vec2f(1500.f, 2000.f), 10, 55.f);
//
//    SpawnEnemies(Scene, Player, 10, Vec2f(1500.f, 2000.f), 10, 55.f);
//    SpawnEnemies(Scene, Player, 10, Vec2f(600.f, 1000.f), 5, 55.f);
//    SpawnEnemies(Scene, Player, 10, Vec2f(360.f, 200.f), 10, 55.f);
//    SpawnEnemies(Scene, Player, 10, Vec2f(2160.f, 150.f), 10, 55.f);
//    SpawnEnemies(Scene, Player, 10, Vec2f(2300.f, 500.f), 10, 55.f);
}

void SpawnEnemies( CRIGameScene& Scene, CRIPlayer& Player, const int Count,
    const Vec2f From, const int MaxRowLength, const float Dispersion,
    vector<int> Textures )
{
    using ci::randBool; using ci::randFloat; using ci::randInt;

    assert(Count >= 0);
    assert(MaxRowLength > 0);
    assert(Dispersion > 0.f);
    assert(!Textures.empty());

    // @FIXME hard coded values

    const CRIMovable::SizeT Size = CRIMovable::SizeT(60.f, 60.f);
    const CRIMovable::VelT VelocityBase = CRIMovable::VelT(200.f, 200.f);
    CRIMovable::PosT CurPos = From;
    
    int CurRow = 0;
    for (int i = 0; i != Count; ++i)
    {
        //const float Speed = randInt(10) < 9 ? 100 : 200;
        const float Speed = 50;
        //const float Speed = 10;
        CRIEnemy* const Enemy = new CRIEnemy(Player, Size, CurPos);
        Enemy->SetTextureDescriptor(Textures[randInt() % Textures.size()]);
        Enemy->SetSpeed(Speed);
        Scene.AddObject(*Enemy);

        CurPos.x += Size.x + randFloat(1.f, Dispersion);
        if (CurRow++ == MaxRowLength)
        {
            CurRow = 0;
            CurPos.x = From.x;
            CurPos.y += Size.y + randFloat(1.f, Dispersion);
        }
    }
}
