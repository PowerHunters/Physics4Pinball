#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"

class PhysBody;
struct SDL_Texture;

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();

	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

	//Flipper actions ===============================
	void engageFlipper(PhysBody *flipper, float impulse);

public:
	//PhysBodies ==================================
	PhysBody* ball = NULL;
	PhysBody* flipper_r = NULL;
	PhysBody* flipper_l = NULL;
	//PhysBody* starter = NULL;

	//Textures ====================================
	SDL_Texture* ball_tex;
	//SDL_Texture* starter_tex;
	////Sfx=========================================
	//uint flipper_fx;
	//uint lose_fx;
	//uint starter_fx;

};