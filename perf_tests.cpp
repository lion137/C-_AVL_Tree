#include <iostream>
#include <map>
#include <unordered_map>
#include <chrono>
#include <random>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include "TreeMap.h"

using namespace std::chrono;

void t() {
	TreeMap<int, std::string> tr1{};
	auto st = high_resolution_clock::now();
	for (int i = 0; i < 9999999; ++i){
		tr1.put(i, "A");
		assert(tr1.getBalance()  <= abs(1));
	}
	auto en = high_resolution_clock::now();
	auto d = duration_cast<microseconds>(en - st);
	std::cout << "put time TreeMap"<<d.count()<<"\n";
}

void m() {
	std::map<int, std::string> m;
	auto st = high_resolution_clock::now();
	for (int i = 0; i < 9999999; ++i)
		m[i] = "A";
	auto en = high_resolution_clock::now();
	auto d = duration_cast<microseconds>(en - st);
	std::cout << "put time map"<<d.count()<<"\n";
}

void u() {
	std::unordered_map<int, std::string> m;
	auto st = high_resolution_clock::now();
	for (int i = 0; i < 9999999; ++i)
		m[i] = "A";
	auto en = high_resolution_clock::now();
	auto d = duration_cast<microseconds>(en - st);
	std::cout << "put time unordered_map "<<d.count()/1000000<<"\n";
}

void lookUpT() {
	std::random_device rd; 
    std::mt19937 eng(rd()); 
    std::uniform_int_distribution<> distr(0, 32669);
	TreeMap<int16_t, int16_t> tr1{};
	for (int i = 0; i < 39999999; ++i){
		tr1.put(i % 32670, 12345);
	}
	auto st = high_resolution_clock::now();
	for (int i = 0; i < 39999999; ++i)
		tr1.get(distr(eng));
	auto en = high_resolution_clock::now();
	auto d = duration_cast<microseconds>(en - st);
	std::cout << "lookup time TreeMap "<<d.count()<<"\n";
}

void lookUpM() {
	std::map<int16_t, int16_t> m;
	for (int i = 0; i < 39999999; ++i)
			m[i % 32670] = 12345;
	auto st = high_resolution_clock::now();
	for (int i = 0; i < 39999999; ++i)
		m.find(rand() % 32669) ;
	auto en = high_resolution_clock::now();
	auto d = duration_cast<microseconds>(en - st);
	std::cout << "lookup time STL map "<<d.count()<<"\n";
}

void lookUpUM() {
	std::unordered_map<int, std::string> m;
	for (int i = 0; i < 99999999; ++i)
			m[i] = "A";
	auto st = high_resolution_clock::now();
	for (int i = 0; i < 99999999; ++i)
		m.find(i) ;
	auto en = high_resolution_clock::now();
	auto d = duration_cast<microseconds>(en - st);
	std::cout << "lookup time unordered_map "<<d.count()<<"\n";
}


int main() {
	//t();
	//m(); 
	lookUpT();
	lookUpM();
	//lookUpUM();
	return 0;
}