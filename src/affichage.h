#ifndef AFFICHAGE_H
#define AFFICHAGE_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#define SCREEN_HEIGHT 800
#define SCREEN_WIDTH 600

void afficherDamier(SDL_Renderer *renderer);
void dessinerBouton(SDL_Renderer *renderer, TTF_Font *font, const char *texte, int x, int y, int pressed, int largeur, int hauteur, int arrondi, Uint8 r, Uint8 g, Uint8 b);
void dessinerTitre(SDL_Renderer *renderer, TTF_Font *font, const char *texte, int x, int y, float scale);
SDL_Texture* loadTexture(const char *path, SDL_Renderer *renderer);

#endif