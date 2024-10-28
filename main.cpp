#include <QCoreApplication>
#include "mytcpserver.h"

//#include <filesystem>
//#include <iostream>

#include "Db_sing.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    MyTcpServer server;
//    std::cout << std::filesystem::current_path() << "\n";
//    qDebug()<< MyDataBase::getInstance()->query("SELECT * FROM User");
//    qDebug()<< MyDataBase::getInstance()->query("SELECT name FROM sqlite_master WHERE type='table';");
//    qDebug()<<MyDataBase::getInstance()->stat_all();
    return a.exec();
}
