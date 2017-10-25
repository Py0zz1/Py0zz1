/*
#define _WINSOCK_DEPRECATED_NO_WARNINGS 

#include "pcap.h"
#include <stdio.h>
#include <WinSock2.h>

#pragma comment (lib,"ws2_32.lib")
#pragma comment (lib,"wpcap.lib")
#define BUF_SIZE 100
#define SNAPLEN 65535
struct ether_mac
{
	unsigned char mac_add[6];
};
struct ether_header
{
	struct ether_mac src_mac;
	struct ether_mac des_mac;
	unsigned short ether_type;
};
struct arp_header
{
	unsigned short hardware_type;
	unsigned short protocol_type;
	unsigned char hardware_size;
	unsigned char protocol_size;
	unsigned short opcode;
	struct ether_mac send_mac;
	struct in_addr send_ip;
	struct ether_mac target_mac;
	struct in_addr target_ip;
};

int main()
{
	pcap_if_t *alldevs=NULL;
	pcap_if_t *dev;
	pcap_t *use_dev;
	char errbuf[BUF_SIZE];
	unsigned char packet[BUF_SIZE];
	int i,dev_num;
	
	if (pcap_findalldevs(&alldevs, errbuf) < 0)
	{
		printf("Find Device Error!\n");
		return -1;
	}

	for (i = 0, dev = alldevs; dev != NULL; dev = dev->next)
	{
		printf("%d번 Device (%s) ", ++i, dev->name);
		if (dev->description) printf("- %s\n", dev->description);
	}
	printf("사용할 장치번호 입력 : ");
	scanf("%d", &dev_num);

	for (i = 0, dev = alldevs; i < dev_num - 1; dev = dev->next, i++);
	
	if ((use_dev = pcap_open_live(dev->name, SNAPLEN, 1, 1000, errbuf)) == NULL)
	{
		printf("pcap_open Error!\n");
		pcap_freealldevs(alldevs);
		return -1;
	}
	printf("pcap open success!\n");

	struct ether_header eh;
	memset(packet, 0, sizeof(packet));
	eh.ether_type = htons(0x0806);

	
	packet[0] = 0x01;
	packet[1] = 0x02;
	packet[2] = 0x03;
	packet[3] = 0x04;
	packet[4] = 0x05;
	packet[5] = 0x06;

	packet[6] = 0x07;
	packet[7] = 0x08;
	packet[8] = 0x09;
	packet[9] = 0x10;
	packet[10] = 0x11;
	packet[11] = 0x12;

	//ether_type (ARP로 설정)
	packet[12] = 0x08;
	packet[13] = 0x06;
	
	//hrd_type (Ethernet로 설정)
	packet[14] = 0x00;
	packet[15] = 0x01;
	
	//proto_type (IPv4 로 설정)
	packet[16] = 0x08;
	packet[17] = 0x00;

	//hrd_size (6)
	packet[18] = 0x06;
	//proto_size (4)
	packet[19] = 0x04;

	//Opcode (request)
	packet[20] = 0x00;
	packet[21] = 0x01;

	// Sender MAC (My mac)
	packet[22] = 0xcc;
	packet[23] = 0xb0;
	packet[24] = 0xda;
	packet[25] = 0x18;
	packet[26] = 0x59;
	packet[27] = 0x2e;
	//Sender IP (MY ip)
	packet[28] = 192;
	packet[29] = 168;
	packet[30] = 0;
	packet[31] = 100;
	
	//Target MAC
	packet[32] = 0x00;
	packet[33] = 0x00;
	packet[34] = 0x00;
	packet[35] = 0x00;
	packet[36] = 0x00;
	packet[37] = 0x00;
	//Target IP
	packet[38] = 192;
	packet[39] = 168;
	packet[40] = 0;
	packet[41] = 103;

	//end
	//for (int i = 42; i < BUF_SIZE; i++) packet[i] = 0;
	if (pcap_sendpacket(use_dev, packet, 42) != 0)
	{
		printf("pcap_send Fail!\n");
		pcap_freealldevs(alldevs);
		return -1;
	}

	int res;
	struct pcap_pkthdr *header;
	const unsigned *pkt_data;

	while((res = pcap_next_ex(use_dev, &header, &pkt_data)) >= 0)
	{
		if (res == 0) continue;
			
		
	

	}

	

	


}
*/
#define _WINSOCK_DEPRECATED_NO_WARNINGS 

