#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/select.h>
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include "base.h"

void	send_all_client(s_env* env,int	nbytes, char* buff, int i)
{
	int	j;

	printf("Essaye d4envoyer %d\n",nbytes);
	j = 0;
	while (j <= env->fdmax)
	{
		if (FD_ISSET(j,&env->master))
			if (j != env->sockfd && j != i)
				send(j,buff,nbytes,0);
		j = j + 1;
	}
}

void	new_client(s_env* env)
{
	int	newfd;

	env->addr_size = sizeof env->their_addr;
	newfd = accept(env->sockfd, (struct sockaddr *)&env->their_addr,&env->addr_size);
	printf("connex %d\n", newfd);
	if (newfd > env->fdmax)
		env->fdmax = newfd;
	FD_SET(newfd, &env->master);
}

void	read_fd(s_env* env)
{
	int	i;
	char 	buff[256];
	int	nbytes;

	i = 0;
	while (i <= env->fdmax)
	{
		if (FD_ISSET(i,&env->fdreads))
		{
			if (i == env->sockfd)
				new_client(env);
			else
			{
				nbytes = recv(i, buff, sizeof buff, 0);
				if(nbytes <= 0)
				{
					close(i);
					FD_CLR(i,&env->master);
				}
				else
					send_all_client(env,nbytes,buff,i);
			}
		}
		i = i + 1;
	}
}

int	main(void)
{
	struct addrinfo	hints;
	struct addrinfo	*res;
	s_env		env;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; 
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	getaddrinfo("127.0.0.1", "3490", &hints, &res);
	env.sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	bind(env.sockfd, res->ai_addr, res->ai_addrlen);
	listen(env.sockfd, 10);
	FD_ZERO(&env.fdreads);
	FD_ZERO(&env.master);
	FD_SET(env.sockfd,&env.master);
	env.fdmax = env.sockfd;
	while (1)
	{
		env.fdreads = env.master;
		printf("AVANT\n");
		select(env.fdmax + 1, &env.fdreads, NULL, NULL, NULL);
		printf("APRES\n");
		read_fd(&env);
	}
}
