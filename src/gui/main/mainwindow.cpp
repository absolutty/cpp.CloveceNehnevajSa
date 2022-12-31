#include <QMessageBox>
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <iostream>
#include "../popups/NumberOfPlayers.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    NumberOfPlayers playerNumberPopup;
    QString* string;
    playerNumberPopup.show(this, &string);

    if (string != nullptr) { //START game
        std::cout << "Hra bude obsahovat " << string->toInt() << " hracov." <<std::endl;
    } else { //ABANDON game
        throw std::invalid_argument("Nespravne zadany pocet!");
    }

    delete string;
}

MainWindow::~MainWindow()
{
    delete ui;
}

