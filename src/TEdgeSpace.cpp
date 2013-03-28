//---------------------------------------------------------------------------
/*
  TEdgeSpace - class to calculate the variables for linear meshspacing

  -------------------
  created : 23-Jul-2004 Markus Hartinger    -  markus.hartinger@imperial.ac.uk
  ------------------
*/


#pragma hdrstop
#include <cstdlib>
#include "TEdgeSpace.h"

int myround(double x) {
    return (x - floor(x) < 0.5) ? int(floor(x)) : int(ceil(x));
};


TEdgeSpace::TEdgeSpace()
{
    // ini
    L = 0.0;
    ds = 0.0;
    de = 0.0;
    r = 0.0;
    k = 0.0;
    n = 0;

    // ----- defining TFuncStart -----
    FuncStarts.push_back(TFuncStart("ds", "de", "r_de_ds"));
    FuncStarts.push_back(TFuncStart("ds", "r", "n_ds_r"));
    FuncStarts.push_back(TFuncStart("ds", "k", "n_ds_k"));
    FuncStarts.push_back(TFuncStart("ds", "n", "k_ds_n"));
    FuncStarts.push_back(TFuncStart("de", "r", "ds_de_r"));
    FuncStarts.push_back(TFuncStart("de", "k", "n_de_k"));
    FuncStarts.push_back(TFuncStart("de", "n", "k_de_n"));
    FuncStarts.push_back(TFuncStart("r", "k", "n_r_k"));
    FuncStarts.push_back(TFuncStart("r", "n", "k_r_n"));
    FuncStarts.push_back(TFuncStart("k", "n", "r_k_n"));

    // ----- defining TFuncLink -----
    // level 0 - NULL
    FuncLinks["de_ds_r"] = new TFuncLink(&TEdgeSpace::de_ds_r, "");
    // level 1 - de_ds_r
    FuncLinks["ds_k_n"] = new TFuncLink(&TEdgeSpace::ds_k_n, "de_ds_r");
    // level 2 - ds_k_n
    FuncLinks["r_k_n"] = new TFuncLink(&TEdgeSpace::r_k_n,  "ds_k_n");
    FuncLinks["k_r_n"] = new TFuncLink(&TEdgeSpace::k_r_n,  "ds_k_n");
    // level 3
        // k_r_n
    FuncLinks["n_r_k"] = new TFuncLink(&TEdgeSpace::n_r_k,  "k_r_n");
    FuncLinks["n_ds_r"] = new TFuncLink(&TEdgeSpace::n_ds_r,  "k_r_n");
        // r_k_n
    FuncLinks["k_de_n"] = new TFuncLink(&TEdgeSpace::k_de_n,  "r_k_n");
    FuncLinks["n_de_k"] = new TFuncLink(&TEdgeSpace::n_de_k,  "r_k_n");
    FuncLinks["k_ds_n"] = new TFuncLink(&TEdgeSpace::k_ds_n,  "r_k_n");
    FuncLinks["n_ds_k"] = new TFuncLink(&TEdgeSpace::n_ds_k,  "r_k_n");
    // level 4
    FuncLinks["r_de_ds"] = new TFuncLink(&TEdgeSpace::r_de_ds,  "n_ds_r");
    FuncLinks["ds_de_r"] = new TFuncLink(&TEdgeSpace::ds_de_r,  "n_ds_r");
}

double TEdgeSpace::rtbis(double (TEdgeSpace::*func)(double), double x1, double x2, double xacc, int MaxSteps)
{ 
  	int j; 
    double dx, f, fmid, xmid, rtb; 
 
   	f = (this->*func)(x1); 
    fmid = (this->*func)(x2); 
   	if (f*fmid >= 0.0) { 
    	cout << "Root must be bracketed for bisection in rtbis\n"; 
        throw(1);
    } 
   	rtb = f < 0.0 ? (dx=x2-x1, x1) : (dx=x1-x2, x2);  
    for (j=1; j<=MaxSteps; j++) { 
	    fmid = (this->*func)(xmid=rtb+(dx *= 0.5)); 
  		if (fmid <= 0.0) rtb=xmid; 
    	if (fabs(dx) < xacc || fmid == 0.0) return rtb; 
   	} 
    cout << "Too many bisections in rtbis\n"; 
   	return 0.0; 
} 
 
double TEdgeSpace::k_de_n_func(double k)
{
    return (1 / pow(k, n-1)) * (1-pow(k,n)) / (1-k) - L/de;
}

void TEdgeSpace::k_de_n()
{
    double kmin, kmax;

    if (fabs(n*de - L) > UNITOL) {
        // determine k > 1 or k < 1
        if (n*de > L) {
            kmax = pow(RMAX, 1.0/(float(n)-1.0));
            kmin = pow(1+DELRMIN, 1.0/(n-1.0));
        } else {
            kmin = pow(1.0/RMAX, 1.0/(float(n)-1.0));
            kmax = pow(1-DELRMIN, 1.0/(n-1.0));
        }
        k = rtbis(&TEdgeSpace::k_de_n_func, kmin, kmax, KACCURACY, KSTEPS);
    } else
        k = 1;
}

