//
// Created by Matth on 2/18/2023.
//

#ifndef CHATSERVER_SOCKETREADER_H
#define CHATSERVER_SOCKETREADER_H

#include <QObject>
#include <QRunnable>
#include <QTcpSocket>
#include <QMutexLocker>

#include "src/dtos/DataMessage.h"

class SocketReader : public QObject, public QRunnable
{
Q_OBJECT
public:
    explicit SocketReader(QObject* parent = nullptr, QTcpSocket* socket = nullptr);
    ~SocketReader() override = default;

    void write(const dc::DataMessage* message);
    dc::DataMessage* read();
    void run() override;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "NotImplementedFunctions"

signals:
    void writeData(const dc::DataMessage* message);

#pragma clang diagnostic pop

private:
    QTcpSocket* m_socket;
    QMutex m_mutex;
    QDataStream m_stream;
};

#endif //CHATSERVER_SOCKETREADER_H
