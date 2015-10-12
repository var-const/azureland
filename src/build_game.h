#pragma once

#include <memory>
#include <vector>

namespace cinder {
template <typename T> class Vec2;
}

class App;
class GameScene;
class Player;

void BuildGame(App& App);
std::unique_ptr<Player> CreatePlayer(App& App, GameScene& Scene);
void CreateEnemies(GameScene& Scene, Player& player);
void SpawnEnemies(GameScene& Scene, Player& player, int Count,
    cinder::Vec2<float> From, int MaxRowLength, float Dispersion,
    const std::vector<int>& Textures);
