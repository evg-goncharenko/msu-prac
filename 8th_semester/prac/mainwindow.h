#include <QMainWindow>
#include <QPushButton>
#include <QTextEdit>
#include <QLabel>
#include <QPainter>
#include <QWidget>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QTableWidget>
#include <json.hpp>
using json = nlohmann::json;

class MainWindow : public QWidget
    {
    Q_OBJECT
    public:
    MainWindow(QWidget *parent = nullptr);


    private slots:
        void handler();
        void handler_to_the_end();
        void prehandler();
        void brief_handler();

    private:
        std::vector<QPushButton*>  buttons;
        std::vector<QTextEdit*>    text_edits;
        std::vector<QLabel*>       labels;


        std::vector<QPushButton*>  preh_buttons;
        std::vector<QTextEdit*>    preh_text_edits;
        std::vector<QLabel*>       preh_labels;

        bool delete_brief = false;
        std::vector<QPushButton*>  brief_buttons;
        std::vector<QTextEdit*>    brief_text_edits;
        std::vector<QLabel*>       brief_labels;

        QTableWidget * tableWidget = nullptr;

        int screen = 1; //1st screen


        json jf;
        int max_steps = -1;
        char trend = 'B';

    };





























