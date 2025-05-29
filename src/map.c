#include "map.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 800
#define LARGEURJEU 11
#define HAUTEURJEU 19
#define TAILLE_SPRITE_X (SCREEN_WIDTH / LARGEURJEU)
#define TAILLE_SPRITE_Y (SCREEN_HEIGHT / HAUTEURJEU)

extern void afficherMenuJouer(SDL_Renderer* renderer, TTF_Font* font);

void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination) {
    SDL_Rect offset = { x, y, TAILLE_SPRITE_X, TAILLE_SPRITE_Y };
    SDL_BlitScaled(source, NULL, destination, &offset);
}

void prepare_sprite(int x, int y, SDL_Surface* source, SDL_Surface* destination) {
    apply_surface(x, y, source, destination);
}

int estTypeDeCase(int i, int j) {
    if ((j == 9) && (i == 2 || i == 8)) return 2;
    if (j == 9) return 0;
    if ((i == 2 || i == 8) && ((j >= 3 && j <= 8) || (j >= 10 && j <= 15))) return 1;
    if ((j == 3 || j == 15) && (i >= 3 && i <= 7)) return 1;
    return 3;
}

void prepareAllSpriteJeu(int largeur, int hauteur, SDL_Surface **TabSprite, SDL_Surface* destination) {
    for (int j = 0; j < hauteur; j++) {
        for (int i = 0; i < largeur; i++) {
            int type = estTypeDeCase(i, j);
            int x = i * TAILLE_SPRITE_X;
            int y = j * TAILLE_SPRITE_Y;

            switch (type) {
                case 0: prepare_sprite(x, y, TabSprite[0], destination); break;
                case 1: prepare_sprite(x, y, TabSprite[1], destination); break;
                case 2: prepare_sprite(x, y, TabSprite[3], destination); break;
                case 3:
                    if ((i + j) % 5 == 0 && TabSprite[4] != NULL)
                        prepare_sprite(x, y, TabSprite[4], destination);
                    else
                        prepare_sprite(x, y, TabSprite[2], destination);
                    break;
            }
        }
    }
}

void dessinerTourChateau(SDL_Surface* destination, int x, int y, Uint32 couleurToit, int estTourRoi) {
    Uint32 gris1 = SDL_MapRGB(destination->format, 120, 120, 120);
    Uint32 gris2 = SDL_MapRGB(destination->format, 150, 150, 150);
    Uint32 gris3 = SDL_MapRGB(destination->format, 180, 180, 180);
    Uint32 noir = SDL_MapRGB(destination->format, 0, 0, 0);

    int largeur = TAILLE_SPRITE_X + 20;
    int briqueW = largeur / 6;
    int briqueH = 5;
    int lignesBriques = 12;
    int colonnesBriques = 6;

    for (int row = 0; row < lignesBriques; row++) {
        for (int col = 0; col < colonnesBriques; col++) {
            SDL_Rect brique = { x + col * briqueW, y + 12 + row * briqueH, briqueW, briqueH };
            Uint32 couleur = (row + col) % 3 == 0 ? gris1 : ((row + col) % 3 == 1 ? gris2 : gris3);
            SDL_FillRect(destination, &brique, couleur);
        }
    }

    for (int i = 0; i < 5; i++) {
        SDL_Rect crenel = { x + i * (largeur / 5) + 3, y + 4, largeur / 6, 8 };
        SDL_FillRect(destination, &crenel, gris3);
    }

    for (int i = 0; i < 10; i++) {
        SDL_Rect etage = { x + i, y - 10 + (9 - i), largeur - 2 * i, 1 };
        SDL_FillRect(destination, &etage, couleurToit);
    }

    SDL_Rect porte = { x + largeur / 3, y + 12 + lignesBriques * briqueH - 8, largeur / 3, 12 };
    SDL_FillRect(destination, &porte, noir);

    SDL_Rect base = { x - 4, y + 12 + lignesBriques * briqueH, largeur + 8, 6 };
    SDL_FillRect(destination, &base, gris2);

    if (estTourRoi) {
        Uint32 jaune = SDL_MapRGB(destination->format, 255, 204, 0);

        int couronneLargeur = 28;
        int cx = x + TAILLE_SPRITE_X / 2 - couronneLargeur / 2 + 8;
        int cy = y + 26;

        SDL_Rect bandeGauche = { cx - 6, cy - 2, 4, 18 };
        SDL_Rect bandeDroite = { cx + couronneLargeur + 2, cy - 2, 4, 18 };
        SDL_FillRect(destination, &bandeGauche, jaune);
        SDL_FillRect(destination, &bandeDroite, jaune);

        SDL_Rect p1 = { cx + 4,  cy - 2, 4, 4 };
        SDL_Rect p2 = { cx + 12, cy - 6, 4, 4 };
        SDL_Rect p3 = { cx + 20, cy - 2, 4, 4 };
        SDL_FillRect(destination, &p1, jaune);
        SDL_FillRect(destination, &p2, jaune);
        SDL_FillRect(destination, &p3, jaune);

        SDL_Rect corps = { cx + 2, cy, 24, 10 };
        SDL_FillRect(destination, &corps, jaune);

        SDL_Rect bande1 = { cx + 2, cy + 10, 24, 2 };
        SDL_Rect bande2 = { cx + 2, cy + 12, 24, 2 };
        SDL_FillRect(destination, &bande1, noir);
        SDL_FillRect(destination, &bande2, noir);
    }
}

