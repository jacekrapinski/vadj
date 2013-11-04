#include "wykres_obserwacje.h"
#include "ui_wykres_obserwacje.h"
#include <iostream>

wykres_obserwacje::wykres_obserwacje(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::wykres_obserwacje)
{
    ui->setupUi(this);
}

wykres_obserwacje::~wykres_obserwacje()
{
    delete ui;
}

void wykres_obserwacje::get_filename(QString f)
{
    filename=f;
    ui->etykieta->setText(f);
    nazwa_plika=f;
};

void wykres_obserwacje::plotuj(QString nazwa_wektora,int iteracje)
{
    std::vector<double> epoki,dane,v,wyr,p;
    double a,b;
    czytaj_plika_raw(nazwa_plika,epoki,dane);
    vector<double>::iterator i;
    for (i=epoki.begin();i<epoki.end();++i)
    {
        p.push_back(1);
    }

    regresja_linia_prosta(epoki,dane,p,v,wyr,a,b);
    //QCPBars *myBars = new QCPBars(ui->customPlot->xAxis, ui->customPlot->yAxis);
    //ui->customPlot->addPlottable(myBars);
    //myBars->setData(QVector<double>::fromStdVector(epoki), QVector<double>::fromStdVector(dane));
    QVector<double> x,y;
    y.push_back(wyr.at(0));
    y.push_back(wyr.at(wyr.size()-1));
    x.push_back(epoki.at(0));
    x.push_back(epoki.at(wyr.size()-1));
    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setData(QVector<double>::fromStdVector(epoki), QVector<double>::fromStdVector(dane));
    ui->customPlot->addGraph();
    ui->customPlot->graph(1)->setPen(QPen(Qt::red));
    ui->customPlot->graph(1)->setLineStyle(QCPGraph::lsStepCenter);
    ui->customPlot->graph(1)->setData(QVector<double>::fromStdVector(epoki), QVector<double>::fromStdVector(wyr));
    ui->customPlot->addGraph();
    ui->customPlot->graph(2)->setPen(QPen(Qt::green));
    ui->customPlot->graph(2)->setData(x,y);
    // give the axes some labels:
    ui->customPlot->xAxis->setLabel("nr punktu");
    ui->customPlot->yAxis->setLabel("Wartosc");
    ui->customPlot->xAxis->setRange(0,epoki.size()+1);
    ui->customPlot->yAxis->setRange(*std::min_element(dane.begin(),dane.end()),*std::max_element(dane.begin(),dane.end()));
    ui->customPlot->rescaleAxes();
    ui->customPlot->plotLayout()->insertRow(0);
    ui->customPlot->plotLayout()->addElement(0, 0, new QCPPlotTitle(ui->customPlot, nazwa_wektora));
    wyr.clear();
    v.clear();
    odporna(iteracje,epoki,dane,p,v,wyr,a,b);
    x.clear();
    y.clear();
    y.push_back(wyr.at(0));
    y.push_back(wyr.at(wyr.size()-1));
    x.push_back(epoki.at(0));
    x.push_back(epoki.at(wyr.size()-1));
    ui->customPlot->addGraph();
    ui->customPlot->graph(3)->setPen(QPen(Qt::black));
    ui->customPlot->graph(3)->setData(x,y);
    ui->customPlot->replot();
};


void wykres_obserwacje::czytaj_plika_raw(QString nazwa, std::vector<double> &epoki, std::vector<double> &dane)
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
};
