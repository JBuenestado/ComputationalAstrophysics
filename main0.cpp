//
//  main.cpp
//  SortRLO
//
//  Created by Daniel Ariza Quintana on 15/01/2023.
//  Copyright © 2023 Daniel Ariza Quintana. All rights reserved.
//

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
    
    double A[MAX][24], CE[MAX], RLOB=0;
    int i=0, j=0, n=0, ce=0, C,cuatro, cinco;
    long int ID, prim=0, ult=0;
    string FILE="./datos.txt";
    ifstream fich;
    ofstream print;
    bool   siete, XRayB;
    
    //Set the number of colums of the table
    C=22;
           
    //Now we open our file and read the first imput, which should be the Id of the first line. IMPORTANT TO DELETE DE HEADERS AND CHANGE THE COMAS TO DOTS.
    //fich.open("./datos.txt");

    //We convert CSV into a new file in case we don't have it in txt;
    if(!fich.is_open()) CSV("./output_0.csv");
    
    if(!fich.is_open()) cout<<"Error al leer fichero.";
    if(fich.is_open()) cout<<"Fichero abierto con éxito.";
    for(j=0;j<C;j++)fich>>A[i][j];ID=A[0][0];//we read the first line to get the ID.
    fich.close();
    n=0;
    //This reader will work with the first line of the matrix already filled.
    
    fich.open("./datos.txt");if(!fich.is_open()) cout<<"Error al leer fichero.";if(fich.is_open()) cout<<"Fichero abierto con éxito.";
    
    print.open("RLOBAr.csv");if(!print.is_open()) cout<<"Error al escribir el RLO";
    
    while (!fich.eof())
    {
        i=0;j=0;ce=0;
    
        while(ID==A[i][0])
        {
            i++;
            for(j=0;j<C;j++)fich>>A[i][j];
        }
        n=i;//we set the size of the matrix, it's n-1 since the last line is from the next Id.
        
        //We sort and check if it's an X-ray Binary and satisfies our criteria. We start assuming it's not.
        XRayB=false;
        i=0;
        
        while((!XRayB)&&(i<n))
        {
            if((A[i][2]==4)||(A[i][2]==5)||(A[i][2]==6))
            {
                if(A[i][9]==0)
                {
                    XRayB=true;
                    prim=i;
                }
                else XRayB=false;
            }
            else if((A[i][9]==4)||(A[i][9]==5)||(A[i][9]==6))
            {
                if(A[i][2]==0)
                {
                    XRayB=true;
                    prim=i;
                }
                else XRayB=false;
            }
            i++;
        }
        if(XRayB)
        {
                //Now we check to see if the star undergoes the hole process of RLO or CE or non.
                cinco=-2;cuatro=-1;siete=false;
                for(i=0;i<n;i++)
                {
                    int temp = A[i][C-3];

                    if(temp==4)cuatro=i;
                    if(temp==5)cinco=i;
                    if(temp==7)siete=true;
                }
                if(siete)CE[ce]=ID;
                
                //When we know the star undergoes RLO we print the data on another file.//In order to have the acretion rate calculated and all the data we need 3 for cicles.
                if((cuatro<cinco)&&(cuatro>prim))
                {
                    
                    for(i=0;i<cuatro;i++)
                    {
                        for(j=0;j<C;j++)print<<A[i][j]<<",";
                        print<<endl;
                    }
                    
                    if(A[cuatro][2]!=0){
                        for(i=cuatro;i<cinco;i++)
                        {
                            for(j=0;j<C;j++)print<<A[i][j]<<",";
                            print<<(A[i][8]-A[i-1][8])/A[i][20]<<","<<endl;
                        }
                    }
                    else{
                        for(i=cuatro;i<cinco;i++)
                        {
                            for(j=0;j<C;j++)print<<A[i][j]<<",";
                            print<<(A[i][15]-A[i-1][15])/A[i][20]<<","<<endl;
                        }
                    }
                    
                    for(i=cinco;i<n;i++)
                    {
                        for(j=0;j<C;j++)print<<A[i][j]<<",";
                        print<<endl;
                    }
                    RLOB++;
        
                }
        }
        
        //Now we need to prepair to read the next ID chunk. We define the new ID and the new first row.
        ID=A[n][0];
        for(j=0;j<C;j++)A[0][j]=A[n][j];
        
    }
    print<<endl<<"There are RLOB="<<RLOB<<endl;
    print.close();fich.close();
    cout<<"Programa finalizado"<<endl;
    
    return 0;
}

//We read the CSV file.
void CSV(string arch)
{
    ifstream archivo;
    ofstream print;
    string linea;
    char delimitador = ',';
    int i;
    
    archivo.open(arch);
    print.open("datos.txt");
    // we discard first line
    getline(archivo, linea);
    // we read the rest of lines
    i=0;
    while (getline(archivo, linea))
    {
        
       
        stringstream stream(linea); // String to stream
        string x[MAX];
        // Extract all values
        for(i=0;i<22;i++)
        {
            getline(stream, x[i], delimitador);
            print<<x[i]<<"  ";
        }
        print<<endl;

    }
    archivo.close();
    print.close();
    
    return;
}
