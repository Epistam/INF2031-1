// Couche référentiel : fonctions et procédures de bas niveau d'accès aux données (à la base de données en l'occurrence)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "include/referentiel.h"

/**************
 * Opérations *
 **************/

bool ajouter_operation(Operation op, sqlite3 *bdd){ // Penser à cast time_t à long long int

	// Préparation requête
	sqlite3_stmt *stmt;
	sqlite3_prepare_v2(bdd, "INSERT INTO operations VALUES(NULL, ?, ?, ?, ?)", -1, &stmt, NULL);
	sqlite3_bind_int(stmt, 1, (long long int)op.operation_date);
	sqlite3_bind_int(stmt, 2, op.operation_expediteur);
	sqlite3_bind_int(stmt, 3, op.operation_destinataire);
	sqlite3_bind_double(stmt, 4, op.operation_montant);
	
	// Exécution
	int res = sqlite3_step(stmt);
	
	// Nettoyage de la requête en mémoire, et gestion des erreurs
	sqlite3_finalize(stmt);
	if(res != SQLITE_DONE) return -1;
	else return 0;
}

Operation *recup_operations(int compte_id, sqlite3 *bdd) {

	sqlite3_stmt *stmt;
	// Opération d'origine, base de la liste chaînée qui sera renvoyée par la fonction, et opération courante qui sera "tête" de liste pendant sa création
	Operation *op_origine, *op; 

	// Préparation de la requête
	sqlite3_prepare_v2(bdd, "SELECT * FROM operations WHERE operation_expediteur = ? OR operation_destinataire = ? ORDER BY operation_date", -1, &stmt, NULL);
	sqlite3_bind_int(stmt, 1, compte_id);
	sqlite3_bind_int(stmt, 2, compte_id);

	// Exécution de la requête : première opération
	// S'il n'y a pas d'opérations pour ce compte, on retourne directement NULL. 
	// Sinon, on définit l'opération d'origine qui sera retournée de toutes façons et on base l'opération courante dessus
	if(sqlite3_step(stmt) == SQLITE_DONE) return NULL; 
	else {
		op_origine = malloc(sizeof(Operation));
		op = op_origine;

		op_origine->operation_id = sqlite3_column_int(stmt,0);
		// Ici on fait le cast pour éviter tout problème. 
		// On est dans le sens BDD -> Application et le type de la variable est time_t dans l'application
		op_origine->operation_date = (time_t)sqlite3_column_int(stmt,1);
		op_origine->operation_expediteur = sqlite3_column_int(stmt,2);
		op_origine->operation_destinataire = sqlite3_column_int(stmt,3);
		op_origine->operation_montant = sqlite3_column_int(stmt,4);
		op_origine->suivante = NULL; // Défini à NULL au cas où on rentre pas dans la boucle d'après
	}

	// Exécution de la requête : suite
	// On continue à descendre dans le tableau en allouant et liant le prochain noeud à l'opération courante, puis on passe à ce noeud et on le remplit
	while(sqlite3_step(stmt) != SQLITE_DONE) {
		// A la première itération, op et op_origine pointent tous deux sur le même chaînon. 
		// Donc en touchant à op ici, on change bien aussi op_origine->suivant 
		// (on a plutôt intérêt étant donné que c'est l'élément de la liste qu'on retourne et qu'il n'est pas orphelin dans le cas général).
		op->suivante = malloc(sizeof(Operation)); 
		op = op->suivante;

		op->operation_id = sqlite3_column_int(stmt,0);
		op->operation_date = (time_t)sqlite3_column_int(stmt,1);
		op->operation_expediteur = sqlite3_column_int(stmt,2);
		op->operation_destinataire = sqlite3_column_int(stmt,3);
		op->operation_montant = sqlite3_column_int(stmt,4);
		op->suivante = NULL; // Défini à NULL au cas où on rentre pas dans l'itération d'après
	}
	
	// On libère stmt à la fin de la fonction
	sqlite3_finalize(stmt);
	// On retourne un pointeur sur l'opération d'origine grâce à laquelle on pourra parcourir la liste chaînée
	return op_origine; 
}

/**********
 * Compte *
 **********/