void afficherTours(SDL_Surface* destination) {
    Uint32 rouge = SDL_MapRGB(destination->format, 200, 30, 30);
    Uint32 bleu = SDL_MapRGB(destination->format, 30, 60, 200);

    dessinerTourChateau(destination, 5 * TAILLE_SPRITE_X - 14, 1 * TAILLE_SPRITE_Y, rouge, 1);
    dessinerTourChateau(destination, 5 * TAILLE_SPRITE_X - 14, 16 * TAILLE_SPRITE_Y, bleu, 1);
    dessinerTourChateau(destination, 2 * TAILLE_SPRITE_X - 6, 3 * TAILLE_SPRITE_Y, rouge, 0);
    dessinerTourChateau(destination, 8 * TAILLE_SPRITE_X - 6, 3 * TAILLE_SPRITE_Y, rouge, 0);
    dessinerTourChateau(destination, 2 * TAILLE_SPRITE_X - 6, 14 * TAILLE_SPRITE_Y, bleu, 0);
    dessinerTourChateau(destination, 8 * TAILLE_SPRITE_X - 6, 14 * TAILLE_SPRITE_Y, bleu, 0);
}

void dessinerMurs(SDL_Surface* destination) {
    Uint32 pierre = SDL_MapRGB(destination->format, 100, 100, 100);
    for (int i = 0; i < LARGEURJEU; i++) {
        SDL_Rect mur_haut = { i * TAILLE_SPRITE_X, 0, TAILLE_SPRITE_X, 8 };
        SDL_Rect mur_bas = { i * TAILLE_SPRITE_X, SCREEN_HEIGHT - 8, TAILLE_SPRITE_X, 8 };
        SDL_FillRect(destination, &mur_haut, pierre);
        SDL_FillRect(destination, &mur_bas, pierre);
    }
}

