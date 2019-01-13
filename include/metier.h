#ifndef METIER_H
#define METIER_H

bool echange_liquidites(int compte_id, double montant, int type, sqlite3 *bdd); // type = 0 => crédit, 1 => débit
bool virement(int compte_id1, int compte_id2, double montant, sqlite3 *bdd);

#endif
