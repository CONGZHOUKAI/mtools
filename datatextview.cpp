#include "datatextview.h"
#include "ui_datatextview.h"
#include "sink.h"
#include <QDebug>
#include <QTime>

class DataTextViewSink : public Sink
{
public:
    DataTextViewSink(DataTextView* textView)
    {
        _textView = textView;
    }

protected:
    virtual void feedIn(const QVariant& data) override
    {
        _textView->addData(data);
    };

private:
    DataTextView* _textView;
};



DataTextView::DataTextView(Sink * sink, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DataTextView)
{
    ui->setupUi(this);
    _sink = new DataTextViewSink(this);
    sink->connectFollower(_sink);
}

DataTextView::~DataTextView()
{
    delete ui;
}

QString insertSpacesInHexString(const QString &hexString) {
    QString spacedHexString;
    for (int i = 0; i < hexString.length(); i += 2) {
        spacedHexString += hexString.mid(i, 2);
        if (i + 2 < hexString.length()) {
            spacedHexString += " ";
        }
    }
    return spacedHexString;
}

void DataTextView::addData(const QVariant& data)
{
    bool ishex = ui->cbHEX->checkState();
    bool isTimestamp = ui->cbTimestamp->checkState();
    bool isMerge = ui->cbMerge->checkState();

    QString prefixStr =  isTimestamp ? QTime::currentTime().toString("HH:mm:ss.zzz") : "";
    QString dirStrIn = " >> ";
    QString rawDataStr =  ishex ? insertSpacesInHexString(data.toByteArray().toHex().toUpper()) : data.toString();


    if (isMerge) {
        QString mergedData;
        QStringList lines =  ui->tbTextView->toPlainText().split("\n");
        foreach (const QString &line, lines) {
            QString _line = line;

            if( _line.split(dirStrIn).last() == rawDataStr) {
                continue;
            }
            mergedData += line+"\n";
        }
        ui->tbTextView->clear();
        if(mergedData.length() > 0)
            ui->tbTextView->append(mergedData.chopped(1));

    }
    ui->tbTextView->append(prefixStr + dirStrIn + rawDataStr);
}


void DataTextView::on_pbClear_clicked()
{
    ui->tbTextView->clear();
}


void DataTextView::on_cbMerge_stateChanged(int arg1)
{
    if (arg1) {
        ui->tbTextView->clear();
    }
}

