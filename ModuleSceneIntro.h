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
	p2List<PhysBody*> circles;
	//p2List<PhysBody*> boxes;
	p2List<PhysBody*> edges;

	SDL_Texture* ball;
	SDL_Texture* pinball;

	//SDL_Texture* box;
	//SDL_Texture* rick;
};
