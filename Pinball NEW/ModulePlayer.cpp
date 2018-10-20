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

	flipper_l = App->physics->CreateFlipper(b2Vec2(187, 920), 70, 10, b2Vec2(147, 920), -30 , 30 );

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

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
	{
		engageFlipper(flipper_l);
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

void ModulePlayer::engageFlipper(PhysBody *flipper)
{
	if (flipper)
	{
		flipper->body->ApplyAngularImpulse(-9.0f, true);
	}
}
