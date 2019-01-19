#ifndef REF_H
#define REF_H

#define MAX_TITULAIRES 10
#define FICHIER_BDD "bdd/banque_test_set.db"
//#define FICHIER_BDD "bdd/banque_vide.db"
#define PERMISSION_DECOUVERT_DEFAUT 0 // Non

/**************
 * Structures * 
 **************/
// On fera les typedef ici : c'est la couche la plus proche des données, donc le meilleur endroit pour définir des type de données
// De plus, la structure pyramidale de l'application fait en sorte qu'il n'y ait pas de problèmes d'interdépendance entre les headers et entre les structs : pas besoin donc de faire un fichier struct.h indépendant

// Type booléen
typedef enum {false, true} bool;

// "Objets" tels que définis dans la BDD (à quelques exceptions près...)
typedef struct {
	unsigned int titulaire_id;
	char *titulaire_nom;
	char *titulaire_prenom;
} Titulaire;

typedef struct {
	unsigned int compte_id;
	unsigned int compte_type; // Types de compte listés dans la table 'compte_type'
	bool compte_decouvert_autorise;
	int *compte_titulaires;
} Compte;

// La struct opération sera modélisée par une liste chaînée dans le programme : 
// Etant donné que la structure s'appelle récursivement, il faut la déclarer avant de faire le typedef
struct Operation_s;

typedef struct Operation_s {
	unsigned int operation_id;
	time_t operation_date; // time_t est le type que retourne la "temps" de time.h. On castera cette valeur en long long int au moment de faire l'enregistrement BDD. 
	unsigned int operation_expediteur;
	unsigned int operation_destinataire;
	double operation_montant; // Toujours positif, le sens est déterminé par l'identité de l'expéditeur et du destinataire
	struct Operation_s *suivante;
//	unsigned int operation_type; // Type 0 = echange liquidités, type 1 = virement // On ignore parce que virement avec un des 2 id 0 <=> échange liquidités
} Operation;

/**************
 * Opérations *
 **************/

bool ajouter_operation(Operation op, sqlite3 *bdd); // TEST OK
Operation *recup_operations(int compte_id, sqlite3 *bdd); // TEST TODO

/****************************
 * Opérations sur le compte *
 ****************************/

// Edition de compte
bool ajouter_compte(Compte *compte, sqlite3 *bdd); // OK
bool modifier_compte(Compte compte, sqlite3 *bdd); // OK
bool ajouter_compte_titulaire(int compte_id, int titulaire_id, sqlite3 *bdd);
bool enlever_compte_titulaire(int compte_id, int titulaire_id, sqlite3 *bdd);
// bool supprimer_compte(int id);

// Récupération de données compte
Compte *recup_compte(int compte_id, sqlite3 *bdd); // Id = 0 => on génère un compte vide



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
