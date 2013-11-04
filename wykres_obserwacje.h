#ifndef WYKRES_OBSERWACJE_H
#define WYKRES_OBSERWACJE_H

#include <QDialog>
#include <QString>
#include <aux.h>

namespace Ui {
class wykres_obserwacje;
}

class wykres_obserwacje : public QDialog
{
    Q_OBJECT
    
public:
    explicit wykres_obserwacje(QWidget *parent = 0);
    ~wykres_obserwacje();
    void get_filename(QString filename);
    QString nazwa_plika;
    void plotuj(QString nazwa_wektora,int iteracje);
    void czytaj_plika_raw(QString nazwa, std::vector<double> &epoki, std::vector<double> &dane);
    
private slots:

private:
    Ui::wykres_obserwacje *ui;
    QString filename;
};

#endif // WYKRES_OBSERWACJE_H
