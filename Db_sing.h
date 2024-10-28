#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <vector>
#include <string>

using std::vector;
using std::string;

class MyDataBase;

class SingletonDestroyer
{
    private:
        MyDataBase * p_instance;
    public:
        ~SingletonDestroyer() { delete p_instance;}
        void initialize(MyDataBase * p){p_instance = p;}
};


class MyDataBase
{
    private:
        static MyDataBase * p_instance;
        static SingletonDestroyer destroyer;
        QSqlDatabase db;
    protected:
        MyDataBase(){
            db = QSqlDatabase::addDatabase("QSQLITE");
            db.setDatabaseName("D:\\Progs\\Skynet\\Users.db");
            db.open();
            if(!db.isOpen())
                qDebug()<<db.lastError().text();
        }
        MyDataBase(const MyDataBase& )=delete;
        MyDataBase& operator = (MyDataBase &)= delete;
        ~MyDataBase() {db.close();}
        friend class SingletonDestroyer;
    public:
        static MyDataBase* getInstance(){
            if (!p_instance)
            {
                p_instance = new MyDataBase();
                destroyer.initialize(p_instance);
            }
            return p_instance;
        }
        QString query (QString q){
            QSqlQuery query_obj(db);
            query_obj.exec(q);
            QString res="";
//            if (db.isOpen()) qDebug() << "111";
            while (query_obj.next()){
                vector<QString> qwert;
                QString login = query_obj.value("login").toString();
                QString password = query_obj.value("password").toString();
                qwert.push_back(login);
                qwert.push_back(password);
                //qDebug()<<login <<" " << password<<"\n";
                res.append(login).append('|').append(password).append("||");
            }
            return res;
        }
        void add (QString login, QString password, QString email){
            QSqlQuery query(db);
            query.prepare("INSERT INTO User (login,password,email) VALUES(:login, :password, :email)");
            query.bindValue(":login", login);
            query.bindValue(":password", password);
            query.bindValue(":email", email);
            query.exec();
        }

        void login(QString login, QString socket)
        {
            QSqlQuery query(db);
            query.prepare("UPDATE User SET socket=:socket WHERE login=:login");
            query.bindValue(":socket", socket);
            query.bindValue(":login", login);
            query.exec();
        }

        void logout(QString login)
        {
            QSqlQuery query(db);
            query.prepare("UPDATE User SET socket=NULL WHERE login=:login");
            query.bindValue(":login", login);
            query.exec();
        }
        QByteArray stat(QString socket)
        {
            QSqlQuery query(db);
            query.prepare("SELECT task1, task2 FROM User WHERE socket=:socket;");
            query.bindValue(":socket", socket);
            query.exec();
            QString task1;
            QString task2;
            while (query.next()){
                task1 = query.value("task1").toString();
                task2 = query.value("task2").toString();
                qDebug() << "@" << task1 << " & " << task2 << "$\n";
            }
//            QByteArray stata = query.value("stat").toByteArray();
//            qDebug() << query.executedQuery() << "###";
            QByteArray stat = QByteArray::fromStdString((task1+" "+task2).toStdString());
            qDebug()<<stat;
            return "check "+stat;
        }
        QByteArray stat_all()
        {
            QSqlQuery query(db);
            query.prepare("SELECT * FROM User;");
            query.exec();
            QString res="";
            QString task2;
            int ind_1 = query.record().indexOf("task1");
            int ind_2 = query.record().indexOf("task2");
            while (query.next()){
                res+= query.value(ind_1).toString()+" "+query.value(ind_2).toString()+" ";
            }
            //res = "eewwe";
//            QByteArray stata = query.value("stat").toByteArray();
//            qDebug() << query.executedQuery() << "###";
            //QByteArray stat = QByteArray::fromStdString((task1+" "+task2).toStdString());
            //return stat;
            return "stat_all "+res.toUtf8();
        }

};


//MyDataBase * SingletonDestroyer::p_instance;
