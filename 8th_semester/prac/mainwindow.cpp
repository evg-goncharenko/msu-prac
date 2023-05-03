#include "mainwindow.h"
#include <iostream>
 #include <numeric>
#include "fund.cpp"

Environment *env_ptr = nullptr;


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

        connect(buttons[0], &QPushButton::released, this, &MainWindow::prehandler);


        std::ifstream ifs("/Users/evgenijgoncarenko/Documents/MacBook/program/github/msu-prac/8th_semester/prac/config.json");
        jf = json::parse(ifs);
}





void MainWindow::prehandler()
    {


    if(preh_labels.size() == 0)
        {

        labels[0]->setVisible(false);
        labels[1]->setVisible(false);
        text_edits[0]->setVisible(false);
        text_edits[1]->setVisible(false);
        buttons[0]->setVisible(false);



        preh_labels.push_back( new QLabel("Название актива", this) );
        preh_labels.push_back( new QLabel("Цена одной единицы", this) );
        preh_labels.push_back( new QLabel("Риск данного актива", this) );
        preh_labels[0]->setGeometry(QRect(QPoint(200, 300), QSize(300, 50)));
        preh_labels[1]->setGeometry(QRect(QPoint(200, 400), QSize(300, 50)));
        preh_labels[2]->setGeometry(QRect(QPoint(200, 500), QSize(300, 50)));

        preh_labels[0]->setWordWrap(true);
        preh_labels[1]->setWordWrap(true);

        preh_text_edits.push_back( new QTextEdit("", this)   );
        preh_text_edits.push_back( new QTextEdit("", this)   );
        preh_text_edits.push_back( new QTextEdit("", this)   );
        preh_text_edits[0]->setGeometry(QRect(QPoint(500, 300), QSize(100, 50)));
        preh_text_edits[1]->setGeometry(QRect(QPoint(500, 400), QSize(100, 50)));
        preh_text_edits[2]->setGeometry(QRect(QPoint(500, 500), QSize(100, 50)));

        preh_buttons.push_back( new QPushButton("Закончить ввод активов", this)   );
        preh_buttons[0]->setGeometry(QRect(QPoint(200, 600), QSize(200, 50)));

        preh_buttons.push_back( new QPushButton("Ввод данных актива", this)   );
        preh_buttons[1]->setGeometry(QRect(QPoint(400, 600), QSize(200, 50)));


        connect(preh_buttons[0], &QPushButton::released, this, &MainWindow::brief_handler);
        connect(preh_buttons[1], &QPushButton::released, this, &MainWindow::prehandler);

        }
    else
        {
        std::string asset_name = preh_text_edits[0]->toPlainText().toStdString();
        int cost =    std::stoi( preh_text_edits[1]->toPlainText().toStdString());
        int risk = std::stoi( preh_text_edits[2]->toPlainText().toStdString());

        jf["MARKET"][asset_name] = cost;
        jf["RISK"][asset_name] = risk;

        preh_text_edits[0]->setPlainText("");
        preh_text_edits[1]->setPlainText("");
        preh_text_edits[2]->setPlainText("");
        }


        preh_labels[0]->setVisible(true);
        preh_labels[1]->setVisible(true);
        preh_labels[2]->setVisible(true);
        preh_text_edits[0]->setVisible(true);
        preh_text_edits[1]->setVisible(true);
        preh_text_edits[2]->setVisible(true);
        preh_buttons[0]->setVisible(true);
        preh_buttons[1]->setVisible(true);


    }








