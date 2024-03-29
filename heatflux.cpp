/* Readme: To print the values to plot gnuplot, .txt file is suppose to be created. The .txt file should not hold any title name or
   index name, as it can create error in plotting graphs in Gnuplot, therefore comment out the lines which prints title and print 
   the desired output, display of initial matrix is only for reference, therefore avoid printing in .txt file as it is not need to plot 
   graph, only optimised matrix and vector component is needed to plotted in this code*/

#include <iostream>
#include <cmath>
#include <fstream>
#include <iomanip>
#define M_PI 3.141592653589793238462643383279502884L
using namespace std;

/*-----------Wall condition defined in matrix function-------------------*/
/*-----------substitute the wall temperature given in main program-----------*/ 
void Matrix(int rows, int cols, double wall_1[], double wall_2[], double wall_3[], double wall_4[], double** T) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (i == 0 && j < rows-1) {
                T[i][j] = wall_2[j];                                
            } else if (i == cols-1 && j > 0) {
                T[i][j] = wall_4[j];
            } else if (j == rows-1 && i < cols-1) {
                T[i][j] = wall_3[i];
            } else if (j == 0 && i > 0) {
                T[i][j] = wall_1[i];
            } else {
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
        for (int i = 1; i < rows-1; ++i) {
            for (int j = 1; j < cols-1; ++j) {
                double oldValue = T[i][j];
                T[i][j] = (0.25 * (T[i - 1][j] + T[i + 1][j] + T[i][j - 1] + T[i][j + 1]));
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

int main () {
    
    const int rows = 41;                      /*row size = mesh size+1*/
    const int cols = 41;                      /*col size = mesh size+1*/
    int hor_mesh = rows-1;                    /*horizontal mesh size is the total elements in plate*/
    int ver_mesh = cols-1;                    /*vertical mesh size is the total elements in plate*/
    double *wall_1 = new double[hor_mesh];    /*declaring array value type in double*/
    double *wall_3 = new double[hor_mesh];    /*declaring array value type in double*/
    double t_mit = 300;                       /*initial condition - given*/
    double del_t = 100;                       /*initial condition - given*/
    //double theta;

    /*-----Array T with two elements type-----*/
    double** T = new double*[rows];
    for (int i = 0; i < rows; ++i) {
        T[i] = new double[cols];
    }

    /*-----change the file name if needed-----*/
    ofstream myfile;
    myfile.open ("mesh_40.txt");

    for (int i = 0; i <= hor_mesh; ++i) {
        wall_1[i] = t_mit + del_t * sin(M_PI * (i / static_cast<double>(hor_mesh)));
        wall_3[i] = 300;
    }

    double *wall_2=new double [ver_mesh];                   /*declaring array value type in double*/
    double *wall_4=new double [ver_mesh];                   /*declaring array value type in double*/
    
    for (int j = 0; j <= ver_mesh; ++j) {
        wall_2[j]=300;
        wall_4[j]=300;
    }

    Matrix(rows, cols, wall_1, wall_2, wall_3, wall_4, T);
    
    /*------print the initial matrix without temperature of internal nodes------*/
    cout << "Initial Matrix:" << endl;
    printMatrix(rows, cols, T, myfile, "Inital Matrix");

    gaussSeidel(rows, cols, T, 0.0001);
    
    /*------print matrix with temperature of internal nodes------*/
    cout << "\n#Optimized Matrix:" << endl;
    printMatrix(rows, cols, T, myfile, "Optimized Matrix:");

    /*------print matrix with temperature of internal nodes------*/
    double len = 1000.0;
    double dx = len/hor_mesh;
    double dy = len/ver_mesh;    

    /*------print matrix for vector components------*/
    cout << "Vectors for T matrix components:" << endl;
    for (int i = 0; i < rows-1 ; ++i) {
        for (int j = 0; j < cols-1 ; j++) {
            double x1 = i * dx;                                                                 // x-coordinate of the starting point
            double y1 = j * dy;                                                                 // y-coordinate of the starting point
            double dx1 = T[i + 1][j] - T[i][j];                                                 // Change in x
            double dy1 = T[i][j + 1] - T[i][j];                                                 // Change in y
            cout << "Component (" << i << ", " << j << "): " << dx1 << "\t" << dy1 << endl;
            myfile << i << setw(13) << j << setw(13) << dx1 << setw(13) << setw(13) << dy1 << endl;
        }
    }

    myfile.close();

    return 0;
}
