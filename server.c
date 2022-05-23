#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include "outils.h"

#define Max_char 1000
#define Backlog 1

//Structure de données de l'utilisateur
typedef struct User
{
    char pseudo[Max_char];
    char password[Max_char];
} User;

//Fonction de creation de fichier, ou utilisation du fichier existant, pour gérer les pseudo et mots de passe
void saveTofile(User user)
{
    FILE *fp = NULL;
    fp = fopen("base_donnees.txt", "a");

    if (fp == NULL) {
        fprintf(stderr, "Erreur lors de l'ouverture de la base de données : %s\n", strerror(errno));
    } else {
        fprintf(fp, "pseudo:%s\npassword:%s\n", user.pseudo, user.password);
        printf("%s vient de créer un compte", user.pseudo);
        fclose(fp);
    }
}

//Fonction permettant d'initialiser la base de données. Un compte ADMIN est créé.
void initialisation_base()
{
    FILE *fp = NULL;
    fp = fopen("base_donnees.txt", "w");

    if (fp == NULL) {
        fprintf(stderr, "Un problème est survenu avec la base de donnees : %s\n", strerror(errno));
    } else {
        fprintf(fp, "pseudo:%s\npassword:%s\n", "ADMIN", "1234");
        fclose(fp);
    }
}

//Recherche dans le fichier texte si l'utilisateur existe déjà. Retourne 1 s'il existe, 2 sinon.
int recherche_base(User user)
{
    User tmp;
    FILE *fp = NULL;
    fp = fopen("base_donnees.txt", "r");

    if (fp == NULL) {
        fprintf(stderr, "Un problème est survenu dans la base données : %s\n", strerror(errno));
        return -1;
    } else {
        printf("- - -   Recherche dans la base    - - -\n");
        while ((fscanf(fp, "pseudo:%s\npassword:%s\n", tmp.pseudo, tmp.password)) != EOF) {
            if ((strcmp(user.pseudo, tmp.pseudo) == 0) && (strcmp(user.password, tmp.password) == 0)) {
                printf("- - -     Utilisateur trouvé      - - -\n");
                fclose(fp);
                return 1;
            }
        }
    }
    fclose(fp);
    return 2;
}

//Obtiens la ligne du pseudo du client dans la database
int get_file_line(User user)
{
    User tmp;
    FILE *fp = NULL;
    fp = fopen("base_donnees.txt", "r");

    int line;
    int counter = 0;

    if (fp == NULL) {
        fprintf(stderr, "Un problème est survenu dans la base données : %s\n", strerror(errno));
        return -1;
    } else {
        while ((fscanf(fp, "pseudo:%s\npassword:%s\n", tmp.pseudo, tmp.password)) != EOF) {
            counter += 2;
            if ((strcmp(user.pseudo, tmp.pseudo) == 0) && (strcmp(user.password, tmp.password) == 0)) {
                line = counter;
                fclose(fp);
                return line-1; //line -1 car la ligne est celle du mdp comme on fait double vérif, je veux renvoyer le numéro de ligne du compte
            }
        }
    }
    fclose(fp);
    return -1;
}

//Fonction pour supprimer un compte de la base (pseudo + mdp)
int delete_account(User user, char * clientSend, int socket)
{
    if(strcmp(clientSend, "!delete") == 0) {
        FILE *fp;
        FILE *tempFile;

        char path[] = "base_donnees.txt";
        int counter = 0;
        int line = get_file_line(user);

        fp  = fopen(path, "r");
        tempFile = fopen("delete-line.tmp", "w");

        if (fp == NULL || tempFile == NULL) {
            fprintf(stderr, "Ouverture du fichier impossible : %s\n", strerror(errno));
            return -1;
        }
        rewind(fp);

        char buffer[Max_char];
        int count = 1;

        while ((fgets(buffer, sizeof(char) * (Max_char - 1), fp)) != NULL) {
            if (line != count && line+1 != count)
                fputs(buffer, tempFile);

            count++;
        }
        fclose(fp);
        fclose(tempFile);

        remove(path);
        rename("delete-line.tmp", path);
        close(socket);
        printf("\n- - -      Client déconnecté      - - -\n");
        return 1;
    }
    return 0;
}

//Change le pseudo de l'utilisteur
void rename_account(User user, User user_tmp)
{
    FILE *fp;
    FILE *tempFile;

    char path[] = "base_donnees.txt";
    int counter = 0;
    int line = get_file_line(user);

    fp  = fopen(path, "r");
    tempFile = fopen("rename_account.tmp", "w");

    if (fp == NULL || tempFile == NULL) {
            fprintf(stderr, "Ouverture du fichier impossible : %s\n", strerror(errno));
            return;
        }
    
    rewind(fp);

    char buffer[Max_char];
    int count = 1;

    while ((fgets(buffer, sizeof(char) * (Max_char - 1), fp)) != NULL) {
        if (line != count)
                fputs(buffer, tempFile);
        if (line == count) {
            fprintf(tempFile, "pseudo:%s", user_tmp.pseudo);
        }
        count++;
    }
    fclose(fp);
    fclose(tempFile);
    remove(path);
    rename("rename_account.tmp", path);
}

