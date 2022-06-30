#pragma once

#include <QtWidgets/QMainWindow>
#include <QtNetwork>
#include "ui_Qtsever.h"

class Qtsever : public QMainWindow
{
    Q_OBJECT

public:
    Qtsever(QWidget *parent = nullptr);
    ~Qtsever();

private:
    Ui::QtseverClass ui;
private slots:
    void clicked_button_open();
    void clicked_button_close();
    void newuser();
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
};
