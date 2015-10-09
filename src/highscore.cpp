#include "stdafx.h"

#include "highscore.h"

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

namespace {

bool IsChar(const KeyEvent& Event)
{
    const int Code = Event.getCode();
    if (Code >= 32 && Code <= 64) {
        return true;
    }
    if (Code >= 91 && Code <= 122) {
        return true;
    }

    return false;
}

template <typename T> string ToString(const T& Val)
{
    using std::stringstream;

    static stringstream Stream;
    Stream.str("");
    Stream << Val;
    return Stream.str();
}

} // unnamed

CRIHighscore::CRIHighscore(const int NewScore)
  : CRIGameObject(PosT(), SizeT())
  , m_NewScore(NewScore)
{
}

void CRIHighscore::Load(const string& FileName)
{
    m_FileName = FileName;
    ReadFromFile();
}

void CRIHighscore::ReadFromFile()
{
    using ci::app::console;
    using std::exception;
    using std::endl;
    using std::make_pair;

    try {
        const YAML::Node Scores = YAML::LoadFile(m_FileName);
        if (Scores && Scores.IsSequence()) {
            for (auto&& s : Scores) {
                const int Score = s["score"].as<int>(0);
                const string& Name = s["name"].as<string>("");
                m_Scores.insert(make_pair(Score, Name));
            }
        }
    }
    catch (exception& E) {
        console() << E.what() << endl;
    }
}

void CRIHighscore::OnKeyDown(const int KeyCode, const KeyEvent Event)
{
    if (m_State == States::Display) {
        return;
    }

    if (m_State == States::Death) {
        if (KeyCode != KeyEvent::KEY_RETURN) {
            return;
        }

        if (m_Scores.size() < m_MaxEntries ||
            m_Scores.rbegin()->first < m_NewScore) {
            PromptForName();
        }
        else {
            DisplayScores();
        }
        return;
    }

    if (KeyCode == KeyEvent::KEY_RETURN && !m_CurName.empty()) {
        AddScore();
        DisplayScores();
    }
    else if (KeyCode == KeyEvent::KEY_BACKSPACE && !m_CurName.empty()) {
        m_CurName.resize(m_CurName.size() - 1);
    }
    else if (IsChar(Event) && m_CurName.size() < m_MaxNameLength) {
        m_CurName += Event.getChar();
    }
}

void CRIHighscore::DoDraw()
{
    using ci::Vec2f;
    using ci::app::getWindowBounds;
    using ci::gl::color;
    using ci::gl::drawSolidRect;
    using ci::gl::drawString;
    using ci::gl::enableAlphaBlending;
    using ci::gl::SaveColorState;

    {
        SaveColorState S;
        enableAlphaBlending();
        color(0.f, 0.f, 0.f, 0.7f);
        drawSolidRect(getWindowBounds());
    }

    if (m_State == States::Death) {
        drawString(
            "The inevitable happened", Vec2f(400.f, 400.f), m_Color, m_Font);
    }

    if (m_State == States::Prompt) {
        drawString(
            "Your sacrifice is timeless", Vec2f(380.f, 350.f), m_Color, m_Font);
        drawString("Enter your name:", Vec2f(450.f, 450.f), m_Color, m_Font);
        drawString(m_CurName, Vec2f(450.f, 550.f), m_Color, m_Font);
    }

    if (m_State == States::Display) {
        drawString("Here, obedient to Spartan law, we lie:",
            Vec2f(300.f, 100.f), m_Color, m_Font);

        Vec2f CurOffset = Vec2f(200.f, 200.f);
        int Counter = 1;
        for (auto&& score : m_Scores) {
            if (Counter == m_MaxEntries + 1) {
                break;
            }

            drawString(ToString(Counter), CurOffset - Vec2f(60.f, 0.f), m_Color,
                m_Font);
            drawString(score.second, CurOffset, m_Color, m_Font);
            drawString(ToString(score.first), CurOffset + Vec2f(400.f, 0.f),
                m_Color, m_Font);
            CurOffset.y += 100.f;
            ++Counter;
        }
    }
}

void CRIHighscore::AddScore()
{
    using std::make_pair;
    using std::ofstream;

    assert(Validate());

    m_Scores.insert(make_pair(m_NewScore, m_CurName));
    assert(m_Scores.size() <= m_MaxEntries);

    ofstream Output = ofstream(m_FileName);
    if (Output) {
        YAML::Node Text;
        for (auto&& score : m_Scores) {
            YAML::Node Entry;
            Entry["score"] = score.first;
            Entry["name"] = score.second;
            Text.push_back(Entry);
        }

        Output << Text;
    }
}

void CRIHighscore::DisplayScores() { m_State = States::Display; }

bool CRIHighscore::Validate()
{
    return !m_CurName.empty() && m_CurName.length() < m_MaxNameLength;
}

void CRIHighscore::PromptForName() { m_State = States::Prompt; }