bool ajouter_compte(Compte *compte, sqlite3 *bdd){
	// Préparation de la requête
	sqlite3_stmt *stmt;
	sqlite3_prepare_v2(bdd, "INSERT INTO comptes VALUES(NULL, ?, ?)", -1, &stmt, NULL);
	sqlite3_bind_int(stmt, 1, compte->compte_type);
	sqlite3_bind_int(stmt, 2, PERMISSION_DECOUVERT_DEFAUT); // Pas besoin de cast car la constante est déjà en entier

	// Exécution de la requête
	int res = sqlite3_step(stmt);
	
	// Libération de l'objet requête
	sqlite3_finalize(stmt);

	// S'il y a eu une erreur, retourner -1
	if(res != SQLITE_DONE) return -1;
	else { // Sinon on récupère le numéro de compte (id de la dernière insertion, c'est-à-dire celle du nouveau compte
		// Préparation
		sqlite3_stmt *stmt2;
		sqlite3_prepare_v2(bdd, "SELECT last_insert_rowid()", -1, &stmt2, NULL);
		
		// Exécution et complétion du compte
		sqlite3_step(stmt2);
		compte->compte_id = sqlite3_column_int(stmt2,0);
		
		// Libération de stmt2
		sqlite3_finalize(stmt2);

		return 0;
	}
}

bool modifier_compte(Compte compte, sqlite3 *bdd){

	// Compte préparation de la requête
	sqlite3_stmt *stmt;
	sqlite3_prepare_v2(bdd, "UPDATE comptes SET compte_type = ?, compte_decouvert_autorise = ? WHERE compte_id = ?", -1, &stmt, NULL);
	sqlite3_bind_int(stmt, 1, compte.compte_type);
	// Booléen en local, entier en BDD
	sqlite3_bind_int(stmt, 2, (int)compte.compte_decouvert_autorise);
	sqlite3_bind_int(stmt, 3, compte.compte_id);

	// Exécution requête
	int res = sqlite3_step(stmt);
	
	// Libération de la requête
	sqlite3_finalize(stmt);

	// Traitement des erreurs
	if(res != SQLITE_DONE) return -1;
	else return 0;
}

bool ajouter_compte_titulaire(int compte_id, int titulaire_id, sqlite3 *bdd){ 
	
	sqlite3_stmt *stmt;
	sqlite3_prepare_v2(bdd, "INSERT INTO compte_titulaire VALUES(?, ?)", -1, &stmt, NULL);
	sqlite3_bind_int(stmt, 1, compte_id);
	sqlite3_bind_int(stmt, 2, titulaire_id);
	
	int res = sqlite3_step(stmt);
	
	sqlite3_finalize(stmt);

	if(res != SQLITE_DONE) return -1;
	else return 0;
	
}

bool enlever_compte_titulaire(int compte_id, int titulaire_id, sqlite3 *bdd){
	sqlite3_stmt *stmt;
	sqlite3_prepare_v2(bdd, "DELETE FROM compte_titulaire WHERE compte_id = ? AND titulaire_id = ?", -1, &stmt, NULL);
	sqlite3_bind_int(stmt, 1, compte_id);
	sqlite3_bind_int(stmt, 2, titulaire_id);
	
	int res = sqlite3_step(stmt);
	
	sqlite3_finalize(stmt);
	
	if(res != SQLITE_DONE) return -1;
	else return 0;
}

Compte *recup_compte(int compte_id, int *titulaires_nb, sqlite3 *bdd){
	// Allocation du compte 
	Compte *compte = malloc(sizeof(Compte));
	
	// On vérifie qu'on désire un compte non vide
	if(compte_id) {
		// Préparation requête
		sqlite3_stmt *stmt;
		sqlite3_prepare_v2(bdd, "SELECT * FROM comptes WHERE compte_id = ?", -1, &stmt, NULL);
		sqlite3_bind_int(stmt, 1, compte_id);
		sqlite3_step(stmt);
	
		// On remplit les différents champs de compte
		compte->compte_id = sqlite3_column_int(stmt,0);
		compte->compte_type = sqlite3_column_int(stmt,1);
		compte->compte_decouvert_autorise = (bool)sqlite3_column_int(stmt,2);

		// Remplissage des titulaires du compte
		compte->compte_titulaires = recup_titulaires_ids(compte_id,titulaires_nb,bdd);

		sqlite3_finalize(stmt);
	}

	return compte;
}

