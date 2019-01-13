// Couche référentiel : fonctions et procédures de bas niveau d'accès aux données (à la base de données en l'occurrence)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "include/referentiel.h"

/*****************************
 * Fonctions opérationnelles *
 *****************************/
// Opérations sur les... opérations
bool ajouter_operation(Operation op, sqlite3 *bdd){ // Penser à cast time_t à long long int
	
}

Operation *recup_operations(int compte_id, sqlite3 *bdd) {

	sqlite3_stmt *stmt;
	Operation *op_origine, *op;
	
	sqlite3_prepare_v2(bdd, "SELECT * FROM comptes WHERE operation_expediteur = ? OR operation_destinaire ?	ORDER BY operation_date", -1, &stmt, NULL);
	
	sqlite3_bind_int(stmt, 1, compte_id);
	sqlite3_bind_int(stmt, 2, compte_id);
	if(sqlite3_step(stmt) == SQLITE_DONE) return NULL; // On remplit la première opération celle qu'on retournera (à moins qu'il n'y en ait pas auquel cas on retourne NULL)
	else {
		op_origine = malloc(sizeof(Operation));
		op = op_origine;

		op_origine->operation_id = sqlite3_column_int(stmt,0);
		op_origine->operation_type = sqlite3_column_int(stmt,1);
		op_origine->operation_date = sqlite3_column_int(stmt,2);
		op_origine->operation_expediteur = sqlite3_column_int(stmt,3);
		op_origine->operation_destinataire = sqlite3_column_int(stmt,4);
		op_origine->suivante = NULL; // Défini à NULL au cas où on rentre pas dans la boucle d'après
	}
	while(sqlite3_step(stmt) != SQLITE_DONE) {
		op->suivante = malloc(sizeof(Operation)); // op et op_origine pointent tous deux sur le même objet à la première itération. Donc en assignant ici on change aussi op_origine->suivant
		op = op->suivante;
		
		op->operation_id = sqlite3_column_int(stmt,0);
		op->operation_type = sqlite3_column_int(stmt,1);
		op->operation_date = sqlite3_column_int(stmt,2);
		op->operation_expediteur = sqlite3_column_int(stmt,3);
		op->operation_destinataire = sqlite3_column_int(stmt,4);
		op->suivante = NULL; // Défini à NULL au cas où on rentre pas dans la boucle d'après
	}

	return op_origine; // On retourne un pointeur sur l'opération d'origine grâce à laquelle on pourra parcourir la liste chaînée
}

/****************************
 * Opérations sur le compte *
 ****************************/

// Opérations basiques

// Edition de compte
bool ajouter_compte(Compte compte, sqlite3 *bdd){
}
bool modifier_compte(Compte compte, int id, sqlite3 *bdd){
}
bool ajouter_compte_titulaire(int compte_id, int titulaire_id, sqlite3 *bdd){
}
bool enlever_compte_titulaire(int compte_id, int titulaire_id, sqlite3 *bdd){
}
// bool supprimer_compte(int id){
//}

// Récupération de données : si on veut juste un compte ivde, id = 0
Compte *recup_compte(int compte_id, sqlite3 *bdd){
	Compte *compte = malloc(sizeof(Compte));
	
	if(compte_id) { // implicite : != 0
		// On prépare la requête
		sqlite3_stmt *stmt;
		sqlite3_prepare_v2(bdd, "SELECT * FROM comptes WHERE id = ?", -1, &stmt, NULL);
		sqlite3_bind_int(stmt, 1, compte_id);
		sqlite3_step(stmt);
	
		// On remplit les différents champs de compte
		compte->compte_id = sqlite3_column_int(stmt,0);
		compte->compte_type = sqlite3_column_int(stmt,1);
		compte->compte_decouvert_autorise = (bool)sqlite3_column_int(stmt,2);

		// Remplissage des titulaires du compte
		compte->compte_titulaires = recup_titulaires(compte_id,bdd);
	}
	return compte;
}

/*******************************
 * Opérations sur le titulaire *
 *******************************/

bool ajouter_titulaire(Titulaire titulaire, sqlite3 *bdd){
}
int *recup_titulaires(int compte_id, sqlite3 *bdd) {

	int *liste_titulaires = malloc(MAX_TITULAIRES*sizeof(int));

	if(compte_id) {
		// On prépare la requête
		sqlite3_stmt *stmt;
		sqlite3_prepare_v2(bdd, "SELECT * FROM compte_titulaire WHERE compte_id = ?", -1, &stmt, NULL);
		sqlite3_bind_int(stmt, 1, compte_id);
		
		// On trouve toutes les occurences de ce compte dans la table compte_titulaire et on ajoute les id des titulaires à la liste
		int i;
		for(i = 0 ; sqlite3_step(stmt) != SQLITE_DONE ; i++) liste_titulaires[i] = sqlite3_column_int(stmt,1);
	}
	return liste_titulaires;
}
// Pas de suppression titulaire (à moins que ? Juste le supprimer de tous ses comptes)
