#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sqlite3.h>
#include "include/referentiel.h"
#include "include/metier.h"
#include "include/ihm_texte.h"

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

	sqlite3 *bdd;
	sqlite3_open(FICHIER_BDD,&bdd);

/*					int titulaires_nb = malloc(sizeof(int));
					int *titulaires_ids = recup_titulaires_ids(3, &titulaires_nb, bdd);
					for(int i = 0 ; i < titulaires_nb ; i++) printf("%d,",titulaires_ids[i]);
					puts("");*/

/*

					int *titulaires_nb = malloc(sizeof(int));
					Compte *compte = recup_compte(3, titulaires_nb, bdd);
					printf("Numéro de compte : %d\n",compte->compte_id);
					printf("Type de compte : %d\n",compte->compte_type);
					printf("Découvert autorisé ? %d\n",compte->compte_decouvert_autorise);
					int i;
					puts("Liste des titulaires : ");
					for(i = 0 ; i < *titulaires_nb; i++) printf("%d,\n",compte->compte_titulaires[i]);
					puts("");*/
					
					int *types_nb = malloc(sizeof(int));
					char **types = recup_compte_types(types_nb, bdd);
					int i;
					for(i = 0 ; i < *types_nb ;i++) printf("%s\n",types[i]);

					//text_run(bdd);
					//enlever_compte_titulaire(7, 10, bdd);


					// Récup titulaires d'un compte
					/*
					int *titulaire_ids = NULL;
					int *tit_len = malloc(sizeof(int));
					titulaire_ids = recup_titulaires(3, tit_len, bdd);
					int i;
					puts("Liste des titulaires du compte 3 :");
					for(i = 0 ; i < *tit_len ; i++) printf("%d,",titulaire_ids[i]);
				*/	
					// Récup comptes d'un titulaire
					/*
					int *comptes_ids = NULL;
					int *comptes_len = malloc(sizeof(int));
					comptes_ids = recup_comptes(2, comptes_len, bdd);
					int i;
					for(i = 0 ; i < *comptes_len ; i++) printf("%d,",comptes_ids[i]);
					*/
					// Test ajouter opération
					/*
						Operation *op = malloc(sizeof(Operation));
						op->operation_type = 1;
						op->operation_date = 42;
						op->operation_expediteur = 1;
						op->operation_destinataire = 2;
						op->operation_montant = 69.0;
					*/
					// Test ajouter + modifie rcompte
				/*	
						Titulaire *titulaire = malloc(sizeof(Titulaire));
						
						titulaire->titulaire_nom = strdup("Test");
						titulaire->titulaire_prenom = strdup("Test");

						ajouter_titulaire(titulaire, bdd);
						printf("%d",titulaire->titulaire_id);						
						printf("%d",modifier_titulaire(*titulaire, bdd));
					*/
					// Test modifier_compte
					/*
						Compte *compte = malloc(sizeof(Compte));
						compte->compte_id = 1;
						compte->compte_type = 42;
						compte->compte_decouvert_autorise = true;	

						printf("%d",modifier_compte(*compte,bdd));
					*/

	sqlite3_close(bdd);

	return EXIT_SUCCESS;
}
