/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   MainForm.cpp
 * Author: USER
 *
 * Created on 16 de octubre de 2016, 10:19 PM
 */

#include "MainForm.h"
#include "QFileDialog"
MainForm::MainForm() {
    widget.setupUi(this);
    connect(widget.btnSearch, SIGNAL(clicked()), this, SLOT(clickBtnSearch()));
}

MainForm::~MainForm() {

}

void MainForm::clickBtnSearch() {
    QString path = QFileDialog::getOpenFileName(this, tr("(*.tif)"), "/");
    if (path.isNull() == false) {
        widget.txtPath->setText(path);
    }
}


