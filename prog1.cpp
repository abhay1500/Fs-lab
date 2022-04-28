#include<iostream>
#include<stdlib.h>
#include<string>
#include<fstream>
#include<conio.h>

using namespace std;
int main(){
    system("clear");
    int ch,n;
    string names,rev;
    char infile[30],outfile[30];
    fstream fpin,fpout;

    for(;;){
        cout<<"\n1.Std Input to std Output\n2.File to std Output.\n";
        cout<<"Enter your choice.\n";
        cin>> ch;

        switch(ch){
            case 1: cout<<"Enter the number of names:\n";
                    cin>>n;
                    for(int i=0;i<n;i++){
                        cout<<"\nEnter names here:";
                        cin>>names;
                        rev.erase();
                        for(int j=names.length()-1;j>=0;j--){
                            rev+=names[j];
                        }
                        cout<<"\nReveresed:  "<<rev;
                    }
            /*case 2: cout<<"Eter the Input filename\n";
                    cin>>infile;
                    fpin.open(infile,ios::in);
                    while(!fpin){
                        fpin>>rev;

                        cout<<rev;
                        cout<<"\n";


                    }*/
            case 2: cout<<"Enter the Input filename\n";
                    cin>>infile;
                    cout<<"Enter the OUtput filename\n";
                    cin>>outfile;
                    fpin.open(infile,ios::in);
                    fpout.open(outfile,ios::out);
                    if(!fpin || !fpout)
                    {
                        cout<<"Fatal error! Unable to open the files.";
                        exit(0);
                    }
                    while(fpin){
                        getline(fpin,names);
                        rev.erase();
                        for(int j=names.length()-1;j>=0;j--){
                            rev+=names[j];
                        }
                        fpout<<rev<<endl;
                    }
                    fpin.close();
                    fpout.close();
        }


    }
    return 0;
}

