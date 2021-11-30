#include "setting.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>

Setting::Setting(QObject *parent)
    : QObject(parent) {

}

Setting::~Setting() {

}

QVariant Setting::get(const QString& key) const {
    SettingMap::ConstIterator it = m_map.find(key);
    if(it != m_map.end()) {
        return it.value();
    } else {
        return QVariant();
    }
}

void Setting::set(const QString& key, const QVariant& value) {
    m_changed = true;
    m_map.remove(key);
    m_map.insert(key, value);
}

bool Setting::load(const QString& fileName) {
    m_map.clear();
    QFile file(fileName);
    if(file.open(QIODevice::ReadOnly)) {
        m_fileName = fileName;
        QByteArray byte_array =  file.readAll();
        file.close();
        QJsonParseError json_error;
        QJsonDocument document = QJsonDocument::fromJson(byte_array,&json_error);
        if(json_error.error == QJsonParseError::NoError) {
            if(document.isObject()) {
                QJsonObject json = document.object();
                QJsonObject::ConstIterator it = json.begin();
                while(it != json.end()) {
                    m_map.insert(it.key(), it.value().toVariant());
                    it++;
                }
             }
         }
    }
    return m_map.size() >0 ? true : false;
}

bool Setting::save(const QString& fileName) {
    if(!m_changed) {
        return true;
    }

    QJsonObject json;
    SettingMap::Iterator it = m_map.begin();
    QJsonValue value;
    while(it != m_map.end()) {
        QJsonValue value = QJsonValue::fromVariant(it.value());
        json.insert(it.key(),value);
        it++;
    }

    QJsonDocument document;
    document.setObject(json);
    QByteArray byte_array = document.toJson(QJsonDocument::Indented);
    QFile file(fileName);
    if(file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        file.write(byte_array);
        file.close();
        file.flush();
        return true;
    }
    return false;
}

void Setting::file(const QString &fileName) {
    m_fileName = fileName;
}

bool Setting::load() {
    if(m_fileName.length() > 0) {
        return load(m_fileName);
    }
    return false;
}

bool Setting::save() {
    if(m_fileName.length() > 0) {
        return save(m_fileName);
    }
    return false;
}
