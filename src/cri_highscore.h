#pragma once

#include "cri_app.h"
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
	CRIHighscore(const int NewScore, const CRIApp& app);

	void Load(const std::string& FileName);

    void on_key_down(KeyCode keycode, KeyEvent Event); // override

private:
    typedef std::multimap<int, std::string, std::greater<int> > ScoresContT;
    typedef ScoresContT::const_iterator ScoresConstIterT;

	enum States
	{
		StateDeath,
		StatePrompt,
		StateDisplay
	};

	void DoDraw(); // override

    void ReadFromFile();

	void AddScore();
	void DisplayScores();
	void PromptForName();

	bool Validate();

    int m_NewScore;
    ScoresContT m_Scores;
    int m_MaxEntries;
	std::string m_FileName;

	std::string m_CurName;
	int m_MaxNameLength;

	States m_State;

	ci::Font m_Font;
	ci::Color m_Color;

    const CRIApp* app_{};
};