int *recup_comptes_ids(int titulaire_id, int *comptes_nb, sqlite3 *bdd) {
	
	int *liste_comptes_ids = NULL;

	// On prépare la requête
	sqlite3_stmt *stmt;
	sqlite3_prepare_v2(bdd, "SELECT * FROM compte_titulaire WHERE titulaire_id = ?", -1, &stmt, NULL);
	sqlite3_bind_int(stmt, 1, titulaire_id);
	
	// On trouve toutes les occurences de ce compte dans la table compte_titulaire et on ajoute les id des titulaires à la liste
	for(*comptes_nb = 0 ; sqlite3_step(stmt) != SQLITE_DONE ; (*comptes_nb)++) {
		liste_comptes_ids = realloc(liste_comptes_ids,(*comptes_nb+1)*sizeof(int));
		liste_comptes_ids[*comptes_nb] = sqlite3_column_int(stmt,0);
	}
	sqlite3_finalize(stmt);
	
	return liste_comptes_ids;
}

char **recup_compte_types(int *types_nb, sqlite3 *bdd) {

	char **compte_types = NULL;
	
	// On prépare la requête
	sqlite3_stmt *stmt;
	sqlite3_prepare_v2(bdd, "SELECT * FROM compte_type", -1, &stmt, NULL);
	
	// On trouve toutes les occurences de ce compte dans la table compte_titulaire et on ajoute les id des titulaires à la liste
	for(*types_nb = 0 ; sqlite3_step(stmt) != SQLITE_DONE ; (*types_nb)++) {
		compte_types = realloc(compte_types,(*types_nb+1)*sizeof(void*));
		compte_types[*types_nb] = strdup((const char*)sqlite3_column_text(stmt, 1)); // sqlite retourne des char non signés alors que strdup prend des char signés
	}

	sqlite3_finalize(stmt);

	return compte_types;
}

/*************
 * Titulaire *
 *************/

bool ajouter_titulaire(Titulaire *titulaire, sqlite3 *bdd){

	sqlite3_stmt *stmt;
	sqlite3_prepare_v2(bdd, "INSERT INTO titulaires VALUES(NULL, ?, ?)", -1, &stmt, NULL);
	sqlite3_bind_text(stmt, 1, titulaire->titulaire_nom, strlen(titulaire->titulaire_nom), NULL);
	sqlite3_bind_text(stmt, 2, titulaire->titulaire_prenom, strlen(titulaire->titulaire_prenom), NULL);
	
	int res = sqlite3_step(stmt);
	
	sqlite3_finalize(stmt);

	if(res != SQLITE_DONE) return -1;
	else {
		sqlite3_stmt *stmt2;
		sqlite3_prepare_v2(bdd, "SELECT last_insert_rowid()", -1, &stmt2, NULL);
		
		sqlite3_step(stmt2);

		titulaire->titulaire_id = sqlite3_column_int(stmt2,0);
		
		sqlite3_finalize(stmt2);

		return 0;
	}
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

int *recup_titulaires_ids(int compte_id, int *titulaires_nb, sqlite3 *bdd) { 

	int *liste_titulaires_ids = NULL; 
	
	// On prépare la requête
	sqlite3_stmt *stmt;
	sqlite3_prepare_v2(bdd, "SELECT * FROM compte_titulaire WHERE compte_id = ?", -1, &stmt, NULL);
	sqlite3_bind_int(stmt, 1, compte_id);
	
	// On trouve toutes les occurences de ce compte dans la table compte_titulaire et on ajoute les id des titulaires à la liste
	for(*titulaires_nb = 0 ; sqlite3_step(stmt) != SQLITE_DONE ; (*titulaires_nb)++) { // Note : ++ est prioritaire sur *...
		// On aggrandit la tableau à chaque fois
		liste_titulaires_ids = realloc(liste_titulaires_ids,(*titulaires_nb+1)*sizeof(int));
		liste_titulaires_ids[*titulaires_nb] = sqlite3_column_int(stmt,1);
	}

	// Nettoyage
	sqlite3_finalize(stmt);

	// Et on retourne le pointeur sur leur liste
	return liste_titulaires_ids;
}
