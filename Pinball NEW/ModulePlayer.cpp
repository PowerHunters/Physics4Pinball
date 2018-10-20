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
	//starter = App->physics->CreateStarter(313, 484, 10, 10, starter_tex);


	/*starter_tex = App->textures->Load("textures/ball.png");*/
	//flipper_fx = App->audio->LoadFx("sounds/fx/flipper_sound.ogg");
	//lose_fx = App->audio->LoadFx("sounds/fx/loser.ogg");
	//starter_fx = App->audio->LoadFx("sounds/fx/launcher.ogg");
	


	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");
	App->textures->Unload(ball_tex);
	//App->textures->Unload(starter_tex);

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


	//float push_force = 0.0f;

	//if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	//{
	//	push_force += 175.0f;
	//	starter->push(0, push_force);
	//}
	//else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP)
	//{
	//	push_force = -500.0f;
	//	starter->push(0, push_force);
	//	/*App->audio->playFx(start_fx);*/
	//}
	//else
	//	push_force = 0.0f;

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

	//--------Starter----------------------------------------------
	//int x, y;
	//starter->GetPosition(x, y);
	//SDL_Rect rect = { 0, 0, 22, 22 };
	//App->renderer->Blit(starter_tex, x, y, &rect);

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
