#include "cri_stdafx.h"

#include "cri_build_game.h"

#include "cri_app.h"
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
    
    App.SetScene(Scene);
}

CRIPlayer* CreatePlayer( CRIApp& App )
{
    using ci::app::getWindowSize;

    // @FIXME hard coded values
    const CRIMovable::SizeT Size = CRIMovable::SizeT(30.f, 30.f);
    const CRIMovable::PosT Pos = CRIMovable::PosT(getWindowSize().x / 2.f,
        getWindowSize().y - 30.f);
    CRIPlayer* const Player = new CRIPlayer(Size, Pos);
    Player->SetSpeed(300.f);
    App.AddInputListener(*Player);

    return Player;
}