#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <stdbool.h>
#include "outils.h"
#include <errno.h>

#define MAX_CHAR 1000

int main () 
{
    int socketClient = socket(AF_INET, SOCK_STREAM, 0);
    if (socketClient == -1) {
        fprintf(stderr, "Echec de la création du socket client : %s\n", strerror(errno));
        return -1;
    }
    int counter = 0;
    int userChoice = 0;
    bool authenticated;    
    bool a_quitte = false;

    char tmpp[1024], code[MAX_CHAR] = {0};
    char pseudo[MAX_CHAR] = {0};
    char password[MAX_CHAR] = {0};
    
    struct sockaddr_in addrClient;

    printf("_____    %s, %s    _____\n", __DATE__, __TIME__);

    addrClient.sin_family = AF_INET;
    addrClient.sin_addr.s_addr = INADDR_ANY;
    addrClient.sin_port = htons(8080);

    //Conexion au serveur
    if (connect(socketClient, (struct sockaddr*)&addrClient, sizeof addrClient) < 0) {
        fprintf(stderr, "La connexion au serveur a échouée : %s\n", strerror(errno));
        return -1;
    }

    //Authentification, definition des condition de pseudo/mots de passe, envoie les infos cryptées au serveur
    do {
        if (counter == 3) {
            printf("Au revoir");
            return 0;
        }
        printf("Votre pseudo : ");
        fgetstr(pseudo, sizeof(char) * (MAX_CHAR - 1), stdin);
        printf("Votre mot de passe : ");
        fgetstr(password, sizeof(char) * (MAX_CHAR - 1), stdin);
        counter++;

        send(socketClient, pseudo, sizeof(char) * (MAX_CHAR - 1), 0);
        send(socketClient, password, sizeof(char) * (MAX_CHAR - 1), 0);

        printf("- - -   Envoie les informations   - - -\n");
        read(socketClient,code, MAX_CHAR);

        if (strcmp(code, "2") == 0) {
            memset(pseudo, 0, sizeof(char) * (MAX_CHAR - 1));
            memset(password, 0, sizeof(char) * (MAX_CHAR - 1));
            printf("Vous n'avez pas encore de compte, voulez-vous en créer un ?\nOui(1) ou Non(2) : ");
            fgetstr(tmpp, sizeof(int), stdin);
            userChoice = atoi(tmpp);

            if(userChoice == 1) {
                printf("- - -        Nouveau compte       - - -\n");
                printf("Entrez votre pseudo : ");
                fgetstr(pseudo, sizeof(char) * (MAX_CHAR - 1), stdin);
                if(strlen(pseudo) < 3 || string_space(pseudo) != 0) {
                    while(strlen(pseudo) < 3 || string_space(pseudo) != 0) {
                        printf("Veuillez taper un pseudo de plus de 3 caractères et sans espaces:\n");
                        printf("Votre pseudo : ");
                        fgetstr(pseudo, sizeof(char) * (MAX_CHAR - 1), stdin);
                    }
                }
                printf("Entrez votre mot de passe : ");
                fgetstr(password, sizeof(char) * (MAX_CHAR - 1), stdin);
                if(strlen(password) < 6 || string_space(password)) {
                    while(strlen(password) < 6 || string_space(password)) {
                        printf("Veuillez taper un mot de passe d'au moins 6 caractères et sans espaces:\n");
                        printf("Votre mot de passe : ");
                        fgetstr(password, sizeof(char) * (MAX_CHAR - 1), stdin);
                    }
                }
                encrypt(pseudo);
                encrypt(password);
                send(socketClient, pseudo, sizeof(char) * (MAX_CHAR - 1), 0);
                send(socketClient, password, sizeof(char) * (MAX_CHAR - 1), 0);
                decrypt(pseudo);
                decrypt(password);
                printf("- - - Création du compte réussie  - - -\n");
            }else{
                printf("Un compte est nécessaire pour communiquer. Au revoir.\n");

                return -1;
            }
        }
        authenticated = true;
    }while(!authenticated);

    //Mise en place de l'interface de messagerie avec les commandes disponibles
    printf("- - - Bienvenue dans la messagerie- - -\n\n");
    printf("\n!help pour obtenir la liste des commandes\n\n");

    while(!a_quitte){
        char userInput[MAX_CHAR] = {0};
        char serverResponse[MAX_CHAR] = {0};
        printf("%s > ", pseudo);
        fgetstr(userInput, sizeof(char) * (MAX_CHAR - 1), stdin);

        send(socketClient, userInput, sizeof(char) * (MAX_CHAR - 1), 0);
            
        if (strcmp(userInput, "!rename") == 0) {
            printf("Votre nouveau pseudo : ");
            fgets(userInput, sizeof(char) * (MAX_CHAR - 1), stdin);
            if(strlen(userInput) < 3 || string_space(userInput) != 0) {
                while(strlen(userInput) < 3 || string_space(userInput) != 0) {
                    printf("Veuillez taper un pseudo de plus de 3 caractères et sans espaces:\n");
                    printf("Votre pseudo : ");
                    fgets(userInput, sizeof(char) * (MAX_CHAR - 1), stdin);
                }
            }
            printf("Votre nouveau pseudo vous sera attribuer lors de votre prochaine connexion.\n");
            encrypt(userInput);
            send(socketClient, userInput, sizeof(char) * (MAX_CHAR - 1), 0);
            decrypt(userInput);
            } else {
                if (strcmp(userInput, "!exit") == 0) {
                    a_quitte = utilisateur_quit(userInput, socketClient);
                } else {
                    if (strcmp(userInput, "!delete") == 0) {
                        printf("Compte supprimé\n");
                        return 0;
                    } else {
                        if (strcmp(userInput, "!rename_password") == 0) {
                            printf("Votre nouveau mot de passe : ");
                            fgets(userInput, sizeof(char) * (MAX_CHAR - 1), stdin);
                            if(strlen(userInput) < 6 || string_space(userInput)) {
                                while(strlen(userInput) < 6 || string_space(userInput)) {
                                    printf("Veuillez taper un mot de passe d'au moins 6 caractères et sans espaces:\n");
                                    printf("Votre mot de passe : ");
                                    fgets(userInput, sizeof(char) * (MAX_CHAR - 1), stdin);
                                }
                            }
                            encrypt(userInput);
                            send(socketClient, userInput, sizeof(char) * (MAX_CHAR - 1), 0);
                            decrypt(userInput);
                        } else {
                            if (strcmp(userInput, "!help") == 0) {
                                help();
                            } else {
                                if (strcmp(userInput, "!crypt") == 0) {
                                    printf("Cryptage par clé de chiffrement\n\n");
                                    encrypt(pseudo);
                                    encrypt(password);
                                    printf("pseudo : %s\nmot de passe :%s\n",pseudo,password);
                                    decrypt(pseudo);
                                    decrypt(password);
                                } else {
                                    read(socketClient, serverResponse, MAX_CHAR);
                                    printf("Server : %s", serverResponse);
                                }
                            }
                        }  
                    }
                }
            }  
        }
    printf("\nAu revoir %s, à bientôt.\n", pseudo);
    return 0;
}