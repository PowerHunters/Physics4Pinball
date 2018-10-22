#ifndef __MODULEUI_H__
#define __MODULEUI_H__

#include "Module.h"
#include "p2Point.h"

class ModuleUI : public Module
{

public:

	ModuleUI(Application* app, bool start_enabled = true);
	~ModuleUI();


	bool Start();
	update_status PostUpdate();
	bool CleanUp();

	void AddPoints(int points);

public:

	int font_1 = 0;
	//char CurrScore_text[10];
	//char balls_text[10];

	int font_2 = 1;
	//char HighScore_text[10];

	// Player info ===================================
	uint current_score = 0;
	uint multiplier = 1;
	uint high_score = 0;

};

#endif //__MODULEUI_H__

