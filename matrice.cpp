#include <stdexcept>
#include "Matrice.h"

#define EPS 1e-10

using std::ostream;  using std::istream;  using std::endl;
using std::domain_error;

//constructeur + destructeur

Matrix::Matrix(int rows, int cols, double valeuraremplir) : rows_(rows), cols_(cols)
{
        all = std::vector<std::string>(0);
        allocSpace();
        for (int i = 0; i < rows_; ++i) {
                for (int j = 0; j < cols_; ++j) {
                        p[i][j] = valeuraremplir;
                }
        }
}

Matrix::Matrix() : rows_(1), cols_(1)
{
        all = std::vector<std::string>(0);
        allocSpace();
        p[0][0] = 0;
}

Matrix::Matrix(const Matrix& m) : rows_(m.rows_), cols_(m.cols_)
{
        all = std::vector<std::string>(0);
        allocSpace();
        for (int i = 0; i < rows_; ++i) {
                for (int j = 0; j < cols_; ++j) {
                        p[i][j] = m.p[i][j];
                }
        }
}

Matrix::~Matrix()
{
        for (int i = 0; i < rows_; ++i) {
                delete[] p[i];
        }
        delete[] p;
}

//plus de constructeurs

Matrix Matrix::createSquare(int size, double valeuraremplir)
{
        Matrix temp(size, size);
        for (int i = 0; i < temp.rows_; ++i) {
                for (int j = 0; j < temp.cols_; ++j) {
                        temp.p[i][j] = valeuraremplir;
                }
        }
        return temp;
}

Matrix Matrix::createIdentity(int size)
{
        Matrix temp(size, size);
        for (int i = 0; i < temp.rows_; ++i) {
                for (int j = 0; j < temp.cols_; ++j) {
                        if (i == j) {
                                temp.p[i][j] = 1;
                        }
                        else {
                                temp.p[i][j] = 0;
                        }
                }
        }
        return temp;
}


//getters

int Matrix::getNbCols() const{
        return cols_;
}
int Matrix::getNbRows() const {
        return rows_;
}
std::vector<std::string> Matrix::getAll() const {
        return all;
}
const std::vector<std::string>& Matrix::getAllref() const {
        return all;
}

//surcharge operateur interne (methode)

Matrix& Matrix::operator=(const Matrix& m)
{
        if (this == &m) {
                return *this;
        }

        if (rows_ != m.rows_ || cols_ != m.cols_) {
                for (int i = 0; i < rows_; ++i) {
                        delete[] p[i];
                }
                delete[] p;

                rows_ = m.rows_;
                cols_ = m.cols_;
                allocSpace();
        }

        for (int i = 0; i < rows_; ++i) {
                for (int j = 0; j < cols_; ++j) {
                        p[i][j] = m.p[i][j];
                }
        }
        return *this;
}

Matrix& Matrix::operator+=(const Matrix& m)
{
        for (int i = 0; i < rows_; ++i) {
                for (int j = 0; j < cols_; ++j) {
                        p[i][j] += m.p[i][j];
                }
        }
        return *this;
}

Matrix& Matrix::operator-=(const Matrix& m)
{
        for (int i = 0; i < rows_; ++i) {
                for (int j = 0; j < cols_; ++j) {
                        p[i][j] -= m.p[i][j];
                }
        }
        return *this;
}

Matrix& Matrix::operator*=(const Matrix& m)
{
        Matrix temp(rows_, m.cols_);
        for (int i = 0; i < temp.rows_; ++i) {
                for (int j = 0; j < temp.cols_; ++j) {
                        for (int k = 0; k < cols_; ++k) {
                                temp.p[i][j] += (p[i][k] * m.p[k][j]);
                        }
                }
        }
        return (*this = temp);
}

Matrix& Matrix::operator*=(double num)
{
        for (int i = 0; i < rows_; ++i) {
                for (int j = 0; j < cols_; ++j) {
                        p[i][j] *= num;
                }
        }
        return *this;
}

Matrix& Matrix::operator/=(double num)
{
        for (int i = 0; i < rows_; ++i) {
                for (int j = 0; j < cols_; ++j) {
                        p[i][j] /= num;
                }
        }
        return *this;
}


//transpose

