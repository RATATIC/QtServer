#include "QtServer.h"
#include "ui_qtserver.h"

#define BUFFER_SIZE 1024

QtServer::QtServer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::QtServer)
{
    ui->setupUi(this);

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(clicked_button_open()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(clicked_button_close()));
}

void QtServer::clicked_button_open() {
    tcpServer = new QTcpServer(this);
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newUser()));
    if (!tcpServer->listen(QHostAddress::LocalHost, 6666) && server_status == 0) {
        qDebug() << QObject::tr("Unable to start the server: %1.").arg(tcpServer->errorString());

    }
    else {
        server_status = 1;
        qDebug() << tcpServer->isListening() << " TCPSocket listen on port";
        qDebug() << "Server started";
    }
}

void QtServer::clicked_button_close() {
    if (server_status == 1) {

        foreach(int i, SClients.keys()) {
            QTextStream os(SClients[i]);
            os.setAutoDetectUnicode(true);
            os << QDateTime::currentDateTime().toString() << "\n";
            SClients[i]->close();
            SClients.remove(i);
        }

        tcpServer->close();
        qDebug() << "Server stopped";
        server_status = 0;
    }
}

void QtServer::newUser() {

    qDebug() << "start one";

    if (server_status == 1) {
        qDebug() << "New connection";
        QTcpSocket* clientSocket = tcpServer->nextPendingConnection();
        int idusersocs = clientSocket->socketDescriptor();

        SClients[idusersocs] = clientSocket;
        connect(SClients[idusersocs], SIGNAL(readyRead()), this, SLOT(readFileInfo()));
    }
}

void QtServer::readFileInfo() {
    soc = (QTcpSocket*)sender();
    int idusersocs = soc->socketDescriptor();

    QDataStream in(soc);
    QString filename;
    recivedFileSize = 0;

    in >> fileSize;
    in >> filename;

    qDebug() << fileSize << filename;

    file = new QFile(filename);

    if (file->open(QIODevice::WriteOnly)) {
        disconnect(SClients[idusersocs], SIGNAL(readyRead()), this, SLOT(readFileInfo()));
        connect(SClients[idusersocs], SIGNAL(readyRead()), this, SLOT(readFile()));
    }
}
void QtServer::readFile() {
    QDataStream in(soc);
    char buffer[BUFFER_SIZE];
    while (!in.atEnd()) {
        qint64 toFile = in.readRawData(buffer, sizeof(buffer));
        recivedFileSize += toFile;
        file->write(buffer, toFile);
    }

    if (recivedFileSize == fileSize) {
        file->close();
        file = NULL;

        int idusersocs = soc->socketDescriptor();
        disconnect(SClients[idusersocs], SIGNAL(readyRead()), this, SLOT(readFile()));
        soc->close();
        SClients.remove(idusersocs);
        qDebug() << "end read file";
    }
}

QtServer::~QtServer()
{
    delete ui;
}

