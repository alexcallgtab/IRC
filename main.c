#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/select.h>
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>

int	read_fd()
{
	i = 0;
	while (i <= fdmax)
	{
		if (FD_ISSET(i,&fdreads))
		{
			if (i == sockfd)
			{
			newfd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);
			printf("connex %d\n", newfd);
			if (newfd > fdmax)
			fdmax = newfd;
			FD_SET(newfd, &master);
			}
			else 
			{
			if(recv(i,buff,sizeof buff,0) <= 0)
			{
				close(i);
				FD_CLR(i,&master);
			}
			else
				printf("MESSAGE DE %d :%s\n",i,buff);
			}
			}
			i = i + 1;
	}

int	main(void)
{
	struct	sockaddr_storage their_addr;
	socklen_t addr_size;
	struct	addrinfo hints, *res;
	int	sockfd;
	fd_set	fdreads;
	fd_set 	master;
	
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
		rev = select(fdmax+1, &fdreads, NULL, NULL, NULL);
		
		
		}

	

	
}

