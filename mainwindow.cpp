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
//TPicture Picture;


void MainWindow::on_buttonOpen_clicked()
{
    Scene = TScene();

    Scene.Picture = TPicture(ui->widgetDraw->width(), ui->widgetDraw->height());


    QString fileName = QFileDialog::getOpenFileName(nullptr, "Открыть файл", "", "Все файлы (*.*)");

    Scene.FileName = fileName;

    Scene.InitScene();

    Scene.Picture.InitBuffer();
    Scene.Picture.DrawBackground();

    Scene.Draw();

    drawingWidget->setPicture(Scene.Picture);
}

