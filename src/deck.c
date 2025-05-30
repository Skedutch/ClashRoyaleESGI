#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "troupe.h"
#include "affichage.h"
#include "deck.h"
#include "menu_jouer.h"

extern void afficherTexteStat(SDL_Renderer *renderer, TTF_Font *font, const char *texte, SDL_Color couleur, int x, int *y);

void afficherTexteStat(SDL_Renderer *renderer, TTF_Font *font, const char *texte, SDL_Color couleur, int x, int *y) {
    SDL_Surface *surface = TTF_RenderText_Solid(font, texte, couleur);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect rect = { x, *y, surface->w, surface->h };
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    *y += surface->h + 2;
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}
extern void afficherTexteStat(SDL_Renderer *renderer, TTF_Font *font, const char *texte, SDL_Color couleur, int x, int *y);

void afficherDeck(SDL_Renderer *renderer, TTF_Font *font, Joueur *joueur);


void sauvegarderDeck(Joueur *joueur) {
    FILE *f = fopen("deck.txt", "w");
    if (!f) return;
    for (int i = 0; i < MAX_DECK; i++) {
        if (joueur->deck[i]) fprintf(f, "%s\n", joueur->deck[i]->nom);
    }
    fclose(f);
}

void chargerDeck(Joueur *joueur, SDL_Renderer *renderer) {
    FILE *f = fopen("deck.txt", "r");
    if (!f) return;
    char nom[64];
    int i = 0;
    while (fgets(nom, sizeof(nom), f) && i < MAX_DECK) {
        nom[strcspn(nom, "\n")] = 0;
        for (int j = 0; j < joueur->nbCartes; j++) {
            if (strcmp(joueur->collection[j]->nom, nom) == 0) {
                joueur->deck[i++] = joueur->collection[j];
                break;
            }
        }
    }
    fclose(f);
}

