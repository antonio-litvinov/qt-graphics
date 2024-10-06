#include "drawingwidget.h"

DrawingWidget::DrawingWidget(QWidget *parent)
    : QWidget{parent}
{

}

void DrawingWidget::setPicture(TPicture &new_picture)
{
    picture = new_picture;
    update();
}

void DrawingWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if (picture.GetWidth() > 0)
    {
        for (int i = 0; i < picture.GetWidth(); i++) {
            for (int j = 0; j < picture.GetHeight(); j++) {
                painter.setPen(picture.GetColor(i,j));
                painter.drawPoint(i,j);
            }
        }
     }

}
