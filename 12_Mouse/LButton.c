#include <SDL.h>
#include "./LButton.h"

void initButton(LButton *button) {
	(button->mPosition).x = 0;
	(button->mPosition).y = 0;

	button->mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
}
void setPosition(LButton *button, int x, int y) {
	(button->mPosition).x = x;
	(button->mPosition).y = y;
}

void handleEvent(LButton *button, SDL_Event *e) {
	if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN ||
			e->type == SDL_MOUSEBUTTONUP) {
		int x, y;
		SDL_GetMouseState(&x, &y);

		int inside = 1;

		if (x < (button->mPosition).x)
			inside = 0;
		else if (x > (button->mPosition).x + BUTTON_WIDTH)
			inside = 0;
		else if (y < (button->mPosition).y)
			inside = 0;
		else if (y > (button->mPosition).y + BUTTON_HEIGHT)
			inside = 0;

		if (!inside)
			button->mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
		else {
			switch (e->type) {
				case SDL_MOUSEMOTION:
					button->mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
					break;

				case SDL_MOUSEBUTTONDOWN:
					button->mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
					break;

				case SDL_MOUSEBUTTONUP:
					button->mCurrentSprite = BUTTON_SPRITE_MOUSE_UP;
					break;
			}
		}
	}
}

void renderButton(LButton *button, LTexture *texture, SDL_Renderer *renderer, SDL_Rect *clip) {
	renderTexture(texture, renderer, (button->mPosition).x, (button->mPosition).y, clip, 0, NULL, SDL_FLIP_NONE);
}
