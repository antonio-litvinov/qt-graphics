#include "drawingwidget.h"

DrawingWidget::DrawingWidget(QWidget *parent)
    : QWidget{parent}
{

}

void DrawingWidget::setPicture(const TPicture &new_picture)
{
    picture = new_picture;
    update();
}

void DrawingWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    for (int i = 0; i < picture.GetHeight(); ++i) {
        for (int j = 0; j < picture.GetWidth(); ++j) {
            painter.setPen(picture.GetColor(i,j));
            painter.drawPoint(i,j);
        }
    }
}