Matrix Matrix::transpose()
{
        Matrix ret(cols_, rows_);
        for (int i = 0; i < rows_; ++i) {
                for (int j = 0; j < cols_; ++j) {
                        ret.p[j][i] = p[i][j];
                }
        }
        return ret;
}


//allocation car c'est un tableau a la C

void Matrix::allocSpace()
{
        p = new double*[rows_];
        for (int i = 0; i < rows_; ++i) {
                p[i] = new double[cols_];
        }
}

//division par la somme de la colonne pour rendre la matrice Stochastique

double Matrix::sumCol(int j) {
        double sum(0);
        for(int i = 0; i < cols_; ++i) {
                sum += p[i][j];
        }
        return sum;
}

void Matrix::stochatisation() {
        for (int j = 0; j < cols_; ++j) {
                double sum(sumCol(j));
                if (sum != 0) {
                        for (int i = 0; i < rows_; ++i) {
                                p[i][j] = p[i][j] / sum;
                        }
                }
                else {
                        for (int i = 0; i < rows_; ++i) {
                                p[i][j] = 1.0 / getNbRows();
                        }
                }

        }
}

//en fait ce n'est pas une stochatisation puisqu'elle ne fixe pas les colonnes nuls
void Matrix::stochatisationFaulty() {
        for (int j = 0; j < cols_; ++j) {
                double sum(sumCol(j));
                if (sum != 0) {
                        for (int i = 0; i < rows_; ++i) {
                                p[i][j] = p[i][j] / sum;
                        }
                }

        }
}

//implementation pour le vecteur all qui contient mes "personnes"

void Matrix::addToAll(std::string obj) {
        all.push_back(obj);
}

void Matrix::fillAll(nlohmann::json json) {
        for (nlohmann::json::iterator it = json.begin(); it != json.end(); ++it) {
                addToAll(it.key());
        }
}


//remplissage de la matrice

void Matrix::fillRow(int i, nlohmann::json json) {
        for (size_t c(0); c < json[all[i]]["followers"].size(); ++c) {
                ptrdiff_t pos = find(all.begin(), all.end(), json[all[i]]["followers"][c]) - all.begin();
                        p[i][pos] = 1;
        }

}
void Matrix::fill(nlohmann::json json) {
        for (int i(0); i < rows_; ++i) {
                fillRow(i, json);
        }
}

//avoir la matrice M du debut sans le damping factor

void Matrix::getMatrixM(nlohmann::json j) {
        fillAll(j);
        fill(j);
        stochatisation();
}

void Matrix::getMatrixMFaulty(nlohmann::json j) {
        fillAll(j);
        fill(j);
        stochatisationFaulty();
}

//pour comparer le pagerank avec epsilon
bool Matrix::diffVecteur(Matrix& m, double eps) {
        for (int i(0); i < m.getNbRows(); ++i) {
                if ((p[i][0] - m.p[i][0]) > eps) {
                        return true;
                }
        }
        return false;
}

//surcharge des operateurs externes

Matrix operator+(const Matrix& m1, const Matrix& m2)
{
        Matrix temp(m1);
        return (temp += m2);
}

Matrix operator-(const Matrix& m1, const Matrix& m2)
{
        Matrix temp(m1);
        return (temp -= m2);
}

Matrix operator*(const Matrix& m1, const Matrix& m2)
{
        Matrix temp(m1);
        return (temp *= m2);
}

Matrix operator*(const Matrix& m, double num)
{
        Matrix temp(m);
        return (temp *= num);
}

Matrix operator*(double num, const Matrix& m)
{
        return (m * num);
}

Matrix operator/(const Matrix& m, double num)
{
        Matrix temp(m);
        return (temp /= num);
}

ostream& operator<<(ostream& os, const Matrix& m)
{
        for (int i = 0; i < m.rows_; ++i) {
                os << m.p[i][0];
                for (int j = 1; j < m.cols_; ++j) {
                        os << " " << m.p[i][j];
                }
                os << endl;
        }
        return os;
}

istream& operator >> (istream& is, Matrix& m)
{
        for (int i = 0; i < m.rows_; ++i) {
                for (int j = 0; j < m.cols_; ++j) {
                        is >> m.p[i][j];
                }
        }
        return is;
}
