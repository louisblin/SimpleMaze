#define MAC

#include <stdio.h>
#include <stdlib.h>
#include <string.h> // pour utiliser strcat(str1, str2)

#ifndef  MAC
#include <windows.h>
#endif

//// Prototypes (declaration des functions pour pouvoir les reorganiseer)
// Chargement d'un niveau
void recupTab(char matrice[15][19], int *x, int *y, int niveauID);
void recupPartie(char matrice[15][19], int *x, int *y, FILE *fic);
void chargerNiveauN(const int n);

// Execution d'un niveau
void jouerNiveauN(char matrice[15][19], int x, int y, const int n);
void deplacement(int *x,int *y, char matrice[15][19]);
int prochaineCaseEstValide(char matrice[15][19], int x, int y);

// Score
void sauverScore(int nouveauScore);

// Sauvegarde / chargement partie
void sauvegarderPartie(int niveau, int score, char matrice[15][19]);
void chargerPartie();

// Utilitaires
void afficherFichier(const char* fichier);
void affichage (char matrice [15][19], int x,int y);
int recupInt();
char recupChar(FILE *fic);
char lireDeLaCommandLine();


//// Constantes globales (visibles dans toutes les functions...)
int scoreEnCours, niveauEnCours;
char characterRecouvert;


int main(void) {

    int choix = -1, choix2 = -1;

    do {
        printf("Bienvenue dans ECE Man\n1. Nouvelle partie\n2. Reprendre partie\n3. Regles\n4. Score\n0. Quitter\n\n");
        choix = recupInt();

        switch(choix)
        {
        case 0:
            break;

        case 1:
            printf("1.Niveau1\n2.Niveau2\n3.Niveau3\n4.Niveau4\n5.Niveau5\n");
            choix2 = recupInt();

            switch(choix2)
            {
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
                chargerNiveauN(choix2); // Pour les choix [1-5], lancher le niveau correspondant
                break;

            default:
                printf("Niveau %d inconnu... retour au menu principal\n", choix2);
                choix = -1; // marqueur d'un choix valide...
            }
            break;

        case 2:
            printf("Chargement de la derniere partie sauvegardee...");
            chargerPartie();
            break;

        case 3:
            printf("Affichage des regles...\n");
            afficherFichier("reglejeux.txt");
            break;

        case 4:
            printf("Affichage des scores...\n");
            afficherFichier("scores.txt");
            break;

        default:
            choix = -1; // marqueur d'un choix valide...
        }

    } while (choix != 0); // on continue a afficher le menu tant que choix n'est pas valide

    return 0;
}

/*****************************************************************************
 * Chargement d'un niveau
 *****************************************************************************/

void chargerNiveauN(const int n) {

    char matrice [15][19];
    int x, y;
    scoreEnCours = 0;
    niveauEnCours = n;
    characterRecouvert = '-';

    recupTab(matrice, &x, &y, n);

    jouerNiveauN(matrice, x, y, n);
}

void recupTab(char matrice[15][19], int *x, int *y, int niveauID){

    char file[15] = "";
    sprintf(file, "niveau%d.txt", niveauID);
    FILE *fic = fopen(file, "r");

    recupPartie(matrice, x, y, fic);
}

void recupPartie(char matrice[15][19], int *x, int *y, FILE *fic){

    if(fic)
    {
        for (int i = 0; i < 15; i++)
        {
            for (int j = 0; j < 19; j++)
            {
                matrice[i][j] = recupChar(fic);

                if(matrice[i][j] == '!')
                {
                    *x = i;
                    *y = j;
                }
            }
        }

        fclose(fic);
    }
    else {
        printf("Impossible de recuperer la partie.");
    }
}

/*****************************************************************************
 * Execution d'un niveau
 *****************************************************************************/

void jouerNiveauN(char matrice[15][19], int x, int y, const int n) {

    do {
        affichage(matrice,x,y);
        deplacement(&x, &y, matrice);

    } while(characterRecouvert != '@');

    // On sauvegarde le score
    sauverScore(scoreEnCours);

    // Niveau N finit, on passe au N+1 s'il existe
    if (n < 5) {
        chargerNiveauN(n + 1);
    }
}

