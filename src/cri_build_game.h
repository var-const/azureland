#pragma once

namespace cinder { template <typename T> class Vec2; }

class CRIApp;
class CRIGameScene;
class CRIPlayer;

void BuildGame(CRIApp& App);
CRIPlayer* CreatePlayer(CRIApp& App);
void CreateEnemies(CRIGameScene& Scene, CRIPlayer& Player);
void SpawnEnemies(CRIGameScene& Scene, CRIPlayer& Player, int Count,
    cinder::Vec2<float> From, int MaxRowLength, float Dispersion);
