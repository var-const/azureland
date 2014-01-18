#include "cri_stdafx.h"

#include "cri_build_game.h"

#include "cri_app.h"
#include "cri_enemy.h"
#include "cri_game_scene.h"
#include "cri_player.h"

#include <cinder/Vector.h>

void BuildGame( CRIApp& App )
{
    using ci::Vec2f;
    using ci::app::getWindowSize;

    CRIGameScene* const Scene = new CRIGameScene(1280 * 3, 1024 * 3);
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
    const CRIMovable::PosT Pos = CRIMovable::PosT(400.f,
        getWindowSize().y - 30.f);
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
    const float MaxHorizOffset = getWindowWidth() - Size.x;
    
    //for (int i = 0; i != 1000; ++i)
    for (int i = 0; i != 100; ++i)
    //for (int i = 0; i != 10; ++i)
    //for (int i = 0; i != 2000; ++i)
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
