#include "cri_stdafx.h"

#include "cri_build_game.h"

#include "cri_app.h"
#include "cri_game_objects.h"
#include "cri_game_scene.h"

#include <cinder/Rand.h>
#include <cinder/Vector.h>

using ci::Vec2f;

#ifndef CHEATS
//#define CHEATS
#endif

void BuildGame( CRIApp& App )
{
    using ci::Vec2f;
    using ci::app::getWindowSize;

    CRIGameScene* const Scene = new CRIGameScene(1280 * 3, 1024 * 3);
    //CreateSceneBounds(*Scene);
    Scene->AddObject( *new CRIObstacle(Vec2f(1280 * 3, 50), Vec2f(1280.f * 3.f / 2.f, 25.f)) );
    Scene->AddObject( *new CRIObstacle(Vec2f(1280 * 3, 50), Vec2f(1280.f * 3.f / 2.f, 1024.f * 3.f - 25.f)) );
    Scene->AddObject( *new CRIObstacle(Vec2f(50, 1024 * 3), Vec2f(25.f, 1024.f * 3.f / 2.f)) );
    Scene->AddObject( *new CRIObstacle(Vec2f(50, 1024 * 3), Vec2f(1280.f * 3.f - 25.f, 1024.f * 3.f / 2.f)) );
    CRIPlayer* const Player = CreatePlayer(App);
    Scene->AddObject(*Player);
    CreateEnemies(*Scene, *Player);
    
    App.SetScene(Scene);
}

CRIPlayer* CreatePlayer( CRIApp& App )
{
    using ci::app::getWindowSize;

    // @FIXME hard coded values
    const CRIMovable::SizeT Size = CRIMovable::SizeT(30.f, 30.f);
    const CRIMovable::PosT Pos = CRIMovable::PosT(440.f,
        //getWindowSize().y - 30.f);
        1500.f);
        //90.f);
    //const CRIMovable::PosT Pos = CRIMovable::PosT(getWindowSize().x / 2.f,
    //    getWindowSize().y - 30.f);
#ifdef CHEATS
    const int PlayerHealth = 10000;
#else
    const int PlayerHealth = 100;
#endif

    CRIPlayer* const Player = new CRIPlayer(Size, Pos, PlayerHealth);
#ifdef CHEATS
    Player->SetSpeed(1800.f);
#else
    //Player->SetSpeed(130.f);
    Player->SetSpeed(180.f);
#endif
    //Player->SetVelocity(CRIMovable::VelT(0.f, -300.f));
    App.AddInputListener(*Player);

    return Player;
}

void CreateEnemies( CRIGameScene& Scene, CRIPlayer& Player )
{
    SpawnEnemies(Scene, Player, 200, Vec2f(300.f, 2400.f), 10, 5.f);
    SpawnEnemies(Scene, Player, 200, Vec2f(2000.f, 200.f), 10, 5.f);

    SpawnEnemies(Scene, Player, 100, Vec2f(120.f, 90.f), 10, 5.f);
    SpawnEnemies(Scene, Player, 100, Vec2f(360.f, 200.f), 10, 15.f);
    SpawnEnemies(Scene, Player, 100, Vec2f(1700.f, 1400.f), 10, 15.f);
    SpawnEnemies(Scene, Player, 100, Vec2f(2700.f, 2400.f), 10, 5.f);

    SpawnEnemies(Scene, Player, 50, Vec2f(200.f, 1000.f), 10, 55.f);
    SpawnEnemies(Scene, Player, 50, Vec2f(2800.f, 600.f), 10, 55.f);
    SpawnEnemies(Scene, Player, 50, Vec2f(1500.f, 2000.f), 10, 55.f);

    SpawnEnemies(Scene, Player, 10, Vec2f(1500.f, 2000.f), 10, 55.f);
    SpawnEnemies(Scene, Player, 10, Vec2f(600.f, 1000.f), 5, 55.f);
    SpawnEnemies(Scene, Player, 10, Vec2f(360.f, 200.f), 10, 55.f);
    SpawnEnemies(Scene, Player, 10, Vec2f(2860.f, 150.f), 10, 55.f);
    SpawnEnemies(Scene, Player, 10, Vec2f(2300.f, 500.f), 10, 55.f);
}

void SpawnEnemies( CRIGameScene& Scene, CRIPlayer& Player, const int Count,
    const Vec2f From, const int MaxRowLength, const float Dispersion )
{
    using ci::randFloat; using ci::randInt;

    assert(Count >= 0);
    assert(MaxRowLength > 0);
    assert(Dispersion > 0.f);

    // @FIXME hard coded values

    const CRIMovable::SizeT Size = CRIMovable::SizeT(20.f, 20.f);
    const CRIMovable::VelT VelocityBase = CRIMovable::VelT(200.f, 200.f);
    CRIMovable::PosT CurPos = From;
    
    int CurRow = 0;
    for (int i = 0; i != Count; ++i)
    //for (int i = 0; i != 1000; ++i)
    //for (int i = 0; i != 300; ++i)
    //for (int i = 0; i != 2500; ++i)
    //for (int i = 0; i != 100; ++i)
    //for (int i = 0; i != 10; ++i)
    //for (int i = 0; i != 2000; ++i)
    //for (int i = 0; i != 30; ++i)
    //for (int i = 0; i != 30; ++i)
    {
        const float Speed = randInt(10) < 9 ? 100 : 200;
        CRIEnemy* const Enemy = new CRIEnemy(Player, Size, CurPos);
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
