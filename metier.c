// Couche métier de l'application : prise en charge des fonctions proposées au client par l'interface, et appel aux fonctions de plus bas niveau
// pour accéder aux données 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sqlite3.h>
#include "include/referentiel.h"
#include "include/metier.h"

// Ne consistent qu'en l'ajout d'une opération puisque les comptes n'ont pas de montant à proprement parler, mais se définissent par la suite des opérations effectuées
bool echange_liquidites(int compte_id, double montant, int type, sqlite3 *bdd){
	Operation *op = malloc(sizeof(Operation));

	// id autoincrement??
	// Type de l'opération : échange liquidités
	// TODO : virer ??? Si que ça comme opération on peut le déduire en voyant que l'un des 2 comptes a pour ID 0
	op->operation_type = 0;

	// On récupère la date dans le format time_t (un entier de longueur définie par les locales du système) qu'on castera en long long int (entier de 64 bits) pour l'enregistrer en BDD
	op->operation_date = time(NULL);
	
	op->operation_expediteur = type ? 0 : compte_id; // Opération avec l'utilisateur 0 <=> crédit 
	op->operation_destinataire = type ? compte_id : 0;

	op->operation_montant = montant;

	// Ecriture opération
	if(ajouter_operation(*op, bdd)) return 1; // Le nombre != EXIT_SUCCESS = 0 sera répercuté jusqu'en haut auquel cas on fermera la BDD, on la restaurera et on arrêtera le programme TODO

	// Pas besoin d'inclure les free dans le if erreur, on ne risque pas le memory leak vu qu'on ne fera plus rien
	free(op);
	
	return 0;
}
bool virement(int compte_id1, int compte_id2, double montant, sqlite3 *bdd){

	Operation *op = malloc(sizeof(Operation));

	// id autoincrement???
	op->operation_type = 1;

	// On récupère la date dans le format time_t (un entier de longueur définie par les locales du système) qu'on castera en long long int (entier de 64 bits) pour l'enregistrer en BDD
	op->operation_date = time(NULL);
	
	// On définit les deux partis de l'opération
	op->operation_expediteur = compte_id1;
	op->operation_destinataire = compte_id2; 

	// Ainsi que son montant
	op->operation_montant = montant;

	// Ecriture opération
	// Le nombre != EXIT_SUCCESS = 0 sera répercuté jusqu'en haut auquel cas on fermera la BDD, on la restaurera et on arrêtera le programme TODO
	if(ajouter_operation(*op, bdd)) return 1; 	
	
	// Pas besoin d'inclure les free dans le if erreur, on ne risque pas le memory leak vu qu'on ne fera plus rien
	free(op);
	
	return 0;
}
