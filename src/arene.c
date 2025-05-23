
#include "arene.h"
#include "troupe.h"
#include "affichage.h"

#define TAILLE_SPRITE 40
#define LARGEURJEU 11
#define HAUTEURJEU 19
#define MAX_TROUPES 100
#define NB_CARTES 4
#define MAX_ELIXIR 10

typedef struct {
    SDL_Texture* texture;
    float x, y;
    float vitesse;
    SDL_Rect rect;
    int actif;
    int lane;
    int camp; // 0 = joueur, 1 = bot
    int vie;
    int degats;
} Troupe;

typedef struct {
    const char* nomFichier;
    SDL_Texture* texture;
    SDL_Rect bouton;
    int coutElixir;
    int vie;
    int degats;
} Carte;

typedef struct {
    SDL_Rect rect;
    int vie;
    SDL_Color couleur;
} Tour;

int collision(SDL_Rect a, SDL_Rect b) {
    return SDL_HasIntersection(&a, &b);
}



void afficherElixir(SDL_Renderer* renderer, TTF_Font* font, int elixir) {
    char txt[32];
    sprintf(txt, "Elixir: %d / %d", elixir, MAX_ELIXIR);
    SDL_Color blanc = {255, 255, 255, 255};
    SDL_Surface* surf = TTF_RenderText_Solid(font, txt, blanc);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_Rect r = { 500, 780, surf->w, surf->h };
    SDL_RenderCopy(renderer, tex, NULL, &r);
    SDL_FreeSurface(surf);
    SDL_DestroyTexture(tex);
}

void dessinerTour(SDL_Renderer* renderer, Tour tour) {
    SDL_SetRenderDrawColor(renderer, tour.couleur.r, tour.couleur.g, tour.couleur.b, 255);
    SDL_RenderFillRect(renderer, &tour.rect);
}

