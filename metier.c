// Couche métier de l'application : prise en charge des fonctions proposées au client par l'interface, et appel aux fonctions de plus bas niveau
// pour accéder aux données 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sqlite3.h>
#include "include/referentiel.h"
#include "include/metier.h"

// Tout est virement, même les débits et co
bool virement(int compte_id1, int compte_id2, double montant, sqlite3 *bdd){

	Operation *op = malloc(sizeof(Operation));

//	op->operation_type = 1;

	// On récupère la date dans le format time_t (un entier de longueur définie par les locales du système) 
	// qu'on castera en long long int (entier de 64 bits) pour l'enregistrer en BDD
	op->operation_date = time(NULL);
	
	// On définit les deux partis de l'opération
	op->operation_expediteur = compte_id1;
	op->operation_destinataire = compte_id2; 

	// Ainsi que son montant
	op->operation_montant = montant;

	// Ecriture opération
	// Le nombre != EXIT_SUCCESS = 0 sera répercuté jusqu'en haut auquel cas on fermera la BDD, 
	// on la restaurera et on arrêtera le programme TODO
	if(ajouter_operation(*op, bdd)) return 1; 	
	
	// Pas besoin d'inclure les free dans le if erreur, on ne risque pas le memory leak vu qu'on ne fera plus rien
	free(op);
	
	return 0;
}

double calc_solde(int compte_id, time_t date, sqlite3 *bdd) {
	double solde = 0;

	// Récupération de toutes les opérations impliquant le compte
	Operation *op = recup_operations(compte_id, bdd); 

	// Cas où il n'y a pas encore eu d'opérations sur le compte 
	if(op == NULL) return 0;
	else {
		// On avance dans la liste chaînée des opérations en additionnant les soldes
		while(op != NULL && op->operation_date < date) {
			// On soustrait si ce compte est l'expéditeur
			if(op->operation_expediteur == compte_id) solde -= op->operation_montant; 
			// Et on additionne si c'est le destinataire
			if(op->operation_destinataire == compte_id) solde += op->operation_montant; 
			// Et on passe à l'opération suivante
			op = op->suivante;
		}

		return solde;
	}
}
