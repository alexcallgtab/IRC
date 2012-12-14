#ifndef _BASE_H_
#define _BASE_H_

typedef struct	t_env
{
	struct sockaddr_storage	their_addr;
	socklen_t		addr_size;
	int			newfd;
	int			sockfd;
	int			fdmax;
	fd_set			master;
	fd_set			fdreads;
}		s_env;

#endif
