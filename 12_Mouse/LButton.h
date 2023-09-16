#include "./LTexture.h"

// Button constants
#define test
#define BUTTON_WIDTH  300
#define BUTTON_HEIGHT 200
#define TOTAL_BUTTONS   4

enum LButtonSprite {
	BUTTON_SPRITE_MOUSE_OUT = 0,
	BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
	BUTTON_SPRITE_MOUSE_DOWN = 2,
	BUTTON_SPRITE_MOUSE_UP = 3,
	BUTTON_SPRITE_TOTAL = 4
};
typedef enum LButtonSprite LButtonSprite;

struct LButton {
	SDL_Point mPosition;
	LButtonSprite mCurrentSprite;
};
typedef struct LButton LButton;

void initButton(LButton *button);
void setPosition(LButton *button, int x, int y);
void handleEvent(LButton *button, SDL_Event *e);
void renderButton(LButton *button, LTexture *texture, SDL_Renderer *renderer, SDL_Rect *clip);

		

