#include "mainwindow.h"

#include <QTableView>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QPushButton>
#include <QBoxLayout>
#include <QFormLayout>
#include <QDateEdit>
#include <QLabel>
#include <QHeaderView>

#include "paymentstablemodel.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    auto dummy = new QWidget();
    setMinimumSize(800, 600);
    view = new QTableView(dummy);
    dataModel = new paymentsTableModel();
    view->setModel(dataModel);
    view->horizontalHeader()->setStretchLastSection(true);

    signBox = new QComboBox(dummy);
    signBox->addItems({"пополнение", "расход"});

    amountSpinBox = new QDoubleSpinBox(dummy);
    amountSpinBox->setDecimals(2);
    amountSpinBox->setRange(0, std::numeric_limits<double>::max());

    descLineEdit = new QLineEdit(dummy);

    date = new QDateEdit(dummy);
    date->setDate(QDateTime::currentDateTime().date());

    submit = new QPushButton("Submit", dummy);

    auto main_l = new QHBoxLayout(dummy);
    auto table_l = new QVBoxLayout();
    table_l->addWidget(view, 1);

    auto label = new QFormLayout();
    leftAmount = new QLabel("0");
    positivesLeft = new QLabel("0");
    negativesLeft = new QLabel("0");
    label->addRow("Всего пришло: ", positivesLeft);
    label->addRow("Всего ушло: ", negativesLeft);
    label->addRow("Всего баланс: ", leftAmount);
    table_l->addLayout(label);

    main_l->addLayout(table_l, 3);
    auto newTrans_l = new QFormLayout();
    newTrans_l->addRow("Тип: ", signBox);
    newTrans_l->addRow("Сумма: ", amountSpinBox);
    newTrans_l->addRow("Дата: ", date);
    newTrans_l->addRow("Описание: ", descLineEdit);
    newTrans_l->addRow("", submit);
    connect(submit, &QPushButton::clicked, this, &MainWindow::addTrans);
    main_l->addLayout(newTrans_l, 2);
    setCentralWidget(dummy);
    visualizeCurrentAmount();
}

MainWindow::~MainWindow()
{
}

void MainWindow::addTrans()
{
    double amount = amountSpinBox->value();
    if(signBox->currentIndex() == 1){
        amount *= -1;
    }
    QString desc = descLineEdit->text();
    QDateTime dt = date->dateTime();
    dataModel->addTransactions(dt, amount, desc);
    visualizeCurrentAmount();
}

void MainWindow::visualizeCurrentAmount()
{
    positivesLeft->setText(QString::number(dataModel->countPositives(), 'f', 2));
    negativesLeft->setText(QString::number(dataModel->countNegatives(), 'f', 2));
    leftAmount->setText(QString::number(dataModel->countTotal(), 'f', 2));
    view->resizeColumnsToContents();
}

