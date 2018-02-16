#ifndef __PageRank_H__
#define __PageRank_H__

#include "Matrice.h"
#include "json.h"
#include <vector>
#include <utility>

#define EPS 1e-15

class PageRank {
public:

	PageRank();

	Matrix getVp();
	std::vector<std::string> getAll();
	std::vector<std::pair<double, std::string>> getRank();

	double getVp(int);
	std::string getAll(int);
	double getAllFirst(int i);
	std::string getAllSecond(int i);


	int getSize();

	void calculate(nlohmann::json, double = 0.85, int = 100);
	void calculate(const char*, double = 0.85, int = 100);

	void calculateFaulty(nlohmann::json, int = 100);
	void calculateFaulty(const char*, int = 100);

	void display() const;
	void displayRank() const;

	void fillRank();
	void sortRankDescending();
	void sortRankAscending();
	void calculateRankDescending();
	void calculateRankAscending();

	int calculateEpsilon(const char*, double = 0.85, double = EPS);
	int calculateEpsilon(nlohmann::json j, double = 0.85, double = EPS);

	void calculateEpsilonFaulty(const char*, double = 0.85, double = EPS);
	void calculateEpsilonFaulty(nlohmann::json j, double = 0.85, double = EPS);


private:
	Matrix Vp;
	std::vector<std::string> all;
	std::vector<std::pair<double, std::string>> rank;
};

#endif