void deplacement(int *x,int *y, char matrice[15][19]){

    char touche;

#ifndef MAC
    touche = getch();
#else
    // version mac...
    touche = lireDeLaCommandLine();
#endif
    int target_x = *x, target_y = *y;

    switch (touche) {

        case 'z':
            target_x--;
            break;

        case 's':
            target_x++;
            break;

        case 'q':
            target_y--;
            break;

        case 'd':
            target_y++;
            break;

        case 'p':
            sauvegarderPartie(niveauEnCours, scoreEnCours, matrice);
            printf("Partie sauvegardee.");
            exit(EXIT_SUCCESS);

        default:
            return; // on ignore une touche invalide...
    }

    if (!prochaineCaseEstValide(matrice, target_x, target_y)) {
        return;  // on ignore si nouvelle emplacement invalide...
    }


    // Update la case actuelle
    switch(characterRecouvert) {
        case '-':
            matrice[*x][*y] = '0';
            break;

        case '=':
            matrice[*x][*y] = '-';
            break;
    }

    // Action en fonction du type de la prochaine case
    switch(matrice[target_x][target_y]) {

        case 'P':
            // TODO: a completer
            break;

        case 'E':
            // TODO: a completer
            break;

        case 'S':
            // TODO: a completer
            break;

        case 'M':
            // TODO: a completer
            break;
    }

    // Update les coordonees
    *x = target_x;
    *y = target_y;

    // Update de la grille
    characterRecouvert = matrice[*x][*y];
    matrice[*x][*y] = '!';

    // TODO: definir quand le score augmente/baisse. Pour le moment c'est a chaque
    // mouvement
    scoreEnCours++;
}

/*
 * Retourne un nombre positif si c'est [x,y] est dans la zone de jeu, et valide
 */
int prochaineCaseEstValide(char matrice[15][19], int x, int y) {

    return 0 <= x && x < 15 &&
           0 <= y && y < 19 &&
           matrice[x][y] != '#' &&
           matrice[x][y] != '0';
}

/*****************************************************************************
 * Scores.
 *****************************************************************************/

/*
 * Sauvegarde le score `nouveauScore`, s'il est plus grand que ceux deja enregistres.
 *
 * Les scores sont sauvegardes dans le fichier `scores.txt`, avec la structure:
 * 56
 * 34
 * 23
 * 23
 * 21
 */
void sauverScore(int nouveauScore) {

    const int MAX_NB_SCORE = 5; // Nombre max de scores sauvegardes.
    const int MAX_SCORE_LONGUEUR = 10; // Taille max (nb de characteres) d'un score

    FILE *filePtr = fopen("scores.txt", "r+");
    if (filePtr != NULL) {

        // On doit reecrire tout le fichier des scores pour le modifier...
        char nouveauxScores[MAX_NB_SCORE * MAX_SCORE_LONGUEUR] = "";

        char ligne[MAX_SCORE_LONGUEUR];
        char scoreOut[MAX_SCORE_LONGUEUR] = "";
        int scoreDejaInsere = 0, anciensScoresRencontres = 0;

        // Lire le fichier ligne par ligne, donc score par score
        while(fgets(ligne, MAX_SCORE_LONGUEUR, filePtr) != NULL) {

            anciensScoresRencontres++;

            int score = atoi(ligne); // atoi() convertit char[] -> int

            if (nouveauScore > score && (scoreDejaInsere == 0)) {
                score = nouveauScore;
                scoreDejaInsere = 1; // retenir qu'on a deja svg le score
            }

            sprintf(scoreOut, "%d\n", score);
            strcat(nouveauxScores, scoreOut);
        }

        // Rajouter le nouveau score si l'ancienne liste n'est pas full
        if (anciensScoresRencontres < 5) {

            sprintf(scoreOut, "%d\n", nouveauScore);
            strcat(nouveauxScores, scoreOut);
        }

        // Reecrire le fichier des scores...
        fclose(filePtr);
        filePtr = fopen("scores.txt", "w"); // "w" ecrase le fichier
        if (filePtr != NULL) {
            fwrite(nouveauxScores, 1, strlen(nouveauxScores), filePtr);
            fclose(filePtr);
        }
    }
    // Creer une nouvelle fiche de scores...
    else {

        char nouveauxScores[MAX_SCORE_LONGUEUR];
        sprintf(nouveauxScores, "%d\n", nouveauScore);

        filePtr = fopen("scores.txt", "w");
        if (filePtr != NULL) {
            fwrite(nouveauxScores, 1, strlen(nouveauxScores), filePtr);
            fclose(filePtr);
        }

    }
}

