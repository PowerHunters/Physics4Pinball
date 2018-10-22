#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleFonts.h"
#include "ModuleUI.h"
#include "ModulePlayer.h"
#include <stdio.h>

ModuleUI::ModuleUI(Application* app, bool start_enabled) : Module(app, start_enabled) {}

ModuleUI::~ModuleUI() {}

bool ModuleUI::Start()
{
	LOG("Loading UI");

	font_1 = App->fonts->Load("textures/font_1.png", "0123456789", 1);

	current_score = 0;
	//previous_score = 0;

	return true;
}

bool ModuleUI::CleanUp()
{
	LOG("Unloading user interface")

	App->fonts->UnLoad(font_1);
	return true;
}
update_status ModuleUI::PostUpdate()
{
	////highscore logic
	//if (score1 > high_score) {
	//	high_score = score1; //App->player1->score
	//}

	//if (score2 > high_score) {
	//	high_score = score2;
	//}

	//scores
	if (App->scene_intro->IsEnabled() == true ) {
		//score1
		sprintf_s(CurrScore_text, 10, "%7d", current_score);
		App->fonts->BlitText(20, 21, font_1, CurrScore_text);
		LOG("score = ", current_score);
		////score2
		//sprintf_s(score2_text, 10, "%7d", score2);
		//App->fonts->BlitText(SCREEN_MIDDLE + 73, SCORES_HEIGHT, font_score1, score2_text);
		//App->fonts->BlitText(SCREEN_MIDDLE + 48, SCORES_HEIGHT, font_score1, "P2");

		////highscore print
		//sprintf_s(HighScore_text, 13, "%7d", high_score);
		//App->fonts->BlitText(125, SCORES_HEIGHT + 1, font_highscore, HighScore_text);
		//App->fonts->BlitText(104, SCORES_HEIGHT + 1, font_highscore, "HI-");
	}

	//balls
	if (App->scene_intro->IsEnabled() == true) {
		sprintf_s(balls_text, 10, "%7d", App->player->lifes);
		App->fonts->BlitText(436, 22, font_1, balls_text);
	}

	//Add balls
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN && App->scene_intro->IsEnabled() == true) {
		App->player->lifes++;
	}

	return UPDATE_CONTINUE;
}

void ModuleUI::AddPoints(int points)
{
	current_score += points * multiplier;
}
