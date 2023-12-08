/********************************************************************************
** Form generated from reading UI file 'clientwidget.ui'
**
** Created by: Qt User Interface Compiler version 6.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CLIENTWIDGET_H
#define UI_CLIENTWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_clientWidget
{
public:
    QGridLayout *gridLayout;
    QSpacerItem *horizontalSpacer;
    QProgressBar *progressBar;
    QLabel *label_2;
    QLineEdit *lineEditIP;
    QLabel *label_3;
    QLineEdit *lineEditPort;
    QSpacerItem *horizontalSpacer_2;
    QLabel *label;
    QPushButton *buttonConnect;
    QTextEdit *textEdit;
    QPushButton *buttonDisconnect;

    void setupUi(QWidget *clientWidget)
    {
        if (clientWidget->objectName().isEmpty())
            clientWidget->setObjectName("clientWidget");
        clientWidget->resize(400, 300);
        gridLayout = new QGridLayout(clientWidget);
        gridLayout->setObjectName("gridLayout");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 0, 1, 2);

        progressBar = new QProgressBar(clientWidget);
        progressBar->setObjectName("progressBar");
        progressBar->setValue(24);

        gridLayout->addWidget(progressBar, 4, 0, 1, 5);

        label_2 = new QLabel(clientWidget);
        label_2->setObjectName("label_2");

        gridLayout->addWidget(label_2, 3, 0, 1, 1);

        lineEditIP = new QLineEdit(clientWidget);
        lineEditIP->setObjectName("lineEditIP");

        gridLayout->addWidget(lineEditIP, 2, 1, 1, 3);

        label_3 = new QLabel(clientWidget);
        label_3->setObjectName("label_3");
        QFont font;
        font.setFamilies({QString::fromUtf8("Rockwell")});
        font.setPointSize(26);
        label_3->setFont(font);

        gridLayout->addWidget(label_3, 0, 2, 1, 1);

        lineEditPort = new QLineEdit(clientWidget);
        lineEditPort->setObjectName("lineEditPort");

        gridLayout->addWidget(lineEditPort, 3, 1, 1, 3);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 0, 3, 1, 2);

        label = new QLabel(clientWidget);
        label->setObjectName("label");

        gridLayout->addWidget(label, 2, 0, 1, 1);

        buttonConnect = new QPushButton(clientWidget);
        buttonConnect->setObjectName("buttonConnect");

        gridLayout->addWidget(buttonConnect, 2, 4, 2, 1);

        textEdit = new QTextEdit(clientWidget);
        textEdit->setObjectName("textEdit");

        gridLayout->addWidget(textEdit, 5, 0, 1, 5);

        buttonDisconnect = new QPushButton(clientWidget);
        buttonDisconnect->setObjectName("buttonDisconnect");

        gridLayout->addWidget(buttonDisconnect, 6, 4, 1, 1);


        retranslateUi(clientWidget);

        QMetaObject::connectSlotsByName(clientWidget);
    } // setupUi

    void retranslateUi(QWidget *clientWidget)
    {
        clientWidget->setWindowTitle(QCoreApplication::translate("clientWidget", "Form", nullptr));
        label_2->setText(QCoreApplication::translate("clientWidget", "server port :", nullptr));
        lineEditIP->setText(QCoreApplication::translate("clientWidget", "127.0.0.1", nullptr));
        label_3->setText(QCoreApplication::translate("clientWidget", "Hello, user!", nullptr));
        lineEditPort->setText(QCoreApplication::translate("clientWidget", "8000", nullptr));
        label->setText(QCoreApplication::translate("clientWidget", "server IP :", nullptr));
        buttonConnect->setText(QCoreApplication::translate("clientWidget", "connect", nullptr));
        buttonDisconnect->setText(QCoreApplication::translate("clientWidget", "disconnect", nullptr));
    } // retranslateUi

};

namespace Ui {
    class clientWidget: public Ui_clientWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLIENTWIDGET_H
