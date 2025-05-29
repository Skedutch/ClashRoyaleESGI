#ifndef TROUPES_H
#define TROUPES_H
#include <SDL2/SDL.h>


typedef enum { SOL, AIR } Tposition;
typedef enum { CIBLE_SOL, CIBLE_AIR, CIBLE_TOUS } Tcible;

typedef struct {
    char nom[30];
    Tposition maPosition;
    Tcible cibleAttaquable;
    int pointsDeVie;
    int degats;
    int vitesseAttaque;
    float vitesseDeplacement;
    int portee;
    int coutEnElixir;
    float posX;
    float posY;
    int peutAttaquer;
    SDL_Texture *imageCarte;
} Tunite;




#define MAX_CARTES 20
#define MAX_DECK 8

typedef struct {
    Tunite *collection[MAX_CARTES];
    Tunite *deck[MAX_DECK];
    int nbCartes;
} Joueur;

Tunite *creerTroupe(const char *nom, float posX, float posY, SDL_Renderer *renderer);
void afficherStatsUnite(Tunite *u);
void initialiserJoueur(Joueur *j, SDL_Renderer *renderer);

int estDansDeck(Joueur *j, Tunite *carte);
void retirerDuDeck(Joueur *j, Tunite *carte);
int ajouterAuDeck(Joueur *j, Tunite *carte);


#endif