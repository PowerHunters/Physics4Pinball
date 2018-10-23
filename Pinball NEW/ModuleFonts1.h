#ifndef __ModuleFonts1_H__
#define __ModuleFonts1_H__

#include "Module.h"
#include "SDL\include\SDL_pixels.h"

#define MAX_FONTS 10
#define MAX_FONT_CHARS 256

struct SDL_Texture;

struct Font1
{
	char table[MAX_FONT_CHARS];
	SDL_Texture* graphic = nullptr;
	uint rows, len, char_w, char_h, row_chars;
};

class ModuleFonts1 : public Module
{
public:

	ModuleFonts1(Application* app, bool start_enabled = true);
	~ModuleFonts1();

	// Load Font
	int Load(const char* texture_path, const char* characters, uint rows = 1);
	void UnLoad(int font_id);

	// Create a surface from text
	void BlitText(int x, int y, int bmp_font_id, const char* text) const;

private:

	Font1 fonts[MAX_FONTS];
};


#endif // __ModuleFonts1_H_