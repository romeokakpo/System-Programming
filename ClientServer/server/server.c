#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>
#include <time.h>
#include <math.h>

#define true 1
#define false 0
#define SIZE 66560

int init();
void traite_socket(int sock, const char *argv[]);

int main(int argc, const char *argv[])
{
  int sd = init();
  int longueur;
  struct sockaddr_in client_addr;
  if (sd > 0)
  {
    longueur = sizeof(struct sockaddr_in);
    while (true)
    {
      int sock_connectee = accept(sd, (struct sockaddr *)&client_addr, &longueur);
      if (sock_connectee < 0)
      {
        write(1, "Erreur lors de la connexion du client.\n", 40);
        return -1;
      }

      switch (fork())
      {
      case 0: /*fils*/
        close(sd);
        // Traitement du sock connectée
        write(1, "[+] Connexion d'un client.\n", 28);
        traite_socket(sock_connectee, argv);
        sleep(2);
        write(1, "[-] Fin ! Fermerture du serveur.\n", 34);
        exit(EXIT_SUCCESS);
        break;
      case -1: /*Erreur*/
        write(1, "Erreur de fork.\n", 17);
        break;
      default: /*Père*/
        close(sock_connectee);
        signal(SIGCHLD, SIG_IGN);
        break;
      }
    }
  }
  return 0;
}

int init()
{
  int sd = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in server_adr;
  if (sd < 0)
  {
    write(1, "Erreur lors de la création du socket.\n", 40);
    return sd;
  }
  server_adr.sin_family = AF_INET;
  server_adr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_adr.sin_port = htons(PORT);

  int bd = bind(sd, (struct sockaddr *)&server_adr, sizeof(server_adr));
  if (bd < 0)
  {
    perror("bind()");
    return bd;
  }
  if (listen(sd, 10) == -1)
  {
    write(1, "Erreur d'écoute\n", 18);
    return -1;
  }
  fprintf(stdout, "Ecoute sur : IP = %s, Port = %u \n", inet_ntoa(server_adr.sin_addr), ntohs(server_adr.sin_port));
  return sd;
}

void traite_socket(int sock, const char *argv[])
{
  char buffer[SIZE];
  char receive[2], send[256];
  fd_set setin;

  int file_fd = open(argv[1], O_RDONLY);
  long lu;

  // Envoi du nom du fichier
  write(sock, argv[1], strlen(argv[1]));

  while (true)
  {
    FD_ZERO(&setin);
    FD_SET(sock, &setin);
    if (select(sock + 1, &setin, NULL, NULL, NULL) < 0)
    {
      perror("select");
      exit(EXIT_FAILURE);
    }
    if (FD_ISSET(sock, &setin))
    {
      if ((lu = read(file_fd, buffer, SIZE)) > 0)
      {
        write(sock, buffer, lu);
        bzero(buffer, SIZE);
        if (lu < SIZE)
        {
          break;
        }
      }
      else
      {
        break;
      }
    }
  }
}