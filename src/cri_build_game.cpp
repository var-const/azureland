#include "cri_stdafx.h"

#include "cri_build_game.h"

#include "cri_app.h"
#include "cri_game_scene.h"

#include <cinder/Vector.h>

void BuildGame( CRIApp& app )
{
    using ci::Vec2f;
    using ci::app::getWindowSize;

    CRIGameScene* const scene = new CRIGameScene();

    const Vec2f screen_size = getWindowSize();
    const Vec2f screen_center = screen_size / 2.f;

    { // Minimize p's scope. @TODO: -> fun
        auto p = make_unique<PlayerAvatar>(*scene, settings.projectile_speed,
            Movable::size_type{30.f, 30.f},
            Movable::pos_type{getWindowSize().x / 2.f,
            getWindowSize().y - 30.f});
        p->SetMovementParams(settings.player_speed, settings.player_stop_time);
        p->set_reload_time(std::chrono::milliseconds(settings.player_reload_time));
        //p->SetParticles("Player");
        game_state->SetPlayer(*p);
        scene->add_object(move(p));
    }

    // Set game scene boundaries
    auto const wall_width = 100.f;
    using sz_t = Movable::size_type;
    using pos_t = Movable::pos_type;
    CreateWall(*scene, sz_t(screen_size.x, wall_width),
        pos_t(screen_center.x, screen_size.y + wall_width / 2.f));
    CreateWall(*scene, sz_t(screen_size.x, wall_width),
        pos_t(screen_center.x, -wall_width / 2.f));
    CreateWall(*scene, sz_t(wall_width, screen_size.y),
        pos_t(-wall_width / 2.f, screen_center.y));
    CreateWall(*scene, sz_t(wall_width, screen_size.y),
        pos_t(screen_size.x + wall_width / 2.f, screen_center.y));
    
    CreateEnemies(*scene, *game_state, settings.enemy_count,
        settings.enemy_speed_base, settings.enemy_speed_jitter);

    scene->SetGameState(move(game_state));
    app.SetMainWidget(move(scene));
}
