/*
 * Cliente simples usando sockets
 * Adaptado por Ramide Dantas (IFPE) do livro de Andrew S. Tanenbaum
 * 2019-11-11
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define SERVER_PORT 12345 /* arbitrary, but client & server must agree */
#define BUF_SIZE 4096 /* block transfer size */

void fatal(char *string) {
	printf("%s\n", string);
	exit(1);
}

int main(int argc, char **argv) {
	int c, s, bytes;
	char buf[BUF_SIZE]; /* buffer for incoming file */
	struct hostent *h; /* info about server */
	struct sockaddr_in channel; /* holds IP address */

	setvbuf(stdout, NULL, _IONBF, 0);

	if (argc != 3)
		fatal("Uso: cliente server-name file-name");

	printf("Cliente: resolvendo nome do host \"%s\" ...\n", argv[1]);

	h = gethostbyname(argv[1]); /* look up host’s IP address */

	if (!h)
		fatal("Cliente - ERRO: \"gethostbyname\" falhou!");

	printf("Cliente: criando e configurando o socket...\n");

	s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (s < 0)
		fatal("Cliente - ERRO: \"socket\" falhou!");

	memset(&channel, 0, sizeof(channel));
	channel.sin_family = AF_INET;
	memcpy(&channel.sin_addr.s_addr, h->h_addr, h->h_length);
	channel.sin_port = htons(SERVER_PORT);

	printf("Cliente: conectando com servidor %s na porta %d...", argv[1], SERVER_PORT);

	c = connect(s, (struct sockaddr *) &channel, sizeof(channel));
	if (c < 0)
		fatal("Cliente - ERRO: \"connect\" falhou!");

	printf("OK\n");

	printf("Cliente: solicitando arquivo \"%s\"...\n", argv[2]);

	write(s, argv[2], strlen(argv[2]) + 1);

	printf("Cliente: recenbendo dados...\n");
	printf("-------------------------- \n");
	while (1) {
		bytes = read(s, buf, BUF_SIZE); /* read from socket */
		if (bytes <= 0)
			break;
		write(1, buf, bytes); /* write to standard output */
//		sleep(5);
	}
	printf("--------------------------\n");
	printf("Cliente: finalizado com sucesso.\n");
}
