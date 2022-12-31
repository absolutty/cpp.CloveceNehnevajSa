//
// Created by adaha on 31. 12. 2022.
//
#include <QString>
#include <QInputDialog>
#include <QDir>
#include "Popup.h"
#include <iostream>

class NumberOfPlayers : public Popup{
public:
    void show(QWidget* parent, QString** resultData) override {
        bool ok;
        QString text = QInputDialog::getText(parent, "QInputDialog::getText()",
                                             "User name:", QLineEdit::Normal,
                                             QDir::home().dirName(), &ok);

        text.toInt(&ok);

        if (ok && !text.isEmpty()) {
            *resultData = new QString(text);
        } else {
            *resultData = nullptr;
        }
    }
};


