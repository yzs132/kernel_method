#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <math.h>
#include <iomanip>
#include "ip.hpp"

float similarityIP(string ip1, string ip2){
	//cout << ip1  << ip2 << endl;
	struct sockaddr_in myaddr;

	//myaddr.sin_family = AF_INET;
	inet_aton(ip1.c_str(), &myaddr.sin_addr);
	uint32_t ip_n1 = (uint32_t)myaddr.sin_addr.s_addr;
	inet_aton(ip2.c_str(), &myaddr.sin_addr);
	uint32_t ip_n2 = (uint32_t)myaddr.sin_addr.s_addr;
	
	uint32_t ip_xor = ~(ip_n1 ^ ip_n2);
	//cout << std::hex << std::setw(8) << std::setfill('0') << ip_n1 << ", " << ip_n2 << ", "<< ip_xor << endl;

	int samebits[4];
	int count = 0;
	for (int i = 1; i <= 32; i++) {
		count += (ip_xor & 1);
		ip_xor = (ip_xor >> 1);
		if ( i%8 == 0 ){
			//cout << count << endl;
			samebits[i/8-1] = count;
			count = 0;
		}
	}
	float numerator = 0; float denominator = 10;
	for (int i=0; i<4; i++){
		numerator += ((4-i)*samebits[i]/8);
		//denominator += samebits[i];
	}
	//cout << numerator << ", " << denominator << endl;
	float res = numerator/denominator;
	return res;
}

float matchingVol (float vol1, float vol2, float sigma){
	float res = exp (-(vol1-vol2)*(vol1-vol2)/sigma/sigma);
	return res;
}
