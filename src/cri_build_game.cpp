#include "cri_stdafx.h"

#include "cri_build_game.h"

#include "cri_app.h"
#include "cri_game_objects.h"
#include "cri_game_scene.h"

#include <cinder/Vector.h>

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
    const CRIMovable::PosT Pos = CRIMovable::PosT(40.f,
        //getWindowSize().y - 30.f);
        900.f);
        //90.f);
    //const CRIMovable::PosT Pos = CRIMovable::PosT(getWindowSize().x / 2.f,
    //    getWindowSize().y - 30.f);
    CRIPlayer* const Player = new CRIPlayer(Size, Pos);
    Player->SetSpeed(400.f);
    //Player->SetVelocity(CRIMovable::VelT(0.f, -300.f));
    App.AddInputListener(*Player);

    return Player;
}

void CreateEnemies( CRIGameScene& Scene, CRIPlayer& Player )
{
    using ci::app::getWindowWidth;

    // @FIXME hard coded values

    const CRIMovable::SizeT Size = CRIMovable::SizeT(20.f, 20.f);
    const CRIMovable::PosT StartPos = CRIMovable::PosT(90.f, 100.f);
    const CRIMovable::VelT VelocityBase = CRIMovable::VelT(200.f, 200.f);
    CRIMovable::PosT CurPos = StartPos;
    const float MaxHorizOffset = getWindowWidth() * 2 - Size.x;
    
    //for (int i = 0; i != 3; ++i)
    for (int i = 0; i != 300; ++i)
    //for (int i = 0; i != 2500; ++i)
    //for (int i = 0; i != 100; ++i)
    //for (int i = 0; i != 10; ++i)
    //for (int i = 0; i != 2000; ++i)
    //for (int i = 0; i != 30; ++i)
    //for (int i = 0; i != 30; ++i)
    {
        CRIEnemy* const Enemy = new CRIEnemy(Player, Size, CurPos);
        Scene.AddObject(*Enemy);

        CurPos.x += Size.x + 1.f;
        if (CurPos.x >= MaxHorizOffset)
        {
            CurPos.x = StartPos.x;
            CurPos.y += Size.y + 1.f;
        }
    }
}
