#include <QMainWindow>
#include <QPushButton>
#include <QTextEdit>
#include <QLabel>
#include <QPainter>
#include <QWidget>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QTableWidget>

class MainWindow : public QWidget
    {
    Q_OBJECT
    public:
    MainWindow(QWidget *parent = nullptr)
        {

        }


    private slots:
        void MainWindow::handler();

    private:
        std::vector<QPushButton*>  buttons;
        std::vector<QTextEdit*>    text_edits;
        std::vector<QLabel*>       labels;

        QTableWidget tableWidget = nullptr;

        int screen = 1; //1st screen


    };





























