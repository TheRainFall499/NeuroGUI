#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->label->setText("Channels: ");
    ui->horizontalSlider->setMaximum(8);
    ui->horizontalSlider->setMinimum(0);
    ui->spinBox->setMaximum(8);
    ui->spinBox->setMinimum(0);

    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), ui->spinBox, SLOT(setValue(int)));
    connect(ui->spinBox, SIGNAL(valueChanged(int)), ui->horizontalSlider, SLOT(setValue(int)));

    ui->startButton->setText("Start");
    ui->stopButton->setText("Stop");
}

MainWindow::~MainWindow()
{
    delete ui;
}
