#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	ball_tex = NULL;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");
	ball_tex = App->textures->Load("textures/ball.png");

	int left_flipper[26] = {
	135, 912,
	139, 907,
	146, 904,
	153, 906,
	203, 944,
	207, 950,
	205, 956,
	199, 960,
	195, 960,
	160, 942,
	140, 932,
	135, 926,
	133, 919
	};

	flipper_l = App->physics->CreateFlipper(b2Vec2(0, 0), left_flipper, 26, b2Vec2(0, 0), -45 , 70 );

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update
update_status ModulePlayer::Update()
{
	// Prepare for raycast =====================================================

	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();

	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN && ball == NULL)
	{
		ball = App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 11);
		ball->listener = this;
	}

	// All draw functions ======================================================

	//--------Ball------------------------------------------------
	if (ball != nullptr)
	{
		int x, y;
		ball->GetPosition(x, y);
		SDL_Rect rect = { 0, 0, 22, 22 };
		App->renderer->Blit(ball_tex, x, y, &rect);
	}

	return UPDATE_CONTINUE;
}

void ModulePlayer::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	if (ball == bodyA && App->scene_intro->bouncers.find(bodyB) != -1)
	{
		App->audio->PlayFx(App->scene_intro->bonus_fx);
	}

	if (ball == bodyA && App->scene_intro->sensor == bodyB)
	{
		App->audio->PlayFx(App->scene_intro->bonus_fx);
	}

}


