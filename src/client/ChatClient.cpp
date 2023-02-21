//
// Created by Matthew Crowell on 2/18/2023.
//

#include "src/client/ChatClient.h"

dc::ChatClient::ChatClient(QObject *parent, QTcpServer* server)
        : QObject{parent}
        , m_socket{server->nextPendingConnection()}
{
    connect(m_socket.get(), &QTcpSocket::readyRead, this, &ChatClient::readyRead);
    connect(m_socket.get(), &QTcpSocket::disconnected, this, &ChatClient::disconnected);
}

dc::ChatClient::ChatClient(dc::ChatClient &&other) noexcept
    : QObject{other.parent()}
    , m_socket{std::move(other.m_socket)}
{
}

dc::ChatClient &dc::ChatClient::operator=(dc::ChatClient &&other) noexcept
{
    this->setParent(other.parent());
    other.setParent(nullptr);
    m_socket.swap(other.m_socket);
    return *this;
}

QTcpSocket *dc::ChatClient::socket() const
{
    return this->m_socket.get();
}
