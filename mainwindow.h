#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QTimer>
#include <QMainWindow>
#include "drawingwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void UpdateLights();

protected:
    void showEvent(QShowEvent *event) override;

private slots:
    void on_Timer();

    void on_ButtonMinusX_clicked();

    void on_ButtonPlusX_clicked();

    void on_ButtonPlusY_clicked();

    void on_ButtonMinusY_clicked();

    void on_SliderKa_valueChanged(int value);

    void on_SliderKd_valueChanged(int value);

    void on_SliderKs_valueChanged(int value);

    void on_ButtonPlusZ_clicked();

    void on_ButtonMinusZ_clicked();

    void on_ButtonLightAdd_clicked();

    void on_ButtonLightDelete_clicked();

    void on_ButtonLightMinusX_clicked();

    void on_ButtonLightPlusX_clicked();

    void on_ButtonLightMinusY_clicked();

    void on_ButtonLightPlusY_clicked();

    void on_ComboModel_currentTextChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    DrawingWidget *drawingWidget;
};
#endif // MAINWINDOW_H
