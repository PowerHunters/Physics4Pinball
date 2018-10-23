#include <string>
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

using namespace std;

ModuleUI::ModuleUI(Application* app, bool start_enabled) : Module(app, start_enabled) {}

ModuleUI::~ModuleUI() {}

bool ModuleUI::Start()
{
	LOG("Loading UI");

	font_1 = App->fonts->Load("textures/font_1.png", "0123456789", 1);
	font_2 = App->fonts->Load("textures/font_2.png", "0123456789", 1);

	current_score = 0;

	return true;
}

bool ModuleUI::CleanUp()
{
	LOG("Unloading user interface")

	App->fonts->UnLoad(font_1);
	App->fonts->UnLoad(font_2);

	return true;
}
update_status ModuleUI::Update()
{
	//highscore logic
	if (current_score > high_score) {
		high_score = current_score; //App->player1->score
	}

	char const* str_score = nullptr;
	string score = to_string(current_score);
	str_score = score.c_str();
	App->fonts->BlitText(20, 21, font_1, str_score);

	char const* str_HighScore = nullptr;
	string HighScore = to_string(high_score);
	str_HighScore = HighScore.c_str();
	App->fonts->BlitText(224, 21, font_2, str_HighScore);

	
	string lifes = to_string(App->player->lifes);
	char const* str_lifes = lifes.c_str();;
	App->fonts->BlitText(436, 21, font_1, str_lifes);


	////scores
	//
	//	//score1
	//	sprintf_s(CurrScore_text, 10, "%i", current_score);
	//	App->fonts->BlitText(20, 21, font_1, CurrScore_text);

	//	//highscore print
	//	sprintf_s(HighScore_text, 10, "%i", high_score);
	//	App->fonts->BlitText(125, 224, font_2, HighScore_text);

	////balls

	//	sprintf_s(balls_text, 10, "%i", App->player->lifes);
	//	App->fonts->BlitText(436, 22, font_1, balls_text);


	////Add balls
	//if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN && App->scene_intro->IsEnabled() == true) {
	//	App->player->lifes++;
	//}

	return UPDATE_CONTINUE;
}

void ModuleUI::AddPoints(int points)
{
	current_score += points * multiplier;
}
