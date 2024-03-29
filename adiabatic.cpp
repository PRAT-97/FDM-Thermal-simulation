/* Readme: To print the values to plot gnuplot, .txt file is suppose to be created. The .txt file should not hold any title name or
   index name, as it can create error in plotting graphs in Gnuplot, therefore comment out the lines which prints title and print 
   the desired output, display of initial matrix is only for reference, therefore avoid printing in .txt file as it is not need to plot 
   graph, only optimised matrix is needed to plotted in this code*/

#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <cstring>
#include <filesystem>
#define M_PI 3.141592653589793238462643383279502884L
using namespace std;
namespace fs = std::filesystem;

/*-----------Wall condition defined in matrix function-------------------*/
/*-----------substitute the wall temperature given in main program-----------*/ 
void initializeMatrix(int rows, int cols, double wall_1[], double wall_2[], double wall_3[], double wall_4[], double** T) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (i == 0 && j == 0) {
                T[i][j] = wall_2[j];
            } else if (i == rows-1 && j > 0) {
                T[i][j] = wall_4[j];
            } else if (j == cols-1 && i < rows-1) {
                T[i][j] = wall_3[i];
            } else if (j == 0 && i > 0) {
                T[i][j] = wall_1[i];    
            }else {
                T[i][j] = 0.0;  
            }
        }
    }
}


/*------Gauss-Seidel iteration equation to define wall temperature function-------*/
void gaussSeidel(int rows, int cols, double** T, double tolerance) {
    double diff;
    do {
        diff = 0.0;
        for (int i = 0; i < rows - 1; ++i) {
            for (int j = 1; j < cols - 1; ++j) {
                double oldValue = T[i][j];
                if (i == 0 && j > 0 && j < cols-1){
                    T[i][j]= 0.25 * (2* T[i + 1][j] + T[i][j - 1] + T[i][j + 1]);
                } else {
                    T[i][j] = 0.25 * (T[i - 1][j] + T[i + 1][j] + T[i][j - 1] + T[i][j + 1]);
                }
                diff = max(diff, abs(T[i][j] - oldValue));
            }
        }
    /*----tolerance given to converge the solution-------*/
    } while (diff > tolerance);
}

/*----------------print the matrix function-------------------*/
void printMatrix(int rows, int cols, double** T, ofstream& outFile, const char* matrixName) {
    cout << matrixName<< ":" << endl;
    outFile <<"#" << matrixName << ":" << endl;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            cout << T[i][j] << "\t";
            outFile << T[i][j] << "\t";
        }
        cout << endl;
        outFile << endl;
    }
    cout << endl;
    outFile << endl;
}

/*----------------main program begins-------------------*/

int main() {
    
    const int rows = 41;                         /*row size = mesh size+1*/ 
    const int cols = 41;                         /*col size = mesh size+1*/
    int hor_mesh = rows-1;                       /*horizontal mesh size is the total elements in plate*/
    int ver_mesh = cols-1;                       /*vertical mesh size is the total elements in plate*/
    double *wall_1 = new double[hor_mesh];       /*declaring array value type in double*/
    double *wall_3 = new double[hor_mesh];       /*declaring array value type in double*/
    double t_mit = 300;                          /*initial condition - given*/
    double del_t = 100;                          /*initial condition - given*/
    double *wall_2 = new double [ver_mesh];
    double *wall_4 = new double [ver_mesh];
    
    /*-----Array T with two elements type-----*/
    double** T = new double*[rows];
    for (int i = 0; i < rows; ++i) {
        T[i] = new double[cols];
    }
    int i;
    int j;
    
    /*-----change the file name if needed-----*/
    ofstream myfile;
    myfile.open ("mesh40.txt");


    for (i = 0; i <= hor_mesh; ++i) {
        wall_1[i] = t_mit + del_t * sin(M_PI * (static_cast<double>(i) / static_cast<double>(hor_mesh)));
        wall_3[i] = 300;
    }
    
    for (j = 0; j <= ver_mesh; ++j) {
        wall_2[j]=300;
        wall_4[j]=300;
    }

    initializeMatrix(rows, cols, wall_1, wall_2, wall_3, wall_4, T);

    /*------print the initial matrix without temperature of internal nodes------*/
    cout << "Initial Matrix:" << endl;
    printMatrix(rows, cols, T, myfile, "Inital Matrix");

    gaussSeidel(rows, cols, T, 0.0001);

    /*------print matrix with temperature of internal nodes------*/
    cout << "\nOptimized Matrix:" << endl;
    printMatrix(rows, cols, T, myfile, "Optimized Matrix:");

     myfile.close();  

    return 0;
}