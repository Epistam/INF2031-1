#ifndef UI_GRAPHIQUE_H
#define UI_GRAPHIQUE_H

typedef struct {
	char *texte_gauche;
	char *texte_droit;
	int couleur_entree;
	bool selectionnable;
} Menu_entree;

struct Menu_s;

typedef struct {
	// Caractéristiques de ce menu
	int menu_id;
	char *menu_titre;
	int menu_type; // 0 pour formulaire, 1 pour menu à plusieurs entrées

	// Nombre d'options que ce soit un formulaire ou un menu
	int menu_nb_options; 	
	
	char **menu_formulaires;
	Menu_entree **menu_entrees; // tableau de pointeurs sur les entrées du menu
	
	struct Menu_s *menu_parent; // pointeur vers parents
	struct Menu_s **menu_enfants; // tableau de pointeurs vers les enfants
} Menu;

// Procédure (mode graphique) consistant en une boucle évènementielle rendant le programme interactif
void graphique_run(sqlite3 *bdd);

#endif
