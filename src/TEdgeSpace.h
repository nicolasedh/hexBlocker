//---------------------------------------------------------------------------
/*
  TEdgeSpace - class to calculate the variables for linear meshspacing

  -------------------
  created : 23-Jul-2004 Markus Hartinger    -  markus.hartinger@imperial.ac.uk
  ------------------
*/
#ifndef TEdgeSpaceH
#define TEdgeSpaceH

#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <map>

#define KACCURACY 0.000001
#define KSTEPS 500
#define NACCURACY 0.05
#define NSTEPS 40
#define DELRMIN 0.0001
#define RMAX 10000.0
#define UNITOL 0.0001

using namespace std;


int myround(double x);


/*   TEdgeSpace
     define L and two variables out of r, k, n, ds, de and the 
     other variables will be calculated */ 
class TEdgeSpace
{
public:
    // ----- constructor -----
    TEdgeSpace();
private:
    // ----- variables -----
    double L;   // length of edge
    double r;   // total expansion ratio
    double k;   // cell-to-cell expansion ratio
    int n;      // number of cells
    double ds;  // start spacing
    double de;  // end spacing
    // ----- functions ------
    // total expansion ratio
    void r_de_ds() { r = de/ds; }               // sets r = f(de, ds)
    void r_k_n() { r = pow(k, n-1); }           // sets r = f(k, n)
    // cell-to-cell expansion ratio
    void k_r_n() { k = pow(r, 1.0/(n-1.0)); }       // sets k = f(r, n)
    double k_de_n_func(double k);               // relation of: L, de, n, k
    void k_de_n();                              // sets k = f(L, de, n) with bisection
    double k_ds_n_func(double k);               // relation of: L, ds, n, k
    void k_ds_n();                              // sets k = f(L, ds, n) with bisection
    // number of cells
    double n_ds_r_func(double n);               // relation of: L, ds, r, n
    void n_ds_r();                              // sets n = f(L, ds, r) with bisection
    void n_ds_k();                              // sets n = f(L, ds, k)
    void n_r_k();                               // sets n = f(r, k)
    void n_de_k();                              // sets n = f(L, de, k)
    // start spacing
    void ds_de_r() { ds = de / r; }             // sets ds = f(de, r)
    void ds_k_n();                              // sets ds = f(L, k, n)
    // end spacing
    void de_ds_r() { de = ds * r; }             // sets dr = f(ds, r)
    // bisection method for root finding in func between x1, and x2 with accuray xacc
    double rtbis(double (TEdgeSpace::*func)(double), double x1, double x2, double xacc, int MaxSteps);
    // assigning arguments 
    void AssignArg(string ArgName, double Arg); 
    // ----- input ----- 
public: 
    // get
    double getL() { return L;}
    double getr() { return r;}
    double getk() { return k;}
    double getds() { return ds;}
    double getde() { return de;}
    int getn() { return n;}
    // calc
    void calc(string ArgName1, string ArgName2, double Length, double Arg1, double Arg2); 
    void test(); 
public: 
    // TFuncLink - contains pointer to member functions of TEdgeSpace 
    // determines the order of function calls 
    class TFuncLink 
    { 
    public: 
        TFuncLink(void (TEdgeSpace::*AFunc)(), string ANextLink) 
            {Func=AFunc; NextLink=ANextLink;} 
        void (TEdgeSpace::*Func)(); 
        string NextLink; 
    }; 
    // TFuncStart - determines which funktion is called first given two arguments 
    class TFuncStart 
    { 
    public: 
        TFuncStart(string AArgName1, string AArgName2, string ANextLink) 
            {ArgName1=AArgName1, ArgName2=AArgName2, NextLink=ANextLink;} 
        string ArgName1; 
        string ArgName2; 
        string NextLink; 
    }; 
private: 
    map<string, TFuncLink*> FuncLinks; 
    vector<TFuncStart> FuncStarts; 
}; 
//---------------------------------------------------------------------------
#endif
