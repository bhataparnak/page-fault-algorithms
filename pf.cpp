
/***************************************************************************************************************
Student Name:- Aparna Krishna Bhat
Replacement Assignment:- This Assignment has been coded using C++.
To compile this file use command g++ pf.cpp -o pf.o
To run this file ./pf.o <filename>
eg input [3 1 2 3 2 3 1 4 2 5 4 1 1 2 2 2 3] where 3 denotes the size of the working set and
The rest of the line denotings the pages requested.
Description:- C++ program that implements the FIFO, Optimal, MFU, and LRU page-replacement
algorithms. Given a page-reference string, where page numbers range from 0 to 9,
apply the page-reference string to each algorithm, and output the number of page faults
incurred by each algorithm.

***************************************************************************************************************/

#include<stdio.h>
#include<iostream>
#include<fstream>
#include<vector>
#include<string.h>
#include<map>
#include<algorithm>

using namespace std;

int full = 0;
int fullOptimal = 0; //To check whether all frames are filled for optimal method
static int f;
static int fOptimal; //for optimal method
int repptrOptimal ; //for optimal method
int repptr;

int Longestopt_Optimal(vector<int> a , int frame[] , int SIZE , int n) {
  int *temp = new int[SIZE];
  int c = 0; //Counter to break the loop once we get two nearest future pages
  int id, i, k, j = SIZE;
  id = 0;
  for (i = fOptimal + 1; i < n; i++) //Checking from the current time of use till the end of string for future references
  {
    for (k = 0; k < j; k++) //Checking whether a page occurs in future or not
    {
      if (a[i] == frame[k]) {
        if (temp[k] != 1) {
          temp[k] = 1;
          c++;
        }

        break;
      }
    }
    if (c == 2)
      break; //Once we get two future pages then we may break
  }
  id = 2;
  while (id != 0) {
    if (temp[id] == 0) //Checking for the page which is not the nearest future reference
      break;
    id--;
  }
  repptrOptimal = id;
  return repptrOptimal; //Returning the replacement pointer with the value of victim page
}

int Pagereplace_Optimal(vector<int> a ,int ele,int frame[] , int SIZE , int n) {
  int temp;
  repptrOptimal = Longestopt_Optimal(a,frame , SIZE , n);
  temp = frame[repptrOptimal];
  frame[repptrOptimal] = ele;
  return temp;
}

void PagefaultOptimal(vector<int> a , int ele , int frame[] , int SIZE , int n) {
  if (fullOptimal != SIZE)
    frame[fullOptimal++] = ele;
  else
     Pagereplace_Optimal(a,ele,frame,SIZE,n);
}

int SearchOptimal(int ele , int frame[]) {
  int i, flag;
  flag = 0;
  if (fullOptimal != 0) {
    for (i = 0; i < fullOptimal; i++)
      if (ele == frame[i]) {
        flag = 1;
        break;
      }
  }
  return flag;
}


int Longest_opt(int n,int ctr[]) {
  int i, max;
  max = 0; //The increment of counter value here is same as that for of LFU
  for (i = 0; i < n; i++) //The page with maximum frequency is selected
    if (ctr[max] < ctr[i])
      max = i;
  repptr = max;
  return repptr;
}

int Page_replace(int ele,int frame[],int n,int ctr[]) {
  int temp;
  repptr = Longest_opt(n,ctr);
  temp = frame[repptr];
  frame[repptr] = ele;
  ctr[repptr] = 1;
  return temp;
}

void Pagefault(int ele,int frame[] , int n , int ctr[]) {
  if (full != n) {
    ctr[full]++;
    frame[full++] = ele;
  } else
      Page_replace(ele,frame,n,ctr);
}

int Search_process(int ele, int frame[] ,int ctr[]) {
  int i, flag;
  flag = 0;
  if (full != 0) {
    for (i = 0; i < full; i++)
      if (ele == frame[i]) {
        flag = 1;
        ctr[i]++;
        break;
      }
  }
  return flag;
}

// Function To calculate page fault using MFU algorithm.
/*MFU algorithm thinks that the page which was used most frequently will not be needed immediately so it will replace the MFU page*/
void mfuMethod(char* fileName)
{
  int i;
  int n, m;
  int count = 0;
  vector<int> p;

        ifstream in(fileName, ios::in);

        int number;
        while (in >> number) {
          p.push_back(number);
        }
        in.close();
        n = p.at(0);
        p.erase(p.begin());
        m = p.size();

        int *frame = new int[n];
        int *ctr = new int[n];

        for(int i=0 ; i <n ; i++)
        {
                ctr[i] = 0;       //Make the counter as 0.
        }

        vector<int> a(m);
        for (int i = 0; i < m; i++)
        {
                a[i] = p[i];
        }


  for (i = 0; i < m; i++) {
    f = i;
    if (Search_process(a[i],frame,ctr) != 1) {
      Pagefault(a[i],frame , n,ctr);
      count++;
    }
  }
  cout << "\n" << "MFU Method ,The number of page faults are " <<  count << "\n";
}

//Function to calculate Page faults using FIFO.
/*In FIFO algorithm, the operating system keeps track of all pages in the memory in a queue, 
the oldest page is in the front of the queue. When a page needs to be replaced, 
the page in the front of the queue is selected for removal.*/

