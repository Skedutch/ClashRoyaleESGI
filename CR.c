#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <stdio.h>

#define SCREEN_HEIGHT 800
#define SCREEN_WIDTH 600

//----------------------------------------------------------------------------------------------------------------------------------------------//

typedef struct {
    SDL_Rect rect;
    SDL_Texture* texture;
} ImageButton;

//----------------------------------------------------------------------------------------------------------------------------------------------//
// Chargement d'une image en texture
SDL_Texture* loadTexture(const char *path, SDL_Renderer *renderer) {
    SDL_Surface *surface = IMG_Load(path);
    if (!surface) {
        printf("Erreur chargement image %s: %s\n", path, IMG_GetError());
        return NULL;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

//----------------------------------------------------------------------------------------------------------------------------------------------//
// Fonction pour dessiner un bouton arrondi avec texte centré + effet "press"
void dessinerBouton1(SDL_Renderer *renderer, TTF_Font *font, const char *texte, int x, int y, int pressed) {
    int largeur = 200;
    int hauteur = 80;
    int arrondi = 20;

    // Couleur plus foncée si pressé
    Uint8 r = pressed ? 220 : 255;
    Uint8 g = pressed ? 180 : 204;
    Uint8 b = pressed ? 0   : 0;
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






//----------------------------------------------------------------------------------------------------------------------------------------------//
// Fonction pour dessiner un bouton arrondi avec texte centré + effet "press"
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


















//----------------------------------------------------------------------------------------------------------------------------------------------//
// FONCTION CRÉATION D'UN BACKGROUND
void afficherDamier(SDL_Renderer *renderer) {
    int tileSize = 25;
    for (int y = 0; y < SCREEN_HEIGHT; y += tileSize) {
        for (int x = 0; x < SCREEN_WIDTH; x += tileSize) {
            if (((x / tileSize) + (y / tileSize)) % 2 == 0) {
                SDL_SetRenderDrawColor(renderer, 13, 38, 74, 255);
            } else {
                SDL_SetRenderDrawColor(renderer, 0, 26, 64, 255);
            }
            SDL_Rect tile = { x, y, tileSize, tileSize };
            SDL_RenderFillRect(renderer, &tile);
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------//
// FONCTION D'AFFICHAGE DU MENU JOUER

void afficherMenuJouer(SDL_Renderer *renderer, TTF_Font *font) {
    SDL_Surface *imgOnlineSurface = IMG_Load("image/Online_menu_jouer.png");
    SDL_Surface *imgBotSurface = IMG_Load("image/Bot_menu_jouer.png");

    if (!imgOnlineSurface || !imgBotSurface) {
        printf("Erreur chargement images menu jouer : %s\n", IMG_GetError());
        return;
    }

    SDL_Texture *imgOnlineTexture = SDL_CreateTextureFromSurface(renderer, imgOnlineSurface);
    SDL_Texture *imgBotTexture = SDL_CreateTextureFromSurface(renderer, imgBotSurface);
    SDL_FreeSurface(imgOnlineSurface);
    SDL_FreeSurface(imgBotSurface);

    SDL_Rect rectOnline = {60, 80, 200, 260};
    SDL_Rect rectBot = {340, 80, 200, 260};

    SDL_Rect boutonRecherche = {SCREEN_WIDTH/2 - 100, 550, 200, 60};
    SDL_Rect boutonFacile = {60, 550, 150, 60};
    SDL_Rect boutonNormal = {230, 550, 150, 60};
    SDL_Rect boutonDifficile = {400, 550, 150, 60};
    SDL_Rect boutonMenu = {SCREEN_WIDTH/2 - 75, 730, 150, 50};

    int mode = 0;  // 0 = rien, 1 = online, 2 = bot
    SDL_Event event;
    int running = 1;

    // === Variables pour effets d’enfoncement
    int pressedRecherche = 0;
    int pressedFacile = 0;
    int pressedNormale = 0;
    int pressedDifficile = 0;
    int pressedMenu = 0;

    // === Variable d’état de la recherche
    int rechercheEnCours = 0;

    while (running) {
        while (SDL_PollEvent(&event)) {
            SDL_Point clic = { event.button.x, event.button.y };

            if (event.type == SDL_QUIT)
                exit(0);
            else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
                running = 0;

            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (SDL_PointInRect(&clic, &rectOnline)) {
                    mode = 1;
                    rechercheEnCours = 0; // Reset quand on change de mode
                    printf("Mode en ligne sélectionné !\n");
                } else if (SDL_PointInRect(&clic, &rectBot)) {
                    mode = 2;
                    rechercheEnCours = 0; // Reset aussi ici
                    printf("Mode contre un bot sélectionné !\n");
                } else if (SDL_PointInRect(&clic, &boutonMenu)) {
                    pressedMenu = 1;
                } else if (mode == 1 && SDL_PointInRect(&clic, &boutonRecherche)) {
                    pressedRecherche = 1;
                } else if (mode == 2) {
                    if (SDL_PointInRect(&clic, &boutonFacile)) pressedFacile = 1;
                    if (SDL_PointInRect(&clic, &boutonNormal)) pressedNormale = 1;
                    if (SDL_PointInRect(&clic, &boutonDifficile)) pressedDifficile = 1;
                }
            }

            else if (event.type == SDL_MOUSEBUTTONUP) {
                if (pressedMenu && SDL_PointInRect(&clic, &boutonMenu)) {
                    printf("Retour au menu principal\n");
                    running = 0;
                } else if (pressedRecherche && SDL_PointInRect(&clic, &boutonRecherche)) {
                    rechercheEnCours = 1;
                    printf("Recherche de partie en ligne...\n");
                } else if (pressedFacile && SDL_PointInRect(&clic, &boutonFacile)) {
                    printf("Difficulté sélectionnée : Facile\n");
                } else if (pressedNormale && SDL_PointInRect(&clic, &boutonNormal)) {
                    printf("Difficulté sélectionnée : Normale\n");
                } else if (pressedDifficile && SDL_PointInRect(&clic, &boutonDifficile)) {
                    printf("Difficulté sélectionnée : Difficile\n");
                }

                // Réinitialisation des appuis
                pressedMenu = pressedRecherche = pressedFacile = pressedNormale = pressedDifficile = 0;
            }
        }

        // === Affichage
        afficherDamier(renderer);

        // Cadre jaune + affichage image
        SDL_SetRenderDrawColor(renderer, 255, 204, 0, 255);
        SDL_RenderDrawRect(renderer, &rectOnline);
        SDL_RenderDrawRect(renderer, &rectBot);
        SDL_RenderCopy(renderer, imgOnlineTexture, NULL, &rectOnline);
        SDL_RenderCopy(renderer, imgBotTexture, NULL, &rectBot);

        // Texte DANS les images
        SDL_Color blanc = {255, 255, 255, 255};

        SDL_Surface *s1 = TTF_RenderText_Solid(font, "Partie en ligne", blanc);
        SDL_Texture *t1 = SDL_CreateTextureFromSurface(renderer, s1);
        SDL_Rect r1 = { rectOnline.x + (rectOnline.w - s1->w)/2, rectOnline.y + rectOnline.h - s1->h - 10, s1->w, s1->h };
        SDL_RenderCopy(renderer, t1, NULL, &r1);
        SDL_FreeSurface(s1); SDL_DestroyTexture(t1);

        SDL_Surface *s2 = TTF_RenderText_Solid(font, "Partie hors-ligne", blanc);
        SDL_Texture *t2 = SDL_CreateTextureFromSurface(renderer, s2);
        SDL_Rect r2 = { rectBot.x + (rectBot.w - s2->w)/2, rectBot.y + rectBot.h - s2->h - 10, s2->w, s2->h };
        SDL_RenderCopy(renderer, t2, NULL, &r2);
        SDL_FreeSurface(s2); SDL_DestroyTexture(t2);

        // === Affichage des boutons selon le mode
        if (mode == 1) {
            // Déterminer couleur et texte dynamiquement
            const char *texteRecherche = rechercheEnCours ? "Recherche en cours" : "Rechercher une partie";
            Uint8 r = rechercheEnCours ? 200 : 255;
            Uint8 g = rechercheEnCours ? 30  : 204;
            Uint8 b = rechercheEnCours ? 30  : 0;

            dessinerBouton(renderer, font, texteRecherche, boutonRecherche.x, boutonRecherche.y,
                           pressedRecherche, boutonRecherche.w, boutonRecherche.h, 20, r, g, b);
        } else if (mode == 2) {
            dessinerBouton(renderer, font, "Facile", boutonFacile.x, boutonFacile.y, pressedFacile, 150, 60, 20, 255, 204, 0);
            dessinerBouton(renderer, font, "Normale", boutonNormal.x, boutonNormal.y, pressedNormale, 150, 60, 20, 255, 204, 0);
            dessinerBouton(renderer, font, "Difficile", boutonDifficile.x, boutonDifficile.y, pressedDifficile, 150, 60, 20, 255, 204, 0);
        }

        dessinerBouton(renderer, font, "Menu", boutonMenu.x, boutonMenu.y, pressedMenu, 150, 50, 20, 255, 204, 0);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(imgOnlineTexture);
    SDL_DestroyTexture(imgBotTexture);
}


//----------------------------------------------------------------------------------------------------------------------------------------------//
// FONCTION D'AFFICHAGE DE L'ÉCRAN DECK

void afficherDeck(SDL_Renderer *renderer, TTF_Font *font) {
    int running = 1;
    SDL_Event event;
    int boutonDeckPressed = 0;
    int boutonJouerPressed = 0;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) exit(0);
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                SDL_Point clic = { event.button.x, event.button.y };
                if (SDL_PointInRect(&clic, &(SDL_Rect){75, 650, 200, 80})) {
                    boutonDeckPressed = 1;
                }
                if (SDL_PointInRect(&clic, &(SDL_Rect){325, 650, 200, 80})) {
                    boutonJouerPressed = 1;
                }
            } else if (event.type == SDL_MOUSEBUTTONUP) {
                SDL_Point clic = { event.button.x, event.button.y };
                if (SDL_PointInRect(&clic, &(SDL_Rect){75, 650, 200, 80}) && boutonDeckPressed) {
                    printf("Bouton ARÈNE cliqué ! Retour au menu\n");
                    running = 0;
                }
                if (SDL_PointInRect(&clic, &(SDL_Rect){325, 650, 200, 80}) && boutonJouerPressed) {
                    printf("Bouton JOUER cliqué depuis le Deck !\n");
                    afficherMenuJouer(renderer, font);
                }
                boutonDeckPressed = boutonJouerPressed = 0;
            }
        }

        afficherDamier(renderer);

        for (int i = 0; i < 8; i++) {
            SDL_Rect carte = {
                50 + (i % 4) * 130,
                100 + (i / 4) * 200,
                100,
                150
            };
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &carte);
        }

        SDL_Color blanc = {255, 255, 255, 255};
        SDL_Surface *textSurface = TTF_RenderText_Solid(font, "Mon Deck", blanc);
        SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_Rect textRect = { (SCREEN_WIDTH - textSurface->w)/2, 20, textSurface->w, textSurface->h };
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);

        dessinerBouton(renderer, font, "Arene", 75, 650, boutonDeckPressed, 200,80,20, boutonDeckPressed ? 220 : 255, boutonDeckPressed ? 180 : 204,boutonDeckPressed ? 0   : 0);
        dessinerBouton(renderer, font, "Jouer", 325, 650, boutonJouerPressed, 200,80,20, boutonJouerPressed ? 220 : 255, boutonJouerPressed ? 180 : 204,boutonJouerPressed ? 0   : 0);

        SDL_RenderPresent(renderer);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------//
// MAIN
int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) { printf("Erreur SDL: %s\n", SDL_GetError()); return 1; }
    if (IMG_Init(IMG_INIT_PNG) == 0) { printf("Erreur SDL_image: %s\n", IMG_GetError()); return 1; }
    if (TTF_Init() < 0) { printf("Erreur SDL_ttf: %s\n", TTF_GetError()); return 1; }

    TTF_Font *font = TTF_OpenFont("police/arial.ttf", 28);
    if (!font) { printf("Erreur chargement police: %s\n", TTF_GetError()); return 1; }

    SDL_Window *window = SDL_CreateWindow("Clash Royale - Menu", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Rect rectDeck = {75, 650, 200, 80};
    SDL_Rect rectJouer = {325, 650, 200, 80};
    int running = 1;
    SDL_Event event;
    int boutonDeckPressed = 0;
    int boutonJouerPressed = 0;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = 0;
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                SDL_Point clic = { event.button.x, event.button.y };
                if (SDL_PointInRect(&clic, &rectDeck)) boutonDeckPressed = 1;
                if (SDL_PointInRect(&clic, &rectJouer)) boutonJouerPressed = 1;
            } else if (event.type == SDL_MOUSEBUTTONUP) {
                SDL_Point clic = { event.button.x, event.button.y };
                if (SDL_PointInRect(&clic, &rectDeck) && boutonDeckPressed) {
                    printf("Bouton DECK cliqué ! Affichage du deck...\n");
                    afficherDeck(renderer, font);
                }
                if (SDL_PointInRect(&clic, &rectJouer) && boutonJouerPressed) {
                    printf("Bouton JOUER cliqué depuis le menu principal !\n");
                    afficherMenuJouer(renderer, font);
                }
                boutonDeckPressed = boutonJouerPressed = 0;
            }
        }

        afficherDamier(renderer);
        dessinerBouton(renderer, font, "Deck", 75, 650, boutonDeckPressed, 200,80,20, boutonDeckPressed ? 220 : 255, boutonDeckPressed ? 180 : 204,boutonDeckPressed ? 0   : 0);
        dessinerBouton(renderer, font, "Jouer", 325, 650, boutonJouerPressed, 200,80,20, boutonJouerPressed ? 220 : 255, boutonJouerPressed ? 180 : 204,boutonJouerPressed ? 0   : 0);
        SDL_RenderPresent(renderer);
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    return 0;
}
