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

public:

	int font_score = 1;
	char CurrScore_text[10];
	char PrevScore_text[10];
	char HighScore_text[10];
	char balls_text[10];

	uint current_score = 0;
	uint previous_score = 0;
	uint high_score = 0;
	uint balls = 0;
};

#endif //__MODULEUI_H__

