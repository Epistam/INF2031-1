#ifndef REF_H
#define REF_H

#define MAX_TITULAIRES 10
#define LONGUEUR_DATE 19 // YYYY:MM:DD:HH:MM:SS

// Couche la plus proche des données : meilleur endroit pour définir un type de données 
/**************
 * Structures *
 **************/

typedef enum {false, true} bool;

typedef struct {
	unsigned int titulaire_id;
	char *titulaire_nom;
	char *titulaire_prenom;
} Titulaire;

typedef struct {
	unsigned int compte_id;
	unsigned int compte_type;
	bool compte_decouvert_autorise;
	int *compte_titulaires;
} Compte;

struct Operation_s;

typedef struct Operation_s {
	unsigned int operation_id;
	unsigned int operation_type; // Type 0 = echange liquidités, type 1 = virement
	time_t operation_date; // On laisse ça à la charge du système hôte, juste un cast à 64 bits avant de coller ça en bdd
	unsigned int operation_expediteur;
	unsigned int operation_destinataire;
	double operation_montant;
	struct Operation_s *suivante;
} Operation;

/*****************************
 * Fonctions opérationnelles *
 *****************************/
// Opérations sur les... opérations
bool ajouter_operation(Operation op, sqlite3 *bdd);
Operation *recup_operations(int compte_id, sqlite3 *bdd);

/****************************
 * Opérations sur le compte *
 ****************************/

// Edition de compte
bool ajouter_compte(Compte compte, sqlite3 *bdd);
bool modifier_compte(Compte compte, int id, sqlite3 *bdd);
bool ajouter_compte_titulaire(int compte_id, int titulaire_id, sqlite3 *bdd);
bool enlever_compte_titulaire(int compte_id, int titulaire_id, sqlite3 *bdd);
// bool supprimer_compte(int id);
// Récupération de données
Compte *recup_compte(int compte_id, sqlite3 *bdd); // Id = 0 => on génère un compte vide

/*******************************
 * Opérations sur le titulaire *
 *******************************/

bool ajouter_titulaire(Titulaire titulaire, sqlite3 *bdd);
// Pas de suppression titulaire (à moins que ? Juste le supprimer de tous ses comptes)
int *recup_titulaires(int compte_id, sqlite3 *bdd);
#endif
