/********************************************************************************
** Form generated from reading UI file 'MainForm.ui'
**
** Created by: Qt User Interface Compiler version 5.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINFORM_H
#define UI_MAINFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>

QT_BEGIN_NAMESPACE

class Ui_MainForm
{
public:
    QLabel *label;
    QLineEdit *lineEdit;
    QPushButton *pushButton;
    QGroupBox *groupBox;
    QRadioButton *radioButton;
    QRadioButton *radioButton_2;
    QRadioButton *radioButton_3;
    QRadioButton *radioButton_4;
    QGraphicsView *graphicsView;
    QPushButton *pushButton_2;
    QLabel *label_2;

    void setupUi(QDialog *MainForm)
    {
        if (MainForm->objectName().isEmpty())
            MainForm->setObjectName(QStringLiteral("MainForm"));
        MainForm->resize(775, 587);
        label = new QLabel(MainForm);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(50, 60, 131, 16));
        lineEdit = new QLineEdit(MainForm);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(180, 60, 321, 20));
        pushButton = new QPushButton(MainForm);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(550, 60, 75, 23));
        groupBox = new QGroupBox(MainForm);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(550, 140, 181, 171));
        radioButton = new QRadioButton(groupBox);
        radioButton->setObjectName(QStringLiteral("radioButton"));
        radioButton->setGeometry(QRect(30, 40, 82, 17));
        radioButton_2 = new QRadioButton(groupBox);
        radioButton_2->setObjectName(QStringLiteral("radioButton_2"));
        radioButton_2->setGeometry(QRect(30, 70, 82, 17));
        radioButton_3 = new QRadioButton(groupBox);
        radioButton_3->setObjectName(QStringLiteral("radioButton_3"));
        radioButton_3->setGeometry(QRect(30, 100, 82, 17));
        radioButton_4 = new QRadioButton(groupBox);
        radioButton_4->setObjectName(QStringLiteral("radioButton_4"));
        radioButton_4->setGeometry(QRect(30, 130, 82, 17));
        graphicsView = new QGraphicsView(MainForm);
        graphicsView->setObjectName(QStringLiteral("graphicsView"));
        graphicsView->setGeometry(QRect(40, 130, 461, 401));
        pushButton_2 = new QPushButton(MainForm);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(590, 340, 75, 23));
        label_2 = new QLabel(MainForm);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(550, 400, 151, 16));
        QFont font;
        font.setPointSize(14);
        font.setItalic(true);
        label_2->setFont(font);

        retranslateUi(MainForm);

        QMetaObject::connectSlotsByName(MainForm);
    } // setupUi

    void retranslateUi(QDialog *MainForm)
    {
        MainForm->setWindowTitle(QApplication::translate("MainForm", "Clasificador de Retinopat\303\255a Diab\303\251tica", 0));
        label->setText(QApplication::translate("MainForm", "Seleccione una imagen:", 0));
        pushButton->setText(QApplication::translate("MainForm", "Buscar", 0));
        groupBox->setTitle(QApplication::translate("MainForm", "Severidad", 0));
        radioButton->setText(QApplication::translate("MainForm", "Normal", 0));
        radioButton_2->setText(QApplication::translate("MainForm", "Leve", 0));
        radioButton_3->setText(QApplication::translate("MainForm", "Moderado", 0));
        radioButton_4->setText(QApplication::translate("MainForm", "Severo", 0));
        pushButton_2->setText(QApplication::translate("MainForm", "Clasificar", 0));
        label_2->setText(QApplication::translate("MainForm", "El resultado es:", 0));
    } // retranslateUi

};

namespace Ui {
    class MainForm: public Ui_MainForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINFORM_H