#include "pcap.h"
#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include <conio.h>

#pragma comment (lib,"ws2_32.lib")
#pragma comment (lib,"wpcap.lib")

#define BUF_SIZE 100
#define IP_HEADER_JMP 14
#define TCP_HEADER_JMP 20
#define DATA_JMP 20	
#define SNAPLEN 65536
struct ether_add
{
	unsigned char mac_add[6];
};
struct ether_header
{
	struct ether_add src_mac;
	struct ether_add des_mac;
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
	struct ether_add send_mac;
	unsigned char send_ip[4];
	struct ether_add target_mac;
	unsigned char target_ip[4];
};

void print_arp_header(const unsigned char *pkt_data);
void print_ether_header(const unsigned char *pkt_data);
void print_ip_header(const unsigned char *pkt_data);
void print_tcp_header(const unsigned char *pkt_data);
void print_data(const unsigned char *pkt_data);

int main(int argc, char **argv)
{
	pcap_if_t *alldevs = NULL;
	pcap_if_t *dev;
	pcap_t *use_dev;
	char errbuf[BUF_SIZE];
	char FILTER_RULE[BUF_SIZE] = "tcp";
	struct bpf_program rule_struct;
	int i, dev_num, res;
	struct pcap_pkt_hdr *header;
	const unsigned char *pkt_data;
	unsigned char packet[BUF_SIZE];

	//port설정 인자값이 있으면 port룰 설정
	//없으면, 모든 패킷 감청
	if (argv[1])
	{
		strcpy(FILTER_RULE, "port ");
		strcat(FILTER_RULE, argv[1]); // FILTER_RULE (port)설정
	}


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

	pcap_freealldevs(alldevs); //캡처 네트워크를 제외한 네트워크 해제

	packet[0] = 0xff;
	packet[1] = 0xff;
	packet[2] = 0xff;
	packet[3] = 0xff;
	packet[4] = 0xff;
	packet[5] = 0xff;

	packet[6] = 0xcc;
	packet[7] = 0xb0;
	packet[8] = 0xda;
	packet[9] = 0x18;
	packet[10] = 0x59;
	packet[11] = 0x2e;

	//ether_type (ARP로 설정)
	packet[12] = 0x08;
	packet[13] = 0x06;

	//hrd_type (Ethernet로 설정)
	packet[14] = 0x00;
	packet[15] = 0x01;

	//proto_type (IPv4 로 설정)
	packet[16] = 0x08;
	packet[17] = 0x00;

	//hrd_size (6)
	packet[18] = 0x06;
	//proto_size (4)
	packet[19] = 0x04;

	//Opcode (request)
	packet[20] = 0x00;
	packet[21] = 0x01;

	// Sender MAC (My mac)
	packet[22] = 0xcc;
	packet[23] = 0xb0;
	packet[24] = 0xda;
	packet[25] = 0x18;
	packet[26] = 0x59;
	packet[27] = 0x2e;
	//Sender IP (MY ip)
	packet[28] = 10;
	packet[29] = 100;
	packet[30] = 111;
	packet[31] = 107;

	//Target MAC
	packet[32] = 0x00;
	packet[33] = 0x00;
	packet[34] = 0x00;
	packet[35] = 0x00;
	packet[36] = 0x00;
	packet[37] = 0x00;
	//Target IP
	packet[38] = 10;
	packet[39] = 100;
	packet[40] = 111;
	packet[41] = 169;

	//end
	//for (int i = 42; i < BUF_SIZE; i++) packet[i] = 0;
	if (pcap_sendpacket(use_dev, packet, 42) != 0)
	{
		printf("pcap_send Fail!\n");
		pcap_freealldevs(alldevs);
		return -1;
	}

	while ((res = pcap_next_ex(use_dev, &header, &pkt_data)) >= 0)
	{
		if (res == 0) continue;

		if (kbhit())
		{
			printf("Pause!\n");
			system("pause");
			if (kbhit()) continue;
		}
		print_ether_header(pkt_data);
		pkt_data += IP_HEADER_JMP;
		print_arp_header(pkt_data);
		//print_ip_header(pkt_data);
		//pkt_data += TCP_HEADER_JMP;
		//print_tcp_header(pkt_data);
		//pkt_data += DATA_JMP;
		//print_data(pkt_data);
	}

}



