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
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB, b2Contact* contact);
	void AddStaticBodies();

public:

	bool reset = false;

	// PhysBodies==================================
	PhysBody* sensor = NULL;
	PhysBody* sensor_death = NULL;

	// Barriers ------------------------------
	bool create_right_barrier = false;
	bool create_left_barrier = true;
	bool destroy_right_barrier = false;
	bool destroy_left_barrier = false;

	PhysBody* right_barrier = NULL;
	PhysBody* left_barrier = NULL;
	PhysBody* sensor_barrier_right = NULL;
	PhysBody* sensor_initial_barrier_left = NULL;
	PhysBody* sensor_final_barrier_left = NULL;

	p2List<PhysBody*> board_parts;
	p2List<PhysBody*> bumpers;
	p2List<PhysBody*> bottom_bumpers;

	//Textures====================================
	SDL_Texture* background_tex;
	SDL_Texture* circle_tex;
	SDL_Rect pinball_rect;

	//Sfx=========================================



	//Delete------------------------
	uint bonus_fx;
};