/*****************************************************************************
 * Sauvegarde / chargement parties...
 *****************************************************************************/

/*
 * Sauvegarde la partie en cours dans `partie_svg.txt`
 * Format de la sauvegarde:
 * <niveau>
 * <score>
 * <char recouvert>
 * <grille>
 */
void sauvegarderPartie(int niveau, int score, char matrice[15][19]) {

    FILE *fic = fopen("partie_svg.txt", "w"); // ecrase la potentielle precedente

    // Sauvegarder le niveau
    fprintf(fic, "%d\n", niveau);

    // Sauvegarder le score
    fprintf(fic, "%d\n", score);

    // Sauvegarder le score
    fprintf(fic, "%c\n", characterRecouvert);

    // Sauvegarder la grille
    if (fic != NULL) {

        for (int i = 0; i < 15; i++) {
            for (int j = 0; j < 19; j++) {
                fputc(matrice[i][j], fic);
            }
            fputc('\n', fic);
        }
    }
    else {
        printf("Une erreur une survenue pendant la sauvegarde... abandon.");
    }
}

void chargerPartie() {

    FILE *fic = fopen("partie.svg", "r");
    char ligne[10];

    // Charger le niveau...
    if(fgets(ligne, 10, fic) != NULL) {
        niveauEnCours = atoi(ligne);
    }
    else {
        printf("Erreur du chargement du niveau...");
    }

    // Charger le score...
    if(fgets(ligne, 10, fic) != NULL) {
        scoreEnCours = atoi(ligne);
    }
    else {
        printf("Erreur du chargement du score...");
    }

    // Charger le character recouvert...
    if(fgets(ligne, 10, fic) != NULL) {
        characterRecouvert = ligne[0];
    }
    else {
        printf("Erreur du chargement du char recouvert...");
    }

    // Charger la grille...
    char matrice [15][19];
    int x, y;

    recupPartie(matrice, &x, &y, fic);

    jouerNiveauN(matrice, x, y, niveauEnCours);
}


/*****************************************************************************
 * Utilitaires
 *****************************************************************************/

/*
 * Affiche un fichier, en prenant le nom de ce fichier comme argument
 */
void afficherFichier(const char* fichier) {

    FILE *filePtr = fopen(fichier, "r");

    if (filePtr != NULL) {

        // Lire les characteres 1 par 1
        char c;
        while((c = getc(filePtr)) != EOF) { // EOF: End Of File

            printf("%c", c);
        }
    }
    else {
        printf("Impossible de lire le fichier %s", fichier);
    }
}

/*
 * Affiche la grille a l'ecran
 */
void affichage (char matrice [15][19], int x,int y){

#ifndef MAC
    system("cls");
#else
    system("clear"); // mac version
#endif

    printf("\n");
    for (int i = 0; i < 15; i++)
    {
        for (int j = 0; j < 19; j++)
        {
            printf("%c", matrice[i][j]);

        }
        printf("\n");
    }

    printf("\nVous etes au `!` [x = %d : y = %d] | score = %d\n", x, y, scoreEnCours);
}

/*
 * Permet de recuperer un nombre >= 0 en verifiant que c'est bien un nombre
 */
int recupInt() {

    char buffer[10];
    scanf("%s", buffer);

    return atoi(buffer);
}

/*
 * Revoie un charactere valide, ou EOF si le fichier est fini
 */
char recupChar(FILE *fic) {

    char c;
    do {
        c = getc(fic);

    // skip les characteres \0 \n \r etc (cf. table ASCII), et
    // s'arrete sur un charactere valide ou la fin du fichier...
    } while ((c < 30 || c > 127) && c != EOF);

    return c;
}

/*
 * Lire des chars de stdin (entree standard)
 */
char lireDeLaCommandLine() {

    /* use system call to make terminal send all keystrokes directly to stdin */
    system ("/bin/stty raw");
    char c = getchar();

    /* use system call to set terminal behaviour to more normal behaviour */
    system ("/bin/stty cooked");

    return c;
}
