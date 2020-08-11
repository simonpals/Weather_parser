#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "htmldataparser.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    htmlParser = new HTMLDataParser;
}

MainWindow::~MainWindow()
{
    delete ui;
    delete htmlParser;
}

void MainWindow::on_pushButton_clicked()
{
    ui->textBrowser->clear();

    QString url = "http://space-weather.ru/api/ion/data/25601960.html";
    QList<QStringList> table = htmlParser->parseWeather(url);

    for(const auto &row : table)
    {
        QString rowText;
        for(const auto &col : row)
        {
            rowText += col+"  ";
        }
        ui->textBrowser->append(rowText);
        ui->textBrowser->append("\n");
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->textBrowser_2->clear();

    QString url = "http://space-weather.ru/index.php?page=ionogrammy";
    ui->textBrowser_2->append(htmlParser->toJSONFormat(htmlParser->parseCities(url)));
}
