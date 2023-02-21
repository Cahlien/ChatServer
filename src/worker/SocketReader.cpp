//
// Created by Matthew Crowell on 2/18/2023.
//

#include "src/worker/SocketReader.h"
#include <QThread>

SocketReader::SocketReader(QObject *parent, QTcpSocket *socket)
    : QObject{parent}
    , m_socket{socket}
    , m_mutex{}
    , m_stream{socket}
{
    connect(this, &SocketReader::writeData, m_socket, [this](const dc::DataMessage* message) -> void {
        QMutexLocker locker{&this->m_mutex};
        m_stream << (*message);
        qDebug() << QThread::currentThread() << ":" <<  *message;
        qDebug() << "Data:" << *message;
    });
}

void SocketReader::run()
{
    auto received{read()};
    write(received);
}

dc::DataMessage* SocketReader::read()
{
    QMutexLocker locker{&m_mutex};
    auto data{new dc::DataMessage{}};
    m_stream >> (*data);
    qDebug() << QThread::currentThread() << ":" <<  *data;
    return data;
}
void SocketReader::write(const dc::DataMessage* message)
{
    emit writeData(message);
}
