#include "PageRank.h"
#include "Matrice.h"
#include <fstream>

using std::ostream;  using std::endl; using std::cout;

PageRank::PageRank() {
	Matrix vp;
	std::vector<std::string> all;
	std::vector<std::pair<double, std::string>> rank;
}

//calcule du pagerank avec random walk + display
void PageRank::calculate(nlohmann::json j, double b, int iteration) {
	Matrix M;
	M = Matrix::createSquare(j.size());
	M.getMatrixM(j);

	all = M.getAll();

	Vp = Matrix(j.size(), 1, (1.0 / (j.size())));

	Matrix E;
	E = Matrix::createSquare(j.size(), 1);

	for (int i(0); i < iteration; ++i) {
		Vp = b * M*Vp + ((1.0 - b) / (M.getNbCols()))*E;
	}

}

void PageRank::calculate(const char* path, double b, int iteration) {

	std::ifstream file(path);
	nlohmann::json j;
	file >> j;

	Matrix M;
	M = Matrix::createSquare(j.size());
	M.getMatrixM(j);

	all = M.getAll();

	Vp = Matrix(j.size(), 1, (1.0 / (j.size())));

	Matrix E;
	E = Matrix::createSquare(j.size(), 1);

	for (int i(0); i < iteration; ++i) {
		Vp = b * M*Vp + ((1.0 - b) / (M.getNbCols()))*E;
	}

}

//pour simulation cas avec probleme
void PageRank::calculateFaulty(const char* path, int iteration) {
	std::ifstream file(path);
	nlohmann::json j;
	file >> j;

	Matrix M;
	M = Matrix::createSquare(j.size());
	M.getMatrixMFaulty(j);

	all = M.getAll();

	Vp = Matrix(j.size(), 1, (1.0 / (j.size())));

	for (int i(0); i < iteration; ++i) {
		Vp = M*Vp;
	}
}

void PageRank::calculateFaulty(nlohmann::json j, int iteration) {

	Matrix M;
	M = Matrix::createSquare(j.size());
	M.getMatrixMFaulty(j);

	all = M.getAll();

	Vp = Matrix(j.size(), 1, (1.0 / (j.size())));

	for (int i(0); i < iteration; ++i) {
		Vp = M * Vp;
	}
}

//calcule avec comparation par rapport a epsilon
int PageRank::calculateEpsilon(nlohmann::json j, double b, double eps) {

	Matrix M;
	M = Matrix::createSquare(j.size());
	M.getMatrixM(j);

	all = M.getAll();

	Vp = Matrix(j.size(), 1, (1.0 / (j.size())));

	Matrix Vp0(Vp);

	Matrix E;
	E = Matrix::createSquare(j.size(), 1);

	int i(0);

	while (Vp.diffVecteur(Vp0, eps) != true) {
		Vp0 = Vp;
		Vp = b * M*Vp0 + ((1.0 - b) / (M.getNbCols()))*E;
		i += 1;
	}
	return i;
}

int PageRank::calculateEpsilon(const char* path, double b, double eps) {

	std::ifstream file(path);
	nlohmann::json j;
	file >> j;

	Matrix M;
	M = Matrix::createSquare(j.size());
	M.getMatrixM(j);

	all = M.getAll();

	Vp = Matrix(j.size(), 1, (1.0 / (j.size())));

	Matrix Vp0(Vp);

	Matrix E;
	E = Matrix::createSquare(j.size(), 1);

	int i(0);

	while (Vp.diffVecteur(Vp0, eps)!=true) {
		Vp0 = Vp;
		Vp = b * M*Vp0 + ((1.0 - b) / (M.getNbCols()))*E;
		i += 1;
	}
	return i;
}

//pour simulation cas avec probleme avec epsilon
void PageRank::calculateEpsilonFaulty(nlohmann::json j, double b, double eps) {
	Matrix M;
	M = Matrix::createSquare(j.size());
	M.getMatrixMFaulty(j);

	all = M.getAll();

	Vp = Matrix(j.size(), 1, (1.0 / (j.size())));

	Matrix Vp0(Vp);

	while (Vp.diffVecteur(Vp0, eps) != true) {
		Vp0 = Vp;
		Vp = M * Vp;
	}
}

void PageRank::calculateEpsilonFaulty(const char* path, double b, double eps) {
	std::ifstream file(path);
	nlohmann::json j;
	file >> j;
	
	Matrix M;
	M = Matrix::createSquare(j.size());
	M.getMatrixMFaulty(j);

	all = M.getAll();

	Vp = Matrix(j.size(), 1, (1.0 / (j.size())));

	Matrix Vp0(Vp);

	while (Vp.diffVecteur(Vp0, eps) != true) {
		Vp0 = Vp;
		Vp = M * Vp;
	}
}

//affichage sans ordre
void PageRank::display() const {
	for (size_t i(0); i < all.size(); ++i) {
		cout << all[i] << " influence is " << Vp(i, 0) << endl;
	}
}

//affichage avec ordre
void PageRank::displayRank() const {
	for (size_t i = 0; i < rank.size(); i++)
	{
		cout << rank[i].second << " influence is "
			<< rank[i].first << endl;
	}
}


//fill et sort rank pour avoir du plus petit au plus grand
void PageRank::fillRank() {
	for (size_t i(0); i < all.size(); ++i) {
		rank.push_back(std::make_pair(Vp(i,0), all[i]));
	}
}

void PageRank::sortRankDescending() {
	std::sort(rank.rbegin(), rank.rend());
}

void PageRank::sortRankAscending() {
	std::sort(rank.begin(), rank.end());
} 

void PageRank::calculateRankDescending() {
	fillRank();
	sortRankDescending();
}

void PageRank::calculateRankAscending() {
	fillRank();
	sortRankAscending();
}


//getters
Matrix PageRank::getVp() {
	return Vp;
}

std::vector<std::string> PageRank::getAll() {
	return all;
}

std::vector<std::pair<double, std::string>> PageRank::getRank() {
	return rank;
}

double PageRank::getVp(int i) {
	return Vp(i, 0);
}

std::string PageRank::getAll(int i) {
	return all[i];
}

double PageRank::getAllFirst(int i) {
		return rank[i].first;
	}

std::string PageRank::getAllSecond(int i) {
	return rank[i].second;
}

int PageRank::getSize() {
	return all.size();
}