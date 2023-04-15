#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QTableView;
class QComboBox;
class QDoubleSpinBox;
class QLineEdit;
class QPushButton;
class QDateEdit;
class QLabel;
class paymentsTableModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void addTrans();
    void visualizeCurrentAmount();


    QTableView *view = nullptr;
    QComboBox *signBox = nullptr;
    QDoubleSpinBox *amountSpinBox = nullptr;
    QLineEdit *descLineEdit = nullptr;
    QPushButton *submit = nullptr;
    QDateEdit *date = nullptr;
    paymentsTableModel *dataModel = nullptr;

    QLabel *positivesLeft = nullptr;
    QLabel *negativesLeft = nullptr;

    QLabel *leftAmount = nullptr;



};
#endif // MAINWINDOW_H
