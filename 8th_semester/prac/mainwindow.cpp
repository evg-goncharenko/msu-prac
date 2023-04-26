#include "mainwindow.h"
#include <iostream>

#include "fund.cpp"




MainWindow::MainWindow(QWidget *parent)
  : QWidget(parent)
{
        setFixedSize(1000, 800);
        setWindowTitle("Система управления инвестиционным портфелем");

        labels.push_back( new QLabel("Срок моделирования в месяцах", this) );
        labels.push_back( new QLabel("Тренд на рынке: бычий(B) или медвежий(M)", this) );
        labels[0]->setGeometry(QRect(QPoint(100, 100), QSize(200, 50)));
        labels[1]->setGeometry(QRect(QPoint(100, 200), QSize(200, 50)));

        labels[0]->setWordWrap(true);
        labels[1]->setWordWrap(true);

        text_edits.push_back( new QTextEdit("12", this)   );
        text_edits.push_back( new QTextEdit("M", this)   );
        text_edits[0]->setGeometry(QRect(QPoint(400, 100), QSize(50, 50)));
        text_edits[1]->setGeometry(QRect(QPoint(400, 200), QSize(50, 50)));


        buttons.push_back( new QPushButton("ОК", this)   );
        buttons[0]->setGeometry(QRect(QPoint(200, 500), QSize(200, 50)));

        connect(buttons[0], &QPushButton::released, this, &MainWindow::handler);
}

void MainWindow::handler()
    {
    if(screen == 1)
        {
        labels[0]->setVisible(false);
        labels[1]->setVisible(false);
        text_edits[0]->setVisible(false);
        text_edits[1]->setVisible(false);
        buttons[0]->setVisible(false);
        }

    if(screen == 1)
        {
        tableWidget = new QTableWidget(12, 3, this);


        for( int row = 0; row < 12; row++ )
        {
            for( int column = 0; column < 3; column++ )
            {
                QString sItem = QString::number( row + column );

                QVariant oVariant(sItem);

                // allocate the widget item
                QTableWidgetItem * poItem = new QTableWidgetItem();
                poItem->setData( Qt::DisplayRole, oVariant );
                pWidget->setItem( row, column, poItem );
            }
        }

        }

    if(screen == 1)
        {
        buttons.push_back( new QPushButton("Совершить шаг моделирования", this)   );
        buttons[1]->setGeometry(QRect(QPoint(500, 50), QSize(100, 50)));
        buttons[1]->show();

        connect(buttons[1], &QPushButton::released, this, &MainWindow::handleButton2);
        }


    update();


    screen = 2;
    }