//change le mot de passe de l'utilisateur
void rename_password(User user, User user_tmp)
{
    FILE *fp;
    FILE *tempFile;

    char path[] = "base_donnees.txt";
    int counter = 0;
    int line = get_file_line(user) + 1; //+1 pour se possitionner sur le mdp

    fp  = fopen(path, "r");
    tempFile = fopen("rename_password.tmp", "w");

    if (fp == NULL || tempFile == NULL) {
            fprintf(stderr, "Ouverture du fichier impossible : %s\n", strerror(errno));
            return;
        }
    
    rewind(fp);

    char buffer[Max_char];
    int count = 1;

    while ((fgets(buffer, sizeof(char) * (Max_char - 1), fp)) != NULL) {
        if (line != count)
                fputs(buffer, tempFile);
        if (line == count) {
            fprintf(tempFile, "password:%s", user_tmp.password);
        }
        count++;
    }
    fclose(fp);
    fclose(tempFile);
    remove(path);
    rename("rename_password.tmp", path);
}

//Vérification si la base existe
int base_existe()
{
    FILE *fp = NULL;
    fp = fopen("base_donnees.txt", "r");

    if (fp == NULL){
        return 1;
    }
    fclose(fp);
    return -1;    
}

int main ()
{
    int socketServer = socket(AF_INET, SOCK_STREAM, 0), socketClient;

    struct sockaddr_in addrServer;
    User user;
    User user_tmp;
    
    printf("_____    %s, %s    _____\n", __DATE__, __TIME__);
    if (base_existe() == 1)
        initialisation_base();

    if (socketServer == -1) {
        fprintf(stderr, "Echec creation socket : %s", strerror(errno));
        return -1;
    }
    printf("- - -         Socket créé         - - -\n");

    //Préparation du socket grâce à la structure sockaddr_in
    addrServer.sin_family = AF_INET;
	addrServer.sin_addr.s_addr = INADDR_ANY;
    addrServer.sin_port = htons(8080);

    //Bind et gestion d'erreur du bind (pas de bind si s_addr = INADDR_ANY, communication locale entre deux terminals)
    //Pour communication réseau : 
    //Ouvrir les ports, autoriser communication dans le par-feu, vérif les subnets au cas où besoin de passerelle réseau
    if ((bind(socketServer, (struct sockaddr *)&addrServer, sizeof addrServer)) > 0) {
        fprintf(stderr, "Le bind a échoué : %s", strerror(errno));
        return -1;
    }
    printf("- - -         Bind réussi         - - -\n");

    //listen
    if ((listen(socketServer, Backlog)) == 0 )
        printf("- - - En attente de connexion... - - -\n");
    
    //connexion
    socketClient = accept(socketServer, (struct sockaddr *) NULL, NULL);
    read(socketClient, user.pseudo, Max_char);
    read(socketClient, user.password, Max_char);

    if (recherche_base(user) == 2) {
        memset(&user, 0, sizeof user);
        printf("- - -     Nouvel utilisateur      - - -\n");
        send(socketClient, "2", sizeof "2", 0);
        read(socketClient, user.pseudo, Max_char);
        read(socketClient, user.password, Max_char);
        decrypt(user.pseudo);
        decrypt(user.password);
        if(strcmp(user.pseudo, "") == 0) {
            printf("\n      Le client n'a pas de compte.\n");
            return 0;
        }
        saveTofile(user);
        printf("\n     Bienvenu à toi %s !\n\n", user.pseudo);
    }else{
        send(socketClient, "1", sizeof "1", 0);
        printf("          %s est de retour !\n\n", user.pseudo);
    }

    //gestion de la reception et l'envoie de message et fermeture du serveur et du client
    bool a_quitte = false;
    while (!a_quitte) {
        char reponseServeur[Max_char] = {0};
        char message_client[Max_char] = {0};

        read(socketClient, message_client, sizeof(char) * (Max_char - 1));

        if (strcmp(message_client, "!rename") == 0) {
            read(socketClient, user_tmp.pseudo, Max_char);
            decrypt(user_tmp.pseudo);
            rename_account(user, user_tmp);
        } else {
            if (utilisateur_quit(message_client, socketClient) == 1) {
                printf("\n- - -      Client déconnecté      - - -\n");
                a_quitte = true;
            } else {
                if (delete_account(user, message_client, socketClient) == 1) {
                    printf("      %s a supprimé son compte.\n",user.pseudo);
                    utilisateur_quit(message_client, socketClient);
                    a_quitte = true;
                } else {
                    if (strcmp(message_client, "!rename_password") == 0) {
                        read(socketClient, user_tmp.password, Max_char);
                        decrypt(user_tmp.password);
                        rename_password(user, user_tmp);
                    } else {
                        if (strcmp(message_client, "!help") == 0) {
                            continue;
                        } else {
                            if (strcmp(message_client, "!crypt") == 0) {
                                continue;
                            } else {
                                fprintf(stdout, "%s : %s\n", user.pseudo, message_client);
                                printf("> ");
                                fgets(reponseServeur, sizeof(char) * (Max_char - 1), stdin);
                                send(socketClient, reponseServeur, sizeof(char) * (Max_char - 1), 0);
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}