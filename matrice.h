#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <iostream>
#include "json.h"
#include <vector>
#include <string>



class Matrix {
public:

        Matrix(int, int, double = 0);
        Matrix();
        Matrix(const Matrix&);
        ~Matrix();
        Matrix& operator=(const Matrix&);

        static Matrix createSquare(int, double = 0);
        static Matrix createIdentity(int);

        int getNbRows() const;
        int getNbCols() const;
        std::vector<std::string> getAll() const;
        const std::vector<std::string> &getAllref() const;

        inline double& operator()(int x, int y) { return p[x][y]; }
        inline double& operator()(int x, int y) const { return p[x][y]; }

        Matrix& operator+=(const Matrix&);
        Matrix& operator-=(const Matrix&);
        Matrix& operator*=(const Matrix&);
        Matrix& operator*=(double);
        Matrix& operator/=(double);

        friend std::ostream& operator<<(std::ostream&, const Matrix&);
        friend std::istream& operator >> (std::istream&, Matrix&);

        Matrix transpose();

        double sumCol(int);
        void stochatisation();
        void stochatisationFaulty();

        void addToAll(std::string);
        void fillAll(nlohmann::json);

        void fillRow(int, nlohmann::json);
        void fill(nlohmann::json);

        void getMatrixM(nlohmann::json);
        void getMatrixMFaulty(nlohmann::json);

        bool diffVecteur(Matrix&, double);

private:
        int rows_, cols_;
        double **p;

        std::vector<std::string> all;

        void allocSpace();
};

Matrix operator+(const Matrix&, const Matrix&);
Matrix operator-(const Matrix&, const Matrix&);
Matrix operator*(const Matrix&, const Matrix&);
Matrix operator*(const Matrix&, double);
Matrix operator*(double, const Matrix&);
Matrix operator/(const Matrix&, double);

#endif
