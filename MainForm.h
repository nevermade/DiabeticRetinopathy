/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MainForm.h
 * Author: USER
 *
 * Created on 31 de agosto de 2016, 11:57 PM
 */

#ifndef _MAINFORM_H
#define _MAINFORM_H

#include "ui_MainForm.h"

class MainForm : public QDialog {
    Q_OBJECT
public:
    MainForm();
    virtual ~MainForm();
private:
    Ui::MainForm widget;
};

#endif /* _MAINFORM_H */
