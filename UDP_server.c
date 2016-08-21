#include "mylib.h"
void main()
{
	char buf[MSGLEN];
	int rec,i;
	struct sockaddr_in addr_me ,addr_other;
	int sock, struct_len = sizeof(addr_other);
	if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
	{
		perror("socket");
	}
	memset(&addr_me,0,sizeof(addr_me));
	addr_me.sin_family = AF_INET;
	addr_me.sin_port = htons(D_PORT);
	inet_aton(D_IP,&addr_me.sin_addr);
	if (bind(sock, (struct sockaddr *)&addr_me, sizeof(addr_me)) == -1)
	{
		perror("bind");
	}
	while(1)
	{
		printf("Waiting data\n");
		if ((rec = recvfrom(sock,buf,MSGLEN,0,(struct sockaddr *)&addr_other,&struct_len)) == -1)
		{
			perror("recvfrom");
		}
		for (i=0;i<rec;i++)
			printf("%c", buf[i]);
		printf("\n");
	}
	close(sock);
}