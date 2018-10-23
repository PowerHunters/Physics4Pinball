#include "Application.h"
#include "Globals.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleSceneIntro.h"
#include "ModuleUI.h"
#include "ModulePlayer.h"


ModuleUI::ModuleUI(Application* app, bool start_enabled) : Module(app, start_enabled) {}

ModuleUI::~ModuleUI() {}

bool ModuleUI::Start()
{
	LOG("Loading UI");

	font_1 = App->textures->Load("textures/font_1.png");
	font_2 = App->textures->Load("textures/font_2.png");

	number_rects[0] = { 0,0,15,18 };
	number_rects[1] = { 15,0,15,18 };
	number_rects[2] = { 30,0,15,18 };
	number_rects[3] = { 45,0,15,18 };
	number_rects[4] = { 60,0,15,18 };
	number_rects[5] = { 75,0,15,18 };
	number_rects[6] = { 90,0,15,18 };
	number_rects[7] = { 105,0,15,18 };
	number_rects[8] = { 120,0,15,18 };
	number_rects[9] = { 135,0,15,18 };


	/*score = 0;*/

	return true;
}

bool ModuleUI::CleanUp()
{
	LOG("Unloading user interface")

	//App->textures->Unload(font_1);
	//App->textures->Unload(font_2);

	return true;
}
update_status ModuleUI::PostUpdate()
{
	//highscore logic
	if (score > high_score) {
		high_score = score;
	}

	// blit score
	int x = 120;
	int y = 0;
	for (int i = 8; i > 0; i--)
	{
		if (digit_score[i] != -1)
		{
			App->renderer->Blit(font_1, x, y, &number_rects[digit_score[i]]);
			x -= 0;
		}
		else break;
	}


	////Add balls
	//if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN && App->scene_intro->IsEnabled() == true) {
	//	App->player->lifes++;
	//}

	return UPDATE_CONTINUE;
}

void ModuleUI::AddPoints(int points)
{
	score += points * multiplier;
	if (score > MAX_SCORE) score = MAX_SCORE;
	int index = 8;
	int aux_score = score;
	while (aux_score > 0)
	{
		int digit = aux_score % 10;
		aux_score /= 10;
		digit_score[index] = digit;
		index--;
	}
}
