#ifndef REF_H
#define REF_H

#define MAX_TITULAIRES 10
#define FICHIER_BDD "bdd/banque_test_set.db"
//#define FICHIER_BDD "bdd/banque_vide.db"
#define PERMISSION_DECOUVERT_DEFAUT 0 // Non


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
	unsigned int compte_type; // 0 = compte courant, 1 = CB, 2 = assurance vie , 3 = livret A
	bool compte_decouvert_autorise;
	int *compte_titulaires;
} Compte;

struct Operation_s;

typedef struct Operation_s {
	unsigned int operation_id;
//	unsigned int operation_type; // Type 0 = echange liquidités, type 1 = virement // On ignore parce que virement avec un des 2 id 0 <=> échange liquidités
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
bool ajouter_operation(Operation op, sqlite3 *bdd); // OK
Operation *recup_operations(int compte_id, sqlite3 *bdd);

/****************************
 * Opérations sur le compte *
 ****************************/

// Edition de compte
bool ajouter_compte(Compte *compte, sqlite3 *bdd); // OK
bool modifier_compte(Compte compte, sqlite3 *bdd); // OK
bool ajouter_compte_titulaire(int compte_id, int titulaire_id, sqlite3 *bdd);
bool enlever_compte_titulaire(int compte_id, int titulaire_id, sqlite3 *bdd);
// bool supprimer_compte(int id);
// Récupération de données
Compte *recup_compte(int compte_id, sqlite3 *bdd); // Id = 0 => on génère un compte vide

double recup_solde(int compte_id, time_t date, sqlite3 *bdd);

/*******************************
 * Opérations sur le titulaire *
 *******************************/

bool ajouter_titulaire(Titulaire *titulaire, sqlite3 *bdd); // OK
bool modifier_titulaire(Titulaire titulaire, sqlite3 *bdd); // OK
// Pas de suppression titulaire (à moins que ? Juste le supprimer de tous ses comptes)
int *recup_titulaires_ids(int compte_id, int *titulaires_nb, sqlite3 *bdd);
int *recup_comptes_ids(int titulaire_id, int *comptes_nb, sqlite3 *bdd);
//bool recherche_titulaire(); // TODO

void recup_compte_types(sqlite3 *bdd);

#endif
