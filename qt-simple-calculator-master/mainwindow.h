#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAbstractButton>
#include <QKeyEvent>
#include <QComboBox>

#include <iostream>
#include <string>
#include <algorithm>
#include <map>

/*
int main()
{
    std::cout << add("A.1", 16, "432.0123", 5) << "\n";
    std::cout << minus("A.1", 11, "432.0123", 5) << "\n";
    std::cout << mult("122.1", 3, "432.0123", 5) << "\n";
    std::cout << del("122.1", 3, "432.0123", 5) << "\n";
    std::cout << power("122.1", 3, "2.0123", 5) << "\n";
    //error

    std::cout << add("A.1", 10, "432.0123", 5) << "\n";
}
*/

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QComboBox *comboBox;

    std::map<char, int> pa = {
        {'0',0},
        {'1',1},
        {'2',2},
        {'3',3},
        {'4',4},
        {'5',5},
        {'6',6},
        {'7',7},
        {'8',8},
        {'9',9},
        {'A',10},
        {'B',11},
        {'C',12},
        {'D',13},
        {'E',14},
        {'F',15},
    };
    char sep = '.';
    void error();

    double transf(std::string x, int base);

    double add(std::string a, int base_a, std::string b, int base_b);
    double minus(std::string a, int base_a, std::string b, int base_b);
    double mult(std::string a, int base_a, std::string b, int base_b);
    double del(std::string a, int base_a, std::string b, int base_b);
    double power(std::string a, int base_a, std::string b, int base_b);

private slots:
    void numberGroup_clicked(QAbstractButton*);
    void actionGroup_clicked(QAbstractButton*);

    void on_actionDel_clicked();
    void on_actionCalc_clicked();
    void on_comma_clicked();
    void on_actionClear_clicked();
    void on_actionPercent_clicked();

private:
    Ui::MainWindow *ui;
    //Digit limit
    const int DIGIT_LIMIT = 16;
    //Flag to check whether the previous button that was clicked was an operator
    bool operatorClicked;
    //Last operator requested
    QChar storedOperator;
    //Flag to check whether a number is stored in memory
    bool hasStoredNumber;
    //Stored number
    QString storedNumber;
    int storedNum;
    //Calculate result based on stored number and displayed number
    void calculate_result();

protected:
    void keyPressEvent(QKeyEvent *e);
};

#endif // MAINWINDOW_H
