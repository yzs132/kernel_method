#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <dirent.h>
#include "ip.hpp"
using namespace std;

int getdir (string dir, vector<string> &files)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }

    while ((dirp = readdir(dp)) != NULL) {
        files.push_back(string(dirp->d_name));
    }
    closedir(dp);
    return 0;
}


int main(){
	
	string dir = string("../../data");
	vector<string> fileset = vector<string>();
	getdir(dir,fileset);
	std::sort (fileset.begin(), fileset.end());
	for (unsigned int i = 0;i < fileset.size();i++) {
                cout << fileset[i] << endl;
        }

	float sigma = 6799600; //standard deviation of bandwidth
	
	string line;

	for (int fi = 2; fi < fileset.size()-1; fi++) {
		std::map<std::pair<string, string>, int> src_dest_map1;
		std::map<std::pair<string, string>, int> src_dest_map2;

		string filename = "../../data/"+fileset[fi];
		ifstream inputfile(filename);

		if (inputfile.is_open()){
			getline(inputfile, line);
			cout << line << endl;
			int linenum = 0;
			while (getline(inputfile, line)){
				//cout << line << endl;
				char *token = std::strtok((char*)line.c_str(), ",");
				int f_index = 0;
			
				string src_ip;
		                string dest_ip;
		                int bw; 
				while (token != NULL){
					if (f_index == 2) {
						src_ip = string(token);
						src_ip.erase(src_ip.begin(), src_ip.begin()+8);
						src_ip.erase(src_ip.end()-1);
						cout << "src ip: " << src_ip << " "; 
					}
					if (f_index == 4) {
						dest_ip = string(token);
						dest_ip.erase(dest_ip.begin(), dest_ip.begin()+8);
						dest_ip.erase(dest_ip.end()-1);
						cout << "dest ip: "<<dest_ip << " ";
					}
					if (f_index == 5) {
						bw = strtol(token, NULL, 10); 
						cout << "bandwidth: " << bw << endl;
					}
					token = std::strtok(NULL, ",");
					f_index++;
				}
				auto search = src_dest_map1.find(std::make_pair(src_ip, dest_ip));
				if(search == src_dest_map1.end()) {
					src_dest_map1.insert(make_pair(std::make_pair(src_ip, dest_ip), bw));
				} else {
					search->second += bw;
				}
				++linenum;
				if (linenum == 100 ) break;
			}
			cout << "map.size(): " << src_dest_map1.size() << endl;
			inputfile.close();
		}

		string filename2 = "../../data/"+fileset[fi+1];
		cout << filename2 << endl;
		ifstream inputfile2(filename2);
				
		if (inputfile2.is_open()){
			getline(inputfile2, line);
			cout << line << endl;
			int linenum = 0;
			while (getline(inputfile2, line)){
				//cout << line << endl;
				char *token = std::strtok((char*)line.c_str(), ",");
				int f_index = 0;
			
				string src_ip;
		                string dest_ip;
		                int bw; 
				while (token != NULL){
					if (f_index == 2) {
						src_ip = string(token);
						src_ip.erase(src_ip.begin(), src_ip.begin()+8);
						src_ip.erase(src_ip.end()-1);
						cout << "src ip: " << src_ip << " "; 
					}
					if (f_index == 4) {
						dest_ip = string(token);
						dest_ip.erase(dest_ip.begin(), dest_ip.begin()+8);
						dest_ip.erase(dest_ip.end()-1);
						cout << "dest ip: "<<dest_ip << " ";
					}
					if (f_index == 5) {
						bw = strtol(token, NULL, 10); 
						cout << "bandwidth: " << bw << endl;
					}
					token = std::strtok(NULL, ",");
					f_index++;
				}
				auto search = src_dest_map2.find(std::make_pair(src_ip, dest_ip));
				if(search == src_dest_map2.end()) {
					src_dest_map2.insert(make_pair(std::make_pair(src_ip, dest_ip), bw));
				} else {
					search->second += bw;
				}
				++linenum;
                                if (linenum == 100 ) break;
			}
			cout << "map.size(): " << src_dest_map2.size() << endl;
			inputfile2.close();
		}

		//loop through the neighboring window/map
		int count = 0;
		double Kw1w2 = 0;
	
		for (auto it1=src_dest_map1.begin(); it1!=src_dest_map1.end(); ++it1){
			for (auto it2=src_dest_map2.begin(); it2!=src_dest_map2.end(); ++it2){
				count++;
				cout << count << endl;	
				cout << it1->first.first << ", " << it2->first.first << endl;
				float S_src = similarityIP(it1->first.first, it2->first.first );
				float S_dest = similarityIP(it1->first.second, it2->first.second);
				float M_vol = matchingVol(it1->second, it2->second, sigma);
				Kw1w2 += S_src * S_dest * M_vol;
				cout << S_src << ", " << S_dest << ", " << M_vol << endl;
			}
		}
		cout << "Kw1w2: " << Kw1w2 << endl;  

	}
	return 0;
}
