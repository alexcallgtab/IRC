#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/select.h>
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>

int	new_client(int fdmax,int sockfd, fd_set* master)
{
	int newfd;
	struct sockaddr_storage their_addr;
	socklen_t               addr_size;

	newfd = accept(sockfd, (struct sockaddr *)&their_addr,
					        &addr_size);
	printf("connex %d\n", newfd);
	if (newfd > fdmax)
	fdmax = newfd;
	FD_SET(newfd, master);
	
	return fdmax;
}
int	read_fd(int sockfd, int fdmax,fd_set* master,fd_set* fdreads)
{
	int	i;
	int	j;
	char 	buff[256];
	int	nbytes;

	i = 0;
	while (i <= fdmax)
	{
		if (FD_ISSET(i,fdreads))
		{
			if (i == sockfd)
				fdmax = new_client(fdmax,sockfd,master);
			else 
			{
				nbytes = recv(i, buff, sizeof buff, 0);
			
				if(nbytes <= 0)
				{
					close(i);
					FD_CLR(i,master);
				}
				else
				{
					printf("Essaye d4envoyer %d\n",nbytes);
					j = 0;
					while (j <= fdmax)
					{
						if (FD_ISSET(j,master))
							if (j != sockfd && j != i)
								send(j,buff,nbytes,0);
						j = j + 1;
					}
				}
			}
		}
		i = i + 1;
	}
	return fdmax;
}

int	main(void)
{
	int	sockfd;
	fd_set	fdreads;
	fd_set 	master;
	int	fdmax;
	struct addrinfo         hints;
	struct addrinfo		*res;
	
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; 
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	getaddrinfo("127.0.0.1", "3490", &hints, &res);
	sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	bind(sockfd, res->ai_addr, res->ai_addrlen);
	listen(sockfd, 10);
	FD_ZERO(&fdreads);
	FD_ZERO(&master);
	FD_SET(sockfd,&master);
	fdmax = sockfd;
	while (1)
	{
		fdreads = master;
		select(fdmax + 1, &fdreads, NULL, NULL, NULL);
		fdmax = read_fd(sockfd, fdmax,&master,&fdreads);
	}
}

