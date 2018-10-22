#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleFonts.h"
#include "ModuleUI.h"
#include "ModulePlayer.h"
#include <stdio.h>

ModuleUI::ModuleUI(Application* app, bool start_enabled) : Module(app, start_enabled) {}

ModuleUI::~ModuleUI() {}

bool ModuleUI::Start()
{
	LOG("Loading UI");

	font_score = App->fonts->Load("Assets/Fonts/font_score.png", "0123456789", 1);

	current_score = 0;
	previous_score = 0;
	balls = 5;

	return true;
}

bool ModuleUI::CleanUp()
{
	LOG("Unloading user interface")

	App->fonts->UnLoad(font_score);

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

	////scores
	//if (App->stage1->IsEnabled() == true && App->gameover->IsEnabled() == false) {
	//	//score1
	//	sprintf_s(score1_text, 10, "%7d", score1);
	//	App->fonts->BlitText(35, SCORES_HEIGHT, font_score1, score1_text);
	//	App->fonts->BlitText(10, SCORES_HEIGHT, font_score1, "P1");

	//	//score2
	//	sprintf_s(score2_text, 10, "%7d", score2);
	//	App->fonts->BlitText(SCREEN_MIDDLE + 73, SCORES_HEIGHT, font_score1, score2_text);
	//	App->fonts->BlitText(SCREEN_MIDDLE + 48, SCORES_HEIGHT, font_score1, "P2");

	//	//highscore print
	//	sprintf_s(HighScore_text, 13, "%7d", high_score);
	//	App->fonts->BlitText(125, SCORES_HEIGHT + 1, font_highscore, HighScore_text);
	//	App->fonts->BlitText(104, SCORES_HEIGHT + 1, font_highscore, "HI-");
	//}

	//if (App->stage4->IsEnabled() == true && App->gameover->IsEnabled() == false) {
	//	//score1
	//	sprintf_s(score1_text, 10, "%7d", score1);
	//	App->fonts->BlitText(35, SCORES_HEIGHT, font_score1, score1_text);
	//	App->fonts->BlitText(10, SCORES_HEIGHT, font_score1, "P1");

	//	//score2
	//	sprintf_s(score2_text, 10, "%7d", score2);
	//	App->fonts->BlitText(SCREEN_MIDDLE + 73, SCORES_HEIGHT, font_score1, score2_text);
	//	App->fonts->BlitText(SCREEN_MIDDLE + 48, SCORES_HEIGHT, font_score1, "P2");

	//	//highscore print
	//	sprintf_s(HighScore_text, 13, "%7d", high_score);
	//	App->fonts->BlitText(125, SCORES_HEIGHT + 1, font_highscore, HighScore_text);
	//	App->fonts->BlitText(104, SCORES_HEIGHT + 1, font_highscore, "HI-");
	//}

	////final score in stage
	///*if (App->stageclear->IsEnabled() == true && App->gameover->IsEnabled() == false) {
	//sprintf_s(score_text, 10, "%7d", App->player1->score);
	//App->fonts->BlitText(20, 100, font_score, score_text);
	//}*/

	////credits
	//if (App->neogeo->IsEnabled() == false) {

	//	if (App->input->keyboard[SDL_SCANCODE_LCTRL] == KEY_STATE::KEY_DOWN || App->input->buttons1[SDL_CONTROLLER_BUTTON_DPAD_UP] == KEY_STATE::KEY_DOWN || App->input->buttons2[SDL_CONTROLLER_BUTTON_DPAD_UP] == KEY_STATE::KEY_DOWN) {
	//		if (credit < 99) {
	//			App->audio->PlayFx(credit_fx);
	//			credit++;
	//			empty_credit = false;
	//		}

	//	}

	//	sprintf_s(credits_text, 17, "%7d", credit);

	//	if (credit == 0) {
	//		credits_rect.w = 38;
	//		App->renderer->Blit(user_interface, CREDITS_X, CREDITS_HEIGHT, &credits_rect, 0.0f);
	//		App->fonts->BlitText(272, CREDITS_HEIGHT, font_credits, "00"); //280
	//	}

	//	else if (credit == 1) {
	//		credits_rect.w = 38;
	//		App->renderer->Blit(user_interface, CREDITS_X, CREDITS_HEIGHT, &credits_rect, 0.0f);
	//		App->fonts->BlitText(272, CREDITS_HEIGHT, font_credits, "01"); //280
	//	}

	//	else if (credit >= 2) {
	//		credits_rect.w = 45;
	//		App->renderer->Blit(user_interface, CREDITS_X, CREDITS_HEIGHT, &credits_rect, 0.0f);
	//		if (credit < 10) {
	//			App->fonts->BlitText(272, CREDITS_HEIGHT, font_credits, "0"); //280
	//			App->fonts->BlitText(232, CREDITS_HEIGHT, font_credits, credits_text); //240
	//		}
	//		else
	//			App->fonts->BlitText(232, CREDITS_HEIGHT, font_credits, credits_text); //240
	//	}

	//	//Main Menu Credits
	//	if ((App->input->Keyboard[SDL_SCANCODE_1] == KEY_STATE::KEY_DOWN || App->input->buttons1[SDL_CONTROLLER_BUTTON_A] == KEY_STATE::KEY_DOWN) && App->mainmenu->IsEnabled() == true && credit_on == true) {
	//		credit--;
	//		credit_on = false;

	//	}
	//	if ((App->input->Keyboard[SDL_SCANCODE_2] == KEY_STATE::KEY_DOWN || App->input->buttons2[SDL_CONTROLLER_BUTTON_A] == KEY_STATE::KEY_DOWN) && App->mainmenu->IsEnabled() == true && credit >= 2 && credit_on == true) {
	//		credit -= 2;
	//		credit_on = false;

	//	}
	//}

	return UPDATE_CONTINUE;
}
