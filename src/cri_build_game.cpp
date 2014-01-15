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

    CRIGameScene* const Scene = new CRIGameScene();
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
    const CRIMovable::PosT StartPos = CRIMovable::PosT(990.f, 100.f);
    const CRIMovable::VelT VelocityBase = CRIMovable::VelT(200.f, 200.f);
    CRIMovable::PosT CurPos = StartPos;
    const float MaxHorizOffset = getWindowWidth() - StartPos.x;
    
    for (int i = 0; i != 20; ++i)
    {
        CRIEnemy* const Enemy = new CRIEnemy(Player, Size, CurPos);
        // Possible rand values: -1, 0, 1, so they move left/stand still/
        // move right
        const float VelX = VelocityBase.x * static_cast<float>(rand() % 3 - 1);
        // Same for y axis
        const float VelY = VelocityBase.y * static_cast<float>(rand() % 3 - 1);
        Enemy->SetVelocity(CRIMovable::VelT(VelX, VelY));
        //Enemy->SetVelocity(CRIMovable::VelT(-200.f, 0.f));
        Scene.AddObject(*Enemy);

        CurPos.x += Size.x + 10.f;
        if (CurPos.x >= MaxHorizOffset)
        {
            CurPos.x = StartPos.x;
            CurPos.y -= Size.y + 10.f;
        }
    }
}
