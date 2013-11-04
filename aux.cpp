#include "aux.h"
#include "iostream"

using namespace Eigen;
void regresja_linia_prosta(vector<double> epoki,vector<double> dane,vector<double> wagi,vector<double> &poprawki,vector<double> &wyrownane,double &a,double &b)
{
    Eigen::MatrixXd A,L,V,X,P;
    A=Eigen::MatrixXd::Zero(dane.size(), 2);
    L=Eigen::MatrixXd::Zero(dane.size(),1);
    P=Eigen::MatrixXd::Zero(dane.size(),dane.size());
    for(int i=0;i<dane.size();i++)
    {
        L(i,0)=-dane.at(i);
        A(i,0)=epoki.at(i);
        A(i,1)=1;
        P(i,i)=wagi.at(i);
    }
    X=Eigen::MatrixXd::Zero(2,1);
    V=Eigen::MatrixXd::Zero(dane.size(),1);
    X=(A.transpose()*P*A).inverse()*(A.transpose()*P*L);
    V=A*X-L;
    for(int i=0;i<dane.size();i++)
    {
        poprawki.push_back(V(i,0));
        wyrownane.push_back(V(i,0)+dane.at(i));
    }
    a=X(0,0);
    b=X(1,0);

    std::cout<<"a="<<a<<" b="<<b<<endl;
};

void odporna(int n_iter,vector<double> epoki,vector<double> dane,vector<double> wagi,vector<double> &poprawki,vector<double> &wyrownane,double &a,double &b)
{
    Eigen::MatrixXd A,L,V,X,X_old,P,dX;
    A=Eigen::MatrixXd::Zero(dane.size(), 2);
    L=Eigen::MatrixXd::Zero(dane.size(),1);
    P=Eigen::MatrixXd::Zero(dane.size(),dane.size());
    for(int i=0;i<dane.size();i++)
    {
        L(i,0)=-dane.at(i);
        A(i,0)=epoki.at(i);
        A(i,1)=1;
        P(i,i)=wagi.at(i);
    }
    X=Eigen::MatrixXd::Zero(2,1);
    X_old=Eigen::MatrixXd::Zero(2,1);
    dX=Eigen::MatrixXd::Zero(2,1);
    V=Eigen::MatrixXd::Zero(dane.size(),1);
    for(int i=0;i<n_iter;++i)
    {
    X=(A.transpose()*P*A).inverse()*(A.transpose()*P*L);
    V=A*X-L;
    dX=X-X_old;
    if(abs(X(0,0))<0.000000001&&abs(X(1,0)<0.000000001))
    {
        break;
    } else
    {
        for(int i=0;i<dane.size();i++)
        {
            P(i,i)=1/(V(i,0)*V(i,0)*V(i,0));
        }
    }
    X_old=X;
    }



    for(int i=0;i<dane.size();i++)
    {
        poprawki.push_back(V(i,0));
        wyrownane.push_back(V(i,0)+dane.at(i));
    }
    a=X(0,0);
    b=X(1,0);
    std::cout<<"a="<<a<<" b="<<b<<" -z odpornej"<<endl;
};
