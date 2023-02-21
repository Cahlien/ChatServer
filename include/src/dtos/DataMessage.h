//
// Created by Matthew Crowell on 2/11/2023.
//

#ifndef CHATSERVER_DataMessage_H
#define CHATSERVER_DataMessage_H

#include <QDataStream>
#include <QPair>
#include <vector>
#include <ostream>
#include <QString>
#include <unordered_map>
#include <QHash>
#include <QDateTime>

namespace dc
{
    class DataMessage
    {
    public:
        DataMessage() = default;
        explicit DataMessage(const QString&);
        virtual ~DataMessage() = default;

        [[nodiscard]]
        QStringView get(const QString&) const;
        void add(const QString&, const QString&);

        friend QDataStream& operator>>(QDataStream&, dc::DataMessage&);
        friend QDataStream& operator<<(QDataStream&, const dc::DataMessage&);
        friend std::ostream& operator<<(std::ostream&, const dc::DataMessage&);
        friend QDebug operator<<(QDebug, const dc::DataMessage&);

    private:
        static quint64 m_id;
        QScopedPointer<QHash<QString, QString>> m_data {new QHash<QString, QString>()};
    };

} // dc

#endif //CHATSERVER_DataMessage_H
