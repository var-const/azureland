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
	explicit CRIHighscore(const int NewScore);

	void Load(const std::string& FileName);
	void OnAddedToScene(); // override

    void OnKeyDown(int KeyCode, cinder::app::KeyEvent Event); // override

private:
	enum States
	{
		StateDisplay,
		StatePrompt,
		StateUninitialized
	};

	void DoDraw(); // override

    void ReadFromFile();

	void AddScore();
	void Display();
	void PromptForName();

	bool Validate();

    int m_NewScore;
    std::multimap<int, std::string, std::greater<int> > m_Scores;
    int m_MaxEntries;
	std::string m_FileName;

	std::string m_CurName;
	int m_MaxNameLength;

	States m_State;

	ci::Font m_Font;
	ci::Color m_Color;
};