///////////////////////////////////print_function///////////////////////////////////////
void print_ether_header(const unsigned char *pkt_data)
{
	struct ether_header *eh;
	eh = (struct ether_header *)pkt_data;
	unsigned short ether_type = ntohs(eh->eth_type);
	if (ether_type == 0x0806) {
		printf("===== ARP ======\n");
		system("pause");
	}
	if (ether_type == 0x0800) printf("===== IPv4 =====\n");
	printf("Src MAC : ");
	for (int i = 0; i <= 5; i++) printf("%02x ", eh->src_mac.mac_add[i]);
	printf("\nDes MAC : ");
	for (int i = 0; i <= 5; i++)printf("%02x ", eh->des_mac.mac_add[i]);
	printf("\n");
}

void print_ip_header(const unsigned char *pkt_data)
{
	struct ip_header *ih;
	ih = (struct ip_header *)pkt_data;
	if (ih->ip_protocol == 0x06)
	{
		printf("(TCP)");
		printf("Src IP : %s\n", inet_ntoa(ih->ip_src_add));
		printf("(TCP)");
		printf("Des IP : %s\n", inet_ntoa(ih->ip_des_add));
	}
	if (ih->ip_protocol == 0x17)
	{
		printf("(UDP)");
		printf("Src IP : %s\n", inet_ntoa(ih->ip_src_add));
		printf("(UDP)");
		printf("Des IP : %s\n", inet_ntoa(ih->ip_des_add));
	}
}
void print_tcp_header(const unsigned char *pkt_data)
{
	struct tcp_header *th;
	th = (struct tcp_header *)pkt_data;

	printf("Src Port : %d\n", ntohs(th->src_port));
	printf("Des Port : %d\n", ntohs(th->des_port));

}
void print_data(const unsigned char *pkt_data)
{
	printf("*DATA*\n");
	printf("%s\n", pkt_data);
	printf("====================\n\n");
}

void print_arp_header(const unsigned char *pkt_data)
{
	struct arp_header *ah;
	ah = (struct arp_header *)pkt_data;
	printf("============ARP HEADER===========\n");
	printf("Hardware_type : %d\n", ntohs(ah->hardware_type));
	printf("Protocol_type : %d\n", ah->protocol_type);
	printf("Hardware_size : %d\n", ah->hardware_size);
	printf("Protocol_size : %d\n", ah->protocol_size);
	printf("Opcode : %d\n", ntohs(ah->opcode));
	printf("Sender MAC : ");
	for (int i = 0; i < 6; i++) printf("%02x ", ah->send_mac.mac_add[i]);
	printf("\nSender IP : %s\n", inet_ntoa(*(struct in_addr *)&ah->send_ip));
	printf("Target MAC : ");
	for (int i = 0; i < 6; i++) printf("%02x ", ah->target_mac.mac_add[i]);
	printf("\nTarget IP : %s\n", inet_ntoa(*(struct in_addr *)&ah->target_ip));
}
