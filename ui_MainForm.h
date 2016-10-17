/********************************************************************************
** Form generated from reading UI file 'MainForm.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINFORM_H
#define UI_MAINFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_MainForm
{
public:
    QPushButton *btnSearch;
    QGraphicsView *graphicsView;
    QLineEdit *txtPath;
    QLabel *label;
    QLabel *label_2;
    QComboBox *comboBox;
    QLabel *label_3;
    QLabel *label_4;

    void setupUi(QDialog *MainForm)
    {
        if (MainForm->objectName().isEmpty())
            MainForm->setObjectName(QStringLiteral("MainForm"));
        MainForm->resize(892, 630);
        btnSearch = new QPushButton(MainForm);
        btnSearch->setObjectName(QStringLiteral("btnSearch"));
        btnSearch->setGeometry(QRect(590, 80, 75, 23));
        graphicsView = new QGraphicsView(MainForm);
        graphicsView->setObjectName(QStringLiteral("graphicsView"));
        graphicsView->setGeometry(QRect(40, 150, 531, 451));
        txtPath = new QLineEdit(MainForm);
        txtPath->setObjectName(QStringLiteral("txtPath"));
        txtPath->setGeometry(QRect(170, 80, 401, 20));
        label = new QLabel(MainForm);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(60, 80, 111, 16));
        label_2 = new QLabel(MainForm);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(590, 170, 131, 16));
        comboBox = new QComboBox(MainForm);
        comboBox->setObjectName(QStringLiteral("comboBox"));
        comboBox->setGeometry(QRect(730, 170, 131, 22));
        label_3 = new QLabel(MainForm);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(590, 260, 91, 16));
        label_4 = new QLabel(MainForm);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(710, 340, 47, 13));

        retranslateUi(MainForm);
        QObject::connect(btnSearch, SIGNAL(clicked()), MainForm, SLOT(selectImage()));

        QMetaObject::connectSlotsByName(MainForm);
    } // setupUi

    void retranslateUi(QDialog *MainForm)
    {
        MainForm->setWindowTitle(QApplication::translate("MainForm", "MainForm", 0));
        btnSearch->setText(QApplication::translate("MainForm", "Buscar", 0));
        label->setText(QApplication::translate("MainForm", "Seleccionar Imagen:", 0));
        label_2->setText(QApplication::translate("MainForm", "Seleccionar tipo de imagen:", 0));
        comboBox->clear();
        comboBox->insertItems(0, QStringList()
         << QApplication::translate("MainForm", "Normal", 0)
         << QApplication::translate("MainForm", "Leve", 0)
         << QApplication::translate("MainForm", "Moderado", 0)
         << QApplication::translate("MainForm", "Severo", 0)
        );
        label_3->setText(QApplication::translate("MainForm", "El resultado es:", 0));
        label_4->setText(QApplication::translate("MainForm", "Resultado", 0));
    } // retranslateUi

};

namespace Ui {
    class MainForm: public Ui_MainForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINFORM_H
