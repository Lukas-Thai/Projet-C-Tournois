#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma warning (disable : 4996)
#pragma warning (disable : 6031)

#define maxTournois 10
#define nbMatchTournois 127
#define nbJoueusesTournois 128
#define nbJoueusesTournoisWTA nbJoueusesTournois*maxTournois
#define lgMot 30
typedef struct {
	char nom[lgMot + 1];
	unsigned int points;
} Joueuse;

typedef struct {
	unsigned int idxGagnante; //index de la gagnante
	unsigned int idxPerdante; //index de la perdante
} Match;

typedef struct {
	char nom[lgMot + 1];
	int date;
	Match dataMatch[nbMatchTournois];
} Tournoi;

typedef struct {
	Tournoi dataTournois[maxTournois];
	Joueuse dataJoueuses[nbJoueusesTournoisWTA];
	unsigned int NombreJoueuse;
	unsigned int nbTournois;
} TournoiWTA;
//fonction de tri des joueuses par ordre alphabétique
// Joueuse* liste[] In/Out Unsigned int taille In
void tri(Joueuse* liste[], unsigned int taille) {
	Joueuse* temporaire;
	for (unsigned int i = 0; i < taille; ++i) {
		for (unsigned int j = 0; j < taille; ++j) {
			if (strcmp(liste[i]->nom, liste[j]->nom) < 0) {
				temporaire = liste[i];
				liste[i] = liste[j];
				liste[j] = temporaire;
			}
		}
	}
}
//fonction de tri de joueuse par ordre décroissant de points, en cas d'égalité par ordre alphabétique
// Joueuse* liste[] In/Out Unsigned int taille In
void tri2(Joueuse* liste[], unsigned int taille) {
	Joueuse liste_trie[nbJoueusesTournoisWTA];
	Joueuse* max = liste[0];
	unsigned int posmax = 0;
	tri(liste, taille);
	for (unsigned int i = 0; i < taille; ++i) {
		for (unsigned int j = 0; j < taille; ++j) {
			if (liste[j]->points > max->points) {
				max = liste[j];
				posmax = j;
			}
		}
		liste_trie[i] = *max;
		liste[posmax]->points = 0;
	}
	for (unsigned int i = 0; i < taille; ++i) {
		liste[i] = &liste_trie[i];
	}
}
//affiche toutes les joueuses d'un tournoi par ordre alphabétique et leur points obtenu lors de ce tournoi, sprint 3
//TournoiWTA* wta In, char nom_tournoi[] In, int date_tournoi In, int nb_tournois In
void affichage_joueuses_tournoi(TournoiWTA* wta, char nom_tournoi[], int date_tournoi, int nb_tournois) { 
	Joueuse* liste_joueuse[nbJoueusesTournois];
	int placecle[] = { 0,64,96,112,120,124,126 };
	int point[] = { 10,45,90,180,360,720,1200,2000 };
	const int taille = sizeof(placecle) / sizeof(int);
	for (int i = 0; i < nb_tournois; ++i) {
		if (strcmp(wta->dataTournois[i].nom, nom_tournoi) == 0 && wta->dataTournois[i].date == date_tournoi) { //vérification de l'existence du tournois
			printf("%s %d\n", nom_tournoi, date_tournoi);  //print le nom du tournois et sa date
			for (int j = 0; j < nbMatchTournois; ++j) { //boucle pour assigner les points à la gagnante en fonction de sa place
				for (int h = 0; h < taille; ++h) {
					if (j == nbMatchTournois - 1) { //cas exceptionnel de la finale
						wta->dataJoueuses[wta->dataTournois[i].dataMatch[j].idxPerdante].points = point[taille - 1];
						wta->dataJoueuses[wta->dataTournois[i].dataMatch[j].idxGagnante].points = point[taille];
						liste_joueuse[j + 1] = &wta->dataJoueuses[wta->dataTournois[i].dataMatch[j].idxGagnante];
						break;
					}
					else if (placecle[h] <= j && j < placecle[h + 1]) {
						wta->dataJoueuses[wta->dataTournois[i].dataMatch[j].idxPerdante].points = point[h];
						break;
					}
				}
				liste_joueuse[j] = &wta->dataJoueuses[wta->dataTournois[i].dataMatch[j].idxPerdante];
			}
			tri(liste_joueuse, nbJoueusesTournois); //tri de la liste par ordre alphabétique
			for (int h = 0; h < nbJoueusesTournois; ++h) {
				printf("%s %d\n", liste_joueuse[h]->nom, liste_joueuse[h]->points); //affichage du résultat de la liste trié
				liste_joueuse[h]->points = 0; //remise des points à 0 pour de futur affichage
			}
			break;
		}
		else if (i == nb_tournois - 1) {
			printf("tournoi inconnu\n"); //cas du tournoi non trouvé
		}
	}
}
//affiche tout les matchs d'un tournoi,sprint 1 
//TournoiWTA* wta In , char nom_tournoi[] In , int date In, int nb_tournois In
void affichage_matchs_tournoi(TournoiWTA* wta, char nom_tournoi[], int date, int nb_tournois) { 
	char* partie[] = { "64emes de finale\n","32emes de finale\n","16emes de finale\n","8emes de finale\n","quarts de finale\n","demi-finales\n","finale\n" };
	int placecle[] = { 0,64,96,112,120,124,126 };
	for (int i = 0; i < nb_tournois; ++i) {
		if (strcmp(wta->dataTournois[i].nom, nom_tournoi) == 0 && wta->dataTournois[i].date == date) { //vérification de l'existence du tournois
			printf("%s %i\n", nom_tournoi, date);  //print le nom du tournois et sa date
			for (int j = 0; j < nbMatchTournois; ++j) {
				for (int h = 0; h < (sizeof(placecle) / sizeof(int)); ++h) {
					if (j == placecle[h]) {
						printf("%s", partie[h]);
					}
				}
				printf("%s %s\n", wta->dataJoueuses[wta->dataTournois[i].dataMatch[j].idxGagnante].nom, wta->dataJoueuses[wta->dataTournois[i].dataMatch[j].idxPerdante].nom); //print le nom de la gagnante et de la perdante du match
			}
			break;
		}
		else if (i == nb_tournois - 1) {
			printf("tournoi inconnu\n");
		}
	}
}
//vérifie si le nom de la joueuse est présente dans la liste puis indique ça présence ou non avec sa position dans la liste de joueuse
//TournoiWTA* ins In, char nom[] In, int* position Out, int* presence Out
void verification_presence(TournoiWTA* ins, char nom[], int* position, int* presence) {
	for (unsigned int i = 0; i < ins->NombreJoueuse; ++i) { //boucle qui parcours la liste des joueuses pour regarder leur présence dans la liste des joueuses
		if (strcmp(ins->dataJoueuses[i].nom, nom) == 0) {
			*presence = 1;  // la valeur de 1 indique que la joueuse a été trouvé
			*position = i; //retiens la position si trouvé
		}
	}
}
//enregistre les matchs des joueuses dans tournoi et recense les joueuses
//TournoiWTA* ins In/Out, int nb_tournoi In
void enregistrement_tournoi(TournoiWTA* ins, int nb_tournoi) {
	char gagnant[lgMot + 1];
	char perdant[lgMot + 1];
	int position = 0; //variable qui indique la présence ou non de la joueuse dans la liste de joueuse
	int presence = 0; //position dans la liste de la joueuse 
	Match matchs;
	Joueuse joue;
	for (int i = 0; i < nbMatchTournois; ++i) { // boucle d'enregistrement pour enregistrer tout les match
		scanf("%s", gagnant); // prends le premier nom pour la gagante
		scanf("%s", perdant); // prends le premeir nom pour la perdante
		verification_presence(ins, perdant, &position, &presence);
		if (presence == 0) { //cas de la joueuse non trouvé
			strcpy(joue.nom, perdant);
			joue.points = 0;
			ins->dataJoueuses[ins->NombreJoueuse] = joue;
			matchs.idxPerdante = ins->NombreJoueuse;
			++ins->NombreJoueuse;
		}
		else { //cas ou elle est trouvé
			matchs.idxPerdante = position;
			presence = 0;
		}
		verification_presence(ins, gagnant, &position, &presence);
		if (presence == 0) { //cas de la joueuse non trouvé
			strcpy(joue.nom, gagnant);
			joue.points = 0;
			ins->dataJoueuses[ins->NombreJoueuse] = joue;
			matchs.idxGagnante = ins->NombreJoueuse;
			++ins->NombreJoueuse;
		}
		else { // cas ou elle est trouvé
			matchs.idxGagnante = position;
			presence = 0;
		}
		ins->dataTournois[nb_tournoi].dataMatch[i] = matchs; //ajout du match avec les id des joueuses dans les données du tournoi
	}
}
//affiche les matchs d'une joueuse dans un tournoi, sprint2
//TournoiWTA* wta In, char ville[] In, int date In, char nomf[] In, int nb_tournois In
void afficher_matchs_joueuse(TournoiWTA* wta, char ville[], int date, char nomf[], int nb_tournois) { 
	int jpre = 0; //présence ou non de la joueuse
	for (unsigned int i = 0; i < wta->NombreJoueuse; ++i) { //vérification de l'existene de la joueuse
		if (strcmp(wta->dataJoueuses[i].nom, nomf) == 0) {
			jpre = 1;
			break;
		}
		else if (i == wta->NombreJoueuse - 1) { //cas de la joueuse non trouvé au sein des joueuses recensé 
			printf("joueuse inconnue");
		}
	}
	for (int i = 0; i < nb_tournois; ++i) { //boucle pour vérifier que le tournoi existe
		if (jpre == 0) { //si la joueuse n'existe pas, pas besoin de chercher le tournoi
			break;
		}
		if (strcmp(wta->dataTournois[i].nom, ville) == 0 && wta->dataTournois[i].date == date) { //vérification de l'existence du tournois
			for (int j = 0; j < nbMatchTournois; ++j) {//recherche des match ou son nom apparait
				if (strcmp(wta->dataJoueuses[wta->dataTournois[i].dataMatch[j].idxGagnante].nom, nomf) == 0) {
					printf("%s %s\n", wta->dataJoueuses[wta->dataTournois[i].dataMatch[j].idxGagnante].nom, wta->dataJoueuses[wta->dataTournois[i].dataMatch[j].idxPerdante].nom); //Affichage des match ou elle apparait 
				}
				else if (strcmp(wta->dataJoueuses[wta->dataTournois[i].dataMatch[j].idxPerdante].nom, nomf) == 0) {
					printf("%s %s\n", wta->dataJoueuses[wta->dataTournois[i].dataMatch[j].idxGagnante].nom, wta->dataJoueuses[wta->dataTournois[i].dataMatch[j].idxPerdante].nom);
				}
			}
		}
		else if (i == nb_tournois - 1) { //cas ou le tournoi est introuvable
			printf("tournoi inconnu\n");
		}
	}
}
//Permet de calculer les points des joueuses selon les règles du WTA(4 derniers tournois) et de les afficher par ordre décroissant de point puis par ordre alphabétique s'il y a égalité, sprint 4 et 5
//TournoiWTA* wta In, int nb_tournois In
void afficher_classement(TournoiWTA* wta, int nb_tournois) {
	Joueuse* liste[nbJoueusesTournoisWTA];
  int nb_dernier_tournoi=4;
	int placecle[] = { 0,64,96,112,120,124,126 };
	int point[] = { 10,45,90,180,360,720,1200,2000 };
	const int taille = sizeof(placecle) / sizeof(int);
	int i = 0;
	if (nb_tournois > nb_dernier_tournoi) { //permet d'avoir uniquement les 4 derniers tournoi de calculer 
		i = nb_tournois - nb_dernier_tournoi;
	}
	for (i = i; i < nb_tournois; ++i) { //parcours tout leur tournoi et match pour assigner les points
		for (int j = 0; j < nbMatchTournois; ++j) {
			for (int h = 0; h < taille; ++h) {
				if (j == nbMatchTournois - 1) {
					wta->dataJoueuses[wta->dataTournois[i].dataMatch[j].idxPerdante].points += point[taille - 1];
					wta->dataJoueuses[wta->dataTournois[i].dataMatch[j].idxGagnante].points += point[taille];
					break;
				}
				else if (placecle[h] <= j && j < placecle[h + 1]) {
					wta->dataJoueuses[wta->dataTournois[i].dataMatch[j].idxPerdante].points += point[h];
					break;
				}
			}
		}
	}
	for (unsigned int i = 0; i < wta->NombreJoueuse; ++i) { //création d'une liste à trier
		liste[i] = &wta->dataJoueuses[i];
	}
	tri2(liste, wta->NombreJoueuse); //tri de la liste
	for (unsigned int h = 0; h < wta->NombreJoueuse; ++h) {
		if (liste[h]->points > 0) {
			printf("%s %d\n", liste[h]->nom, liste[h]->points); //Affiche la joueuse et son score
			wta->dataJoueuses[h].points = 0; //Remise à 0 des points pour de futur commande
		}
	}
}

