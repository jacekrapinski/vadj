#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include <armadillo>
#include <QFileDialog>
#include <QApplication>
#include <QtGui>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QRegExp>
#include <sstream>
#include <string>
#include <vector>
#include "wykres.h"
#include "wykres_obserwacje.h"
#include "qcustomplot.h"
#include <cmath>
#include "Eigen/Eigen"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QStringList naglowki_tabeli,naglowki_tabeli_h;
    naglowki_tabeli << "OD" << "DO" <<"n"<< "V"<<"Poprawka";
    naglowki_tabeli_h << "Punkt" << "V";
    ui->tableWidget->setHorizontalHeaderLabels(naglowki_tabeli);
    ui->tableWysokosci->setHorizontalHeaderLabels(naglowki_tabeli_h);
    //ui->listPunktow->setHidden(true);
    ui->listWidget->setHidden(true);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QStringList fileName = QFileDialog::getOpenFileNames(this, tr("Open File"), "", tr("Files (*.*)"));
    QFileInfo ppp(fileName.at(0));
    this->sciezka=ppp.path();
    QStringList::const_iterator i;
    for(i=fileName.begin();i!=fileName.end();++i)
    {
        QString wektor;
        wektor=*i;
        QString sub;
        sub=wektor.mid(wektor.lastIndexOf("_")-4,12);
        ui->listWidget->addItem(sub);
    }

    for(i=fileName.begin();i!=fileName.end();++i)
    {
        QTableWidgetItem* item = new QTableWidgetItem();
        int i_s;
        QString l;
        l=*i;
        i_s=l.lastIndexOf("_");
        QString sub;
        if (i_s>0)
        {
            double srednia=0;
            int n=0;

            if(ui->checkBox_ruchy->isChecked()==false)
            {
            czytaj_plika(l, srednia, n);
            }
            else
            {
            czytaj_plika_ruchy(l,srednia,n);
            }

            sub=l.mid(i_s-4,4);
            int row=ui->tableWidget->rowCount();
                ui->tableWidget->insertRow(row);
                ui->tableWidget->setItem(row, 0, new QTableWidgetItem(sub));
                if(ui->checkBox_ruchy->isChecked()==false)
                {
                ui->tableWidget->setItem(row, 2, new QTableWidgetItem(QString::number(n)));
                }
                else
                {
                ui->tableWidget->setItem(row, 2, new QTableWidgetItem(QString::number(n-1)));
                }
                ui->tableWidget->setItem(row, 3, new QTableWidgetItem(QString::number(srednia)));
                ui->tableWidget->setItem(row, 4, new QTableWidgetItem(QString::number(0)));
            if(ui->listPunktow->findItems(sub, Qt::MatchWrap | Qt::MatchWildcard).isEmpty())
            {
            ui->listPunktow->addItem(sub);
            }
            sub=l.mid(i_s+1,4);
            ui->tableWidget->setItem(row, 1, new QTableWidgetItem(sub));
            if(ui->listPunktow->findItems(sub, Qt::MatchWrap | Qt::MatchWildcard).isEmpty())
            {
                ui->listPunktow->addItem(sub);
            }

        }
    }
    for(int i=0; i<ui->listPunktow->count();++i)
    {
        int row=ui->tableWysokosci->rowCount();
        ui->tableWysokosci->insertRow(row);
        ui->tableWysokosci->setItem(row,0,new QTableWidgetItem(ui->listPunktow->item(i)->text()));
        ui->tableWysokosci->setItem(row,1,new QTableWidgetItem(QString::number(0)));
    }
    int ile_obs;
    int pb=0;
    ile_obs=ui->listWidget->count();
    int ile_param;
    ile_param=ui->listPunktow->count()-1;
 //   arma::mat A(ui->listWidget->count(),ui->listPunktow->count()-1);
 //   arma::mat L(ui->listWidget->count(),1);
    ui->textEdit->append(QString::number(ile_obs));
    ui->textEdit->append(QString::number(ile_param));



    int ile_elem=ile_obs*ile_param;
    ui->progressBar->setMaximum(ile_elem);

    for (int i=0;i<ile_obs;i++)
    {
        for (int j=0;j<ile_param;j++)
        {

            ui->progressBar->setValue(pb);
            pb++;
        }
   // L(i,0)=0;
    }
    ui->progressBar->setValue(ile_elem);
    ui->listPunktow->setCurrentRow(0);
    ui->pushButton_wykres_dane->setEnabled(true);
    ui->tableWidget->setCurrentCell(0,0);
}

