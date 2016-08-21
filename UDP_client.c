#include "mylib.h"
void main()
{
	struct sockaddr_in addr_other;
	int sock ,struck_len = sizeof(addr_other);
	char buf[MSGLEN];
	if ((sock = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP)) == -1)
	{
		perror("socket");
	}
	memset(&addr_other,0,struck_len);
	addr_other.sin_family = AF_INET;
	addr_other.sin_port = htons(D_PORT);
	inet_aton(D_IP,&addr_other.sin_addr);
	while(1)
	{
		printf("ENTER MESSAGE: ");
		scanf("%s",buf);
		if ((sendto(sock,buf,3,0,(struct sockaddr*)&addr_other,struck_len)) == -1)
		{
			perror("sendto");
		}
		memset(buf,'\0',MSGLEN);
		printf("%s\n",buf);
	}
	close(sock);
}