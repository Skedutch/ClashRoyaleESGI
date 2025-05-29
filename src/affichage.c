#include "affichage.h"

SDL_Texture* loadTexture(const char *path, SDL_Renderer *renderer) {
    SDL_Surface *surface = IMG_Load(path);
    if (!surface) {
        printf("Erreur image %s: %s\n", path, IMG_GetError());
        return NULL;  // PROTECTION
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture) {
        printf("Erreur texture %s: %s\n", path, SDL_GetError());
    }

    return texture;
}


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

void afficherDamier(SDL_Renderer *renderer) {
    int tileSize = 25;
    for (int y = 0; y < SCREEN_HEIGHT; y += tileSize) {
        for (int x = 0; x < SCREEN_WIDTH; x += tileSize) {
            if ((x / tileSize + y / tileSize) % 2 == 0) {
                SDL_SetRenderDrawColor(renderer, 13, 26, 74, 255);  // Bleu foncé
            } else {
                SDL_SetRenderDrawColor(renderer, 33, 50, 110, 255); // Bleu plus clair
            }
            SDL_Rect tile = { x, y, tileSize, tileSize };
            SDL_RenderFillRect(renderer, &tile);
        }
    }
}

void dessinerTitre(SDL_Renderer *renderer, TTF_Font *font, const char *texte, int x, int y, float scale) {
    SDL_Color texte_jaune = {204, 204, 0, 255};
    SDL_Color colorOmbre = {0, 0, 0, 150};       // Noir semi-transparent

    // 1. Rendu de l’ombre
    SDL_Surface *surfaceOmbre = TTF_RenderUTF8_Blended(font, texte, colorOmbre);
    SDL_Texture *textureOmbre = SDL_CreateTextureFromSurface(renderer, surfaceOmbre);

    SDL_Rect destOmbre;
    destOmbre.w = surfaceOmbre->w * scale;
    destOmbre.h = surfaceOmbre->h * scale;
    destOmbre.x = x - destOmbre.w / 2 + 4;  // Décalage ombre (X + 4, Y + 4)
    destOmbre.y = y - destOmbre.h / 2 + 4;
    SDL_FreeSurface(surfaceOmbre);
    SDL_RenderCopy(renderer, textureOmbre, NULL, &destOmbre);
    SDL_DestroyTexture(textureOmbre);

    // 2. Rendu du texte principal
    SDL_Surface *surfaceTexte = TTF_RenderUTF8_Blended(font, texte, texte_jaune);
    SDL_Texture *textureTexte = SDL_CreateTextureFromSurface(renderer, surfaceTexte);

    SDL_Rect destTexte;
    destTexte.w = surfaceTexte->w * scale;
    destTexte.h = surfaceTexte->h * scale;
    destTexte.x = x - destTexte.w / 2;
    destTexte.y = y - destTexte.h / 2;
    SDL_FreeSurface(surfaceTexte);
    SDL_RenderCopy(renderer, textureTexte, NULL, &destTexte);
    SDL_DestroyTexture(textureTexte);
}