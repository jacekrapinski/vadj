#include "Eigen/Eigen"
#include <vector>
#include <math.h>
using namespace std;

    void regresja_linia_prosta(vector<double> epoki,vector<double> dane,vector<double> wagi,
                               vector<double> &poprawki,vector<double> &wyrownane,double &a,double &b);

    void odporna(int n_iter,vector<double> epoki,vector<double> dane,vector<double> wagi,
                  vector<double> &poprawki,vector<double> &wyrownane,double &a,double &b);
