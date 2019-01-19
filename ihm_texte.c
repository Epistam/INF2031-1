// Fonctions relatives à l'interface textuelle (suite de prints et de gets, sélections de menus etc...)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sqlite3.h>
#include "include/referentiel.h"
#include "include/metier.h"
#include "include/interface_texte.h"

int choix_interface(sqlite3 *bdd) {
	int temp;
	temp = 0;
	while(temp != 1 && temp != 2) {
			printf("************************************\n");
			printf("*       Qui etes-vous?             *\n");
			printf("*  ->    Utilisateur (tapez 1)     *\n");
			printf("*  ->      Banque  (tapez 2)       *\n");
			printf("************************************\n");

			scanf("%d", &temp);
			getchar();

			if (temp != 1 && temp!= 2) printf("Ce numero est incorrect\n");
	}
	return temp;
}


int action_titulaire(sqlite3 *bdd){
	int var;
	while(var != 1 && var != 2 && var != 3 && var !=4) {

		printf("************************************\n");
		printf("*       Que voulez-vous faire?     *\n");
		printf("*  ->      virement  (tapez 1)     *\n");
		printf("*  ->      débit     (tapez 2)     *\n");
		printf("*  ->      crédit    (tapez 3)     *\n");
		printf("*  -> affichage du solde (tapez 4) *\n");
		printf("************************************\n");

		scanf("%d",&var);
		getchar();

		if(var != 1 && var != 2 && var != 3 && var !=4) printf("Ce numéro est incorrect\n");
	}
	return var;
}


void menu_virement(sqlite3 *bdd) {

	double argent;
	int cptA, cptB;

	printf("montant du virement\n");
	scanf("%lf",&argent);

	printf("Compte emmeteur\n");
	scanf("%d", &cptA);

	printf("Compte receveur\n");
	scanf("%d", &cptB);

	virement(cptA, cptB, argent, bdd);

}

void debit(int id, sqlite3 *bdd) {
    double argent;
    int compte;

    printf("compte a debiter\n");
    scanf("%d", &compte);

    //if (/*ctrl_cpt(id, compte)*/)
    //{
        printf("montant du retrait\n");
        scanf("%lf", &argent);

		virement(compte, 0, argent, bdd);

        printf("le compte N%d a ete debite de %lf\n", compte, argent);
    //}
}

void credit(sqlite3 *bdd) {
    double argent;
    int compte;
   printf("indiquer le compte a crediter\n");
   scanf("%d", &compte);

   printf("indiquer le montant\n");
   scanf("%lf", &argent);

	virement(0, compte, argent, bdd);
	printf("le compte N%d a ete crediter de %lf\n", compte, argent);
}

bool recherche_titulaire(int id, sqlite3 *bdd)
{

}

void text_run(sqlite3 *bdd) {
    int user;
    user = choix_interface(bdd);
    if (user == 1) {
        int id;
        printf("votre numero titulaire\n");
        scanf("%d", &id);

		int act;
		act = action_titulaire(bdd);

		switch(act)
		{
		case 1:
			menu_virement(bdd);
			break;
		case 2:
			debit(id, bdd);
			break;
		case 3:
			credit(bdd);
			break;
		case 4:
			printf("%lf\n",recup_solde(id,time(NULL),bdd));
			break;
		}
        //} else printf("numero titulaire inconnu");
    } else {
        printf("Bienvenue dans l'onglet Banque\n");
        printf("Quelle operation voulez vous faire?\n");
        printf("Tapez 1 pour creer ajouter un client pourquoi pas lui creer un compte\n");
        printf("Tapez 2 pour creer un compte\n");
        int ceque;
		int titulaire_id;
        scanf("%d",&ceque);
        if (ceque==1)
        {
            Titulaire *titulaire = malloc(sizeof(Titulaire));
			titulaire->titulaire_nom = malloc(25*sizeof(char));
			titulaire->titulaire_prenom = malloc(25*sizeof(char));
            char decision[4]; // Besoin d'inclure le \0
            printf("Quel est le nom du client ?\n ");
            scanf("%s", titulaire->titulaire_nom);
            printf("Son prenom ? \n");
            scanf("%s", titulaire->titulaire_prenom);
            printf("Nous avons un nouveau membre qui est monsieur %s %s \n", titulaire->titulaire_prenom, titulaire->titulaire_nom);
			// TODO création compte
			titulaire_id = ajouter_titulaire(titulaire, bdd);
		  	printf("Le client veut-il proceder a l'ouverture directe d'un compte?\n");

            scanf("%s",decision);

            if (strcmp(decision,"non")==0 || strcmp(decision,"Non")==0 )
            {
                printf("Nous serons toujours presents pour l'ouverture d'un compte\n");
            }
            else if (strcmp(decision,"oui")==0 || strcmp(decision,"Oui")==0)
            {
				int type;
				Compte *compte = malloc(sizeof(Compte));
				puts("Voici les types de comptes disponibles : ");
				recup_compte_types(bdd);
				puts("Tapez le numéro correspondant pour choisir le type voulu.");
                scanf("%d", &type);
				compte->compte_type = type;
				compte->compte_decouvert_autorise = false; // Par défaut
				
				ajouter_compte(compte, bdd);
				getchar();
				ajouter_compte_titulaire(compte->compte_id, titulaire->titulaire_id, bdd);

                printf("Le compte a été créé avec succès.");
            }
		}
        else if(ceque==2)
        {
			int id, type;
			Compte *compte = malloc(sizeof(Compte));
            printf("indiquer l'id client\n");
            scanf("%d", &id);

			puts("Voici les types de comptes disponibles : ");
			recup_compte_types(bdd);
			puts("Tapez le numéro correspondant pour choisir le type voulu.");
            scanf("%d", &type);

			compte->compte_type = type;
			compte->compte_decouvert_autorise = false;
            ajouter_compte(compte, bdd);
            
			printf("compte fait");
        }
        else
        {
            printf("Veuillez bien chosir entre les deux choix.\n");
        }
	}
}

