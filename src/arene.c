
#include "arene.h"
#include "troupe.h"
#include "affichage.h"

void lancerArene(SDL_Renderer *renderer, TTF_Font *font, int niveau) {
    SDL_Event event;
    int running = 1;

    Tunite* troupes[50];
    int nbTroupes = 0;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = 0;
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                SDL_Point clic = { event.button.x, event.button.y };
                if (clic.y < 600 && nbTroupes < 50) {
                    Tunite *t = creerTroupe("chevalier", clic.x, clic.y, renderer);
                    troupes[nbTroupes++] = t;
                }
            }
        }

        // Affichage du fond
        afficherDamier(renderer);

        // Tours fixes (visuel simplifié, statiques)
        SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255); // Rouge
        SDL_Rect tourRouge = { 300, 100, 40, 60 };
        SDL_RenderFillRect(renderer, &tourRouge);

        SDL_SetRenderDrawColor(renderer, 0, 0, 200, 255); // Bleu
        SDL_Rect tourBleue = { 300, 500, 40, 60 };
        SDL_RenderFillRect(renderer, &tourBleue);

        // Affichage des troupes
        for (int i = 0; i < nbTroupes; i++) {
            troupes[i]->posY -= troupes[i]->vitesseDeplacement;  // Monte vers le haut
            SDL_Rect rect = { (int)troupes[i]->posX, (int)troupes[i]->posY, 64, 64 };
            SDL_RenderCopy(renderer, troupes[i]->imageCarte, NULL, &rect);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16); // 60 FPS
    }
}
