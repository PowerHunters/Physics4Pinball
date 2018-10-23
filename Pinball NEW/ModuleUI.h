#ifndef __MODULEUI_H__
#define __MODULEUI_H__

#include "Module.h"
#include "p2Point.h"

#define MAX_SCORE 999999999
#define MAX_DIGITS 9

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

	SDL_Texture * font_1;
	SDL_Texture* font_2;

	SDL_Rect number_rects[10];

	int digit_score[MAX_DIGITS] = { -1,-1,-1,-1,-1,-1,-1,-1,0 };
	//int digit_lives[1] = { App->player->lives };

	// Player info ===================================
	int score = 0;
	int multiplier = 1;
	int high_score = 0;

};

#endif //__MODULEUI_H__

