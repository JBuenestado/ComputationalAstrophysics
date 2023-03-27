//
//  main.cpp
//  SortRLO
//
//  Created by Daniel Ariza Quintana on 15/01/2023.
//  Adjustments by Jorge Buenestado Dominguez
//  Copyright © 2023 Daniel Ariza Quintana. All rights reserved.
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
    
    double A[MAX][24], RLOB=0;
    int i=0, j=0, n=0, C, rst, rend, r0, r1, be, mr0, mr1, ts, k;
    long int ID, prim=0;
    string FILE="./datos.txt";
    ifstream fich;
    ofstream print;
    bool XRayB;
    
    /*      Initial parameters    
    Set the parameters for the columns output, only change if different, currently : 
        SCOL="RemnantType:Mass:Radius:Luminosity:Temperature:Phase:dMcumul_RLO"
        BCOL="RL0:RL1:Period:BEvent:BTimestep:BWorldtime" */
    //Set the number of colums of the table. You need to change this number on the void function at the end of the program too
    C=22;
    //set the remnant types columns, ID colum is nº = 0
    r0 = 2; r1 = 9;
    //set the BEvents  columns, ID colum is nº = 0
    be = 19;
    //set the Cumulated  Mass RLO columns ID colum is nº = 0 (dMcumul_RLO_X)
    mr0 = 8; mr1 = 15;
    //set the timestep  columns, ID colum is nº = 0
    ts = 20; 

    //We open our file and read the first imput, which should be the Id of the first line. IMPORTANT TO DELETE  HEADERS AND CHANGE THE COMAS TO DOTS.
    //If not, you can directly input the data with a "output_0.csv" or changing the name archive at line 57 to .csv header
    //fich.open("./datos.txt");//cambiar cada vez que cambies el input

    //We convert CSV into a new file in case we don't have it in txt;
    if(!fich.is_open())
    {   
        CSV("./output_0.csv");
        fich.open("./datos.txt");
    };
    //if(!fich.is_open())fich.open("./datos.txt");
    if(fich.is_open()) cout<<"File opened successfuly."; else cout<<"Error reading file.";
    //for(j=0;j<C;j++)
    fich>>A[0][0];ID=A[0][0];
    //fich>>ID;//we read the first line to get the ID.
    fich.close();
    n=0;

    //This reader will work with the first line of the matrix already filled. 
    fich.open("./datos.txt");if(!fich.is_open()) cout<<"Error reading file.";if(fich.is_open()) cout<<"File opened successfuly.";
    print.open("RLOBAr.csv");if(!print.is_open()) cout<<"Error printing RLO";
    print<<"ID,name,RemnantType_0,Mass_0,Radius_0,Luminosity_0,Temperature_0,Phase_0,dMcumul_RLO_0,RemnantType_1,Mass_1,Radius_1,Luminosity_1,Temperature_1,Phase_1,dMcumul_RLO_1,RL0,RL1,Period,BEvent,BTimestep,Semimajor,Mdot,AccRate"<<endl;
    
    while (!fich.eof())
    {
        i=0;j=0;
        while(ID==A[i][0])
        {
            i++;
            for(j=0;j<C;j++)fich>>A[i][j];
        }
        n=i;//we set the size of the matrix, it's n-1 since the last line is from the next Id.
        
        //We sort and check if it's an X-ray Binary and satisfies our criteria. 
        XRayB=false;
        i=0;
        
        while((!XRayB)&&(i<n))
        {
            //if((((A[i][r0]==4)||(A[i][r0]==5)||(A[i][r0]==6))&&(A[i][r1]==0))||(((A[i][r1]==4)||(A[i][r1]==5)||(A[i][r1]==6))&&(A[i][r0]==0))) 
            if((((A[i][r0]==4)||(A[i][r0]==5)||(A[i][r0]==6))&&(A[i][r1]==0)))
            {
                XRayB=true;
                prim=i;
            }
            else if(((A[i][r1]==4)||(A[i][r1]==5)||(A[i][r1]==6))&&(A[i][r0]==0))
            {
                XRayB=true;
                prim=i;
            }
            i++;
        }
        if(XRayB)
        {
            //Now we check to see if the star undergoes the whole process of RLO or no//We initialize the variables
            rend=-1;rst=-1;
            for(i=0;i<n;i++)
            {
                if(A[i][be]==4)rst=i;
                if(A[i][be]==5)rend=i;
            }    
                //When we know the star undergoes RLO we print the data on another file.
                //In order to have the acretion rate calculated and all the data we need 3 for cicles.
            if((rst<rend)&&(rst>prim))
            { 
                    for(i=0;i<rst;i++)
                    {
                        for(j=0;j<C;j++)print<<A[i][j]<<",";
                        print<<"0,0"<<endl;
                    }     
                    if(A[rst][r0]!=0)
                    {
                        for(i=rst;i<rend;i++)
                        {
                            for(j=0;j<C;j++)print<<A[i][j]<<",";
                            print<<(A[i][mr0]-A[i-1][mr0])<<","; //Print Mdot
                            print<<(A[i][mr0]-A[i-1][mr0])/A[i][ts]<<","<<endl; //Print Acc 
                        }
                    }
                else{
                    for(i=rst;i<rend;i++)
                    {
                        for(j=0;j<C;j++)print<<A[i][j]<<",";
                        print<<(A[i][mr1]-A[i-1][mr1])<<","; //Print Mdot
                        print<<(A[i][mr1]-A[i-1][mr1])/A[i][ts]<<","<<endl; //Print acc
                    }
                }                                
                for(i=rend;i<n;i++)
                {
                    for(j=0;j<C;j++)print<<A[i][j]<<",";
                    print<<"0,0"<<endl;
                }
                RLOB++;
            }
        }
        
        //Now we need to prepair to read the next ID chunk. We define the new ID and the new first row.
        ID=A[n][0];
        for(j=0;j<C;j++)A[0][j]=A[n][j];   
    }
    print<<endl<<"There are RLOB="<<RLOB<<endl;
    print.close();
    print.open("num.txt");
    print<<RLOB<<endl;
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
    C = 22;
    archive.open(arch);
    print.open("datos.txt");
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
