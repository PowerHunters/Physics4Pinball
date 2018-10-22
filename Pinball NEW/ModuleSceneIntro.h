#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"

#define COMBO_DELAY_FRAMES 200
#define KEEP_INSIDE_FRAMES 60
class PhysBody;

struct Combo_letter
{
	iPoint position;
	SDL_Rect actived_rect;
};

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

	// PhysBodies========================================

	// Barriers ----------------------------------
	bool create_right_barrier = false;
	bool create_left_barrier = true;
	bool destroy_right_barrier = false;
	bool destroy_left_barrier = false;

	PhysBody* right_barrier = NULL;
	PhysBody* left_barrier = NULL;
	PhysBody* sensor_barrier_right = NULL;
	PhysBody* sensor_initial_barrier_left = NULL;
	PhysBody* sensor_final_barrier_left = NULL;

	// Chocolate combo -------------------------
	Combo_letter combo_letters[9];
	uint combo_letters_amount = 0;
	int combo_delay_frames = 0;
	bool combo_delay = false;
	bool activate_final_target = false;
	bool create_targets = true;
	PhysBody* targets[4];
	PhysBody* final_target = NULL;

	// Bonuses ---------------------------------
	int keep_inside_frames = 0;
	bool keep_player_magnet;
	bool keep_player_top;
	iPoint pos_multiplier_bonus[4];
	iPoint pos_magnet_bonus[3];


	// Other sensors, collisions and bumpers------
	p2List<PhysBody*> board_parts;
	p2List<PhysBody*> bumpers;
	p2List<PhysBody*> slingshots;
	PhysBody* top_hole = NULL;
	PhysBody* magnet_hole = NULL;
	PhysBody* sensor_death = NULL;

	// Textures ==================================
	SDL_Texture* background_tex;
	SDL_Texture* combo_letters_tex;
	SDL_Texture* circle_tex;
	SDL_Rect pinball_rect;

	// Sfx ========================================
	uint chocolate_combo_sfx;


	//Delete------------------------
	uint bonus_fx;

};
