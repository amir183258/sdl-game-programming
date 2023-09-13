struct LTexture {
	SDL_Texture *mTexture;

	int mWidth;
	int mHeight;
};
typedef struct LTexture LTexture;

int loadFromFile(LTexture *texture, SDL_Renderer *renderer, char *path);
void freeTexture(LTexture *texture);
void renderTexture(LTexture *texture, SDL_Renderer *renderer, int x, int y, SDL_Rect *clip);
int getTextureWidth(LTexture *texture);
int getTextureHeight(LTexture *texture);
void initTexture(LTexture *texture);
void setColor(LTexture *texture, Uint8 red, Uint8 green, Uint8 blue);
void setBlendMode(LTexture *texture, SDL_BlendMode blending);
void setAlpha(LTexture *texture, Uint8 alpha);

