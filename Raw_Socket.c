#include "mylib.h"

u_char s_mac[] = {0xdc, 0x0e, 0xa1, 0xa4, 0x5e, 0x7e};
u_char d_mac[] = {0x08, 0xed, 0xb9, 0x30, 0xf4, 0xc9};

unsigned short csum(unsigned short *ptr,int nbytes) 
{
	long sum;
	unsigned short oddbyte;
	short answer;

	sum=0;
	while(nbytes>1) {
		sum+=*ptr++;
		nbytes-=2;
	}
	if(nbytes==1) {
		oddbyte=0;
		*((u_char*)&oddbyte)=*(u_char*)ptr;
		sum+=oddbyte;
	}

	sum = (sum>>16)+(sum & 0xffff);
	sum = sum + (sum>>16);
	answer=(short)~sum;

	return(answer);
}

void main()
{
	int sock, psize;
	char datagram[MSGLEN], *data, *pseudogram, buf[MSGLEN];
	if ((sock = socket(AF_PACKET, SOCK_RAW, IPPROTO_RAW)) == -1)
	{
		perror("socket");
		exit(1);
	}
	memset(datagram,0,MSGLEN);
	struct ethhdr *eth = (struct ethhdr*) datagram;
	struct iphdr *iph = (struct iphdr *) (datagram + sizeof(struct ethhdr));
	struct udphdr *udph = (struct udphdr *) (datagram + sizeof(struct iphdr) + sizeof(struct ethhdr));
	struct sockaddr_ll addr_other;
	struct pseudo_header psh;

	data = datagram  + sizeof(struct udphdr) + sizeof(struct iphdr);
	printf("Enter string : ");
	scanf("%s",buf);
	strcpy(data,buf);

	memset(&addr_other, 0, sizeof(addr_other));
	addr_other.sll_family = AF_PACKET;
	addr_other.sll_ifindex = 2;
	addr_other.sll_halen = 6;	
	memcpy(addr_other.sll_addr, d_mac, 6);

	memcpy(eth->h_dest,d_mac,6);
	memcpy(eth->h_source,s_mac,6);
	eth->h_proto = 0x00;

	iph->ihl = 5;
	iph->version = 4;
	iph->tos = 0;
	iph->tot_len = sizeof(struct iphdr) + sizeof(struct udphdr) + strlen(data);
	iph->id = htonl(54321);
	iph->frag_off = 0;
    iph->ttl = 65;
    iph->protocol = IPPROTO_UDP;
    iph->saddr = inet_addr(S_IP);
    iph->daddr = inet_addr(D_IP);
    iph->check = csum ((unsigned short *) datagram, iph->tot_len);

	udph->source = htons(S_PORT);
	udph->dest = htons(D_PORT);
	udph->len = htons(sizeof(struct udphdr) + strlen(data));

	psh.source_address = inet_addr(S_IP);
	psh.dest_address = inet_addr(D_IP);
	psh.placeholder = 0;
	psh.protocol = IPPROTO_UDP;
	psh.udp_length = htons(sizeof(struct udphdr) + strlen(data));
	psize = sizeof(struct pseudo_header) + sizeof(struct udphdr) + strlen(data);
	pseudogram = malloc(psize);
	memcpy(pseudogram, (char*)&psh, sizeof(struct pseudo_header));
	memcpy(pseudogram + sizeof(struct pseudo_header), udph, sizeof(struct udphdr) + strlen(data));
	udph->check = csum((unsigned short*)pseudogram, psize);

	if (sendto(sock, datagram, MSGLEN, 0, (struct sockaddr*)&addr_other, sizeof(addr_other)) == -1)
	{
		perror("sendto");
		exit(1);
		close(sock);
	}
}