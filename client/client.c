#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/select.h>
#include <stdlib.h>

#define true 1
#define SIZE 66560

int init();
int main(int argc, char const *argv[])
{
  char content[SIZE], buffer[256], filename[256];
  bzero(content, SIZE);
  int sd = init();
  fd_set setin, setout;
  long lu, count = 0, file_fd;

  while (true)
  {
    FD_ZERO(&setin);
    FD_SET(sd, &setin);

    if (select(sd + 1, &setin, NULL, NULL, NULL) < 0)
    {
      printf("Erreur d'entrée sortie\n");
      return -1;
    }
    if (FD_ISSET(sd, &setin))
    {
      if (count == 0)
      {
        // Filename
        lu = read(sd, buffer, 256);
        strncpy(filename, buffer, lu);
        filename[lu] = '\0';
        file_fd = open(filename, O_CREAT | O_WRONLY, 0755);
        count++;
        write(sd, "1", 1);
      }
      else
      {
        lu = read(sd, content, SIZE);
        write(file_fd, content, lu);
        bzero(content, SIZE);
        write(sd, "1", 1);
      }
    }
  }
  printf("%s téléchargé avec succès !\n", filename);
  return 0;
}

int init()
{
  int sd = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in server_adr;
  if (sd < 0)
  {
    printf("Erreur lors de la création du socket.\n");
    return sd;
  }
  server_adr.sin_family = AF_INET;
  server_adr.sin_addr.s_addr = inet_addr("127.0.0.1");
  server_adr.sin_port = htons(PORT);

  if (connect(sd, (struct sockaddr *)&server_adr, sizeof(server_adr)) < 0)
  {
    printf("Erreur de connexion.\n");
    return -1;
  }
  fprintf(stdout, "Connectée sur : IP = %s, Port = %u \n", inet_ntoa(server_adr.sin_addr), ntohs(server_adr.sin_port));
  return sd;
}