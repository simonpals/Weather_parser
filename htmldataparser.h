#ifndef HTMLDATAPARER_H
#define HTMLDATAPARER_H

#include <QObject>

class QWebEnginePage;

class HTMLDataParser : public QObject
{
    Q_OBJECT
public:
    explicit HTMLDataParser(QObject *parent = nullptr);
    ~HTMLDataParser();

    QString toJSONFormat(const QList<QStringList> &table);
    QList<QStringList> parseWeather(const QString &url);
    QList<QStringList> parseCities(const QString &url);

signals:
    void loadPageFinished(QList<QStringList> table);

protected slots:
    void pageFinished(bool);

protected:
    void examineChildElements(QWebEnginePage *parentElement);
    void reсreateWebPage();    
    void clear();

private:
    QWebEnginePage *wPage;
};

#endif // HTMLDATAPARER_H
