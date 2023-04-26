#include "mainwindow.h"
#include <iostream>

#include "fund.cpp"




MainWindow::MainWindow(QWidget *parent)
  : QWidget(parent)
{
        setWindowTitle("Система управления инвестиционным портфелем");
        setFixedSize(1000, 800);

        labels.push_back( new QLabel("Срок моделирования в месяцах", this) );
        labels.push_back( new QLabel("Тренд на рынке: бычий(B) или медвежий(M)", this) );
        labels[0]->setGeometry(QRect(QPoint(200, 300), QSize(300, 50)));
        labels[1]->setGeometry(QRect(QPoint(200, 400), QSize(300, 50)));

        labels[0]->setWordWrap(true);
        labels[1]->setWordWrap(true);

        text_edits.push_back( new QTextEdit("12", this)   );
        text_edits.push_back( new QTextEdit("M", this)   );
        text_edits[0]->setGeometry(QRect(QPoint(500, 300), QSize(100, 50)));
        text_edits[1]->setGeometry(QRect(QPoint(500, 400), QSize(100, 50)));


        buttons.push_back( new QPushButton("ОК", this)   );
        buttons[0]->setGeometry(QRect(QPoint(250, 500), QSize(300, 100)));

        connect(buttons[0], &QPushButton::released, this, &MainWindow::handler);
}

void MainWindow::handler()
    {
    static Environment *env_ptr = nullptr;

    if(screen == 1)
        {
        max_steps = std::stoi( text_edits[0]->toPlainText().toStdString());
        // mode = text_edits[1]->toPlainText().toStdString()[0];


        std::ifstream ifs("config.json");
        json jf = json::parse(ifs);

        env_ptr = new Environment(jf);

        }

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
                tableWidget->setItem( row, column, poItem );
            }
        }

        tableWidget->show();
        tableWidget->setGeometry(QRect(QPoint(200, 200), QSize(500, 500)));

        }

    if(screen == 1)
        {
        buttons.push_back( new QPushButton("Совершить шаг моделирования", this)   );
        buttons[1]->setGeometry(QRect(QPoint(500, 50), QSize(100, 50)));
        buttons[1]->show();

        connect(buttons[1], &QPushButton::released, this, &MainWindow::handler);
        }


    if(max_steps > 0)
        {
        max_steps--;

        (*env_ptr).step();
        }


    update();


    screen = 2;
    }
