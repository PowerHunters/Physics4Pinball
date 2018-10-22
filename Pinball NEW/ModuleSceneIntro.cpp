#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModulePlayer.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	//Textures===================================
	background_tex =  circle_tex = NULL;
	pinball_rect.x = 0;
	pinball_rect.y = 0;
	pinball_rect.w = 518;
	pinball_rect.h = 1080;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;
	//Textures===============================================
	background_tex = App->textures->Load("textures/Pinball.png");
	//PhyBodies==============================================
	AddStaticBodies();
	sensor_death = App->physics->CreateRectangleSensor(205+56/2, 1046+6/2, 56, 6, 0); //the x and y take pos from the center
	final_target = App->physics->CreateRectangleSensor(59, 533, 10, 39, 45);
	final_target->listener = this;
	top_hole = App->physics->CreateCircle(237, 167, 17, false);
	top_hole->listener = this;
	magnet_hole = App->physics->CreateCircle(438, 281, 17, false);
	magnet_hole->listener = this;
	// Barriers ---------------------------------------
	sensor_barrier_right = App->physics->CreateRectangleSensor(370, 134, 60, 6, 90);
	sensor_barrier_right->listener = this;
	sensor_initial_barrier_left = App->physics->CreateRectangleSensor(92, 167, 36, 8, 56);
	sensor_initial_barrier_left->listener = this;
	sensor_final_barrier_left = App->physics->CreateRectangleSensor(140, 147, 80, 6, 90);
	sensor_final_barrier_left->listener = this;
	//Delete-------------------------------------------
	bonus_fx = App->audio->LoadFx("sfx/bonus.wav");

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	if (reset == true)
	{
		if (left_barrier == NULL) create_left_barrier = true;
		if (right_barrier) destroy_right_barrier = true;
		reset = false;
	}

	//--------Barriers----------------------------------------------
	if (create_right_barrier == true)
	{
		right_barrier = App->physics->CreateRectangle(397, 139, 33, 6, -60, false);
		create_right_barrier = false;
	}
	if (create_left_barrier == true)
	{
		left_barrier = App->physics->CreateRectangle(101, 159, 33, 6, 57, false);
		create_left_barrier = false;
	}

	if (destroy_left_barrier == true)
	{
		left_barrier->body->GetWorld()->DestroyBody(left_barrier->body);
		left_barrier = nullptr;
		destroy_left_barrier = false;
	}

	if (destroy_right_barrier == true)
	{
		right_barrier->body->GetWorld()->DestroyBody(right_barrier->body);
		right_barrier = nullptr;
		destroy_right_barrier = false;
	}

	// All draw functions ======================================================
	//------Background--------------------------------------------
	App->renderer->Blit(background_tex, SCREEN_WIDTH / 2 - pinball_rect.w / 2, 0, &pinball_rect);

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB, b2Contact* contact)
{
	if (sensor_barrier_right == bodyA && App->player->ball == bodyB && right_barrier == nullptr)
	{
		// Create barrier =================================
		create_right_barrier = true;
		// Destroy barrier if dead ================================================
		//if (--lifes)
		//	create_barrier = false;
	}

	if (sensor_final_barrier_left == bodyA && App->player->ball == bodyB && left_barrier == nullptr)
	{
		// Create barrier =================================
		create_left_barrier = true;
	}

	if (sensor_initial_barrier_left == bodyA && App->player->ball == bodyB && left_barrier != nullptr)
	{
		// Destroy barrier =================================
		destroy_left_barrier = true;
	}
}