double TEdgeSpace::k_ds_n_func(double k)
{
    return  (1-pow(k,n)) / (1-k) - L/ds;
}

void TEdgeSpace::k_ds_n()
{
    double kmin, kmax;

    if (fabs(n*ds - L) > UNITOL) {
    // determine k > 1 or k < 1
        if (n*ds < L) {
            kmax = pow(RMAX, 1.0/(float(n)-1.0));
            kmin = pow(1+DELRMIN, 1.0/(n-1.0));
        } else {
            kmin = pow(1.0/RMAX, 1.0/(float(n)-1.0));
            kmax = pow(1-DELRMIN, 1.0/(n-1.0));
        }
        k = rtbis(&TEdgeSpace::k_ds_n_func, kmin, kmax, KACCURACY, KSTEPS);
    } else   
        k = 1;
}

double TEdgeSpace::n_ds_r_func(double n)
{
    return (1-pow(r,(n/(n-1))))/(1-pow(r,(1/(n-1)))) - L/ds;
}

void TEdgeSpace::n_ds_r()
{
    double dmin;

	dmin =  r > 1 ? ds : r*de;
 
    if (r != 1) 
        n = myround(rtbis(&TEdgeSpace::n_ds_r_func, 0, L/dmin, NACCURACY, NSTEPS)); 
    else 
        n = myround(L/dmin); 
}

void TEdgeSpace::n_ds_k()
{
    double h = float(k-1.0);
    n = fabs(h) > KACCURACY ? (myround(log10(1.0 - L/ds*(1.0 - k))/log10(k))) : (myround(L/ds));
}

void TEdgeSpace::n_r_k()
{ 
    double h = float(k-1.0);
    n = fabs(float(k-1.0)) > KACCURACY ? (n = myround(log10(r)/log10(k) + 1)) : 1;
}

void TEdgeSpace::n_de_k()
{
    double h = float(k-1.0);
    n = fabs(h) > KACCURACY ? (n = myround(log10(1.0/(1.0+L/de*(1.0-k)/k))/log10(k))) : (myround(L/de));
}

void TEdgeSpace::ds_k_n()
{
    double h = float(k-1.0);
    ds = fabs(h) > KACCURACY ? (ds = L * (1-k) / (1 - pow(k,n))) : (L/n);
}

void TEdgeSpace::AssignArg(string ArgName, double Arg)
{
    if      (ArgName == "ds") ds = Arg;
    else if (ArgName == "de") de = Arg;
    else if (ArgName == "r") r = Arg;
    else if (ArgName == "k") k = Arg;
    else if (ArgName == "n") n = myround(Arg);
    else {
        cout << "Argument '" << ArgName << "' not recognised - exiting...\n";
        throw(-1);
    }
}

void TEdgeSpace::calc(string ArgName1, string ArgName2, double Length, double Arg1, double Arg2)
{
    int i;
    string NextLink="";
    TFuncLink *FuncLink;
    void (TEdgeSpace::*Func)();

    // ini
    L = Length;
    AssignArg(ArgName1, Arg1);
    AssignArg(ArgName2, Arg2);
    // selecting FuncStart
    for(i=0; i<FuncStarts.size(); i++) {
        if (((FuncStarts[i].ArgName1 == ArgName1) && (FuncStarts[i].ArgName2 == ArgName2)) ||
            ((FuncStarts[i].ArgName2 == ArgName1) && (FuncStarts[i].ArgName1 == ArgName2))) {
            NextLink = FuncStarts[i].NextLink;
        }
    }
    // run calculations 
    while (NextLink != "") {
        FuncLink = NULL;
        FuncLink = FuncLinks[NextLink];
        Func = FuncLink->Func;
       	(this->*Func)();
        NextLink = FuncLink->NextLink;
    }
}

void TEdgeSpace::test()
{
    L = 1;
    ds = 0.001;
    de = 0.01;
    r = 10;
    n = 255;
    k = 1.00908;
//    r_de_ds();
//    r_k_n();
//    k_r_n();
//    k_de_n();
//    k_ds_n();
    n_ds_r();
//    n_ds_k();
//    n_r_k();
//    n_de_k();
//    ds_de_r();
//    ds_k_n();
//    de_ds_r();

/*    double h;

    cout << "k:" << k << "\n";

    h = k-1.0;
    h = fabs(h);
    n = h > KACCURACY ? (round(log10(1.0 - L/ds*(1.0 - k))/log10(k))) : (round(L/ds));
*/
}


//---------------------------------------------------------------------------

#pragma package(smart_init)

