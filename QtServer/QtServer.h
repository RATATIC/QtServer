#ifndef QTSERVER_H
#define QTSERVER_H

#include <QMainWindow>
#include <QtNetwork>

QT_BEGIN_NAMESPACE
namespace Ui { class QtServer; }
QT_END_NAMESPACE

class QtServer : public QMainWindow
{
    Q_OBJECT

public:
    QtServer(QWidget *parent = nullptr);
    ~QtServer();

private slots:
    void clicked_button_open();
    void clicked_button_close();
    void newUser();
    void readFile();
    void readFileInfo();

private:
    QTcpServer* tcpServer;
    int server_status = 0;
    QFile* file;
    QMap<int, QTcpSocket*> SClients;
    qint64 fileSize;
    qint64 recivedFileSize;
    QTcpSocket* soc;

private:
    Ui::QtServer *ui;
};
#endif // QTSERVER_H
