#pragma once

#include "cri_game_object.h"
#include "cri_interface_input.h"

#include <cinder/Font.h>
#include <cinder/Color.h>

#include <functional>
#include <map>
#include <string>

class CRIHighscore : public CRIGameObject, public CRIInterfaceInput
{
public:
    explicit CRIHighscore(int NewScore);

    void Load(const std::string& FileName);

    void OnKeyDown(int KeyCode, cinder::app::KeyEvent Event) override;

private:
    typedef std::multimap<int, std::string, std::greater<int> > ScoresContT;

    enum class States
    {
	Death,
	Prompt,
	Display
    };

    void DoDraw() override;

    void ReadFromFile();

    void AddScore();
    void DisplayScores();
    void PromptForName();

    bool Validate();

    int m_NewScore{};
    ScoresContT m_Scores;
    int m_MaxEntries{5};
    std::string m_FileName;

    std::string m_CurName;
    int m_MaxNameLength{16};

    States m_State{States::Death};

    ci::Font m_Font{"Verdana", 48};
    ci::Color m_Color{1.f, 1.f, 1.f};
};
