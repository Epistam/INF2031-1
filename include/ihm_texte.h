#ifndef UI_TEXTE_H
#define UI_TEXTE_H

int choix_interface(sqlite3 *bdd);
int action_titulaire(sqlite3 *bdd);
int banque(sqlite3 *bdd);
bool recherche_titulaire(int id, sqlite3 *bdd);
int titulaire(sqlite3 *bdd);

void menu_virement(sqlite3 *bdd);

void debit(int id, sqlite3 *bdd);
void credit(sqlite3 *bdd);

#endif
