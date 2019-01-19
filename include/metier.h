#ifndef METIER_H
#define METIER_H

// Virement d'un montant donné d'un compte à un autre
bool virement(int compte_id1, int compte_id2, double montant, sqlite3 *bdd);
// Calcul du solde d'un compte à une date donnée
double calc_solde(int compte_id, time_t date, sqlite3 *bdd);

#endif
