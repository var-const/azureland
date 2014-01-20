#include "cri_stdafx.h"

#include "cri_highscore.h"

#include <cinder/Vector.h>
#include <cinder/app/App.h>
#include <cinder/app/KeyEvent.h>
#include <cinder/gl/gl.h>

#include <yaml-cpp/yaml.h>

#include <algorithm>
#include <exception>
#include <fstream>
#include <ostream>

using ci::app::KeyEvent;
using std::string;

namespace
{

bool IsChar(const KeyEvent& Event)
{
    const int Code = Event.getCode();
    if (Code >= 32 && Code <= 64)
    {
        return true;
    }
    if (Code >= 91 && Code <= 122)
    {
        return true;
    }

    return false;
}

} // unnamed

CRIHighscore::CRIHighscore( const int NewScore )
: CRIGameObject(PosT(), SizeT())
, m_NewScore(NewScore)
, m_State(StateUninitialized)
, m_MaxNameLength(16)
, m_MaxEntries(10)
, m_Color(1.f, 1.f, 1.f)
, m_Font("Verdana", 48)
{
}

void CRIHighscore::Load( const string& FileName )
{
    using std::sort;

    m_FileName = FileName;
    ReadFromFile();

    if (m_Scores.size() < m_MaxEntries || m_Scores.rbegin()->first < m_NewScore)
    {
        PromptForName();
    }
    else
    {
        Display();
    }
}

void CRIHighscore::ReadFromFile()
{
    using ci::app::console;
    using std::exception;
    using std::endl; using std::make_pair;

    try
    {
        const YAML::Node Scores = YAML::LoadFile(m_FileName);
        if (Scores && Scores.IsSequence())
        {
            for (YAML::const_iterator Iter = Scores.begin(); Iter != 
                Scores.end(); ++Iter)
            {
                const int Score = (*Iter)["score"].as<int>(0);
                const string& Name = (*Iter)["name"].as<string>("");
                m_Scores.insert(make_pair(Score, Name));
            }
        }
    }
    catch (exception& E)
    {
        console() << E.what() << endl;
    }
}

void CRIHighscore::OnAddedToScene()
{
    //GetScene().AddInputListener(*this);
}

void CRIHighscore::OnKeyDown( const int KeyCode, const KeyEvent Event )
{
    if (m_State != StatePrompt)
    {
        return;
    }

    if (KeyCode == KeyEvent::KEY_RETURN && !m_CurName.empty())
    {
        AddScore();
        Display();
    }
    else if (KeyCode == KeyEvent::KEY_BACKSPACE && !m_CurName.empty())
    {
        m_CurName.resize(m_CurName.size() - 1);
    }
    else if (IsChar(Event) && m_CurName.size() < m_MaxNameLength)
    {
        m_CurName += Event.getChar();
    }
}

void CRIHighscore::DoDraw()
{
    using ci::Vec2f; using ci::gl::drawString;

    if (m_State == StateDisplay)
    {
    //    int Counter = 0;
    //    settings.player_speed = player["speed"].as<T>();
    //    settings.player_stop_time = player["stop_time_ms"].as<T>();
    //    settings.player_reload_time = player["reload_time_ms"].as<T>();

    //    return settings;
    //    for (??? Iter = m_Score.begin(); Iter != m_Score.end(); ++Iter)
    //    {
    //        drawString(Counter);
    //        drawString((*Iter)["name"], ???);
    //        drawString((*Iter)["score"], ???);
    //    }
    }
    else if (m_State == StatePrompt)
    {
        drawString("The grim reaper got you", Vec2f(100.f, 100.f), m_Color,
            m_Font);
        drawString("Enter your name:", Vec2f(100.f, 300.f), m_Color, m_Font);
        drawString(m_CurName, Vec2f(100.f, 500.f), m_Color, m_Font);
    }
}

void CRIHighscore::AddScore()
{
    using std::make_pair; using std::ofstream;

    assert(Validate());

    m_Scores.insert(make_pair(m_NewScore, m_CurName));
    assert(m_Scores.size() <= m_MaxEntries);

    ofstream Output = ofstream(m_FileName);
    if (Output)
    {
        YAML::Node Text;
        for (ScoresConstIterT Iter = m_Scores.begin(); Iter != m_Scores.end();
            ++Iter)
        {
            YAML::Node Entry;
            Entry["score"] = Iter->first;
            Entry["name"] = Iter->second;
            Text.push_back(Entry);
        }

        Output << Text;
    }
}

void CRIHighscore::Display()
{
    m_State = StateDisplay;
}

bool CRIHighscore::Validate()
{
    return !m_CurName.empty() && m_CurName.length() < m_MaxNameLength;
}

void CRIHighscore::PromptForName()
{
    m_State = StatePrompt;
}
