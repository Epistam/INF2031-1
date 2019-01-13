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

	sqlite3_stmt *stmt;
	sqlite3_prepare_v2(bdd, "INSERT INTO operations VALUES(NULL, ?, ?, ?, ?, ?, ?)", -1, &stmt, NULL);
	sqlite3_bind_int(stmt, 1, op.operation_type);
	sqlite3_bind_int(stmt, 2, (long long int)op.operation_date);
	sqlite3_bind_int(stmt, 3, op.operation_expediteur);
	sqlite3_bind_int(stmt, 4, op.operation_destinataire);
	sqlite3_bind_double(stmt, 5, op.operation_montant);
	
	int res = sqlite3_step(stmt);
	
	sqlite3_finalize(stmt);
	if(res != SQLITE_DONE) return -1;
	else return 0;
			

	// free opeerartion ? en sortant du menu free la liste chaînée
}

Operation *recup_operations(int compte_id, sqlite3 *bdd) {

	sqlite3_stmt *stmt;
	Operation *op_origine, *op; // Opération d'origine qui sera renvoyée par la fonction, base de la liste chaînée, et opération courante qui sera remplie contigüe à la précédente
	
	// Préparation de la requête
	sqlite3_prepare_v2(bdd, "SELECT * FROM comptes WHERE operation_expediteur = ? OR operation_destinaire ?	ORDER BY operation_date", -1, &stmt, NULL);
	sqlite3_bind_int(stmt, 1, compte_id);
	sqlite3_bind_int(stmt, 2, compte_id);

	// Exécution de la requête : première ligne
	// S'il n'y a pas d'opérations pour ce compte, on retourne directement NULL. Sinon, on définit l'opération d'origine qui sera retournée de toutes façons et on base l'opération courante dessus
	if(sqlite3_step(stmt) == SQLITE_DONE) return NULL; 
	else {
		op_origine = malloc(sizeof(Operation));
		op = op_origine;

		op_origine->operation_id = sqlite3_column_int(stmt,0);
		op_origine->operation_type = sqlite3_column_int(stmt,1);
		op_origine->operation_date = (time_t)sqlite3_column_int(stmt,2);
		op_origine->operation_expediteur = sqlite3_column_int(stmt,3);
		op_origine->operation_destinataire = sqlite3_column_int(stmt,4);
		op_origine->suivante = NULL; // Défini à NULL au cas où on rentre pas dans la boucle d'après
	}
	// Exécution de la requête : suite
	// On continue à descendre dans le tableau en allouant et liant le prochain noeud à l'opération courante, puis on passe à ce noeud et on le remplit
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

	sqlite3_finalize(stmt);
	return op_origine; // On retourne un pointeur sur l'opération d'origine grâce à laquelle on pourra parcourir la liste chaînée
}

/****************************
 * Opérations sur le compte *
 ****************************/

// Opérations basiques

// Edition de compte
bool ajouter_compte(Compte compte, sqlite3 *bdd){

	sqlite3_stmt *stmt;
	sqlite3_prepare_v2(bdd, "INSERT INTO comptes VALUES(NULL, ?, ?)", -1, &stmt, NULL);
	sqlite3_bind_int(stmt, 1, compte.compte_type);
	sqlite3_bind_int(stmt, 2, PERMISSION_DECOUVERT_DEFAUT);
	
	int res = sqlite3_step(stmt);
	
	sqlite3_finalize(stmt);
	if(res != SQLITE_DONE) return -1;
	else return 0;
}

bool modifier_compte(Compte compte, int id, sqlite3 *bdd){
	
	sqlite3_stmt *stmt;

	sqlite3_prepare_v2(bdd, "UPDATE comptes SET compte_type = ?, compte_decouvert_autorise = ? WHERE compte_id = ?", -1, &stmt, NULL);
	sqlite3_bind_int(stmt, 1, compte.compte_type);
	sqlite3_bind_int(stmt, 2, compte.compte_decouvert_autorise);
	sqlite3_bind_int(stmt, 3, compte.compte_id);
	
	int res = sqlite3_step(stmt);

	sqlite3_finalize(stmt);
	if(res != SQLITE_DONE) return -1;
	else return 0;
}

bool ajouter_compte_titulaire(int compte_id, int titulaire_id, sqlite3 *bdd){ // Dans métier proposer ajout titulaires séparés par séparateur à la création TODO + TODO : TitulaireS ? Liste de titulaires à add 
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
		sqlite3_step(stmt); // ids uniques
	
		// On remplit les différents champs de compte
		compte->compte_id = sqlite3_column_int(stmt,0);
		compte->compte_type = sqlite3_column_int(stmt,1);
		compte->compte_decouvert_autorise = (bool)sqlite3_column_int(stmt,2);

		// Remplissage des titulaires du compte
		compte->compte_titulaires = recup_titulaires(compte_id,bdd);
		sqlite3_finalize(stmt);
	}

	return compte;
}

/*******************************
 * Opérations sur le titulaire *
 *******************************/

bool ajouter_titulaire(Titulaire titulaire, sqlite3 *bdd){

	sqlite3_stmt *stmt;
	sqlite3_prepare_v2(bdd, "INSERT INTO titulaires VALUES(NULL, ?, ?)", -1, &stmt, NULL);
	sqlite3_bind_text(stmt, 1, titulaire.titulaire_nom, strlen(titulaire.titulaire_nom), NULL);
	sqlite3_bind_text(stmt, 2, titulaire.titulaire_prenom, strlen(titulaire.titulaire_prenom), NULL);
	
	int res = sqlite3_step(stmt);
	
	sqlite3_finalize(stmt);
	if(res != SQLITE_DONE) return -1;
	else return 0;
}
bool modifier_titulaire(Titulaire titulaire, sqlite3 *bdd){
	sqlite3_stmt *stmt;
	sqlite3_prepare_v2(bdd, "UPDATE titulaires SET titulaire_nom = ?, titulaire_prenom = ? WHERE titulaire_id = ?", -1, &stmt, NULL);
	sqlite3_bind_text(stmt, 1, titulaire.titulaire_nom, strlen(titulaire.titulaire_nom), NULL);
	sqlite3_bind_text(stmt, 2, titulaire.titulaire_prenom, strlen(titulaire.titulaire_prenom), NULL);
	sqlite3_bind_int(stmt, 3, titulaire.titulaire_id);

	int res = sqlite3_step(stmt);

	sqlite3_finalize(stmt);
	if(res != SQLITE_DONE) return -1;
	else return 0;

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
		sqlite3_finalize(stmt);
	}
	return liste_titulaires;
}
// Pas de suppression titulaire (à moins que ? Juste le supprimer de tous ses comptes)
