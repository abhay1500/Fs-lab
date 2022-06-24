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
 char usn[15];
	 	 char name[15];
		 char branch[5];
  public: 
 void pack(char[]);
		 void unpack(char[]);
		 void display();
		 void insrec();
		 void delrec();
		 int search(char[]);
};

struct memindex
{
    char key[15];
    int offset;
};

memindex index[10];

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
     char *usn;
     int i,j,pos;
     fp.open(fname,ios::out|ios::ate);
     pack(buffer);
     strcpy(tem,buffer);
     usn=strtok(tem,"|");
     pos=search(usn);
     if(pos!=0)
     {
         cout<<"USN already Exists \n"<<endl;
         fp.close();
         return;
     }
     recount++;
     strcpy(index[recount].key,usn);
     index[recount].offset=fp.tellg();
     fp<<buffer;
     fp<<"\n";
     memindex temp;

     for(i=1;i<recount;i++)
     {
         for(j=1;j<recount;j++)
         {
            if(strcmp(index[j].key,index[j+1].key)>0)
            {
	              strcpy(temp.key,index[j].key);
	              temp.offset=index[j].offset;

	              strcpy(index[j].key,index[j+1].key);
	              index[j].offset=index[j+1].offset;

	              strcpy(index[j+1].key,temp.key);
	              index[j+1].offset=temp.offset;
            }
         }
     }
    fp.close();
}

void student::display()
{
     char buffer[50];
     int i,j;
     if(recount == 0)
     {
        cout<<"No Records to Display"<<endl;
        return;
     }
     cout<<"\t---------------"<<endl;
     cout<<"\t Primary index"<<endl;
     cout<<"\t---------------"<<endl;
     cout<<"\t key\t"<<"offset"<<endl;
     cout<<"\t---------------"<<endl;
     for(i=1;i<=recount;i++)
     {
         cout<<"\t"<<index[i].key<<"\t"<<index[i].offset<<endl;
     }
     cout<<"File Contents are"<<endl;
     fp.open(fname,ios::in);
     for(i=1;i<=recount;i++)
     {
        fp.seekg(index[i].offset,ios::beg);
        fp>>buffer;
        unpack(buffer);
        cout<<endl;
     }
     fp.close();
}

void student::delrec()
{
     char usn[15];
     int pos;
     fp.open(fname,ios::out|ios::ate);
     cout<<"\nEnter the USN:";
     cin>>usn;
     pos=search(usn);
     if(pos==0)
     {
         cout<<"\nRecord not found";
         return;
     }
     fp.seekg(index[pos].offset,ios::beg);
     fp<<"*";
     fp.close();
     for(int i=pos;i<recount;i++)
     {
         strcpy(index[i].key,index[i+1].key);
         index[i].offset=index[i+1].offset;
     }
     cout<<"Record Deleted Successfully"<<endl;
     recount--;
}

int student::search(char usn[])
{
     int low=1,high=recount,mid;
     while(low<=high)
     {
         mid=(low+high)/2;
         if(strcmp(index[mid].key,usn)==0)
	          return mid;
         if(strcmp(index[mid].key,usn)>0)
	          high=mid-1;
         else
	          low=mid+1;
     }
     return 0;
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

void main()
{
     char buffer[50];
     student s1;
     char usn[15];
     int i,j,k,choice,pos;
     clrscr();
     cout<<"Enter the file name: ";
     cin>>fname;
     fp.open(fname,ios::out|ios::trunc);
     fp.close();
     for(;;)
     { 
         cout<<endl;
         cout<<" 1.Insert\n 2.Delete\n 3.Display\n 4.Search\n 5.Exit\n";
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
cout<<"\nEnter the USN to be searched:";
		            cin>>usn;
		            pos=s1.search(usn);
		            if(pos==0)
		            {
	                            cout<<"Record not found..\n";
	                            break;
	                        }
	                        fp.open(fname,ios::in);
                           fp.seekg(index[pos].offset,ios::beg);
	                        fp>>buffer;
	                        s1.unpack(buffer);
	                        fp.close();
	                        break;
         default: 
exit(0);
      }
   }
}