void MainWindow::on_listPunktow_currentRowChanged(int currentRow)
{
    ui->label->setText(ui->listPunktow->currentItem()->text());
}

void MainWindow::czytaj_plika_ruchy(QString nazwa, double &srednia, int &n)
{
        QFile file(nazwa);
        vector<double> dh,dhh,dt;
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream in(&file);
        n=0;
        double suma=0;
        QRegExp rx("\\s\\s*");
        while (!in.atEnd())
        {
             QString line = in.readLine();
             QStringList fields=line.split(rx);
             if(fields.length()>1){
           //      ui->textEdit->append(fields.at(1));
             suma+=fields.at(1).toDouble();
             dh.push_back(fields.at(1).toDouble());
             dt.push_back(fields.at(0).toDouble());
             n++;
             }
        }
        double sum1=0;
        for(unsigned int  i=1;i<dh.size();++i)
        {
            double Dh,Dt,z;
            Dt=dt.at(i)-dt.at(i-1);
            Dh=dh.at(i)-dh.at(i-1);
            z=Dh/Dt;
            dhh.push_back(z);
            sum1+=z;
        }
        srednia=sum1/(n-1);
}

void MainWindow::czytaj_plika(QString nazwa, double &srednia, int &n)
{
    QFile file(nazwa);
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream in(&file);
        n=0;
        double suma=0;
        QRegExp rx("\\s\\s*");
        while (!in.atEnd())
        {
             QString line = in.readLine();
             QStringList fields=line.split(rx);
             if(fields.length()>1){
           //      ui->textEdit->append(fields.at(1));
             suma+=fields.at(1).toDouble();
             n++;
             }
             srednia=suma/n;
        }
}

void MainWindow::czytaj_plika_raw(QString nazwa, vector<double> &epoki, vector<double> &dane)
{
    QFile file(nazwa);
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream in(&file);
        QRegExp rx("\\s\\s*");
        while (!in.atEnd())
        {
             QString line = in.readLine();
             QStringList fields=line.split(rx);
             if(fields.length()>1){
             dane.push_back(fields.at(1).toDouble());
             epoki.push_back(fields.at(0).toDouble());
             }
        }
}

void MainWindow::on_pushButton_2_clicked()
{
    int obs=ui->tableWidget->rowCount();
    int param=ui->listPunktow->count()-1;
    int ind_stalego=ui->listPunktow->currentIndex().row();
          //  arma::mat A(obs,param);
            Eigen::MatrixXd eA,eP,eL;
            eA=Eigen::MatrixXd::Zero(obs, param);
            eL=Eigen::MatrixXd::Zero(obs, 1);
            eP=Eigen::MatrixXd::Zero(obs, obs);
          //  arma::mat L(obs,1);
          //  arma::mat P(obs,obs);
          //  A.zeros();
          //  L.zeros();
          //  P.zeros();


//wypełniamy L i P
            double dh;
            double Hb,Ha;
            for(int i=0; i<obs;i++)
            {
            dh=ui->tableWidget->item(i,3)->text().toDouble();
            int rows = ui->tableWysokosci->rowCount();
            for(int j = 0; j < rows; ++j)
            {
                if(ui->tableWysokosci->item(j, 0)->text() == ui->tableWidget->item(i,1)->text())
                {
                    Hb=ui->tableWysokosci->item(j,1)->text().toDouble();
                    break;
                }
            }
            for(int j = 0; j < rows; ++j)
            {
                if(ui->tableWysokosci->item(j, 0)->text() == ui->tableWidget->item(i,0)->text())
                {
                    Ha=ui->tableWysokosci->item(j,1)->text().toDouble();
                    break;
                }
            }
           // L(i,0)=Hb-Ha-dh;
            eL(i,0)=Hb-Ha-dh;
            if (ui->checkBox->isChecked())
            {
           // P(i,i)=ui->tableWidget->item(i,2)->text().toDouble();
            eP(i,i)=ui->tableWidget->item(i,2)->text().toFloat();
            }
            else
            {
           // P(i,i)=1;
            eP(i,i)=1;
            }
            }
//wypełniamy A
            for(int i=0;i<obs;i++)
            {
                QString pod,pdo;
                pod=ui->tableWidget->item(i,0)->text();
                pdo=ui->tableWidget->item(i,1)->text();

                for (int kol=0;kol<param;kol++)
                {
                 if (pod!=ui->label_2->text())
                 {
                        if (pod==ui->tableWysokosci->item(kol,0)->text())
                        {
                //            A(i,kol)=1;
                            eA(i,kol)=1;
                        }
                 }
                 if (pdo!=ui->label_2->text())
                 {
                        if (pdo==ui->tableWysokosci->item(kol,0)->text())
                        {
              //              A(i,kol)=-1;
                            eA(i,kol)=-1;
                        }
                 }
                }
            }
            Eigen::MatrixXd eX,eV;
            eX=Eigen::MatrixXd::Zero(param,1);
            eV=Eigen::MatrixXd::Zero(obs,1);
            //arma::mat X(param,1);
            //arma::mat V(obs,1);

            eX=(eA.transpose()*eP*eA).inverse()*(eA.transpose()*eP*eL);
            //X=arma::inv(A.t()*P*A)*A.t()*P*L*(-1);
     //       std::cout<<eX;
            eV=eA*eX+eL;
            //V=A*X+L;
            std::string s,sv;
            std::stringstream ss,vv;
            //V.print(vv);
            vv<<eV;
            //X.print(ss);
            ss<<eX;
            sv=vv.str();  //lista poprawek
            s=ss.str();
            QString listah,listav;
            listah=QString::fromStdString(s);
            listav=QString::fromStdString(sv);
            QStringList lsh=listah.split("\n");
            QStringList lsv=listav.split("\n");
            int i=0;

            for(int j=0;j<lsv.size();j++)
            {
                ui->tableWidget->setItem(j,4,new QTableWidgetItem(lsv.at(j)));
            }

            ui->textEdit->append(QString::number(lsh.length()));
            for(int j=0;j<lsh.length()-1;++j)
            {
                if(ui->tableWysokosci->item(i,0)->text()==ui->label->text())
                {
                    ++i;

                }
                    ui->tableWysokosci->setItem(i,1,new QTableWidgetItem(lsh.at(j)));
                    ++i;
            }

            ui->textEdit->append(QString::fromStdString(sv));
            ui->pushButton_3->setEnabled(true);
        }

