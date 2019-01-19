// Fonctions relatives à l'interface graphique de l'application : sélections, menus, arborescence...
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "include/referentiel.h"
#include "include/ihm_graphique.h"

// Procédure (mode graphique) consistant en une boucle évènementielle rendant le programme interactif

// Quand switch de statuts, dans le switch, on devrait pouvoir exécuter des trucs avant et après l'affichage central du menu

Menu *init_menu() {
	Menu *menu = malloc(sizeof(Menu));
	Menu *menu_origine = menu; // On garde la racine de l'arbre	qu'on retournera
	
	menu->menu_id = 0;
	menu->menu_titre = strdup("Identification");
	menu->menu_type = 0; // Formulaire de login
	menu->menu_nb_options = 1;
	menu->menu_formulaires = malloc(1*sizeof(void*));
	menu->menu_formulaires[0] = strdup("Numéro de Compte :");
	menu->menu_entrees = NULL;
	menu->menu_parent = NULL;
	menu->menu_enfants = malloc(1*sizeof(void*));
	menu->menu_enfants[0] = malloc(sizeof(Menu)); // On alloue le prochain menu

	menu->menu_id = 1;
	menu->menu_titre = strdup("Synthèse client");
	menu->menu_type = 1; // Liste des comptes + balance
	//menu->menu_nb_options = ; // A définir avant d'afficher le menu
	menu->menu_formulaires = NULL;
	menu->menu_entrees = NULL;
	menu->menu_parent = NULL;
	menu->menu_enfants = malloc(1*sizeof(void*));
	menu->menu_enfants[0] = malloc(sizeof(Menu)); // On alloue le prochain menu
/*
	// Caractéristiques de ce menu
	int menu_id;
	char *menu_titre;
	int menu_type; // 0 pour formulaire, 1 pour menu à plusieurs entrées

	// Nombre d'options que ce soit un formulaire ou un menu
	int menu_nb_options; 	
	
	char **menu_formulaires;
	Menu_entree **menu_entrees; // tableau de pointeurs sur les entrées du menu
	
	Menu_s *menu_parent; // pointeur vers parents
	Menu_s **menu_enfants; // tableau de pointeurs vers les enfants
*/

	return menu_origine;
}

void menu_attente(Menu menu) {

}

void graphique_run(sqlite3 *bdd) {
/*	Titulaire *titulaire = recup_titulaire(0, bdd); // On récupère un titulaire vide qui sera la version locale du titulaire, un titulaire courant
	Menu *menu = ; // Menu initial
	int compte_id;
	int menu_id;
	while() {
		switch(menu_id) {
			case 0 :
				menu_attente(menu);
				break;
		}
	}*/
}