void afficherMenuPause(SDL_Surface* screen, SDL_Window* window, TTF_Font* font, SDL_Renderer* renderer) {
    SDL_Rect pauseBarG = { 20, 20, 10, 40 };
    SDL_Rect pauseBarD = { 40, 20, 10, 40 };
    SDL_FillRect(screen, &pauseBarG, SDL_MapRGB(screen->format, 255, 255, 255));
    SDL_FillRect(screen, &pauseBarD, SDL_MapRGB(screen->format, 255, 255, 255));

    SDL_Rect boutonMenu = { SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 50, 200, 50 };
    SDL_Rect boutonContinuer = { SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 20, 200, 50 };

    Uint32 grisFonce = SDL_MapRGB(screen->format, 60, 60, 60);
    SDL_FillRect(screen, &boutonMenu, grisFonce);
    SDL_FillRect(screen, &boutonContinuer, grisFonce);

    SDL_Color blanc = {255, 255, 255};
    SDL_Surface* texteMenu = TTF_RenderText_Blended(font, "Menu", blanc);
    SDL_Surface* texteContinuer = TTF_RenderText_Blended(font, "Continuer", blanc);

    if (texteMenu && texteContinuer) {
        SDL_Rect posTexteMenu = {
            boutonMenu.x + (boutonMenu.w - texteMenu->w) / 2,
            boutonMenu.y + (boutonMenu.h - texteMenu->h) / 2,
            texteMenu->w,
            texteMenu->h
        };
        SDL_Rect posTexteContinuer = {
            boutonContinuer.x + (boutonContinuer.w - texteContinuer->w) / 2,
            boutonContinuer.y + (boutonContinuer.h - texteContinuer->h) / 2,
            texteContinuer->w,
            texteContinuer->h
        };

        SDL_BlitSurface(texteMenu, NULL, screen, &posTexteMenu);
        SDL_BlitSurface(texteContinuer, NULL, screen, &posTexteContinuer);

        SDL_FreeSurface(texteMenu);
        SDL_FreeSurface(texteContinuer);
    }

    SDL_UpdateWindowSurface(window);

    int menuActif = 1;
    SDL_Event e;
    while (menuActif && SDL_WaitEvent(&e)) {
        if (e.type == SDL_QUIT)
            exit(0);
        else if (e.type == SDL_MOUSEBUTTONDOWN) {
            int x = e.button.x, y = e.button.y;
            if (x >= boutonMenu.x && x <= boutonMenu.x + boutonMenu.w && y >= boutonMenu.y && y <= boutonMenu.y + boutonMenu.h) {
                menuActif = 0;
                afficherMenuJouer(renderer, font);
            }
            else if (x >= boutonContinuer.x && x <= boutonContinuer.x + boutonContinuer.w && y >= boutonContinuer.y && y <= boutonContinuer.y + boutonContinuer.h) {
                menuActif = 0;
            }
        }
    }
}

void lancer_editeur_map(SDL_Renderer* renderer, TTF_Font* font) {
    SDL_Surface* screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
                                                0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

    SDL_Surface* TabSpriteMap[5] = {
        SDL_LoadBMP("image/map/Eau/Eau.bmp"),
        SDL_LoadBMP("image/map/Terre.bmp"),
        SDL_LoadBMP("image/map/Herbes/Herbe.bmp"),
        SDL_LoadBMP("image/map/Pont.bmp"),
        NULL
    };

    for (int i = 0; i < 4; i++) {
        if (!TabSpriteMap[i]) {
            fprintf(stderr, "Erreur chargement sprite map %d: %s\n", i, SDL_GetError());
            exit(1);
        }
    }

    srand(time(NULL));
    prepareAllSpriteJeu(LARGEURJEU, HAUTEURJEU, TabSpriteMap, screen);
    dessinerMurs(screen);
    afficherTours(screen);

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, screen);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);

    int running = 1;
    SDL_Event event;
    SDL_Window* window = SDL_GetWindowFromID(1); // ou passe-le en paramètre si tu veux être plus précis

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                afficherMenuPause(screen, window, font, renderer);
            }
        }
        SDL_Delay(16);
    }

    for (int i = 0; i < 5; i++) {
        if (TabSpriteMap[i]) SDL_FreeSurface(TabSpriteMap[i]);
    }

    SDL_FreeSurface(screen);
    SDL_DestroyTexture(texture);
}
