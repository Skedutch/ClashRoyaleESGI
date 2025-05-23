#ifndef ARENE_H
#define ARENE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

// Fonction principale utilisée par menu_jouer.c
void lancerArene(SDL_Renderer *renderer, TTF_Font *font, int niveau);

#endif
