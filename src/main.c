#include "affichage.h"
#include "troupe.h"
#include "deck.h"
#include "menu_jouer.h"


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

void dessinerProfil(SDL_Renderer *renderer, TTF_Font *font, Joueur *joueur, SDL_Texture *tropheeTexture){
    char buffer[128];
    SDL_Color blanc = {255, 255, 255, 255};

    // Dimensions
    const int x = 20;
    const int y = 140;
    const int padding = 10;
    const int lineHeight = 30;
    const int boxWidth = 200;
    const int boxHeight = 3 * lineHeight + padding * 2;

    // Dessiner fond semi-transparent
    SDL_Rect fond = {x, y, boxWidth, boxHeight};
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150); // Noir transparent
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_RenderFillRect(renderer, &fond);

    // Texte : Nom
    snprintf(buffer, sizeof(buffer), "%s", joueur->nom);
    SDL_Surface *surf = TTF_RenderUTF8_Blended(font, buffer, blanc);
    SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_Rect dest = {x + padding, y + padding, surf->w, surf->h};
    SDL_FreeSurface(surf);
    SDL_RenderCopy(renderer, tex, NULL, &dest);
    SDL_DestroyTexture(tex);


    // Texte : Trophées
    // Affichage de l’icône du trophée (25x25px)
    SDL_Rect destIcon = {x + padding, y + padding + lineHeight, 25, 25};
    SDL_RenderCopy(renderer, tropheeTexture, NULL, &destIcon);

    snprintf(buffer, sizeof(buffer), "%d", joueur->trophees);
    surf = TTF_RenderUTF8_Blended(font, buffer, blanc);
    tex = SDL_CreateTextureFromSurface(renderer, surf);
    dest = (SDL_Rect){x + padding + 30, y + padding + lineHeight + 2, surf->w, surf->h}; // 30px à droite de l’icône
    SDL_FreeSurface(surf);
    SDL_RenderCopy(renderer, tex, NULL, &dest);
    SDL_DestroyTexture(tex);


    // Texte : Niveau
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




int main(int argc, char *argv[]) {
    (void)argc; (void)argv;
    if (SDL_Init(SDL_INIT_VIDEO) < 0 || IMG_Init(IMG_INIT_PNG) == 0 || TTF_Init() < 0) {
        printf("Erreur init SDL: %s\n", SDL_GetError());
        return 1;
    }

    TTF_Font *font = TTF_OpenFont("police/arial.ttf", 28);
    if (!font) { printf("Erreur police: %s\n", TTF_GetError()); return 1; }

    TTF_Font *fontTitre = TTF_OpenFont("police/arial.ttf", 56);
    if (!fontTitre) { printf("Erreur police: %s\n", TTF_GetError()); return 1; }


    SDL_Window *window = SDL_CreateWindow("Clash Royale", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Surface *tropheeSurface = IMG_Load("image/trophee.png");
    if (!tropheeSurface) {
        printf("Erreur chargement trophée.png: %s\n", IMG_GetError());
        return 1;
    }
    SDL_Texture *tropheeTexture = SDL_CreateTextureFromSurface(renderer, tropheeSurface);
    SDL_FreeSurface(tropheeSurface);


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
        // Animation du titre
        float scale = 1.0f + 0.1f * sin(SDL_GetTicks() * 0.002f);
        dessinerTitre(renderer, fontTitre, "Clash Royale", SCREEN_WIDTH / 2, 100, scale);
        dessinerProfil(renderer, font, &joueur, tropheeTexture);




        dessinerBouton(renderer, font, "Deck", 75, 650, boutonDeckPressed, 200,80,20, 255, 204, 0);
        dessinerBouton(renderer, font, "Jouer", 325, 650, boutonJouerPressed, 200,80,20, 255, 204, 0);
        SDL_RenderPresent(renderer);
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(tropheeTexture);
    SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    return 0;
}
