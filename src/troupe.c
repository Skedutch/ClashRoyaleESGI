#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "troupe.h"
#include "affichage.h"

#define MAX_CARTES 20
#define MAX_DECK   8


static Tunite baseTroupes[] = {
    {"chevalier", SOL, CIBLE_SOL, 1200, 150, 1, 1.0f, 1, 3, 0, 0, 0, NULL},
    {"archer", AIR, CIBLE_TOUS, 88, 120, 1, 1.0f, 3, 2, 0, 0, 0, NULL},
    {"gargouille", AIR, CIBLE_TOUS, 80, 90, 1, 3.0f, 1, 3, 0, 0, 0, NULL},
    {"geant", SOL, CIBLE_SOL, 2000, 50, 1, 0.5f, 1, 5, 0, 0, 0, NULL},
    {"sorcier", SOL, CIBLE_TOUS, 600, 200, 1, 1.2f, 2, 5, 0, 0, 0, NULL},
    {"mini_pekka", SOL, CIBLE_SOL, 800, 300, 1, 1.0f, 1, 4, 0, 0, 0, NULL},
    {"valkyrie", SOL, CIBLE_SOL, 1000, 120, 1, 0.8f, 1, 4, 0, 0, 0, NULL},
    {"bombardier", SOL, CIBLE_SOL, 200, 350, 1, 2.5f, 1, 2, 0, 0, 0, NULL},
    {"bebe_dragon", AIR, CIBLE_TOUS, 1100, 90, 1, 1.5f, 2, 4, 0, 0, 0, NULL},
    {"gobelin", SOL, CIBLE_SOL, 300, 75, 1, 2.0f, 1, 2, 0, 0, 0, NULL},

    // 10 nouvelles troupes équilibrées
    {"barbare", SOL, CIBLE_SOL, 950, 140, 1, 1.1f, 1, 3, 0, 0, 0, NULL},
    {"mage_glace", SOL, CIBLE_TOUS, 700, 100, 1, 1.0f, 3, 4, 0, 0, 0, NULL},
    {"mega_minion", AIR, CIBLE_TOUS, 500, 200, 1, 2.0f, 1, 3, 0, 0, 0, NULL},
    {"cochon", SOL, CIBLE_SOL, 1000, 170, 1, 2.2f, 1, 4, 0, 0, 0, NULL},
    {"squelette", SOL, CIBLE_SOL, 100, 60, 1, 2.8f, 1, 1, 0, 0, 0, NULL},
    {"ballon", AIR, CIBLE_SOL, 90, 110, 1, 1.0f, 3, 4, 0, 0, 0, NULL},
    {"bourreau", SOL, CIBLE_TOUS, 750, 245, 1, 1.3f, 2, 5, 0, 0, 0, NULL},
    {"dragon_enfer", AIR, CIBLE_TOUS, 1300, 60, 1, 1.6f, 3, 5, 0, 0, 0, NULL},
    {"bucheron", SOL, CIBLE_SOL, 1000, 250, 1, 0.9f, 2, 4, 0, 0, 0, NULL},
    {"golem", SOL, CIBLE_SOL, 3000, 90, 1, 0.4f, 1, 8, 0, 0, 0, NULL}
};

static const int nbTroupes = sizeof(baseTroupes) / sizeof(Tunite);

void initialiserJoueur(Joueur *j, SDL_Renderer *renderer) {
    strcpy(j->nom, "Quentin");
    j->niveau = 4;
    j->trophees = 542;

    for (int i = 0; i < nbTroupes; i++) {
        j->collection[i] = creerTroupe(baseTroupes[i].nom, 0, 0, renderer);
    }
    j->nbCartes = nbTroupes;

    for (int k = 0; k < MAX_DECK; k++) {
        j->deck[k] = NULL; // deck vide au départ
    }
}

int ajouterAuDeck(Joueur *j, Tunite *carte) {
    for (int i = 0; i < MAX_DECK; i++) {
        if (j->deck[i] == NULL) {
            j->deck[i] = carte;
            return 1;
        }
    }
    return 0;
}

void retirerDuDeck(Joueur *j, Tunite *carte) {
    for (int i = 0; i < MAX_DECK; i++) {
        if (j->deck[i] == carte) {
            j->deck[i] = NULL;
            return;
        }
    }
}

int estDansDeck(Joueur *j, Tunite *carte) {
    for (int i = 0; i < MAX_DECK; i++) {
        if (j->deck[i] == carte) return 1;
    }
    return 0;
}

Tunite *creerTroupe(const char *nom, float posX, float posY, SDL_Renderer *renderer) {
    for (int i = 0; i < nbTroupes; i++) {
        if (strcmp(baseTroupes[i].nom, nom) == 0) {
            Tunite *u = malloc(sizeof(Tunite));
            if (!u) return NULL;
            *u = baseTroupes[i];
            u->posX = posX;
            u->posY = posY;
            u->peutAttaquer = 1;

            char path[100];
            snprintf(path, sizeof(path), "image/%s.png", nom);
            u->imageCarte = loadTexture(path, renderer);
            if (!u->imageCarte) printf("[AVERTISSEMENT] Image introuvable pour %s\n", nom);

            return u;
        }
    }
    return NULL;
}

void afficherStatsUnite(Tunite *u) {
    if (!u) return;
    printf("----- [%s] -----\n", u->nom);
    printf("Position        : %s\n", u->maPosition == SOL ? "Sol" : "Air");
    printf("Cible           : %s\n", u->cibleAttaquable == CIBLE_SOL ? "Sol" : u->cibleAttaquable == CIBLE_AIR ? "Air" : "Tous");
    printf("Points de vie   : %d\n", u->pointsDeVie);
    printf("Degats          : %d\n", u->degats);
    printf("Attaque (tour)  : %d\n", u->vitesseAttaque);
    printf("Deplacement     : %.1f m/s\n", u->vitesseDeplacement);
    printf("Portée          : %d m\n", u->portee);
    printf("Elixir cout     : %d\n", u->coutEnElixir);
}
