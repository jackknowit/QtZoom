#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QScrollArea>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    // Create scroll area
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    
    // Create drawing board
    drawingBoard = new DrawingBoard(scrollArea);
    
    // Set drawing board as the widget in scroll area
    scrollArea->setWidget(drawingBoard);
    
    // Set scroll area as central widget
    setCentralWidget(scrollArea);
    
    // Set window title and size
    setWindowTitle("Drawing Board");
    resize(800, 600);
}

MainWindow::~MainWindow()
{
    delete ui;
} 