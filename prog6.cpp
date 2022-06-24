#include<iostream.h>
#include<process.h>
#include<conio.h>
#include<fstream.h>
#include<string.h>
#include<stdio.h>

fstream fp;
int recount=0;
char fname[10];

class student
{
   private:
     char name[15];
 	                 char usn[15];
	                 char branch[5];
   public: 
    		     void pack(char[]);
	                 void unpack(char[]);
	                 void display();
	                 void insrec();
	                 void delrec();
	                 int search(char[]);
	                 void sch_scn_index();
};

struct primaryindex
{
     char pkey[15];
     int  offset;
};

struct secondaryindex
{
     char skey[15];
     char pkey[15];
};

student s1;
primaryindex pindex[20];
secondaryindex sindex[20];


void student ::pack(char buffer[])
{
     cout<<"Enter the student USN, Name and Branch:";
     cin>>usn>>name>>branch;
     strcpy(buffer,usn);
     strcat(buffer,"|");
     strcat(buffer,name);
     strcat(buffer,"|");
     strcat(buffer,branch);
}

void student::insrec()
{
     char buffer[50],tem[20];
     char *usn,*name;
     int i,j,pos;
     fp.open(fname,ios::out|ios::ate);
     pack(buffer);
     strcpy(tem,buffer);
     usn=strtok(tem,"|");
     name=strtok(NULL,"|");
     pos=search(usn);
     if(pos!=0)
     {
         cout<<"USN already present\n";
         fp.close();
         return;
     }
     recount++;
     strcpy(pindex[recount].pkey,usn);
     pindex[recount].offset=fp.tellg();

     strcpy(sindex[recount].skey,name);
     strcpy(sindex[recount].pkey,usn);

     fp<<buffer;
     fp<<"\n";
     primaryindex temp;

     for(i=1;i<recount;i++)
     {
        for(j=1;j<recount;j++)
        {
           if(strcmp(pindex[j].pkey,pindex[j+1].pkey)>0)
           {
	            strcpy(temp.pkey,pindex[j].pkey);
	            temp.offset=pindex[j].offset;
	           strcpy(pindex[j].pkey,pindex[j+1].pkey);
	           pindex[j].offset=pindex[j+1].offset;

	           strcpy(pindex[j+1].pkey,temp.pkey);
	           pindex[j+1].offset=temp.offset;
           }
        }
     }
     fp.close();
}

void student::display()
{
     char buffer[50];
     int i,j;
     fp.open(fname,ios::in);
     if(recount == 0)
     {
         cout<<"No Records to Display"<<endl;
         fp.close();
         return;
     }
     cout<<"\t---------------"<<endl;
     cout<<"\t Primary index"<<endl;
     cout<<"\t---------------"<<endl;
     cout<<"\t key  "<<"offset"<<endl;
     cout<<"\t---------------"<<endl;
     for(i=1;i<=recount;i++)
     {
         cout<<"\t"<<pindex[i].pkey<<"\t"<<pindex[i].offset<<endl;
     }

     cout<<"\t----------------------"<<endl;
     cout<<"\t    Secondary index"<<endl;
     cout<<"\t----------------------"<<endl;
     cout<<"\t sec-key\t"<<"pri-key"<<endl;
     cout<<"\t-----------------------"<<endl;
     for(i=1;i<=recount;i++)
     {
         cout<<"\t"<<sindex[i].skey<<"\t\t"<<sindex[i].pkey<<endl;
     }
     cout<<"\t------------------------"<<endl;
     cout<<"File Contents are"<<endl;
     for(i=1;i<=recount;i++)
     {
         fp.seekg(pindex[i].offset,ios::beg);
         fp>>buffer;
         unpack(buffer);
     }
     fp.close();
}

void student::delrec()
{
     char usn[15];
     int pos,j;
     cout<<"\nEnter the usn:";
     cin>>usn;
     fp.open(fname,ios::out|ios::ate);
     pos=search(usn);
     if(pos==0)
     {
         cout<<"\n Record not found";
         fp.close();
         return;
     }
     fp.seekg(pindex[pos].offset,ios::beg);
     fp<<"*";
     fp.close();
     for(int i=pos;i<recount;i++)
     {
         strcpy(pindex[i].pkey,pindex[i+1].pkey);
         pindex[i].offset=pindex[i+1].offset;
     }
     j=1;
     while(strcmp(sindex[j].pkey,usn)!=0)
     {
         j++;
     }
     for(i=j;i<recount;i++)
     {
         strcpy(sindex[i].skey,sindex[i+1].skey);
         strcpy(sindex[i].pkey,sindex[i+1].pkey);
     }
     cout<<"Record deleted successfully"<<endl;
     recount--;
}

void student::sch_scn_index()
{
     char buffer[50];
     char name[15];
     int i,j;
     int pos,flag=0;
     fp.open(fname,ios::in);
     cout<<"Enter the secondary key(name) \n";
     cin>>name;
     for(i=1;i<=recount;i++)
     {
        if(strcmp(sindex[i].skey,name)==0)
        {
            flag=1;
            pos=search(sindex[i].pkey);
            fp.seekg(pindex[pos].offset,ios::beg);
            fp>>buffer;
            unpack(buffer);
        }
     }
     if(flag==0)
     {
         cout<<"Record not found";
     }
     fp.close();
}

void student::unpack(char buffer[])
{
     char *t;
     t = strtok(buffer,"|");
     cout<<"USN : "<<t<<endl;
     t = strtok(NULL,"|");
     cout<<"Name : "<<t<<endl;
     t = strtok(NULL,'\0');
     cout<<"Branch :"<<t<<endl;
}

int student::search(char usn[])
{
     int low=1,high=recount,mid;
     while(low<=high)
     {
         mid=(low+high)/2;
         if(strcmp(pindex[mid].pkey,usn)==0)
              return mid;
         if(strcmp(pindex[mid].pkey,usn)>0)
              high=mid-1;
         else
              low=mid+1;
     }
     return 0;
}
void main()
{
     char buffer[50];
     char usn[15];
     int i,j,k,choice,pos;
     clrscr();
     cout<<"Enter the file name: ";
     cin>>fname;
     fp.open(fname,ios::out|ios::trunc);
     fp.close();
     for(;;)
     {
          cout<<"\n1.Insert\n2.Delete\n3.Display\n";
          cout<<"4.Search using primary key\n";
          cout<<"5.Search using secondary key\n6.Exit\n";
          cout<<"Enter your choice\n";
          cin>>choice;
          switch(choice)
          {
              case 1: 
s1.insrec();
		 	break;
 	           case 2: 
s1.delrec();
	       		break;
             case 3: 
s1.display();
	       		break;
             case 4: 
cout<<"\nEnter the usn to be searched:";
		            cin>>usn;
	       		pos=s1.search(usn);
	       		if(pos==0)
	       		{
		 	     cout<<"Record not found\n";
		                 break;
	       		}
	      		fp.open(fname,ios::in);
	 	  	fp.seekg(pindex[pos].offset,ios::beg);
	                        fp>>buffer;
	                        s1.unpack(buffer);
	                        fp.close();
	                        break;
             case 5: 
s1.sch_scn_index();
	       		break;

            default:
	exit(0);
        }
    }
}
