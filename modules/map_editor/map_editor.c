#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#define LARGEURJEU 11
#define HAUTEURJEU 19
#define TAILLE_SPRITE 40

void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination) {
    SDL_Rect offset = { x, y };
    SDL_BlitSurface(source, NULL, destination, &offset);
}

void prepare_sprite(int x, int y, SDL_Surface* source, SDL_Surface* destination) {
    apply_surface(x, y, source, destination);
}

int estCheminTerre(int i, int j) {
    if ((i == 2 && j >= 3 && j <= 8) || (i == 2 && j >= 10 && j <= 15))
        exit(1);
    if (j == 2 && i >= 3 && i <= 5)
        exit(1);
    if (j == 16 && i >= 3 && i <= 5)
        exit(1);
    if ((i == 2 || i == 8) && j == 9)
        return 2;
    if ((i == 8 && j >= 3 && j <= 8) || (i == 8 && j >= 10 && j <= 15))
        exit(1);
    return 0;
}

void prepareAllSpriteJeu(int largeur, int hauteur, SDL_Surface **TabSprite, SDL_Surface* destination) {
    for (int j = 0; j < hauteur; j++) {
        for (int i = 0; i < largeur; i++) {
            int type = estCheminTerre(i, j);
            if (type == 1)
                prepare_sprite(i * TAILLE_SPRITE, j * TAILLE_SPRITE, TabSprite[1], destination);
            else if (type == 2)
                prepare_sprite(i * TAILLE_SPRITE, j * TAILLE_SPRITE, TabSprite[0], destination);
            else if (j == 9)
                prepare_sprite(i * TAILLE_SPRITE, j * TAILLE_SPRITE, TabSprite[0], destination);
            else
                prepare_sprite(i * TAILLE_SPRITE, j * TAILLE_SPRITE, TabSprite[2], destination);
        }
    }
}

void dessinerTourChateau(SDL_Surface* destination, int x, int y, Uint32 couleurToit) {
    Uint32 gris1 = SDL_MapRGB(destination->format, 120, 120, 120);
    Uint32 gris2 = SDL_MapRGB(destination->format, 150, 150, 150);
    Uint32 gris3 = SDL_MapRGB(destination->format, 180, 180, 180);
    Uint32 noir = SDL_MapRGB(destination->format, 0, 0, 0);

    int largeur = TAILLE_SPRITE + 10;
    int hauteur = (TAILLE_SPRITE * 2 - 10) / 2;

    int briqueW = largeur / 6;
    int briqueH = 4;
    int lignesBriques = 10;
    int colonnesBriques = 6;

    for (int row = 0; row < lignesBriques; row++) {
        for (int col = 0; col < colonnesBriques; col++) {
            SDL_Rect brique = {
                .x = x + col * briqueW,
                .y = y + 10 + row * briqueH,
                .w = briqueW,
                .h = briqueH
            };
            Uint32 couleur = (row + col) % 3 == 0 ? gris1 : ((row + col) % 3 == 1 ? gris2 : gris3);
            SDL_FillRect(destination, &brique, couleur);
        }
    }

    for (int i = 0; i < 5; i++) {
        SDL_Rect crenel = {
            .x = x + i * (largeur / 5) + 2,
            .y = y,
            .w = largeur / 6,
            .h = 8
        };
        SDL_FillRect(destination, &crenel, gris3);
    }

    for (int i = 0; i < 10; i++) {
        SDL_Rect etage = {
            .x = x + i,
            .y = y - 10 + (9 - i),
            .w = largeur - 2 * i,
            .h = 1
        };
        SDL_FillRect(destination, &etage, couleurToit);
    }

    SDL_Rect porte = {
        .x = x + largeur / 3,
        .y = y + 10 + lignesBriques * briqueH - 10,
        .w = largeur / 3,
        .h = 10
    };
    SDL_FillRect(destination, &porte, noir);
}

void afficherTours(SDL_Surface* destination) {
    Uint32 rouge = SDL_MapRGB(destination->format, 200, 30, 30);
    Uint32 bleu = SDL_MapRGB(destination->format, 30, 60, 200);

    dessinerTourChateau(destination, 5 * TAILLE_SPRITE - 10, 1 * TAILLE_SPRITE, rouge);
    dessinerTourChateau(destination, 5 * TAILLE_SPRITE - 10, 16 * TAILLE_SPRITE, bleu);

    dessinerTourChateau(destination, 2 * TAILLE_SPRITE, 3 * TAILLE_SPRITE, rouge);
    dessinerTourChateau(destination, 8 * TAILLE_SPRITE, 3 * TAILLE_SPRITE, rouge);
    dessinerTourChateau(destination, 2 * TAILLE_SPRITE, 14 * TAILLE_SPRITE, bleu);
    dessinerTourChateau(destination, 8 * TAILLE_SPRITE, 14 * TAILLE_SPRITE, bleu);
}

void dessinerMurs(SDL_Surface* destination) {
    Uint32 pierre = SDL_MapRGB(destination->format, 100, 100, 100);
    for (int i = 0; i < LARGEURJEU; i++) {
        SDL_Rect mur_haut = { i * TAILLE_SPRITE, 0, TAILLE_SPRITE, 8 };
        SDL_Rect mur_bas = { i * TAILLE_SPRITE, HAUTEURJEU * TAILLE_SPRITE - 8, TAILLE_SPRITE, 8 };
        SDL_FillRect(destination, &mur_haut, pierre);
        SDL_FillRect(destination, &mur_bas, pierre);
    }
}

void lancer_editeur_map(void) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Erreur SDL_Init: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_Window* window = SDL_CreateWindow("Map Chateau Amelioree",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        LARGEURJEU * TAILLE_SPRITE, HAUTEURJEU * TAILLE_SPRITE,
        SDL_WINDOW_SHOWN);

    if (!window) {
        fprintf(stderr, "Erreur SDL_CreateWindow: %s\n", SDL_GetError());
        SDL_Quit();
        exit(1);
    }

    SDL_Surface* screen = SDL_GetWindowSurface(window);

    SDL_Surface* TabSpriteMap[3] = {
        SDL_LoadBMP("./data/Textures/Map/Eau/Eau.bmp"),
        SDL_LoadBMP("./data/Textures/Map/Terre.bmp"),
        SDL_LoadBMP("./data/Textures/Map/Herbes/Herbe.bmp")
    };

    for (int i = 0; i < 3; i++) {
        if (!TabSpriteMap[i]) {
            fprintf(stderr, "Erreur chargement sprite map %d: %s\n", i, SDL_GetError());
            SDL_Quit();
            exit(1);
        }
    }

    prepareAllSpriteJeu(LARGEURJEU, HAUTEURJEU, TabSpriteMap, screen);
    dessinerMurs(screen);
    afficherTours(screen);
    SDL_UpdateWindowSurface(window);

    int running = 1;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
                running = 0;
            }
        }
        SDL_Delay(16);
    }

    for (int i = 0; i < 3; i++) SDL_FreeSurface(TabSpriteMap[i]);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
