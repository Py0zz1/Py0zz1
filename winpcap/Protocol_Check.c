#define _WINSOCK_DEPRECATED_NO_WARNINGS 
#include <stdio.h>
#include <pcap.h>
#include <WinSock2.h>

#pragma comment (lib,"ws2_32.lib")
#pragma comment (lib,"wpcap.lib")

#define FILTER_RULE "host 127.0.0.1 and port 4540"

struct ether_addr
{
	unsigned char mac_add[6];
};
struct ether_header
{
	struct ether_addr ether_dhost;
	struct ether_addr ether_shost;
	unsigned short ether_type;
};

struct ip_header
{
	unsigned char ip_version : 4;
	unsigned char ip_header_length : 4;
	unsigned char ip_tos;
	unsigned short ip_total_length;
	unsigned short ip_iden;
	unsigned char ip_flag_reserv : 1;
	unsigned char ip_flag_dont : 1;
	unsigned char ip_flag_more : 1;
	unsigned char ip_flag_offset_part1 : 5;
	unsigned char ip_flag_offset_part2;
	unsigned char ip_ttl;
	unsigned char protocol;
	unsigned short check_sum;
	struct in_addr src_add;
	struct in_addr des_add;
};
struct tcp_header
{
	unsigned short src_port;
	unsigned short des_port;
	unsigned int sqn_num;
	unsigned int ack_num;
	unsigned char tcp_offset : 4;
	unsigned char ns : 1;
	unsigned char reserver : 3;
	unsigned char flag_CWR : 1;
	unsigned char flag_ECE : 1;
	unsigned char flag_Urgent : 1;
	unsigned char flag_ACK : 1;
	unsigned char flag_Push : 1;
	unsigned char flag_Reset : 1;
	unsigned char flag_SYN : 1;
	unsigned char flag_Fin : 1;
	unsigned short windows;
	unsigned short check_sum;
	unsigned short Urgent_pointer;
};

void print_ether_header(const unsigned char *data);
int print_ip_header(const unsigned char *data);
int print_tcp_header(const unsigned char *data);

int main()
{
	pcap_if_t *alldevs = NULL;
	char errbuf[PCAP_ERRBUF_SIZE];

	int offset = 0; // 헤더위치 점프 값

	pcap_if_t *d;
	int i;
	if (pcap_findalldevs(&alldevs, errbuf) < 0)
	{
		printf("디바이스 찾는데 실패하였습니다.\n");
		return -1;
	}
	else if (alldevs == NULL)
	{
		printf("디바이스가 없습니다.\n");
		return -1;
	}

	for (d = alldevs, i = 0; d != NULL; d = d->next)
	{
		printf("%d번째 Device:%s", ++i, d->name);
		if (d->description) printf("(%s)\n", d->description);
		else printf("유효한 디바이스가 없습니다.");
	}

	int j;
	printf("캡처할 장치 번호를 입력하세요 : ");
	scanf("%d", &j);
	for (d = alldevs, i = 0; i < j - 1; d = d->next, i++);

	//pcap_open
	pcap_t *fp;
	if ((fp = pcap_open_live(d->name, 65536, 1, 20, errbuf)) == NULL)
	{
		printf("pcap_open Error!\n");
		pcap_freealldevs(alldevs);
		return -1;
	}
	printf("pcap_open 성공!\n");

	struct bpf_program fcode;
	if (pcap_compile(fp, &fcode, FILTER_RULE, 1, NULL) < 0) // 실패시 -1리턴
	{
		printf("pcap_compile Error!\n");
		pcap_freealldevs(alldevs);
		return -1;
	}

	if (pcap_setfilter(fp, &fcode) < 0) // 실패시 -1리턴
	{
		printf("pcap_setfilter Error!\n");
		pcap_freealldevs(alldevs);
		return -1;
	}
	pcap_freealldevs(alldevs); // 캡처할 디바이스 빼고 모든 디바이스 반환

	int res;
	struct pcap_pkthdr *header;
	const unsigned char *pkt_data;

	while ((res = pcap_next_ex(fp, &header, &pkt_data)) >= 0)
	{
		if (res == 0) continue;
		print_ether_header(pkt_data);
		pkt_data += 14; // Ether_header길이만큼 점프
		offset = print_ip_header(pkt_data);
		pkt_data += offset; // Ether_header + IP_header 길이만큼 점프
		offset = print_tcp_header(pkt_data);
		pkt_data += offset; // Ether_header + IP_header + TCP_header 길이만큼 점프
							// 이후론 데이터 구간
	}
	return 0;

}
void print_ether_header(const unsigned char *data)
{
	struct ether_header *eh;
	unsigned short ether_type;
	eh = (struct ether_header *)data;
	ether_type = ntohs(eh->ether_type);

	if (ether_type != 0x8000)
	{
		printf("IPv4가 아닙니다.\n");
		return;
	}
	printf("Src Mac : %02x%02x%02x%02x%02x%02x\n",
		eh->ether_shost.mac_add[0],
		eh->ether_shost.mac_add[1],
		eh->ether_shost.mac_add[2],
		eh->ether_shost.mac_add[3],
		eh->ether_shost.mac_add[4],
		eh->ether_shost.mac_add[5]);
	printf("Des Mac : %02x%02x%02x%02x%02x%02x\n",
		eh->ether_dhost.mac_add[0],
		eh->ether_dhost.mac_add[1],
		eh->ether_dhost.mac_add[2],
		eh->ether_dhost.mac_add[3],
		eh->ether_dhost.mac_add[4],
		eh->ether_dhost.mac_add[5]);
}
int print_ip_header(const unsigned char *data)
{
	struct ip_header *ih;
	ih = (struct ip_header *)data;
	if (ih->protocol == 0x06) printf("Protocol : TCP\n");
	else if (ih->protocol == 0x17) printf("Protocol : UDP\n");

	printf("Src IP : %s\n", inet_ntoa(ih->src_add));
	printf("Dest IP : %s\n", inet_ntoa(ih->des_add));

	return ih->ip_header_length * 4; // header_length = 20 이지만 1비트만 사용하기 위해서 5저장, 4곱해서 총 길이인 20 리턴
}

int print_tcp_header(const unsigned char *data)
{
	struct tcp_header *th;
	th = (struct tcp_header *)data;

	printf("Src Port : %d\n", th->src_port);
	printf("Dest Port : %d\n", th->des_port);

	return th->tcp_offset * 4;
}
