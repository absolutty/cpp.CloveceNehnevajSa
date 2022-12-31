//
// Created by adaha on 31. 12. 2022.
//

#ifndef CLOVECENEHNEVAJSA_POPUP_H
#define CLOVECENEHNEVAJSA_POPUP_H

#include <QWidget>

class Popup {
public:
    virtual void show(QWidget* parent, QString** resultData) = 0;
};

#endif //CLOVECENEHNEVAJSA_POPUP_H
