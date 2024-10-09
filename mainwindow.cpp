#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "scene.h"
#include <QVBoxLayout>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
      drawingWidget(new DrawingWidget(this))
{
    ui->setupUi(this);

    QWidget *placeholder = ui->widgetMain->findChild<QWidget*>("widgetDraw");
    if (placeholder) {
        QVBoxLayout *layout = new QVBoxLayout;
        layout->addWidget(drawingWidget);
        placeholder->setLayout(layout);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

TScene Scene;

void MainWindow::on_buttonOpen_clicked()
{
    QString FileName = QFileDialog::getOpenFileName(nullptr, "Открыть файл", "", "Все файлы (*.*)");

    if (!FileName.isEmpty())
    {
        Scene = TScene(FileName, ui->widgetDraw->width(), ui->widgetDraw->height());

        Scene.Draw();

        drawingWidget->setPicture(Scene.Picture);
    }
}