void MainWindow::brief_handler()
    {


    if(brief_labels.size() == 0)
        {

        //"RISK" : 1, "INVEST_TIME": 5, "QUANTITY" : 100, "COMPANY_NAME": "YANDEX",  "DIVIDEND_PERCENT": 0.02  } ] ,


        preh_labels[0]->setVisible(false);
        preh_labels[1]->setVisible(false);
        preh_labels[2]->setVisible(false);
        preh_text_edits[0]->setVisible(false);
        preh_text_edits[1]->setVisible(false);
        preh_text_edits[2]->setVisible(false);
        preh_buttons[0]->setVisible(false);
        preh_buttons[1]->setVisible(false);




        brief_labels.push_back( new QLabel("Портфель: название актива", this) );
        brief_labels.push_back( new QLabel("Портфель: Количество акций", this) );
        brief_labels.push_back( new QLabel("Портфель: Риск данной акции", this) );
        brief_labels.push_back( new QLabel("Портфель: Дивиденды данной акции", this) );

        brief_labels[0]->setGeometry(QRect(QPoint(200, 200), QSize(300, 50)));
        brief_labels[1]->setGeometry(QRect(QPoint(200, 300), QSize(300, 50)));
        brief_labels[2]->setGeometry(QRect(QPoint(200, 400), QSize(300, 50)));
        brief_labels[3]->setGeometry(QRect(QPoint(200, 500), QSize(300, 50)));

        brief_labels[0]->setWordWrap(true);
        brief_labels[1]->setWordWrap(true);

        brief_text_edits.push_back( new QTextEdit("", this)   );
        brief_text_edits.push_back( new QTextEdit("", this)   );
        brief_text_edits.push_back( new QTextEdit("", this)   );
        brief_text_edits.push_back( new QTextEdit("", this)   );
        brief_text_edits[0]->setGeometry(QRect(QPoint(500, 200), QSize(100, 50)));
        brief_text_edits[1]->setGeometry(QRect(QPoint(500, 300), QSize(100, 50)));
        brief_text_edits[2]->setGeometry(QRect(QPoint(500, 400), QSize(100, 50)));
        brief_text_edits[3]->setGeometry(QRect(QPoint(500, 500), QSize(100, 50)));

        brief_buttons.push_back( new QPushButton("Закончить ввод акций", this)   );
        brief_buttons[0]->setGeometry(QRect(QPoint(200, 600), QSize(200, 50)));

        brief_buttons.push_back( new QPushButton("Ввод данных акций", this)   );
        brief_buttons[1]->setGeometry(QRect(QPoint(400, 600), QSize(200, 50)));


        connect(brief_buttons[0], &QPushButton::released, this, &MainWindow::handler);
        connect(brief_buttons[1], &QPushButton::released, this, &MainWindow::brief_handler);

        }
    else
        {
        if(!delete_brief)
            {
            delete_brief = true;

            jf["BRIEFCASE"] = json::array();
            }

        std::string asset_name = brief_text_edits[0]->toPlainText().toStdString();
        int quantity =    std::stoi( brief_text_edits[1]->toPlainText().toStdString());
        int risk = std::stoi( brief_text_edits[2]->toPlainText().toStdString());
        double dividends = std::stod( brief_text_edits[3]->toPlainText().toStdString());


        std::cout << brief_text_edits[3]->toPlainText().toStdString() << std::endl;
        std::cout << dividends << std::endl;


        //"ASSET"  : "STOCK", "RISK" : 30, "INVEST_TIME": 5, "QUANTITY" : 100, "COMPANY_NAME": "GOOGLE",  "DIVIDEND_PERCENT": 0.03
        json tmp =  {  { "ASSET"   ,"STOCK"} , { "RISK"  , risk} , { "INVEST_TIME" ,5} , { "QUANTITY"  , quantity} , { "COMPANY_NAME" , asset_name } , {  "DIVIDEND_PERCENT" ,  dividends }  };

        jf["BRIEFCASE"].push_back(tmp);

        brief_text_edits[0]->setPlainText("");
        brief_text_edits[1]->setPlainText("");
        brief_text_edits[2]->setPlainText("");
        brief_text_edits[3]->setPlainText("");
        }


        brief_labels[0]->setVisible(true);
        brief_labels[1]->setVisible(true);
        brief_labels[2]->setVisible(true);
        brief_labels[3]->setVisible(true);
        brief_text_edits[0]->setVisible(true);
        brief_text_edits[1]->setVisible(true);
        brief_text_edits[2]->setVisible(true);
        brief_text_edits[3]->setVisible(true);
        brief_buttons[0]->setVisible(true);
        brief_buttons[1]->setVisible(true);




    }









