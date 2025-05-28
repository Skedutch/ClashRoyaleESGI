#include "affichage.h"
#include "troupe.h"
#include "deck.h"
#include "menu_jouer.h"
#include "map.h"

int main(int argc, char *argv[]) {
    (void)argc; (void)argv;
    if (SDL_Init(SDL_INIT_VIDEO) < 0 || IMG_Init(IMG_INIT_PNG) == 0 || TTF_Init() < 0) {
        printf("Erreur init SDL: %s\n", SDL_GetError());
        return 1;
    }

    TTF_Font *font = TTF_OpenFont("police/arial.ttf", 28);
    if (!font) { printf("Erreur police: %s\n", TTF_GetError()); return 1; }

    SDL_Window *window = SDL_CreateWindow("Clash Royale", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    int running = 1;
    SDL_Event event;
    int boutonDeckPressed = 0, boutonJouerPressed = 0;
    SDL_Rect rectDeck = {75, 650, 200, 80}, rectJouer = {325, 650, 200, 80};

    Joueur joueur;
    initialiserJoueur(&joueur, renderer);


    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = 0;

            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                SDL_Point clic = { event.button.x, event.button.y };
                if (SDL_PointInRect(&clic, &rectDeck)) boutonDeckPressed = 1;
                if (SDL_PointInRect(&clic, &rectJouer)) boutonJouerPressed = 1;
            }

            else if (event.type == SDL_MOUSEBUTTONUP) {
                SDL_Point clic = { event.button.x, event.button.y };
                if (SDL_PointInRect(&clic, &rectDeck) && boutonDeckPressed) {
                    afficherDeck(renderer, font, &joueur);
                }
                if (SDL_PointInRect(&clic, &rectJouer) && boutonJouerPressed) {
                    afficherMenuJouer(renderer, font);
                }
                boutonDeckPressed = boutonJouerPressed = 0;
            }
        }

        afficherDamier(renderer);
        dessinerBouton(renderer, font, "Deck", 75, 650, boutonDeckPressed, 200,80,20, 255, 204, 0);
        dessinerBouton(renderer, font, "Jouer", 325, 650, boutonJouerPressed, 200,80,20, 255, 204, 0);
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