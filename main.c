#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/select.h>
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>


int	main(void)
{
	struct	sockaddr_storage their_addr;
	socklen_t addr_size;
	struct	addrinfo hints, *res;
	int	sockfd;
	fd_set	fdreads;
	struct	timeval tv;
	int	newfd;
	int	fdmax;
	int	i;
	char	buff[1024];
	int	len;
	int	rien;
	int	rev;

	len = sizeof buff;	
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; 
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	getaddrinfo("127.0.0.1", "3490", &hints, &res);
	sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	bind(sockfd, res->ai_addr, res->ai_addrlen);
	listen(sockfd, 10);

	FD_ZERO(&fdreads);
	FD_SET(sockfd,&fdreads);
	fdmax = sockfd;


	while (1)
	{
		tv.tv_sec = 3;
        	tv.tv_usec = 0;
		rev = select(FD_SETSIZE, &fdreads, NULL, NULL, NULL);
			/*i = 4;
			rien = 0;
			while (i <= fdmax)
			{
				printf("%d",fdmax);
				if (recv(i,buff,len,0))
				{
					printf(" ici %s \n",buff);
					break;
				}
				i = i + 1;

			}
			printf("ici\n");
			if (i == fdmax + 1)
				rien = 1;
			if (rien == 1)
				{*/
				printf("FD %d \n",rev);
				newfd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);
				printf("connex %d\n", newfd);
				sleep(3);
				fdmax = newfd;
				FD_SET(newfd, &fdreads);

				//}
		
		
	}

	

	
}