void fill_table( QTableWidget * tableWidget,   Environment *env_ptr )
    {

    std::vector<std::string> names;
    std::vector<double> initial_costs;
    std::vector<double> costs;
    std::vector<double> profits;
    std::vector<double> total;

    names = (*env_ptr).get_names();
    costs = (*env_ptr).get_costs();
    initial_costs = (*env_ptr).get_initial_costs();
    profits = (*env_ptr).get_profits();
    total =   (*env_ptr).get_won_by_buying();


    //All:
    names.push_back("TOTAL: ");
    costs.push_back(    std::accumulate(costs.begin(), costs.end(), 0.0) ) ;
    initial_costs.push_back(    std::accumulate(initial_costs.begin(), initial_costs.end(), 0.0) ) ;
    profits.push_back(    std::accumulate(profits.begin(), profits.end(), 0.0) ) ;
    total.push_back(    std::accumulate(total.begin(), total.end(), 0.0) ) ;

    for( int row = 0; row < names.size(); row++ )
    {

        {
            QString sItem = QString::fromStdString(names[row]);

            QVariant oVariant(sItem);

            // allocate the widget item
            QTableWidgetItem * poItem = new QTableWidgetItem();
            poItem->setData( Qt::DisplayRole, oVariant );
            tableWidget->setItem( row, 0, poItem );
        }

        {
            QString sItem =  QString::number(initial_costs[row], 'f', 0);

            QVariant oVariant(sItem);

            // allocate the widget item
            QTableWidgetItem * poItem = new QTableWidgetItem();
            poItem->setData( Qt::DisplayRole, oVariant );
            tableWidget->setItem( row, 1, poItem );
        }


        {
            QString sItem =  QString::number(costs[row], 'f', 0);

            QVariant oVariant(sItem);

            // allocate the widget item
            QTableWidgetItem * poItem = new QTableWidgetItem();
            poItem->setData( Qt::DisplayRole, oVariant );
            tableWidget->setItem( row, 2, poItem );
        }


        {
            QString sItem =  QString::number(profits[row], 'f', 0);

            QVariant oVariant(sItem);

            // allocate the widget item
            QTableWidgetItem * poItem = new QTableWidgetItem();
            poItem->setData( Qt::DisplayRole, oVariant );
            tableWidget->setItem( row, 3, poItem );
        }

        {
            QString sItem = QString::number(total[row], 'f', 0);

            QVariant oVariant(sItem);

            // allocate the widget item
            QTableWidgetItem * poItem = new QTableWidgetItem();
            poItem->setData( Qt::DisplayRole, oVariant );
            tableWidget->setItem( row, 4, poItem );
        }

    }





    tableWidget->show();
    tableWidget->setGeometry(QRect(QPoint(50, 200), QSize(750, 500)));





    }








void MainWindow::handler()
    {
    if(screen == 1)
        {
        max_steps = std::stoi( text_edits[0]->toPlainText().toStdString());
        trend = text_edits[1]->toPlainText().toStdString()[0];



        std::cout << jf.dump() << std::endl;

        env_ptr = new Environment(jf, trend);
        }

    if(screen == 1)
        {

        preh_labels[0]->setVisible(false);
        preh_labels[1]->setVisible(false);
        preh_text_edits[0]->setVisible(false);
        preh_text_edits[1]->setVisible(false);
        preh_buttons[0]->setVisible(false);
        preh_buttons[0]->setVisible(false);

        }


    //Making a step in the environemnt
    if(max_steps > 0)
        {
        max_steps--;

        (*env_ptr).step();
        }




    if(screen == 1)
        {
//         std::vector<std::string> names;
//         names = (*env_ptr).get_names();

        int q = (*env_ptr).get_q_briefcase();

        //Total
        q++;

        tableWidget = new QTableWidget(q, 5, this);

        QStringList name;
        name << "Asset name" << "Initial cost" << "Current cost" << "Extracted profit" << "Asset profit";
        tableWidget->setHorizontalHeaderLabels(name); // no more error:
        }


    fill_table(  tableWidget,   env_ptr );




    if(screen == 1)
        {
        buttons.push_back( new QPushButton("Сделать шаг вперед", this)   );
        buttons[1]->setGeometry(QRect(QPoint(500, 50), QSize(200, 100)));
        //buttons[1]->setWordWrap(true);
        buttons[1]->show();

        connect(buttons[1], &QPushButton::released, this, &MainWindow::handler);
        }


    if(screen == 1)
        {
        buttons.push_back( new QPushButton("До конца", this)   );
        buttons[2]->setGeometry(QRect(QPoint(200, 50), QSize(200, 100)));
        buttons[2]->show();

        connect(buttons[2], &QPushButton::released, this, &MainWindow::handler_to_the_end);
        }


    update();

    screen = 2;
    }









void MainWindow::handler_to_the_end()
    {

    //Making a step in the environemnt
    while(max_steps > 0)
        {
        max_steps--;
        (*env_ptr).step();
        }

    fill_table(  tableWidget,   env_ptr );

    update();

    screen = 2;
    }