void afficherDeck(SDL_Renderer *renderer, TTF_Font *font, Joueur *joueur) {
    chargerDeck(joueur, renderer); 

    int running = 1;
    SDL_Event event;
    int boutonDeckPressed = 0, boutonJouerPressed = 0;
    int indexCarteSelectionnee = -1;
    int scrollOffset = 0;

    const int largeurFenetre = SCREEN_WIDTH;
    const int nbCartesParLigneDeck = 4;
    const int nbCartesParLigneCollection = 5;
    const int largeurCarteDeck = 80;
    const int hauteurCarteDeck = 115;
    const int largeurCarteCol = 70;
    const int hauteurCarteCol = 105;
    const int espacementH = 90;
    const int espacementV = 110;
    const int debutYDeck = 100;
    const int espacementCollection = 120;
    const int margeLateraleDeck = (SCREEN_WIDTH - (nbCartesParLigneDeck * espacementH)) / 2;
    const int margeLateraleCollection = (SCREEN_WIDTH - (nbCartesParLigneCollection * espacementH)) / 2;
    const int debutYCollection = debutYDeck + (2 * espacementV) + espacementCollection;

    TTF_Font *petitePolice = TTF_OpenFont("police/arial.ttf", 14);
    if (!petitePolice) petitePolice = font;

    TTF_Font *fontDeckCollection = TTF_OpenFont("police/arial.ttf", 48);

    while (running) {
        while (SDL_PollEvent(&event)) {
            SDL_Point clic = { event.button.x, event.button.y };
            SDL_Point clicCorrige = { clic.x, clic.y -50- scrollOffset };

            if (event.type == SDL_QUIT) {
                sauvegarderDeck(joueur);
                exit(0);
            }
            if (event.type == SDL_MOUSEWHEEL) {
                scrollOffset += event.wheel.y * 30;
                if (scrollOffset > 0) scrollOffset = 0;
            }
            int footerY = debutYCollection + (joueur->nbCartes / nbCartesParLigneCollection + 1) * 100 + 40 + scrollOffset;
            SDL_Rect rectArene = {largeurFenetre / 2 - 220, footerY, 200, 80};
            SDL_Rect rectJouer = {largeurFenetre / 2 + 20, footerY, 200, 80};

            if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (SDL_PointInRect(&clicCorrige, &rectArene)) boutonDeckPressed = 1;
                if (SDL_PointInRect(&clicCorrige, &rectJouer)) boutonJouerPressed = 1;
            }
            if (event.type == SDL_MOUSEBUTTONUP) {
                if (SDL_PointInRect(&clicCorrige, &rectArene) && boutonDeckPressed) {
                    sauvegarderDeck(joueur);
                    running = 0;
                }
                if (SDL_PointInRect(&clicCorrige, &rectJouer) && boutonJouerPressed) {
                    sauvegarderDeck(joueur);
                    afficherMenuJouer(renderer, font);
                }
                for (int i = 0; i < joueur->nbCartes; i++) {
                    int colX = i % nbCartesParLigneCollection;
                    int rowY = i / nbCartesParLigneCollection;
                    SDL_Rect carteRect = { margeLateraleCollection + colX * espacementH, debutYCollection + rowY * 100 + scrollOffset, largeurCarteCol, hauteurCarteCol };
                    if (SDL_PointInRect(&clic, &carteRect)) {
                        indexCarteSelectionnee = i;
                    }
                }
                if (indexCarteSelectionnee >= 0) {
                    int colX = indexCarteSelectionnee % nbCartesParLigneCollection;
                    int rowY = indexCarteSelectionnee / nbCartesParLigneCollection;
                    SDL_Rect carteRect = { margeLateraleCollection + colX * espacementH, debutYCollection + rowY * 100 + scrollOffset, largeurCarteCol, hauteurCarteCol };
                    int baseX = carteRect.x;
                    int baseY = carteRect.y + carteRect.h + 10;
                    SDL_Rect btnAjout = { baseX, baseY, 140, 30 };
                    if (SDL_PointInRect(&clic, &btnAjout)) {
                        if (estDansDeck(joueur, joueur->collection[indexCarteSelectionnee])) {
                            retirerDuDeck(joueur, joueur->collection[indexCarteSelectionnee]);
                        } else {
                            ajouterAuDeck(joueur, joueur->collection[indexCarteSelectionnee]);
                        }
                        indexCarteSelectionnee = -1;
                    }
                }
                boutonDeckPressed = boutonJouerPressed = 0;
            }
        }

        SDL_RenderClear(renderer);
        afficherDamier(renderer);

        // Couleurs
        SDL_Color texte_blanc = {255, 255, 255, 255};
        SDL_Color texte_jaune = {204, 204, 0, 255};
        SDL_Color texte_ombre = {255, 0, 127, 255};

        // DECK 
        SDL_Surface *surfaceShadowDeck = TTF_RenderText_Solid(fontDeckCollection, "Deck", texte_ombre);
        SDL_Surface *surfaceMainDeck = TTF_RenderText_Solid(fontDeckCollection, "Deck", texte_jaune);

        SDL_Texture *textureShadowDeck = SDL_CreateTextureFromSurface(renderer, surfaceShadowDeck);
        SDL_Texture *textureMainDeck = SDL_CreateTextureFromSurface(renderer, surfaceMainDeck);

        // Position
        SDL_Rect rectDeck = {
            (SCREEN_WIDTH - surfaceMainDeck->w) / 2,
            50 + scrollOffset,
            surfaceMainDeck->w,
            surfaceMainDeck->h
        };
        SDL_Rect rectShadowDeck = rectDeck;
        rectShadowDeck.x += 2;  
        rectShadowDeck.y += 2;

        SDL_RenderCopy(renderer, textureShadowDeck, NULL, &rectShadowDeck);
        SDL_RenderCopy(renderer, textureMainDeck, NULL, &rectDeck);

        SDL_FreeSurface(surfaceShadowDeck);
        SDL_FreeSurface(surfaceMainDeck);
        SDL_DestroyTexture(textureShadowDeck);
        SDL_DestroyTexture(textureMainDeck);

        //  COLLECTION 
        SDL_Surface *surfaceShadowCol = TTF_RenderText_Solid(fontDeckCollection, "Collection", texte_ombre);
        SDL_Surface *surfaceMainCol = TTF_RenderText_Solid(fontDeckCollection, "Collection", texte_jaune);

        SDL_Texture *textureShadowCol = SDL_CreateTextureFromSurface(renderer, surfaceShadowCol);
        SDL_Texture *textureMainCol = SDL_CreateTextureFromSurface(renderer, surfaceMainCol);

        SDL_Rect rectCol = {
            (SCREEN_WIDTH - surfaceMainCol->w) / 2,
            debutYCollection - 60 + scrollOffset,
            surfaceMainCol->w,
            surfaceMainCol->h
        };
        SDL_Rect rectShadowCol = rectCol;
        rectShadowCol.x += 2;
        rectShadowCol.y += 2;

        SDL_RenderCopy(renderer, textureShadowCol, NULL, &rectShadowCol);
        SDL_RenderCopy(renderer, textureMainCol, NULL, &rectCol);

        SDL_FreeSurface(surfaceShadowCol);
        SDL_FreeSurface(surfaceMainCol);
        SDL_DestroyTexture(textureShadowCol);
        SDL_DestroyTexture(textureMainCol);




        for (int i = 0; i < MAX_DECK; i++) {
            int col = i % nbCartesParLigneDeck;
            int row = i / nbCartesParLigneDeck;
            SDL_Rect rect = { margeLateraleDeck + col * espacementH, debutYDeck + row * espacementV + scrollOffset, largeurCarteDeck, hauteurCarteDeck };
            if (joueur->deck[i] && joueur->deck[i]->imageCarte) {
                SDL_RenderCopy(renderer, joueur->deck[i]->imageCarte, NULL, &rect);
            } else {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderFillRect(renderer, &rect);
            }
        }

        int nbLignesCartes = 0;
        for (int i = 0; i < joueur->nbCartes; i++) {
            int col = i % nbCartesParLigneCollection;
            int row = i / nbCartesParLigneCollection;
            SDL_Rect carteRect = { margeLateraleCollection + col * espacementH, debutYCollection + row * 100 + scrollOffset, largeurCarteCol, hauteurCarteCol };
            if (joueur->collection[i] && joueur->collection[i]->imageCarte) {
                SDL_RenderCopy(renderer, joueur->collection[i]->imageCarte, NULL, &carteRect);
            } else {
                SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255);
                SDL_RenderFillRect(renderer, &carteRect);
            }
            if (estDansDeck(joueur, joueur->collection[i])) {
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                SDL_RenderDrawRect(renderer, &carteRect);
            }
            nbLignesCartes = row;
        }

        if (indexCarteSelectionnee >= 0) {
            int colX = indexCarteSelectionnee % nbCartesParLigneCollection;
            int rowY = indexCarteSelectionnee / nbCartesParLigneCollection;
            SDL_Rect carteRect = { margeLateraleCollection + colX * espacementH, debutYCollection + rowY * 100 + scrollOffset, largeurCarteCol, hauteurCarteCol };
            int baseX = carteRect.x;
            int baseY = carteRect.y + carteRect.h + 10;
            SDL_Rect panel = { baseX, baseY, 160, 200 };
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 220);
            SDL_RenderFillRect(renderer, &panel);
            dessinerBouton(renderer, petitePolice,
                estDansDeck(joueur, joueur->collection[indexCarteSelectionnee]) ? "Retirer" : "Ajouter",
                baseX + 10, baseY + 10, 0, 140, 30, 10, 255, 100, 100);
            dessinerBouton(renderer, petitePolice, "Statistiques", baseX + 10, baseY + 50, 0, 140, 30, 10, 100, 255, 100);
            int yStat = baseY + 90;
            char buf[64];
            snprintf(buf, sizeof(buf), "Nom : %s", joueur->collection[indexCarteSelectionnee]->nom);
            afficherTexteStat(renderer, petitePolice, buf, texte_blanc, baseX + 10, &yStat);
            snprintf(buf, sizeof(buf), "Vie : %d", joueur->collection[indexCarteSelectionnee]->pointsDeVie);
            afficherTexteStat(renderer, petitePolice, buf, texte_blanc, baseX + 10, &yStat);
            snprintf(buf, sizeof(buf), "Degats : %d", joueur->collection[indexCarteSelectionnee]->degats);
            afficherTexteStat(renderer, petitePolice, buf, texte_blanc, baseX + 10, &yStat);
            snprintf(buf, sizeof(buf), "Portee : %d", joueur->collection[indexCarteSelectionnee]->portee);
            afficherTexteStat(renderer, petitePolice, buf, texte_blanc, baseX + 10, &yStat);
            snprintf(buf, sizeof(buf), "Elixir : %d", joueur->collection[indexCarteSelectionnee]->coutEnElixir);
            afficherTexteStat(renderer, petitePolice, buf, texte_blanc, baseX + 10, &yStat);
            snprintf(buf, sizeof(buf), "Vitesse : %.1f", joueur->collection[indexCarteSelectionnee]->vitesseDeplacement);
            afficherTexteStat(renderer, petitePolice, buf, texte_blanc, baseX + 10, &yStat);
        }

        int footerY = debutYCollection + (nbLignesCartes + 1) * 100 + 40 ;
        int limiteScroll = debutYCollection + (nbLignesCartes + 1) * 100 + 120 - SCREEN_HEIGHT;
        if (scrollOffset < -limiteScroll) scrollOffset = -limiteScroll;

        // Zone cliquable corrigée: on dessine ET gère les clics au même endroit
        SDL_Rect rectArene = {largeurFenetre / 2 - 220, footerY + scrollOffset, 200, 80};
        SDL_Rect rectJouer = {largeurFenetre / 2 + 20, footerY + scrollOffset, 200, 80};
        
        dessinerBouton(renderer, font, "Arene", rectArene.x, rectArene.y, boutonDeckPressed, rectArene.w, rectArene.h, 20, 255, 204, 0);
        dessinerBouton(renderer, font, "Jouer", rectJouer.x, rectJouer.y, boutonJouerPressed, rectJouer.w, rectJouer.h, 20, 255, 204, 0);        

        SDL_RenderPresent(renderer);
    }
    if (fontDeckCollection) TTF_CloseFont(fontDeckCollection);
    if (petitePolice != font) TTF_CloseFont(petitePolice);
}