int main() {
	int val;
	char nomf[lgMot + 1]; // Nom de famille du joueur
	char mot[lgMot + 1]; //variable dans lequel sera stocké la commande
	char nom[lgMot + 1]; //variable qui sert a stocké le nom des tournois
	int nb_tournois = 0; //variable qui définis le nombre de tournois
	TournoiWTA wta;
	wta.NombreJoueuse = 0;
	wta.nbTournois = 0;
	Tournoi tour;
	while (1) { // ou do {…} while(1);
	  // Lecture de la commande (mot)

	  // si la commande est "definir_nombre_tournois"
	  // appeler la fonction definir_nombre_tournois
	  // …
	  // si la commande est "exit" .
		scanf("%s", mot); // récupère la commande
		if (strcmp(mot, "exit") == 0) { //permet de finir l'execution de la fonction 
			return 0;
		}
		else if (strcmp(mot, "definir_nombre_tournois") == 0) {
			scanf("%i", &nb_tournois); //récupère la valeur qui suit la commande
		}
		else if (strcmp(mot, "enregistrement_tournoi") == 0) {
			scanf("%s", tour.nom); //permet de récupérer le nom du tournois après la commande
			scanf("%i", &tour.date); // permet de récupérer la date du tournois après le nom du tournois
			wta.dataTournois[wta.nbTournois] = tour;
			enregistrement_tournoi(&wta, wta.nbTournois); //début de l'enregistrement des tournois
			wta.nbTournois += 1;
		}
		else if (strcmp(mot, "affichage_matchs_tournoi") == 0) {
			scanf("%s", nom); //récupère le nom du tournois
			scanf("%i", &val); //récupère la date
			affichage_matchs_tournoi(&wta, nom, val, wta.nbTournois);
		}
		else if (strcmp(mot, "afficher_matchs_joueuse") == 0) {
			scanf("%s", nom);
			scanf("%i", &val);
			scanf("%s", nomf);
			afficher_matchs_joueuse(&wta, nom, val, nomf, wta.nbTournois);
		}
		else if (strcmp(mot, "affichage_joueuses_tournoi") == 0) {
			scanf("%s", nom);
			scanf("%i", &val);
			affichage_joueuses_tournoi(&wta, nom, val, wta.nbTournois);
		}
		else if (strcmp(mot, "afficher_classement") == 0) {
			if (wta.nbTournois == 0) { //cas ou aucun tournoi est enregistré
				printf("pas de classsement");
			}
			else {
				afficher_classement(&wta, wta.nbTournois);
			}
		}
		else {
			printf("Commande Inconnu\n");
		}
	}
}