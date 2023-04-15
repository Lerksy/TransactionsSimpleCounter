#ifndef ACCOUNTSTABLEMODEL_H
#define ACCOUNTSTABLEMODEL_H

#include <QAbstractTableModel>
#include <QObject>
#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDateTime>
#include <QApplication>
#include <QFileInfo>

class paymentsTableModel : public QAbstractTableModel
{
private:
    struct transaction{
        QDate mDateOfPayment;
        double mAmount;
        QString mDescription;

        transaction(quint64 dateOfPayment, double amount, QString description){
            mDateOfPayment = QDateTime::fromSecsSinceEpoch(dateOfPayment).date();
            mAmount = amount;
            mDescription = description;
        }

    };
    QSqlDatabase db;
    QList<transaction> transactions;
    const int COLS = 3;
    int ROWS = 0;
    void dbFirstRun();
    void loadDataFromDB();

public:
    paymentsTableModel();
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    double countTotal();
    double countPositives();
    double countNegatives();

public slots:
    void addTransactions(QDateTime date, double amount, QString description);
};

#endif // ACCOUNTSTABLEMODEL_H
