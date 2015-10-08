#pragma once

#include <memory>
#include <vector>

namespace cinder { template <typename T> class Vec2; }

class CRIApp;
class CRIGameScene;
class CRIPlayer;

void BuildGame(CRIApp& App);
std::unique_ptr<CRIPlayer> CreatePlayer(CRIApp& App, CRIGameScene& Scene);
void CreateEnemies(CRIGameScene& Scene, CRIPlayer& Player);
void SpawnEnemies(CRIGameScene& Scene, CRIPlayer& Player, int Count,
    cinder::Vec2<float> From, int MaxRowLength, float Dispersion,
    const std::vector<int>& Textures);
