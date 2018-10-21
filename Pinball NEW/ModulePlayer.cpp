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
	lifes = 3;
	init_position.x = PIXEL_TO_METERS(489);
	init_position.y = PIXEL_TO_METERS(900);
	impulse_force = 0.0f;
	launcher_init_pos.x = 487.0f;
	launcher_init_pos.y = 912.0f + 16;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");
	// Textures ======================================================
	ball_tex = App->textures->Load("textures/ball.png");
	// PhysBodies ====================================================
	int left_flipper[8]
	{
		0,  12,
		0 ,-12, 
		70,  7, 
		70 ,-7
	};

	int right_flipper[8]
	{
		0,  12,
		0 ,-12,
		-70,  7,
		-70 ,-7
	};
	
	flipper_l = App->physics->CreateFlipper(b2Vec2(177, 920), left_flipper, 8, b2Vec2(147, 920), -30 , 30 , flipper_l_joint);
	flipper_r = App->physics->CreateFlipper(b2Vec2(290, 920), right_flipper, 8, b2Vec2(320, 920), -30, 30 , flipper_r_joint);
 	launcher = App->physics->CreateLauncher(launcher_init_pos.x, launcher_init_pos.y, 33, 33, launcher_joint);

	if (flipper_r_joint == NULL)
		LOG("flipper_r_joint null ======================================");

	if (flipper_l_joint == NULL)
		LOG("flipper_l_joint null ======================================");

	if (launcher_joint == NULL)
		LOG("launcher_joint ======================================");


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

	if (reset)
	{
		Reset();
		reset = false;
	}

	// Ball =============================================================
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN && ball == NULL)
	{
		ball = App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 11);
		ball->body->SetBullet(true);
		ball->listener = this;
	}

	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN && ball)
	{
		Reset();
	}

	// Flippers =============================================================
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
	{
		engageFlipper(flipper_l, -9.0f);
	}
	else
		engageFlipper(flipper_l, 2.0f);

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
	{
		engageFlipper(flipper_r, 13.0f);
	}

	// Launcher ==============================================================
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT) 
	{
		launcher_joint->SetMotorSpeed(-2);
		impulse_force += 0.5f;

		if (impulse_force > 40)
		{
			impulse_force = 40;
		}
	}
	else if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP) 
	{
		launcher_joint->SetMotorSpeed(impulse_force);
		impulse_force = 0; //Sfx
	}


	//After being launched set speed to 0 again after reached center point
	//if (METERS_TO_PIXELS(launcher->body->GetPosition().y )<= launcher_init_pos.y && METERS_TO_PIXELS(launcher->body->GetPosition().y)>= launcher_init_pos.y - 0.2f)
	//{
	//	b2Vec2 position(PIXEL_TO_METERS(launcher_init_pos.x), PIXEL_TO_METERS(launcher_init_pos.y));
	//	launcher_joint->SetMotorSpeed(0);
	//	launcher->body->SetTransform(position, 0);
	//}

	//--------Ball------------------------------------------------
	if (ball != nullptr)
	{
		
		int x, y;
		ball->GetPosition(x, y);
		SDL_Rect rect = { 0, 0, 22, 22 };
		//App->renderer->Blit(ball_tex, x, y, &rect, 1.0f, RADTODEG*ball->body->GetAngle());
		App->renderer->Blit(ball_tex, x, y, &rect);

	}

	//--------Starter----------------------------------------------
	int x, y;
	launcher->GetPosition(x, y);
	SDL_Rect rect = { x ,  y, 33, 33 };
	App->renderer->DrawQuad(rect , 255,255,255);

	//--------Barrier----------------------------------------------
	if (create_barrier == true)
		barrier = App->physics->CreateRectangle(397, 139, 33, 3, -60, true);

	return UPDATE_CONTINUE;
}

void ModulePlayer::OnCollision(PhysBody* bodyA, PhysBody* bodyB, b2Contact* contact)
{
	b2WorldManifold worldManifold;
	contact->GetWorldManifold(&worldManifold);

	if (ball == bodyA && App->scene_intro->bouncers.find(bodyB) != -1)
	{
		// Sfx ===========================================
		App->audio->PlayFx(App->scene_intro->bonus_fx);
		// Aplly impulse =================================
		b2Vec2 normal = worldManifold.normal;
		normal *= 1.5f;
		ball->body->SetLinearVelocity({ 0.0f, 0.0f });
		ball->body->ApplyLinearImpulse(normal, ball->body->GetWorldCenter(), true);
	}

	if (ball == bodyA && App->scene_intro->sensor_death == bodyB)
	{
		// Sfx ===========================================
		App->audio->PlayFx(App->scene_intro->bonus_fx); //lose_fx
		// Lifes logic =================================
		--lifes;
		if (lifes <= 0) {
			is_dead = true;
		}
		else
		// Set ball ================================================
		{
			reset = true;
		}
		

	}

	if (ball == bodyA && App->scene_intro->sensor_barrier == bodyB && barrier == nullptr)
	{
		// Create barrier =================================
		create_barrier = true;
		// Destroy barrier if dead ================================================
		//if (--lifes)
		//	create_barrier = false;
	}
}

void ModulePlayer::engageFlipper(PhysBody *flipper, float impulse)
{
	if (flipper)
	{
		flipper->body->ApplyAngularImpulse(impulse, true);
	}
}

void ModulePlayer::Reset ()
{
	if (ball)
	{
		ball->body->SetLinearVelocity({ 0,0 });
		ball->body->SetTransform( init_position, 0);
	}
}
