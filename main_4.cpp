//
//  main.cpp
//  Extractdata
//
//  Created by Jorge Buenestado Dominguez && Daniel Ariza Quintana on 20/01/2023.
//  Copyright © 2023 Jorge Buenestado Dominguez. All rights reserved.
//
//  Note: in order to use this program you need to be using the developer version of the SEVN program or at least a version that includes a DTOUT="eventsrlo"

#include <iostream>
#include <cstring>
#include <fstream>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <sstream>

#define MAX 1000

using namespace std;

void CSV(string arch);

int main(int argc, const char * argv[])
{
    long double A[MAX][24];
    int i=0, j=0, n=0, C=24;
    long int ID, prim=0;
    int num, k, lim, cont = 0, V1, V2, V3, V4, V5, D1, D2, IDv, total,suma = 0;
    double c1, c2, er1, er2, cumul;
    string FILE="./datos2.txt";
    string start="g", end=".dat";
    ifstream fich;
    ofstream print, pr, pri, pr2, pr3, pr4;
    bool timestep;

    //Abrimos el fichero con los datos, comentar si cambias el input
    fich.open("./datos2101_10.txt");
    if(!fich.is_open())
    {
        CSV("./RLOBAr.csv");fich.open("./datos2.txt");
    };
    if(fich.is_open()) cout<<"File opened successfuly."; else cout<<"Error reading file.";

    //we read the first line to get the ID.
    for(j=0;j<C;j++)fich>>A[i][j];ID=A[0][0];fich.close();

    //Obtain RLO number
    fich.open("num.txt");fich>>num;fich.close();
       fich.open("./datos2101_10.txt"); 

    //Output files
    print.open("RLOBArdata.csv");if(!print.is_open()) cout<<"Error printing RLO";
    pr<<"#X     Y       ErX         ErY"<<endl;


    double B[3001][5];
    //Initialize to 0
    for(i=1;i<num;i++){ for(j=0;j<7;j++)B[i][j] = 0;}

    pr.open("grat2.dat");


    // ts = 20; Semimajor =  21; Mdot = 22; Accrate = 23, Mass of star 3 and 10,  Radius 4 and 11
    //Phases V1 = 7, V2 = 14, Mass v1 = 3, v2 = 10
    V1 = 10; V2 = 11; V3 = 10; V4 = 4; V5 = 11;

    //if you want to do with a timestep
    timestep = false;

    if(timestep)
    {
        while(cont<num)
        {
            i=0;j=0;k=0;
            cumul = 0;
            while(ID==A[i][0])
            {   i++;
                for(j=0;j<C;j++)fich>>A[i][j];
                if(k!=0){ cumul = cumul + A[i][V1];}
                if(A[i][V2]!=0)
                {   
                    B[i][1] = cumul; B[i][2] = A[i][V2]; B[i][3] = A[i][V3];
                    k++;
                }
                if(A[i][0]!=ID){cont = cont + 1;}
            }
            n=i;//How many times the ID is repeated
            ID=A[n][0];
            for(j=0;j<C;j++)A[0][j]=A[n][j];
            pri.open("g"+to_string(cont)+".dat");
            for(j=0;j<n;j++)
            {
                if(B[j][1]!=0){pri<<scientific<<setprecision(6)<<B[j][1]*1000000<<"     "<<B[j][2]/1000000<<"     "<<B[j][3]<<endl;}
            }
            pri.close();
        }
    }
    else{
    while(cont<num)
    {   
        i=0;j=0;
        B[cont][0] = ID;
        while(ID==A[i][0])
        {   i++;
            for(j=0;j<C;j++)fich>>A[i][j];

            if(A[i][V1]!=0){pr<<scientific<<setprecision(6)<<A[i][V1]<<"     "<<A[i][V2]<<endl;}
            B[i][1] = A[i][V1];
            B[i][2] = A[i][V2];
            B[i][3] = A[i][V3];
            if(A[i][0]!=ID){cont = cont + 1;}
        }

        n=i;         
        ID=A[n][0]; //How many times the ID is repeated
        for(j=0;j<C;j++)A[0][j]=A[n][j];

        for(j=0;j<n;j++)
        {
            //if(B[j][1]!=0){pri<<scientific<<setprecision(6)<<B[j][1]/1000000<<"     "<<B[j][2]<<"     "<<B[j][3]<<endl;}
        }
    }}

    //Output
    for(i=0;i<num;i++){for(j=0;j<5 ;j++)print<<B[i][j]<<",";print<<endl;}
    print<<endl<<"Total steps:,"<<num<<endl<<"Total count:,"<<cont<<endl<<"Fin del Archivo"<<endl;
    pr.close();
    print.close();
    fich.close();
    cout<<"Programa finalizado"<<endl;
    return 0;
}

//To read the CSV file.
void CSV(string arch)
{
    ifstream archive;
    ofstream print;
    string line;
    char separator = ',';
    int i, C;
    //Set number of columns
    C = 24;
    archive.open(arch);
    print.open("datos2.txt");
    // We get the 1st line to descart it
    getline(archive, line);
    // Get all lines
    i=0;
    while (getline(archive, line))
    {
        stringstream stream(line); // Convert string to stream
        string x[MAX];
        // Extract all line values
        for(i=0;i<C;i++)
        {
            getline(stream, x[i], separator);
            print<<x[i]<<"  ";
        }
        print<<endl;
    }
    archive.close();
    print.close(); 
    return;
}
