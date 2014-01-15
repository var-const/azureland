#pragma once

class CRIApp;
class CRIGameScene;
class CRIPlayer;

void BuildGame(CRIApp& App);
CRIPlayer* CreatePlayer(CRIApp& App);
void CreateEnemies(CRIGameScene& Scene, CRIPlayer& Player);
