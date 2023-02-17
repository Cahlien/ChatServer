//
// Created by Matthew Crowell on 2/11/2023.
//

#include "src/dtos/DataMessage.h"
#include <QTextStream>
#include <QRegularExpression>
#include <QJsonObject>
#include <QJsonDocument>

/*!
 * \namespace dc
 * \inmodule ChatServer
 */

/*!
 * \class dc::DataMessage
 * \inmodule ChatServer
 * \brief The DataMessage class is used to represent a message that is sent between the client and the server.
 * The DataMessage class is used to represent a message that is sent between the client and the server.
 */

namespace dc
{
    quint64 DataMessage::m_id{};

    /*!
     * \brief Construct a new DataMessage object with the given client name.
     * \a client    The name of the client that is sending the message.
     * \inmodule ChatServer
     * \sa DataMessage
     */
    DataMessage::DataMessage(const QString& client)
    {
        QString clientName{client};
        clientName.remove(QRegularExpression("[^a-zA-Z0-9]"));

        this->add("id",
                             clientName +
                             QDateTime::currentDateTime().toString("ddMMyyyyhhmmsszzz") +
                             QString::number(++m_id)
                            );
        qDebug() << "Message id: " << this->get("id");
    }

    /*!
     * \fn DataMessage::get(const QString &key) const
     * \brief Get the value of the given key.
     * Get the value of the given key and returns it as a \c QStringView.
     * \a key The key to get the value for.
     */
    QStringView DataMessage::get(const QString &key) const
    {
        return QStringView{this->m_data->value(key)};
    }

    /*!
     * \brief Add a key/value pair to the message.
     * Add a key/value pair to the message and returns \c void.
     * \a key  The key to add.
     * \a value  The value to add.
     */
    void DataMessage::add(const QString &key, const QString &value)
    {
        this->m_data->insert(key, value);
    }

    QDataStream& operator<<(QDataStream &stream, const DataMessage& object)
    {
        QJsonObject jsonObject{};

        auto keys{object.m_data->keys()};

        std::for_each(keys.begin(), keys.end(), [&jsonObject, &object](const QString& key)
        {
            jsonObject.insert(key, object.m_data->value(key));
        });

        QJsonDocument jsonDocument{jsonObject};
        stream << jsonDocument.toJson(QJsonDocument::Compact);
        return stream;
    }


    QDataStream& operator>>(QDataStream &stream, DataMessage& object)
    {
        // turn stream into a QJsonDocument
        QByteArray byteArray{};
        stream >> byteArray;
        QJsonDocument jsonDocument{QJsonDocument::fromJson(byteArray)};
        QJsonObject jsonObject{jsonDocument.object()};
        QStringList keys{jsonObject.keys()};
        std::for_each(keys.begin(), keys.end(), [&jsonObject, &object](const QString& key)
        {
            object.m_data->insert(key, jsonObject.value(key).toString());
        });

        return stream;
    }

    std::ostream& operator<<(std::ostream &stream, const DataMessage &message)
    {
        QJsonObject jsonObject{};
        auto keys{message.m_data->keys()};
        std::for_each(keys.begin(), keys.end(), [&jsonObject, &message](const QString& key)
        {
            jsonObject.insert(key, message.m_data->value(key));
        });

        QJsonDocument jsonDocument{jsonObject};
        stream << jsonDocument.toJson(QJsonDocument::Indented).toStdString();

        return stream;
    }

    QDebug operator<<(QDebug debug, const DataMessage &message)
    {
        QJsonObject jsonObject{};
        auto keys{message.m_data->keys()};
        std::for_each(keys.begin(), keys.end(), [&jsonObject, &message](const QString& key)
        {
            jsonObject.insert(key, message.m_data->value(key));
        });

        QJsonDocument jsonDocument{jsonObject};
        debug << jsonDocument.toJson(QJsonDocument::Compact);

        return debug;
    }
} // dc