void lancerArene(SDL_Renderer *renderer, TTF_Font *font, int niveau) {
    SDL_Event event;
    int running = 1;

    SDL_Texture* fond = NULL;
    SDL_Surface* herbe = SDL_LoadBMP("image/map/Herbes/Herbe.bmp");
    if (herbe) {
        fond = SDL_CreateTextureFromSurface(renderer, herbe);
        SDL_FreeSurface(herbe);
    }

    Carte cartes[NB_CARTES] = {
        {"image/chevalier.png", NULL, {50, 780, 64, 64}, 3, 100, 10},
        {"image/archer.png", NULL, {150, 780, 64, 64}, 2, 80, 8},
        {"image/valkyrie.png", NULL, {250, 780, 64, 64}, 4, 120, 12},
        {"image/sorcier.png", NULL, {350, 780, 64, 64}, 5, 70, 15}
    };

    for (int i = 0; i < NB_CARTES; i++) {
        SDL_Surface* surf = IMG_Load(cartes[i].nomFichier);
        if (!surf) {
            printf("Erreur chargement carte : %s\n", cartes[i].nomFichier);
            return;
        }
        cartes[i].texture = SDL_CreateTextureFromSurface(renderer, surf);
        SDL_FreeSurface(surf);
    }

    int selectedIndex = 0;
    int elixir = MAX_ELIXIR;
    Uint32 lastElixirTime = SDL_GetTicks();
    Uint32 lastBotTime = SDL_GetTicks();

    Troupe troupes[MAX_TROUPES] = {0};

    Tour tourJoueur = {{(LARGEURJEU/2)*TAILLE_SPRITE - 20, HAUTEURJEU*TAILLE_SPRITE - 60, 40, 40}, 300, {0, 0, 255}};
    Tour tourBot =     {{(LARGEURJEU/2)*TAILLE_SPRITE - 20, 20, 40, 40}, 300, {255, 0, 0}};

    while (running) {
        Uint32 now = SDL_GetTicks();

        if (now - lastElixirTime >= 1000 && elixir < MAX_ELIXIR) {
            elixir++;
            lastElixirTime = now;
        }

        if (now - lastBotTime >= 4000) {
            for (int i = 0; i < MAX_TROUPES; i++) {
                if (!troupes[i].actif) {
                    int type = rand() % NB_CARTES;
                    troupes[i].lane = rand() % 2;
                    troupes[i].x = (troupes[i].lane == 0) ? 100 : 300;
                    troupes[i].y = 80;
                    troupes[i].vitesse = 0.5f;
                    troupes[i].texture = cartes[type].texture;
                    troupes[i].rect.w = 40;
                    troupes[i].rect.h = 40;
                    troupes[i].actif = 1;
                    troupes[i].lane = (troupes[i].x < 300) ? 0 : 1;
                    troupes[i].camp = 1;
                    troupes[i].vie = cartes[type].vie;
                    troupes[i].degats = cartes[type].degats;
                    break;
                }
            }
            lastBotTime = now;
        }

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = 0;

            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                SDL_Point clic = { event.button.x, event.button.y };
                for (int i = 0; i < NB_CARTES; i++) {
                    if (SDL_PointInRect(&clic, &cartes[i].bouton)) {
                        selectedIndex = i;
                    }
                }

                if (clic.y > 600) {
                    if (elixir >= cartes[selectedIndex].coutElixir) {
                        for (int i = 0; i < MAX_TROUPES; i++) {
                            if (!troupes[i].actif) {
                                troupes[i].x = clic.x;
                                troupes[i].y = clic.y;
                                troupes[i].vitesse = 0.5f;
                                troupes[i].texture = cartes[selectedIndex].texture;
                                troupes[i].rect.w = 40;
                                troupes[i].rect.h = 40;
                                troupes[i].actif = 1;
                                troupes[i].lane = (clic.x < 300) ? 0 : 1;
                                troupes[i].camp = 0;
                                troupes[i].vie = cartes[selectedIndex].vie;
                                troupes[i].degats = cartes[selectedIndex].degats;
                                elixir -= cartes[selectedIndex].coutElixir;
                                break;
                            }
                        }
                    }
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        if (fond) {
            for (int j = 0; j < HAUTEURJEU; j++) {
                for (int i = 0; i < LARGEURJEU; i++) {
                    SDL_Rect r = { i * TAILLE_SPRITE, j * TAILLE_SPRITE, TAILLE_SPRITE, TAILLE_SPRITE };
                    SDL_RenderCopy(renderer, fond, NULL, &r);
                }
            }
        }

        dessinerTour(renderer, tourJoueur);
        dessinerTour(renderer, tourBot);

        for (int i = 0; i < MAX_TROUPES; i++) {
            if (!troupes[i].actif) continue;

            // Mouvement
            if (troupes[i].camp == 0)
                troupes[i].y -= troupes[i].vitesse;
            else
                troupes[i].y += troupes[i].vitesse;

            troupes[i].rect.x = (int)troupes[i].x;
            troupes[i].rect.y = (int)troupes[i].y;

            // Collision tour
            if (troupes[i].camp == 0 && collision(troupes[i].rect, tourBot.rect)) {
                tourBot.vie -= troupes[i].degats;
                troupes[i].actif = 0;
            } else if (troupes[i].camp == 1 && collision(troupes[i].rect, tourJoueur.rect)) {
                tourJoueur.vie -= troupes[i].degats;
                troupes[i].actif = 0;
            }

            // Collision entre troupes
            for (int j = 0; j < MAX_TROUPES; j++) {
                if (i != j && troupes[j].actif && troupes[i].camp != troupes[j].camp &&
                    collision(troupes[i].rect, troupes[j].rect)) {
                    troupes[i].vie -= troupes[j].degats;
                    troupes[j].vie -= troupes[i].degats;
                }
            }

            if (troupes[i].vie <= 0) {
                troupes[i].actif = 0;
            }

            if (troupes[i].actif) {
                SDL_RenderCopy(renderer, troupes[i].texture, NULL, &troupes[i].rect);
                if (troupes[i].vie < 100) {
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 128);
                    SDL_RenderFillRect(renderer, &troupes[i].rect);
                }
            }
        }

        for (int i = 0; i < NB_CARTES; i++) {
            SDL_RenderCopy(renderer, cartes[i].texture, NULL, &cartes[i].bouton);
            if (i == selectedIndex) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
                SDL_RenderDrawRect(renderer, &cartes[i].bouton);
            }
        }

        afficherElixir(renderer, font, elixir);
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    for (int i = 0; i < NB_CARTES; i++) SDL_DestroyTexture(cartes[i].texture);
    if (fond) SDL_DestroyTexture(fond);
}
