#include "profil.h"
#include <SDL2/SDL_image.h>
#include <string.h>
#include <stdio.h>

void dessinerProfil(SDL_Renderer *renderer, TTF_Font *font, Joueur *joueur, SDL_Texture *tropheeTexture) {
    char buffer[128];
    SDL_Color blanc = {255, 255, 255, 255};

    const int x = 20;
    const int y = 140;
    const int padding = 10;
    const int lineHeight = 30;
    const int boxWidth = 200;
    const int boxHeight = 3 * lineHeight + padding * 2;

    SDL_Rect fond = {x, y, boxWidth, boxHeight};
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_RenderFillRect(renderer, &fond);

    if (tropheeTexture) {
        SDL_Texture *avatar = IMG_LoadTexture(renderer, joueur->avatarPath);
        if (avatar) {
            SDL_Rect avatarRect = {x + padding, y + padding, 40, 40};
            SDL_RenderCopy(renderer, avatar, NULL, &avatarRect);
            SDL_DestroyTexture(avatar);
        }
    }

    snprintf(buffer, sizeof(buffer), "%s", joueur->nom);
    SDL_Surface *surf = TTF_RenderUTF8_Blended(font, buffer, blanc);
    SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_Rect dest = {x + padding + 50, y + padding, surf->w, surf->h};
    SDL_FreeSurface(surf);
    SDL_RenderCopy(renderer, tex, NULL, &dest);
    SDL_DestroyTexture(tex);

    SDL_Rect destIcon = {x + padding, y + padding + lineHeight, 25, 25};
    SDL_RenderCopy(renderer, tropheeTexture, NULL, &destIcon);

    snprintf(buffer, sizeof(buffer), "%d", joueur->trophees);
    surf = TTF_RenderUTF8_Blended(font, buffer, blanc);
    tex = SDL_CreateTextureFromSurface(renderer, surf);
    dest = (SDL_Rect){x + padding + 30, y + padding + lineHeight + 2, surf->w, surf->h};
    SDL_FreeSurface(surf);
    SDL_RenderCopy(renderer, tex, NULL, &dest);
    SDL_DestroyTexture(tex);

    snprintf(buffer, sizeof(buffer), "Niveau : %d", joueur->niveau);
    surf = TTF_RenderUTF8_Blended(font, buffer, blanc);
    tex = SDL_CreateTextureFromSurface(renderer, surf);
    dest.y += lineHeight;
    dest.w = surf->w;
    dest.h = surf->h;
    SDL_FreeSurface(surf);
    SDL_RenderCopy(renderer, tex, NULL, &dest);
    SDL_DestroyTexture(tex);
}

