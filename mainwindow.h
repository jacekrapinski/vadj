#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    void czytaj_plika(QString nazwa, double &srednia, int &n);
    void czytaj_plika_ruchy(QString nazwa, double &srednia, int &n);
    void czytaj_plika_raw(QString nazwa, std::vector<double> &epoki, std::vector<double> &dane);
    ~MainWindow();
    QString sciezka;
private slots:
    void on_pushButton_clicked();

    void on_listPunktow_currentRowChanged(int currentRow);

    void on_pushButton_2_clicked();

    void on_button_zapisz_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_zapisz_obserwacje_clicked();

    void on_pushButton_wykres_dane_clicked();

    void on_dial_valueChanged(int value);

private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
