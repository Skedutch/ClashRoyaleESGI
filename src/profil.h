#ifndef PROFIL_H
#define PROFIL_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "troupe.h"
#include "affichage.h"


void dessinerProfil(SDL_Renderer *renderer, TTF_Font *font, Joueur *joueur, SDL_Texture *tropheeTexture);
void demanderPseudo(SDL_Renderer *renderer, TTF_Font *font, char *nom, size_t maxLen, char *avatarPath);
void choisirAvatar(SDL_Renderer *renderer, TTF_Font *font, char *avatarPath);

#endif
