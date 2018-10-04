#pragma once
#include "Module.h"
#include "p2List.h"
#include "Globals.h"

class PhysBody;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();

public:
	p2List<PhysBody*> balls;
	p2List<PhysBody*> circles;
	p2List<PhysBody*> fixures;
	//p2List<PhysBody*> boxes;

	SDL_Texture* ball;
	SDL_Texture* pinball;

	SDL_Rect pinball_rect;

	//SDL_Texture* box;
	SDL_Texture* rick;
};
