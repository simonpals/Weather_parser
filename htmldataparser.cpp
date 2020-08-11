#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QEventLoop>
#include <QJsonObject>
#include <QJsonDocument>
#include <QWebEnginePage>
#include <QCoreApplication>

#include "qhtmlparser.h"
#include "htmldataparser.h"

HTMLDataParser::HTMLDataParser(QObject *parent) : QObject(parent)
{
    wPage = nullptr;
}

HTMLDataParser::~HTMLDataParser()
{
    clear();
}

QString HTMLDataParser::toJSONFormat(const QList<QStringList> &table)
{
    QJsonObject recordObject;

    int number = 1;
    for(const auto &row : table)
    {
        int key = 1;
        QJsonObject cityObject;

        for(const auto &col : row)
        {
            cityObject.insert("Key"+QString::number(key++), QJsonValue::fromVariant(col));
        }

        if(row.count())
            recordObject.insert("City"+QString::number(number++), cityObject);
    }

    QJsonDocument doc(recordObject);
    return  doc.toJson();
}

QList<QStringList> HTMLDataParser::parseWeather(const QString &url)
{
    QNetworkAccessManager *manager = new QNetworkAccessManager;
    QEventLoop loop;
    QObject::connect(manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
    QNetworkReply *reply = manager->get(QNetworkRequest(QUrl(url)));
    loop.exec();
    QString data = reply->readAll();

    data = data.replace("&nbsp;", "");

    QList<QStringList> table;
    QHtmlDocument document;
    const bool ok = document.setContent(data);

    if(ok)
    {
        const QHtmlElement html = document.htmlElement();

        const QHtmlElementList divs = html.elementsByTagName("tr");
        foreach (const QHtmlElement &div, divs)
        {
            table.push_back(QStringList());

            const QHtmlElementList cols = div.elementsByTagName("td");

            foreach (const QHtmlElement &col, cols)
            {
                QString text = col.text();
                text = text.simplified();

                if(!col.text().isEmpty())
                {
                    table.last().push_back(text);
                }
                qDebug() << col.tagName() << "   " << col.text();
            }

            if(table.last().size() == 0)
            {
                table.removeLast();
            }
        }
    }

    return table;
}

QList<QStringList> HTMLDataParser::parseCities(const QString &url)
{
    QList<QStringList> table;
    QEventLoop loop;

    reсreateWebPage();

    QObject::connect(this, &HTMLDataParser::loadPageFinished, [&table, &loop](QList<QStringList> table_){
        table = table_;
        loop.quit();
    });

    wPage->load(url);
    loop.exec();

    return table;
}

void HTMLDataParser::reсreateWebPage()
{
    clear();
    wPage = new QWebEnginePage();
    connect(wPage, SIGNAL(loadFinished(bool)), this, SLOT(pageFinished(bool)));
}

void HTMLDataParser::pageFinished(bool)
{
    examineChildElements(wPage);
}

void HTMLDataParser::clear()
{
    if (wPage != nullptr)
    {
        wPage->triggerAction(QWebEnginePage::Stop);
        delete wPage;
        wPage = nullptr;
    }
}

void HTMLDataParser::examineChildElements( QWebEnginePage *parentElement)
{

    parentElement->toPlainText([this, parentElement]( const QString &text)
    {
        QString code = QStringLiteral("function getValueFromSelectTag(){"
                                      "var nCount = document.getElementById('selectTimePeriod').children.length;"
                                      "var s = \"\";"
                                      "var i;"
                                      "for(i = 0; i < nCount; i++){"
                                      "s+=\";\"+document.getElementById('selectTimePeriod').children[i].text;"
                                      "s+=\"#\"+document.getElementById('selectTimePeriod').children[i].getAttribute('value');}"
                                      "return s;} getValueFromSelectTag();"
                                      );

        parentElement->runJavaScript( code, [this]( const QVariant &result) {
            auto data = result.toString();
            QList<QStringList> table;

            QStringList list = data.split(";");
            for(int i = 2; i < list.size(); i++){
                QStringList cols = list.at(i).split("#");
                table.push_back(cols);
            }
            emit loadPageFinished(table);
        });
    });
}


