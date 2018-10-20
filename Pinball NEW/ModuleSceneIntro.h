#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
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
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);
	void AddStaticBodies();

public:
	//PhysBodies==================================
	PhysBody* ball = NULL;
	PhysBody* sensor = NULL;
	p2List<PhysBody*> chains;
	p2List<PhysBody*> circles;

	//Textures====================================
	SDL_Texture* ball_tex;
	SDL_Texture* background_tex;
	SDL_Texture* circle_tex;

	SDL_Rect pinball_rect;

	//Sfx=========================================



	//Delete------------------------
	SDL_Texture* box;
	SDL_Texture* rick;
	uint bonus_fx;
};
