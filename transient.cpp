#include <iostream>
#include <cmath>
#include <fstream>
#include <limits>
#include <iomanip>
using namespace std;

/* ---------print the temperature distribution----------- */ 
void printTemperature(double temperatures[], int division) {
    for (int i = 0; i < division; ++i) {
        cout << temperatures[i] << " ";
    }
    cout << endl;
}

void writeToFile(ofstream& file, const string& title, int t, double temperatures[], int division, double dx) {
    file << " #" << title << " - " << t * 0.1 << " mins" << "\n";
    file << setw(6) << " #i " << setw(13) << " x " << setw(13) << " T " << "\n";
    file << setw(6) << " #- " << setw(13) << " mm " << setw(13) << " C " << "\n";
    file << "#=======================================\n"; 

    for (int i = 0; i < division; ++i) {
        file << setw(5) << i << setw(11) << i * dx << setw(15) << temperatures[i] << "\n "; 
    }

    file << endl;
}

int main() {
    
    /*-------Given input variables ---------*/ 
    const double length = 0.1;                              // Length of the rod
    const int division = 11;                                // Number of points
    const double dx = length / (division - 1);              // Spatial step 
    double dt = 0.1;                                       // Time step 
    const double totalTime = 86400.0;                        // Total simulation time (seconds) Note: chane the value to the time second value when time needed
    const int TimeSteps = static_cast<int>(totalTime / dt);
    double ther_Cond = 0.5;                                 // Thermal conductivity of rod
    const double dens = 1000;                               // density
    const double sp_heat = 1000;                            //specific heat capacity
    const double alpha = ther_Cond/(dens*sp_heat);          //Thermal diffusivity
    const double ambientTemp = 100.0;
    const double initialTemperature = 20.0;                 // Initial temperature of the bar (degrees Celsius)
    const double h = 50.0;                                  // heat transfer coefficient
    int mins = (totalTime/60) + 1;
    int j = -1;
    int i;
    double tsave[division][mins];                           // save values in minutes

    /* -------Name the file name according to the output file required-------- */ 
    ofstream myfile1;
    myfile1.open ("temp_at_min.dat");

    ofstream myfile2;
    myfile2.open ("transient_180s.dat");

    ofstream myfile3;
    myfile3.open ("transient_720s.dat");

    ofstream myfile4;
    myfile4.open ("transient_2880s.dat");

    ofstream myfile5;
    myfile5.open ("transient_86400s.dat");

    ofstream myfile6;
    myfile6.open ("transient_comparision.dat");

    ofstream myfile7;
    myfile7.open ("temp_in_row.dat");

    // Initial conditions
    double temperatures[division] = {0.0};
    for (int i = 1; i <= division; ++i) {
        temperatures[i] = initialTemperature;               // Initial temperature of the rod
    }

    /* ----------FTCS time-step loop-------------- */ 
    for (int t = 0; t <= TimeSteps; ++t) {
        double newTemperatures[division];
        const double Fo = (alpha*dt)/(dx*dx);
        double tolerance = 0.01;
        double diff;
        do {
            diff = 0.0;
                for (int i = 0; i < division; ++i){
                    double oldValue = newTemperatures[i];
                    if (i == 0) {
                        newTemperatures[i] = initialTemperature;                                                                                               // constant initial wall condition
                    } else if ( i == 1){
                        newTemperatures[i] = ((1-2*Fo)*temperatures[i]) + (Fo * (2*temperatures[i - 1])) ;                                                     // adiabatic wall condition   
                    } else if (i == division-1) {
                        newTemperatures[i] = ((1-2*Fo)*temperatures[i]) + (Fo * (2*temperatures[i - 1] + (h*2*dx*(ambientTemp - temperatures[i])/ther_Cond))); // conduction convection wall condition
                    } else {
                        newTemperatures[i] = ((1-2*Fo)*temperatures[i]) + (Fo * (temperatures[i + 1] + temperatures[i - 1]));                                  // FCTS Scheme on rest of points
                    }
                    diff = max(diff, abs(newTemperatures[i] - oldValue));                                                                                      // Convergence criterion
                }
             }
        while (diff > tolerance); 

        /* ----------Looping over time to add newtemperature value to temperature[i]------------- */
        for (int i = 0; i < division; ++i) {                                                                                                                   
            temperatures[i] = newTemperatures[i];                                                                                                                
        }


        /* ----------Printing for different time step values------------- */
        if (t % 600 == 0) {
            /* ----------to save the value for every minute------------- */
            j = j+1;
                myfile1 << " # temperatures in minute" << " - " << t * dt << " mins" << "\n";
                for (i = 0; i < division; ++i ){
                    tsave[i][j]=temperatures[i];
                    //cout << tsave[i][j] << "\n";
                    myfile1 << setw(6) << i << " " << setw(13)<< tsave[i][j] << " " << "\n";
                }
                /* ----------Printing .dat files for plotting for defined time on totaltime on one graph------------- */
                //cout << "Time: " << t * dt << "s - Temperature: ";
                printTemperature(temperatures, division);
                
                if(t == 1800){
                    writeToFile(myfile2, "transient_180s", t, temperatures, division, dx);        

                /* ----------Printing for required time step values in rows------------- */
                    myfile7 << setw(12) << "x in mm:";
                    for (int i = 0; i < division; ++i) {
                    //cout << " " << i << "\n";
                    //cout << temperatures[i];
                    myfile7 << setw(8) << i;
                    }
                    myfile7 << "\n";

                    myfile7 << setw(12) << "T in C:";
                    for (int i = 0; i < division; ++i) {
                        myfile7 << setw(8) << temperatures[i];
                    }
                myfile7 << "\n";
                }

                if(t == 7200){
                    writeToFile(myfile3, "transient_720s", t, temperatures, division, dx);
                }

                if(t == 28800){
                    writeToFile(myfile4, "transient_2880s", t, temperatures, division, dx);
                }

                if(t == 864000){
                    writeToFile(myfile5, "transient_86400s", t, temperatures, division, dx);
                }          
        }
    }
    
    /* ----------Printing .dat files for plotting for 180s to 86400s on one graph------------- */
    myfile6 << setw(6) << " #i " << setw(13)<< " 180s " << setw(13) << " 720s " << setw(13) << "2880s" << setw(13) << "86400s" << "\n";          
    myfile6 << "#================================================================\n";  
    for (int i = 0; i < division; ++i){
        myfile6  << i*dx << setw(13) <<  tsave[i][3]<< setw(13) << tsave[i][12] << setw(13) << tsave[i][48] << setw(13) << tsave[i][1440] << "\n ";
    }
    myfile6 << endl;
   
    myfile1.close();
    myfile2.close();
    myfile3.close();
    myfile4.close();
    myfile5.close();
    myfile6.close();
    myfile7.close();

    return 0;
}