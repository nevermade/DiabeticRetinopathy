/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MainForm.h
 * Author: USER
 *
 * Created on 16 de octubre de 2016, 10:19 PM
 */

#ifndef _MAINFORM_H
#define _MAINFORM_H

#include "ui_MainForm.h"

class MainForm : public QDialog {
    Q_OBJECT
public:
    MainForm();
    virtual ~MainForm();

public slots:
    void clickBtnSearch();  
    
private:
    Ui::MainForm widget;
};

#endif /* _MAINFORM_H */