void ModuleSceneIntro::AddStaticBodies()
{
	//Bumpers============================================
	bumpers.add(App->physics->CreateCircle(SCREEN_WIDTH / 2 + 29, SCREEN_HEIGHT / 2 - 222, 27, false));
	bumpers.add(App->physics->CreateCircle(SCREEN_WIDTH / 2 - 85, SCREEN_HEIGHT / 2 - 222, 27, false));
	bumpers.add(App->physics->CreateCircle(SCREEN_WIDTH / 2 - 29, SCREEN_HEIGHT / 2 - 135, 27, false));
	//Targets============================================
	targets.add (App->physics->CreateRectangleSensor(106, 279, 10, 39, 50));//left-bottom
	targets.add(App->physics->CreateRectangleSensor(142, 250, 9, 39, 50));//left-upper
	targets.add(App->physics->CreateRectangleSensor(318, 242, 9, 39, -50));//right-upper
	targets.add(App->physics->CreateRectangleSensor(355, 271, 10, 39, -50));//right-bottom
	//listeners

	//Slingshots============================================
	int LeftStruct[26] = {
	80, 727,
	80, 821,
	80, 827,
	85, 831,
	130, 856,
	138, 857,
	142, 854,
	145, 850,
	144, 844,
	100, 720,
	96, 716,
	84, 717,
	80, 721
	};

	slingshots.add(App->physics->CreateChain(0, 0, LeftStruct, 26, false));

	int RightStruct[26] = {
		321, 846,
		322, 852,
		328, 856,
		334, 857,
		340, 855,
		383, 829,
		386, 825,
		386, 722,
		382, 716,
		374, 715,
		367, 717,
		364, 722,
		323, 842
	};

	slingshots.add(App->physics->CreateChain(0, 0, RightStruct, 26, false));

	//Board parts=============================================================
	int BigStruct[52] = {
		98, 402,
		102, 397,
		103, 389,
		101, 376,
		83, 290,
		154, 230,
		195, 232,
		198, 228,
		200, 223,
		200, 217,
		197, 212,
		110, 176,
		103, 180,
		95, 187,
		78, 209,
		66, 232,
		61, 248,
		56, 266,
		55, 289,
		55, 313,
		59, 338,
		64, 360,
		72, 381,
		81, 395,
		88, 402,
		94, 403
	};

	board_parts.add(App->physics->CreateChain(0, 0, BigStruct, 52, false));


	int LeftLine[16] = {
		49, 739,
		50, 847,
		143, 901,
		143, 904,
		140, 904,
		45, 848,
		45, 739,
		47, 737
	};

	board_parts.add(App->physics->CreateChain(0, 0, LeftLine, 16, false));

	int RightLine[18] = {
		421, 739,
		421, 847,
		418, 850,
		326, 904,
		323, 904,
		324, 901,
		418, 846,
		418, 739,
		420, 737
	};

	board_parts.add(App->physics->CreateChain(0, 0, RightLine, 18 , false));

	int Pinball[258] = {
		181, 113,
		189, 115,
		194, 121,
		195, 130,
		195, 352,
		11, 460,
		1, 513,
		-52, 513,
		-63, 463,
		-247, 355,
		-247, 105,
		-202, 126,
		-194, 129,
		-187, 130,
		-181, 126,
		-180, 120,
		-189, 98,
		-221, 12,
		-221, 5,
		-218, 1,
		-201, -16,
		-185, -34,
		-202, -66,
		-214, -96,
		-226, -136,
		-236, -179,
		-242, -228,
		-243, -248,
		-238, -281,
		-230, -307,
		-219, -332,
		-206, -354,
		-189, -377,
		-168, -399,
		-143, -419,
		-122, -433,
		-104, -442,
		-80, -452,
		-60, -458,
		-40, -462,
		-16, -465,
		8, -465,
		29, -464,
		45, -462,
		62, -458,
		78, -453,
		96, -446,
		114, -438,
		130, -429,
		146, -418,
		160, -407,
		172, -396,
		184, -383,
		195, -371,
		206, -356,
		215, -341,
		223, -326,
		230, -309,
		236, -292,
		242, -272,
		245, -254,
		248, -226,
		247, 540,
		259, 540,
		259, -480,
		-259, -480,
		-259, 540,
		239, 540,
		239, 511,
		210, 511,
		210, -224,
		209, -242,
		208, -262,
		204, -284,
		194, -309,
		182, -329,
		168, -344,
		139, -374,
		133, -380,
		127, -381,
		121, -380,
		114, -377,
		34, -340,
		22, -334,
		18, -329,
		18, -323,
		22, -320,
		30, -319,
		40, -320,
		47, -319,
		57, -311,
		120, -261,
		125, -256,
		126, -251,
		119, -217,
		103, -142,
		98, -115,
		98, -108,
		105, -103,
		125, -95,
		132, -95,
		139, -100,
		144, -112,
		154, -143,
		159, -161,
		161, -181,
		161, -211,
		161, -253,
		163, -266,
		170, -274,
		179, -277,
		187, -275,
		194, -268,
		196, -255,
		195, -135,
		194, -102,
		190, -73,
		185, -50,
		175, -13,
		175, -5,
		178, 0,
		171, 4,
		168, 12,
		130, 122,
		129, 130,
		134, 133,
		142, 133,
		158, 124,
		174, 115
	};

	board_parts.add(App->physics->CreateChain(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, Pinball, 258, false));

}
