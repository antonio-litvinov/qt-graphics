#ifndef DRAWINGWIDGET_H
#define DRAWINGWIDGET_H

#include "picture.h"
#include <QWidget>
#include <QPainter>

class DrawingWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DrawingWidget(QWidget *parent = nullptr);

    void setPicture(TPicture &new_picture);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    TPicture picture;

signals:

};

#endif // DRAWINGWIDGET_H