void fifoMethod(char* fileName)
{
        int n, m, i, j, k, hit = 0;
        vector<int> p;
        vector<int> hi;

        ifstream in(fileName, ios::in);

        int number;
        while (in >> number) {
                p.push_back(number);
        }
        in.close();


        n = p.at(0);
        p.erase(p.begin());
        m = p.size();

        vector<int> p1(m);
        vector<int> hi1(m);

        for (int i = 0; i < m; i++)
        {
          p1[i] = p[i];
        }



        vector<vector<int> > a(n);
        for (i = 0; i < n; i++) {
                a[i] = vector<int>(m, -1);
        }
        map<int, int> mp;
        for (i = 0; i < m; i++) {
                vector<pair<int, int> > c;
              
                for (std::map<int,int>::iterator it=mp.begin(); it!=mp.end(); ++it){
                  c.push_back(std::make_pair( it->second,it->first ));
                }
                sort(c.begin(), c.end());
                bool hasrun = false;
                for (j = 0; j < n; j++) {
                        if (a[j][i] == p1[i]) {
                                hit++;
                                hi1[i] = 1;
                                mp[p1[i]]++;
                                hasrun = true;
                                break;
                        }
                        if (a[j][i] == -1) {
                                for (k = i; k < m; k++)
                                        a[j][k] = p1[i];
                                mp[p1[i]]++;
                                hasrun = true;
                                break;
                        }
                }
                if (j == n || hasrun == false) {
                        for (j = 0; j < n; j++) {
                                if (a[j][i] == c[c.size() - 1].second) {
                                        mp.erase(a[j][i]);
                                        for (k = i; k < m; k++)
                                                a[j][k] = p1[i];
                                        mp[p1[i]]++;
                                        break;
                                }
                        }
                }
              //  for (auto q : mp) {
                for (std::map<int,int>::iterator it=mp.begin(); it!=mp.end(); ++it){
                        if (it->first != p1[i]) {
                                mp[it->first]++;
                        }
                }


}
        cout << "\n" << "FIFO Method ,The number of Page Faults are " << m - hit << "\n";

}

//Function to calculate Page fault using Optimal Algorithm
/* In Optimal page replacement we replace the page which is not referred to the near future, 
although it can't be practically implemented, but this is most optimal and have minimal miss, 
and is most optimal.*/

void optimalMethod(char* fileName)
{
  int i,n;
  int count = 0;
  vector<int> p;

        ifstream in(fileName, ios::in);

        int number;
        while (in >> number) {
                p.push_back(number);
        }
        in.close();


        int size = p.at(0);
        p.erase(p.begin());
        n = p.size();

        int *frame = new int[size];

        vector<int> a(n) ;


        for (int i = 0; i < n; i++)
        {
                a[i] = p[i];
        }

  for (i = 0; i < n; i++) {
    f = i;
    if (SearchOptimal(a[i] , frame) != 1) {
      PagefaultOptimal(a,a[i] , frame , size,n);
      count++;
    }
  }
  cout << "\n" << "Optimal Method ,The number of page faults are " << count << "\n";
}

//Function to calculate Page fault using LRU algorithm
/*In this algorithm, the operating system keeps track of all pages in the memory in a queue, 
the oldest page is in the front of the queue. When a page needs to be replaced, 
the page which is least recently used is replaced by the incoming page.*/

void lruMethod(char* fileName)
{
        int n, m, i, j, k, hit = 0;
        vector<int> p1;
        vector<int> hi1;
        ifstream in(fileName, ios::in);

        int number;
        while (in >> number) {
                p1.push_back(number);
        }
        in.close();


        n = p1.at(0);
        p1.erase(p1.begin());
        m = p1.size();

        vector<int> p(m);
        vector<int> hi(m);

        for (int i = 0; i < m; i++)
        {
                p[i] = p1[i];
        }


        vector<vector<int> > a(n);
        for (i = 0; i < n; i++) {
                a[i] = vector<int>(m, -1);
        }
        map <int, int> mp;
        for (i = 0; i < m; i++) {
                vector<pair<int, int> > c;
               
                for (std::map<int,int>::iterator it=mp.begin(); it!=mp.end(); ++it){
                        c.push_back(std::make_pair(it->second,it->first ));
                }
                sort(c.begin(), c.end());
                bool hasrun = false;
                for (j = 0; j < n; j++) {
                        if (a[j][i] == p[i]) {
                                hit++;
                                hi[i] = 1;
                                mp[p[i]] = 1;
                                hasrun = true;
                                break;
                        }
                        if (a[j][i] == -1) {
                                for (k = i; k < m; k++)
                                        a[j][k] = p[i];
                                mp[p[i]]++;
                                hasrun = true;
                                break;
                        }
                }
                if (j == n || hasrun == false) {
                        for (j = 0; j < n; j++) {
                                if (a[j][i] == c[c.size() - 1].second) {
                                        mp.erase(a[j][i]);
                                        for (k = i; k < m; k++)
                                                a[j][k] = p[i];
                                        mp[p[i]]++;
                                        break;
                                }
                        }
                }
              
                for (std::map<int,int>::iterator it=mp.begin(); it!=mp.end(); ++it){
                        if (it->first != p[i]) {
                                mp[it->first]++;
                        }
                }
        }


        cout << "\n" << "LRU Method , The number of Page Faults are " << m - hit << '\n';
}

int main(int argc ,char *argv[])
{

        //string fileName;
        if(argc < 2)    // Check for illegal Inputs being passed
        {
                cout << "You must provide an input file as an argument";
                exit(1);
        }

        if(argc > 0)
        {
        //fileName = argv[1];
        fifoMethod(argv[1]);   //Compute page fault using FIFO algorithm
        optimalMethod(argv[1]); // Compute page fault using Optimal algorithm
        lruMethod(argv[1]);     // Compute page fault using LRU algorithm
        mfuMethod(argv[1]);     // Compute page fault Using MFU algorithm
        }
        return 0;
}