#include "affichage.h"
#include "menu_jouer.h"
#include "arene.h"
#include "map.h"

void afficherMenuJouer(SDL_Renderer *renderer, TTF_Font *font) {
    SDL_Surface *imgOnlineSurface = IMG_Load("image/Online_menu_jouer.png");
    SDL_Surface *imgBotSurface = IMG_Load("image/Bot_menu_jouer.png");
    if (!imgOnlineSurface || !imgBotSurface) return;

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

    int mode = 0, running = 1;
    int pressedRecherche = 0, pressedFacile = 0, pressedNormale = 0, pressedDifficile = 0, pressedMenu = 0;
    int rechercheEnCours = 0;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            SDL_Point clic = { event.button.x, event.button.y };
            if (event.type == SDL_QUIT) exit(0);
            else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) running = 0;
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (SDL_PointInRect(&clic, &rectOnline)) { mode = 1; rechercheEnCours = 0; }
                else if (SDL_PointInRect(&clic, &rectBot)) { mode = 2; rechercheEnCours = 0; }
                else if (SDL_PointInRect(&clic, &boutonMenu)) pressedMenu = 1;
                else if (mode == 1 && SDL_PointInRect(&clic, &boutonRecherche)) pressedRecherche = 1;
                else if (mode == 2) {
                    if (SDL_PointInRect(&clic, &boutonFacile)) pressedFacile = 1;
                    if (SDL_PointInRect(&clic, &boutonNormal)) pressedNormale = 1;
                    if (SDL_PointInRect(&clic, &boutonDifficile)) pressedDifficile = 1;
                }
            } else if (event.type == SDL_MOUSEBUTTONUP) {
                if (pressedMenu && SDL_PointInRect(&clic, &boutonMenu)) running = 0;
                else if (pressedRecherche && SDL_PointInRect(&clic, &boutonRecherche)) rechercheEnCours = 1;
                else if (pressedFacile && SDL_PointInRect(&clic, &boutonFacile)) {
                    lancer_editeur_map(renderer, font); // ← affichage de ta map
                    lancerArene(renderer, font, 1); // ← lancement du niveau Facile
                    running = 0;
                }
                else if (pressedNormale && SDL_PointInRect(&clic, &boutonNormal)) {
                    lancer_editeur_map(renderer, font); // ← affichage de ta map
                    lancerArene(renderer, font, 2); // ← lancement du niveau Moyen
                    running = 0;
                }
                else if (pressedDifficile && SDL_PointInRect(&clic, &boutonDifficile)) {
                    lancer_editeur_map(renderer, font); // ← affichage de ta map
                    lancerArene(renderer, font, 3); // ← lancement du niveau Difficile
                    running = 0;
                }

                pressedMenu = pressedRecherche = pressedFacile = pressedNormale = pressedDifficile = 0;
            }
        }

        afficherDamier(renderer);
        SDL_RenderDrawRect(renderer, &rectOnline);
        SDL_RenderDrawRect(renderer, &rectBot);
        SDL_RenderCopy(renderer, imgOnlineTexture, NULL, &rectOnline);
        SDL_RenderCopy(renderer, imgBotTexture, NULL, &rectBot);

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

        if (mode == 1) {
            const char *texteRecherche = rechercheEnCours ? "Recherche en cours" : "Rechercher une partie";
            Uint8 r = rechercheEnCours ? 200 : 255, g = rechercheEnCours ? 30 : 204, b = rechercheEnCours ? 30 : 0;
            dessinerBouton(renderer, font, texteRecherche, boutonRecherche.x, boutonRecherche.y, pressedRecherche, boutonRecherche.w, boutonRecherche.h, 20, r, g, b);
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
