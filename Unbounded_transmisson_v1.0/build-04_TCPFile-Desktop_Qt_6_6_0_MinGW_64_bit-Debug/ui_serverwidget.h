/********************************************************************************
** Form generated from reading UI file 'serverwidget.ui'
**
** Created by: Qt User Interface Compiler version 6.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SERVERWIDGET_H
#define UI_SERVERWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_serverWidget
{
public:
    QGridLayout *gridLayout;
    QPushButton *buttonFile;
    QTextEdit *textEdit;
    QLabel *label;
    QSpacerItem *horizontalSpacer_3;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *buttonSend;
    QPushButton *pushButton;

    void setupUi(QWidget *serverWidget)
    {
        if (serverWidget->objectName().isEmpty())
            serverWidget->setObjectName("serverWidget");
        serverWidget->resize(460, 360);
        gridLayout = new QGridLayout(serverWidget);
        gridLayout->setObjectName("gridLayout");
        buttonFile = new QPushButton(serverWidget);
        buttonFile->setObjectName("buttonFile");

        gridLayout->addWidget(buttonFile, 2, 0, 1, 1);

        textEdit = new QTextEdit(serverWidget);
        textEdit->setObjectName("textEdit");

        gridLayout->addWidget(textEdit, 1, 0, 1, 3);

        label = new QLabel(serverWidget);
        label->setObjectName("label");
        QFont font;
        font.setFamilies({QString::fromUtf8("Rockwell")});
        font.setPointSize(26);
        label->setFont(font);
        label->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label, 0, 1, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_3, 0, 2, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 0, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(143, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 2, 1, 1, 1);

        buttonSend = new QPushButton(serverWidget);
        buttonSend->setObjectName("buttonSend");

        gridLayout->addWidget(buttonSend, 2, 2, 1, 1);

        pushButton = new QPushButton(serverWidget);
        pushButton->setObjectName("pushButton");

        gridLayout->addWidget(pushButton, 3, 2, 1, 1);


        retranslateUi(serverWidget);

        QMetaObject::connectSlotsByName(serverWidget);
    } // setupUi

    void retranslateUi(QWidget *serverWidget)
    {
        serverWidget->setWindowTitle(QCoreApplication::translate("serverWidget", "serverWidget", nullptr));
        buttonFile->setText(QCoreApplication::translate("serverWidget", "choose", nullptr));
        label->setText(QCoreApplication::translate("serverWidget", "Server", nullptr));
        buttonSend->setText(QCoreApplication::translate("serverWidget", "send", nullptr));
        pushButton->setText(QCoreApplication::translate("serverWidget", "disconnect", nullptr));
    } // retranslateUi

};

namespace Ui {
    class serverWidget: public Ui_serverWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SERVERWIDGET_H