void demanderPseudo(SDL_Renderer *renderer, TTF_Font *font, char *nom, size_t maxLen, char *avatarPath) {
    SDL_StartTextInput();
    SDL_Event event;
    int done = 0;
    char buffer[100] = "";

    const char *avatars[] = {
        "image/avatar/cat_angry.png",
        "image/avatar/cat_beluga.png",
        "image/avatar/cat_crew.png",
        "image/avatar/cat_pouce.png"
    };
    const int NB_AVATARS = 4;
    SDL_Texture *avatarTextures[NB_AVATARS];
    int selectedAvatar = 0;

    for (int i = 0; i < NB_AVATARS; i++) {
        avatarTextures[i] = IMG_LoadTexture(renderer, avatars[i]);
    }

    while (!done) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        afficherDamier(renderer);

        SDL_Color blanc = {255, 255, 255, 255};

        SDL_Surface *surface = TTF_RenderUTF8_Blended(font, "Entrez votre pseudo :", blanc);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect rect = {100, 100, surface->w, surface->h};
        SDL_FreeSurface(surface);
        SDL_RenderCopy(renderer, texture, NULL, &rect);
        SDL_DestroyTexture(texture);

        SDL_SetRenderDrawColor(renderer, 20, 20, 40, 160);
        SDL_Rect zoneFond = {95, 145, 410, 40};
        SDL_RenderFillRect(renderer, &zoneFond);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 180);
        SDL_RenderDrawRect(renderer, &zoneFond);

        if (strlen(buffer) == 0) {
            surface = TTF_RenderUTF8_Blended(font, " ", blanc);
        } else {
            surface = TTF_RenderUTF8_Blended(font, buffer, blanc);
        }
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect inputRect = {100, 150, surface->w, surface->h};
        SDL_FreeSurface(surface);
        SDL_RenderCopy(renderer, texture, NULL, &inputRect);
        SDL_DestroyTexture(texture);

        for (int i = 0; i < NB_AVATARS; i++) {
            SDL_Rect rect = {100 + i * 80, 230, 64, 64};
            SDL_RenderCopy(renderer, avatarTextures[i], NULL, &rect);
            if (i == selectedAvatar) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
                SDL_RenderDrawRect(renderer, &rect);
            }
        }

        SDL_RenderPresent(renderer);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                SDL_StopTextInput();
                return;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mx = event.button.x;
                int my = event.button.y;
                for (int i = 0; i < NB_AVATARS; i++) {
                    SDL_Rect r = {100 + i * 80, 230, 64, 64};
                    if (mx >= r.x && mx <= r.x + r.w && my >= r.y && my <= r.y + r.h) {
                        selectedAvatar = i;
                    }
                }
            } else if (event.type == SDL_TEXTINPUT) {
                if (strlen(buffer) + strlen(event.text.text) < maxLen - 1)
                    strcat(buffer, event.text.text);
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_BACKSPACE && strlen(buffer) > 0) {
                    buffer[strlen(buffer) - 1] = '\0';
                } else if (event.key.keysym.sym == SDLK_RETURN) {
                    if (strlen(buffer) == 0) {
                        strcpy(buffer, "Joueur");
                    }
                    done = 1;
                }
            }
        }
    }

    strncpy(nom, buffer, maxLen);
    strncpy(avatarPath, avatars[selectedAvatar], 100);
    SDL_StopTextInput();

    for (int i = 0; i < NB_AVATARS; i++) {
        SDL_DestroyTexture(avatarTextures[i]);
    }
}

void choisirAvatar(SDL_Renderer *renderer, TTF_Font *font, char *avatarPath) {
    const char *avatars[] = {
        "image/avatar/cat_angry.png",
        "image/avatar/cat_beluga.png",
        "image/avatar/cat_crew.png",
        "image/avatar/cat_pouce.png"
    };
    const int NB_AVATARS = 4;
    SDL_Texture *avatarTextures[NB_AVATARS];
    int selected = -1;

    for (int i = 0; i < NB_AVATARS; i++) {
        avatarTextures[i] = IMG_LoadTexture(renderer, avatars[i]);
    }

    int done = 0;
    SDL_Event e;
    while (!done) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        afficherDamier(renderer);

        SDL_Surface *s = TTF_RenderUTF8_Blended(font, "Choisissez un avatar :", (SDL_Color){255, 255, 255, 255});
        SDL_Texture *t = SDL_CreateTextureFromSurface(renderer, s);
        SDL_Rect r = {100, 50, s->w, s->h};
        SDL_FreeSurface(s);
        SDL_RenderCopy(renderer, t, NULL, &r);
        SDL_DestroyTexture(t);

        for (int i = 0; i < NB_AVATARS; i++) {
            SDL_Rect rect = {100 + i * 80, 150, 64, 64};
            SDL_RenderCopy(renderer, avatarTextures[i], NULL, &rect);
        }

        SDL_RenderPresent(renderer);

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) return;
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                int mx = e.button.x, my = e.button.y;
                for (int i = 0; i < NB_AVATARS; i++) {
                    SDL_Rect r = {100 + i * 80, 150, 64, 64};
                    if (mx >= r.x && mx <= r.x + r.w && my >= r.y && my <= r.y + r.h) {
                        strcpy(avatarPath, avatars[i]);
                        done = 1;
                    }
                }
            }
        }
    }

    for (int i = 0; i < NB_AVATARS; i++) {
        SDL_DestroyTexture(avatarTextures[i]);
    }
}