void MainWindow::on_button_zapisz_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "", tr("Files (*.*)"));
    QFile file(fileName);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    for(int i=0;i<ui->tableWysokosci->rowCount();i++)
    {
        file.write(ui->tableWysokosci->item(i,0)->text().toStdString().c_str());
        file.write(" ");
        file.write(ui->tableWysokosci->item(i,1)->text().toStdString().c_str());
        file.write("\n");
    }
    file.close();
}



void MainWindow::on_pushButton_3_clicked()
{
    Wykres *a=new Wykres;
    vector<string> nazwa;
    vector<double> wartosci;
    for(int i=0;i<ui->tableWysokosci->rowCount();i++)
    {
        nazwa.push_back(ui->tableWysokosci->item(i,0)->text().toStdString());
        wartosci.push_back(ui->tableWysokosci->item(i,1)->text().toDouble());
    }
    a->pobierz_dane(nazwa,wartosci);
    a->show();
}

void MainWindow::on_pushButton_zapisz_obserwacje_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "", tr("Files (*.*)"));
    QFile file(fileName);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    for(int i=0;i<ui->tableWidget->rowCount();i++)
    {
        file.write(ui->tableWidget->item(i,0)->text().toStdString().c_str());
        file.write(" ");
        file.write(ui->tableWidget->item(i,1)->text().toStdString().c_str());
        file.write(" ");
        file.write(ui->tableWidget->item(i,2)->text().toStdString().c_str());
        file.write(" ");
        file.write(ui->tableWidget->item(i,3)->text().toStdString().c_str());
        file.write(" ");
        file.write(ui->tableWidget->item(i,4)->text().toStdString().c_str());
        file.write("\n");
    }
    file.close();
}

void MainWindow::on_pushButton_wykres_dane_clicked()
{
 wykres_obserwacje *a=new wykres_obserwacje;
 a->get_filename(this->sciezka+"/"+ui->tableWidget->item(ui->tableWidget->currentRow(),0)->text()+"_"+ui->tableWidget->item(ui->tableWidget->currentRow(),1)->text()+".dh");
 a->plotuj(ui->tableWidget->item(ui->tableWidget->currentRow(),0)->text()+"_"+ui->tableWidget->item(ui->tableWidget->currentRow(),1)->text(),ui->dial->value());
 a->show();
}

void MainWindow::on_dial_valueChanged(int value)
{
    ui->lcdNumber->display(QString::number(ui->dial->value()));
}
