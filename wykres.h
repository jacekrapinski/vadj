#ifndef WYKRES_H
#define WYKRES_H

#include <QWidget>
#include <vector>
#include <string>
namespace Ui {
class Wykres;
}

class Wykres : public QWidget
{
    Q_OBJECT
    
public:
    explicit Wykres(QWidget *parent = 0);
    ~Wykres();
    void pobierz_dane(std::vector<std::string> nazwy, std::vector<double> wartosci);
    
private slots:
    void on_pushButton_clicked();

private:
    Ui::Wykres *ui;
};

#endif // WYKRES_H
