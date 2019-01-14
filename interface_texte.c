// Fonctions relatives à l'interface textuelle (suite de prints et de gets, sélections de menus etc...)
#include <stdio.h>
#include <stdlib.h>
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
    }
}

