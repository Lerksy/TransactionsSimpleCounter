#include "paymentstablemodel.h"
#include <QSqlError>

void paymentsTableModel::dbFirstRun(){
    QSqlQuery initQuery;
    initQuery.exec("CREATE TABLE IF NOT EXISTS transactions(date bigint, amount real, description varchar(255));");
}

void paymentsTableModel::loadDataFromDB(){
    QSqlQuery loadQuery;
    loadQuery.exec("SELECT * FROM transactions");
    transactions.clear();
    while (loadQuery.next()) {
        transactions.push_back(transaction(loadQuery.value("date").toULongLong(),
                                  loadQuery.value("amount").toDouble(),
                                  loadQuery.value("description").toString()));
    }
    ROWS = transactions.size();
}

paymentsTableModel::paymentsTableModel(){
    QString dbName = QApplication::applicationDirPath()+"/data.db";
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbName);
    QFileInfo dbInfo(dbName);
    if(!dbInfo.exists()){
        db.open();
        dbFirstRun();
    }else{
        db.open();
        loadDataFromDB();
    }

}

int paymentsTableModel::rowCount(const QModelIndex &parent) const{
    Q_UNUSED(parent)
    return ROWS;
}

int paymentsTableModel::columnCount(const QModelIndex &parent) const{
    Q_UNUSED(parent)
    return COLS;
}

QVariant paymentsTableModel::data(const QModelIndex &index, int role) const {
    if (role == Qt::DisplayRole && checkIndex(index)) {
        switch (index.column()) {
        default: {
          return QVariant();
        }
        case 0: {
          return transactions.at(index.row())
              .mDateOfPayment.toString("dd MMM yyyy");
        }
        case 1: {
          return QString::number(transactions.at(index.row()).mAmount, 'f', 2);
        }
        case 2: {
          return transactions.at(index.row()).mDescription;
        }
        }
    }
    if(role == Qt::ToolTipRole && checkIndex(index)){
        switch (index.column()) {
        case 2:
          return transactions.at(index.row()).mDescription;
        default:
          return QVariant();
        }
    }
    return QVariant();
}

QVariant paymentsTableModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return "Date";
        case 1:
            return "Amount";
        case 2:
            return "Description";
        default:
            break;
        }
    }else if(orientation == Qt::Vertical){
        return section+1;
    }
    return QVariant();
}

double paymentsTableModel::countTotal()
{
    double ret = 0;
    for(const auto &trans : transactions){
        ret += trans.mAmount;
    }
    return ret;
}

double paymentsTableModel::countPositives()
{
    double ret = 0;
    for(const auto &trans : transactions){
        if(trans.mAmount > 0){
            ret += trans.mAmount;
        }
    }
    return ret;
}

double paymentsTableModel::countNegatives()
{
    double ret = 0;
    for(const auto &trans : transactions){
        if(trans.mAmount < 0){
            ret += trans.mAmount;
        }
    }
    return ret;
}

void paymentsTableModel::addTransactions(QDateTime date, double amount, QString description){
    beginResetModel();
    QSqlQuery pushQuery;
    pushQuery.exec(QString("INSERT INTO transactions(date, amount, description) VALUES('%1', '%2', '%3');").arg(date.toSecsSinceEpoch()).arg(amount).arg(description));
    qDebug()<<pushQuery.lastError();
    loadDataFromDB();
    endResetModel();
}
