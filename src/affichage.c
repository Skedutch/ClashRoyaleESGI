#include "affichage.h"

SDL_Texture* loadTexture(const char *path, SDL_Renderer *renderer) {
    SDL_Surface *surface = IMG_Load(path);
    if (!surface) { printf("Erreur image %s: %s\n", path, IMG_GetError()); return NULL; }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

void dessinerBouton(SDL_Renderer *renderer, TTF_Font *font, const char *texte, int x, int y, int pressed, int largeur, int hauteur, int arrondi, Uint8 r, Uint8 g, Uint8 b) {
    int decalage = pressed ? 4 : 0;
    roundedBoxRGBA(renderer, x, y + decalage, x + largeur, y + hauteur + decalage, arrondi, r, g, b, 255);
    SDL_Color textColor = {0, 0, 0, 255};
    SDL_Surface *surfaceTexte = TTF_RenderText_Solid(font, texte, textColor);
    SDL_Texture *textureTexte = SDL_CreateTextureFromSurface(renderer, surfaceTexte);
    SDL_Rect rectTexte = {
        x + (largeur - surfaceTexte->w) / 2,
        y + decalage + (hauteur - surfaceTexte->h) / 2,
        surfaceTexte->w,
        surfaceTexte->h
    };
    SDL_RenderCopy(renderer, textureTexte, NULL, &rectTexte);
    SDL_FreeSurface(surfaceTexte);
    SDL_DestroyTexture(textureTexte);
}

void afficherDamier(SDL_Renderer *renderer) {
    int tileSize = 25;
    for (int y = 0; y < SCREEN_HEIGHT; y += tileSize) {
        for (int x = 0; x < SCREEN_WIDTH; x += tileSize) {
            SDL_SetRenderDrawColor(renderer, ((x / tileSize + y / tileSize) % 2) ? 0 : 13, 26, 74, 255);
            SDL_Rect tile = { x, y, tileSize, tileSize };
            SDL_RenderFillRect(renderer, &tile);
        }
    }
}
