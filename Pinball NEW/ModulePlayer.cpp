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


	flipper_r = App->physics->CreateRectangle(360, 770, 99, 19, true);
	flipper_l = App->physics->CreateRectangle(250, 770, 99, 19, true);

	PhysBody* right_joint = App->physics->CreateCircle(401, 772, 5, false);
	PhysBody* left_joint = App->physics->CreateCircle(205, 772, 5, false);

	b2RevoluteJointDef RightFJoint;
	b2RevoluteJointDef LeftFJoint;

	b2RevoluteJoint* joint_r;
	b2RevoluteJoint* joint_l;


	RightFJoint.Initialize(flipper_r->body, right_joint->body, right_joint->body->GetWorldCenter());
	LeftFJoint.Initialize(flipper_l->body, left_joint->body, left_joint->body->GetWorldCenter());

	RightFJoint.lowerAngle = -30 * DEGTORAD;
	RightFJoint.upperAngle = 30 * DEGTORAD;
	RightFJoint.enableLimit = true;
	RightFJoint.collideConnected = false;
	joint_r = (b2RevoluteJoint*)App->physics->world->CreateJoint(&RightFJoint);

	LeftFJoint.lowerAngle = -30 * DEGTORAD;
	LeftFJoint.upperAngle = 30 * DEGTORAD;
	LeftFJoint.enableLimit = true;
	LeftFJoint.collideConnected = false;
	joint_l = (b2RevoluteJoint*)App->physics->world->CreateJoint(&LeftFJoint);

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


