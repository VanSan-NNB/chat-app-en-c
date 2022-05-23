#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int key = 0xFACA;

//Modification de la fonction fgets pour gérer les retours-chariots

char *fgetstr(char *string, int n, FILE *stream)
{
	if (fgets(string, n ,stream) == NULL)
    	return NULL;
	if (string[strlen(string) -1] == '\n')
		string[strlen(string) -1] = 0;
	return string;
}

//Fonction quitter, ferme le socket à l'entrée de la commande

int utilisateur_quit(char *clientSend, int socket)
{
    if (strcmp(clientSend, "!exit") == 0) {
        close(socket);
        return 1;
    }
    return 0;
}

//Compte le nombre d'espace dans une chaine de caractères

int string_space (char * string)
{
    int spaces = 0;
    for(int i = 0; string[i] != '\0'; i++){
    if (string[i] == ' ')
          spaces++;
    }
    return spaces;
}

//cryptage par clé

void encrypt(char *string)
{
    unsigned int i;
    for(i = 0; i < strlen(string); ++i)
        string[i] = string[i] - key;
}

//décryptage par clé

void decrypt(char *string)
{
    unsigned int i;
    for(i = 0; i < strlen(string); ++i)
        string[i] = string[i] + key;
}

//affichage des commandes disponibles au client

void help()
{
    printf("\nListe des commandes : \n\n");

    printf("!exit   : Fermer le client et le serveur\n");
    printf("!delete : Supprimer son compte; entraine un !exit\n");
    printf("!rename : Renommer le pseudo\n");
    printf("!rename_password : Renommer le mot de passe\n");
    printf("!crypt  : Afficher le pseudo et mot de passe crypté\n");

    printf("\nAttention : Une seule modification de compte par session (pseudo, mdp ou suppression)");

    printf("\n\n");
}