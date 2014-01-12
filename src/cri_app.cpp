#include "cri_stdafx.h"

#include "cri_app.h"

#include "cri_game_scene.h"

#include <cassert>

CRIApp::CRIApp()
: m_pScene(0)
{ 
}

CRIApp::~CRIApp()
{
    delete m_pScene;
}

void CRIApp::prepareSettings( Settings* settings )
{
    settings->setWindowSize(1280, 1024);
    settings->setFrameRate(60.f);
}

void CRIApp::setup()
{
    hideCursor();
}

void CRIApp::draw()
{
    if (m_pScene)
    {
        m_pScene->Draw();
    }
}

void CRIApp::update()
{
    if (m_pScene)
    {
        m_pScene->Update();
    }
}

void CRIApp::SetScene( CRIGameScene* pScene )
{
    delete m_pScene;
    m_pScene = pScene;
}

CINDER_APP_BASIC( CRIApp, ci::app::RendererGl )
