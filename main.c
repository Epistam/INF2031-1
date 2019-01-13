#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

#include "include/referentiel.h"

#define STR_MAUVAIS_ARG "Veuillez sélectionner le mode dans lequel vous souhaitez exécuter cette application : './Banque graphique' ou './Banque texte'."

int main(int argc, char *argv[]) { // Récupération des arguments
	// Examen exclusif du premier argument
	/*int is_text, is_graphique;
	if(argc > 1 && (is_text = !strcmp(argv[1],"texte") || is_graphique = !strcmp(argv[1],"graphique"))) {
		if(is_graphique) graphique_run();
		if(is_texte) texte_run(); // Else non nécessaire puisque argv[1] ne peut pas à la fois être "texte" et "graphique"
	}
	else puts(STR_MAUVAIS_ARG);
*/

	//sqlite3 *bdd = init_bdd...;
	//
	// Si graphique_run et text_run, si retour code != 0, restaure + quitte + affiche erreur
	// nécessité d'écrire dans la bdd temp d'abord
	//
	//
	//
	// sqlite free

	return EXIT_SUCCESS;
}
