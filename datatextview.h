#ifndef DATATEXTVIEW_H
#define DATATEXTVIEW_H

#include <QWidget>
#include "sink.h"

namespace Ui {
class DataTextView;
}

class DataTextView : public QWidget
{
    Q_OBJECT

public:
    explicit DataTextView(Sink * sink, QWidget *parent = nullptr);
    ~DataTextView();
    void addData(const QVariant& data);

private slots:
    void on_pbClear_clicked();

    void on_cbMerge_stateChanged(int arg1);

private:
    Ui::DataTextView *ui;
    Sink *_sink;
};

#endif // DATATEXTVIEW_H
