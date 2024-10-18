#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "scene.h"
#include <QVBoxLayout>
#include <QFileDialog>
#include <QColorDialog>

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

    QTimer *timer = new QTimer(this);

    connect(timer, &QTimer::timeout, this, &MainWindow::on_Timer);

    timer->start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

TScene Scene;

QString LoadFile(QString ModelName)
{
    QString ResourceName = ":/" + ModelName + ".ase";
    QString FileName = ModelName + ".ase";

    QFile ResourceFile(ResourceName);
    QFile ModelFile(FileName);

    if (ResourceFile.open(QIODevice::ReadOnly | QIODevice::Text) && ModelFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream in(&ResourceFile);
        QTextStream out(&ModelFile);

        while (!in.atEnd()) {
            QString line = in.readLine();
            out << line << "\n";
        }

        ResourceFile.close();
        ModelFile.close();
    }

    return FileName;
}

void MainWindow::UpdateLights()
{
    ui->ComboLight->clear();

    for (int i = 0; i < Scene.Lights.size(); i++)
        ui->ComboLight->addItem(QString::number(i));

    ui->ComboLight->setCurrentIndex(Scene.Lights.size() - 1);
}

void MainWindow::on_Timer() {
    if (Scene.Clock)
    {
        Scene.Models = Scene.SourceModels;
        Scene.Draw();
        drawingWidget->setPicture(Scene.Picture);
    }
}

void MainWindow::showEvent(QShowEvent *event) {
    QMainWindow::showEvent(event);

    QString FileName = LoadFile("Clock");
    Scene = TScene(FileName, ui->widgetDraw->width(), ui->widgetDraw->height());

    Scene.Draw();

    ui->SliderKa->setValue(Scene.Ka * 10);
    ui->SliderKd->setValue(Scene.Kd * 10);
    ui->SliderKs->setValue(Scene.Ks * 10);

    UpdateLights();

    drawingWidget->setPicture(Scene.Picture);
}


void MainWindow::on_ButtonMinusX_clicked()
{
    if (ui->RadioMove->isChecked())
        Scene.Move.X -= 20;

    if (ui->RadioRotate->isChecked())
        Scene.Rotate.Y += 20;

    if (ui->RadioCamera->isChecked())
        Scene.Camera.Alpha += 20;

    Scene.Models = Scene.SourceModels;
    Scene.Draw();

    drawingWidget->setPicture(Scene.Picture);
}


void MainWindow::on_ButtonPlusX_clicked()
{
    if (ui->RadioMove->isChecked())
        Scene.Move.X += 20;

    if (ui->RadioRotate->isChecked())
        Scene.Rotate.Y -= 20;

    if (ui->RadioCamera->isChecked())
        Scene.Camera.Alpha -= 20;

    Scene.Models = Scene.SourceModels;
    Scene.Draw();

    drawingWidget->setPicture(Scene.Picture);
}


void MainWindow::on_ButtonMinusY_clicked()
{
    if (ui->RadioMove->isChecked())
        Scene.Move.Y -= 20;

    if (ui->RadioRotate->isChecked())
        Scene.Rotate.X += 20;

    if (ui->RadioCamera->isChecked())
        Scene.Camera.Beta -= 20;

    Scene.Models = Scene.SourceModels;
    Scene.Draw();

    drawingWidget->setPicture(Scene.Picture);
}


void MainWindow::on_ButtonPlusY_clicked()
{
    if (ui->RadioMove->isChecked())
        Scene.Move.Y += 20;

    if (ui->RadioRotate->isChecked())
        Scene.Rotate.X -= 20;

    if (ui->RadioCamera->isChecked())
        Scene.Camera.Beta += 20;

    Scene.Models = Scene.SourceModels;
    Scene.Draw();

    drawingWidget->setPicture(Scene.Picture);
}


void MainWindow::on_SliderKa_valueChanged(int value)
{
    Scene.Ka = value / 10.0;

    Scene.Models = Scene.SourceModels;
    Scene.Draw();

    drawingWidget->setPicture(Scene.Picture);

}


void MainWindow::on_SliderKd_valueChanged(int value)
{
    Scene.Kd = value / 10.0;

    Scene.Models = Scene.SourceModels;
    Scene.Draw();

    drawingWidget->setPicture(Scene.Picture);
}


void MainWindow::on_SliderKs_valueChanged(int value)
{
    Scene.Ks = value / 10.0;

    Scene.Models = Scene.SourceModels;
    Scene.Draw();

    drawingWidget->setPicture(Scene.Picture);
}


void MainWindow::on_ButtonPlusZ_clicked()
{
    Scene.Scale.X += 0.2;
    Scene.Scale.Y += 0.2;
    Scene.Scale.Z += 0.2;

    Scene.Models = Scene.SourceModels;
    Scene.Draw();
}


void MainWindow::on_ButtonMinusZ_clicked()
{
    Scene.Scale.X -= 0.2;
    Scene.Scale.Y -= 0.2;
    Scene.Scale.Z -= 0.2;

    Scene.Models = Scene.SourceModels;
    Scene.Draw();
}


void MainWindow::on_ButtonLightAdd_clicked()
{
    TLight Light;

    Light.Point.X = 0;
    Light.Point.Y = 0;
    Light.Point.Z = 100;

    Light.Color = QColorDialog::getColor(Qt::white, this, "Выберите цвет");

    Scene.Lights.push_back(Light);

    UpdateLights();

    Scene.Models = Scene.SourceModels;
    Scene.Draw();
}


void MainWindow::on_ButtonLightDelete_clicked()
{
    if (Scene.Lights.size() > 1)
    {
        Scene.Lights.erase(Scene.Lights.begin() + ui->ComboLight->currentIndex());

        UpdateLights();

        Scene.Models = Scene.SourceModels;
        Scene.Draw();
    }
}


void MainWindow::on_ButtonLightMinusX_clicked()
{
    Scene.Lights[ui->ComboLight->currentIndex()].Point.X -= 20;

    Scene.Models = Scene.SourceModels;
    Scene.Draw();
}


void MainWindow::on_ButtonLightPlusX_clicked()
{
    Scene.Lights[ui->ComboLight->currentIndex()].Point.X += 20;

    Scene.Models = Scene.SourceModels;
    Scene.Draw();
}


void MainWindow::on_ButtonLightMinusY_clicked()
{
    Scene.Lights[ui->ComboLight->currentIndex()].Point.Y -= 20;

    Scene.Models = Scene.SourceModels;
    Scene.Draw();
}


void MainWindow::on_ButtonLightPlusY_clicked()
{
    Scene.Lights[ui->ComboLight->currentIndex()].Point.Y += 20;

    Scene.Models = Scene.SourceModels;
    Scene.Draw();
}

void MainWindow::on_ComboModel_currentTextChanged(const QString &arg1)
{
    QString FileName = LoadFile(arg1);
    Scene = TScene(FileName, ui->widgetDraw->width(), ui->widgetDraw->height());

    Scene.Draw();

    ui->SliderKa->setValue(Scene.Ka * 10);
    ui->SliderKd->setValue(Scene.Kd * 10);
    ui->SliderKs->setValue(Scene.Ks * 10);

    UpdateLights();

    drawingWidget->setPicture(Scene.Picture);
}

