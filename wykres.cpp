#include "wykres.h"
#include "ui_wykres.h"
#include "qcustomplot.h"
#include <vector>
#include <QFileDialog>

using namespace std;

Wykres::Wykres(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Wykres)
{
    ui->setupUi(this);
   // QCustomPlot customPlot;

}

Wykres::~Wykres()
{
    delete ui;
}

void Wykres::pobierz_dane(vector<string> nazwy, vector<double> wartosci)
{
     vector<double> x;
     QVector<QString> opisy;
     for(unsigned int i=0;i<wartosci.size();++i)
     {
         x.push_back(i);
         opisy.push_back(QString::fromStdString(nazwy.at(i)));
      }

      QCPBars *myBars = new QCPBars(ui->customPlot->xAxis, ui->customPlot->yAxis);
      ui->customPlot->addPlottable(myBars);
      myBars->setData(QVector<double>::fromStdVector(x), QVector<double>::fromStdVector(wartosci));
      ui->customPlot->xAxis->setTickVector(QVector<double>::fromStdVector(x));
      ui->customPlot->xAxis->setTickVectorLabels(opisy);
      ui->customPlot->xAxis->setTickLabels(true);
      // create graph and assign data to it:
      ui->customPlot->addGraph();
      // give the axes some labels:
      ui->customPlot->xAxis->setLabel("nr punktu");
      ui->customPlot->yAxis->setLabel("Wartosc");
      ui->customPlot->graph(0)->setLineStyle(QCPGraph::lsNone);
      ui->customPlot->xAxis->setRange(0,wartosci.size()+1);
      ui->customPlot->yAxis->setRange(*std::min_element(wartosci.begin(),wartosci.end()),*std::max_element(wartosci.begin(),wartosci.end()));
      ui->customPlot->rescaleAxes();
      ui->customPlot->replot();
}

void Wykres::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "", tr("Files (*.*)"));
    ui->customPlot->saveJpg(fileName);
}
