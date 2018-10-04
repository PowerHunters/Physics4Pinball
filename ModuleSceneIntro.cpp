#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModulePhysics.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	//inicialize textures to NULL
	ball = NULL;
	pinball = NULL;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	pinball = App->textures->Load("pinball/Pinball.png"); 
	ball = App->textures->Load("pinball/Ball.png");
	/*box = App->textures->Load("pinball/crate.png");
	rick = App->textures->Load("pinball/rick_head.png");*/

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

	fixures.add(App->physics->CreateFixChain(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, Pinball, 258));

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
	//ball in mouse position
	if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 25));
	}

	//ball in start
	if(App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		//boxes.add(App->physics->CreateRectangle(App->input->GetMouseX(), App->input->GetMouseY(), 50, 100));
		circles.add(App->physics->CreateCircle(426, 904, 25));
	}

	if(App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		// Pivot 0, 0
		
	}
	
	// TODO 5: Draw all the circles using "circle" texture
	// Draw all circles
	p2List_item<PhysBody*>* c = circles.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(ball, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	//c = boxes.getFirst();

	//while(c != NULL)
	//{
	//	int x, y;
	//	c->data->GetPosition(x, y);
	//	App->renderer->Blit(box, x, y, NULL, 1.0f, c->data->GetRotation());
	//	c = c->next;
	//}

	//c = ricks.getFirst();

	//while(c != NULL)
	//{
	//	int x, y;
	//	c->data->GetPosition(x, y);
	//	//App->renderer->Blit(rick, x, y, NULL, 1.0f, c->data->GetRotation());
	//	c = c->next;
	//}

	return UPDATE_CONTINUE;
}
