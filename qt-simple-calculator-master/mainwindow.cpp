#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Set up empty display panel
    ui->displayPanel->clear();

    //Set operator and store number flags to false
    operatorClicked = false;
    hasStoredNumber = false;

    //Set number button group listener
    ui->numberGroup->connect(ui->numberGroup,SIGNAL(buttonClicked(QAbstractButton*)),
                             this, SLOT(numberGroup_clicked(QAbstractButton*)));
    //Set operator button group listener
    ui->actionGroup->connect(ui->actionGroup,SIGNAL(buttonClicked(QAbstractButton*)),
                             this, SLOT(actionGroup_clicked(QAbstractButton*)));

    //Set window fix width and height
    this->setFixedSize(QSize(306, 319));
}

MainWindow::~MainWindow()
{
    delete ui;
}

//==================================================================================
//Slot functions
//==================================================================================
// Called whenever a number button is clicked
void MainWindow::numberGroup_clicked(QAbstractButton* button)
{
    //Get string from display
    QString displayLabel = ui->displayPanel->text();

    /* Check if the previous button that was clicked was an operator button.
     * If so, clear the display and set the flag to false. Then proceed to
     * add the digit requested. */
    if (operatorClicked) {
        displayLabel.clear();
        operatorClicked = false;
    }

    //Append the digit only if we are not exceeding the digit limit
    if (displayLabel.length() >= DIGIT_LIMIT) {
        return;
    }

    //Append requested digit
    displayLabel.append(button->text());

    //Set number back to display
    ui->displayPanel->setText(displayLabel);
}

// Called whenever an action button is clicked
void MainWindow::actionGroup_clicked(QAbstractButton* button)
{
    /* If the previous button that was clicked was not an operator, then we just need to save the operator
     * that was requested and exit.
     * If it was though, we need to see whether we just need to save the number that is
     * displayed or if there is already a number stored in memory, perform the calculation and
     * store the result.
     * Example for this case: 5 + 7 + -> We need to save 12 in memory and then save operator. */

    if (operatorClicked) {
        storedOperator = button->text().at(0);
    }
    else {
        if (hasStoredNumber) {
            calculate_result();
        }
        else {
            //Set the flag to indicate that we now have a number stored in memory
            hasStoredNumber = true;
            //Get string from display
            QString storedNumber = ui->displayPanel->text();
            storedNum = comboBox->currentIndex() + 2;
        }
        //Set the flag that the last button that was clicked was an operator
        operatorClicked = true;
        //Store operator in memory
        storedOperator = button->text().at(0);
    }
}

void MainWindow::on_actionDel_clicked()
{
    //Get string from display
    QString displayLabel = ui->displayPanel->text();

    //Check if label is empty
    if (displayLabel.length() == 0) {
        return;
    }

    //Delete last digit from string
    displayLabel.QString::chop(1);
    //Set number back to display
    ui->displayPanel->setText(displayLabel);
}

void MainWindow::on_actionCalc_clicked()
{
    //Get string from display
    QString displayLabel = ui->displayPanel->text();
    /* A number must be saved in memory to be able calculate a result.
     * In addition, a number with at least one digit should be present in the display and
     * The last button that was clicked should not be an operator */

    if (!hasStoredNumber || displayLabel.length() < 1 || operatorClicked) {
        return;
    }

    //Calculate result and set in on display
    calculate_result();

    //Set stored number flag to false (we have it on screen now)
    hasStoredNumber = false;

}

void MainWindow::on_comma_clicked()
{
    //Get string from display
    QString displayLabel = ui->displayPanel->text();

    /* Append the digit only if we are not exceeding the digit limit.
     * More specifically in this case, we need 2 digits to be available.
     * One for the comma and at least another one for a remaining digit.
     * Also check if whether there is another comma already present. */
    if (displayLabel.length() >= (DIGIT_LIMIT - 1) ||
        displayLabel.contains('.', Qt::CaseSensitive)) {
        return;
    }

    //If label is empty, add zero and then append comma
    if (displayLabel.length() == 0) {
        displayLabel = "0";
    }

    //Append comma
    displayLabel.append('.');
    //Set number back to display
    ui->displayPanel->setText(displayLabel);
}

void MainWindow::on_actionClear_clicked()
{
    //Clear display label (for now)
    ui->displayPanel->clear();
    //Set operator and store number flags to false
    operatorClicked = false;
    hasStoredNumber = false;
}

void MainWindow::on_actionPercent_clicked()
{
    QString displayLabel = ui->displayPanel->text();
    std::string str_displayLabel = displayLabel.toStdString();
    std::string str_storedNumber = storedNumber.toStdString();

    //If the displayed number ends with a comma, drop the comma.
     if (displayLabel.endsWith('.',Qt::CaseSensitive)) {
         displayLabel.QString::chop(1);
     }

    int displayNum = comboBox->currentIndex() + 2;

    storedNumber = QString::number(power(str_storedNumber, storedNum, str_displayLabel, displayNum),'g', DIGIT_LIMIT);

    //Set number back to display
    ui->displayPanel->setText(storedNumber);
}

//==================================================================================
//Helper functions
//==================================================================================
void MainWindow::calculate_result() {
    //Get string from display
    QString displayLabel = ui->displayPanel->text();
    std::string str_displayLabel = displayLabel.toStdString();
    std::string str_storedNumber = storedNumber.toStdString();

    //If the displayed number ends with a comma, drop the comma.
     if (displayLabel.endsWith('.',Qt::CaseSensitive)) {
         displayLabel.QString::chop(1);
     }

     int displayNum = comboBox->currentIndex() + 2;

     //Decide what to do according to operation
     if (storedOperator == '+') {
         storedNumber = QString::number(add(str_displayLabel, displayNum, str_storedNumber, storedNum),'g', DIGIT_LIMIT);
     }
     else if (storedOperator == '-') {
         storedNumber = QString::number(minus(str_storedNumber, storedNum, str_displayLabel, displayNum),'g', DIGIT_LIMIT);
     }
     else if (storedOperator == 'x') {
         storedNumber = QString::number(mult(str_displayLabel, displayNum, str_storedNumber, storedNum),'g', DIGIT_LIMIT);
     }
     else if (storedOperator == '/') {
         storedNumber = QString::number(del(str_storedNumber, storedNum, str_displayLabel, displayNum),'g', DIGIT_LIMIT);
     }

     //Set number back to display
     ui->displayPanel->setText(storedNumber);
     ui->comboBox->setCurrentIndex(8);
     ui->comboBox->update();
}

//Keyboard buttons should call the corresponding functions
void MainWindow::keyPressEvent(QKeyEvent *e) {
    switch (e->key()) {
        //Numbers
        case Qt::Key_1:
            numberGroup_clicked(ui->num1);
            break;
        case Qt::Key_2:
            numberGroup_clicked(ui->num2);
            break;
        case Qt::Key_3:
            numberGroup_clicked(ui->num3);
            break;
        case Qt::Key_4:
            numberGroup_clicked(ui->num4);
            break;
        case Qt::Key_5:
            numberGroup_clicked(ui->num5);
            break;
        case Qt::Key_6:
            numberGroup_clicked(ui->num6);
            break;
        case Qt::Key_7:
            numberGroup_clicked(ui->num7);
            break;
        case Qt::Key_8:
            numberGroup_clicked(ui->num8);
            break;
        case Qt::Key_9:
            numberGroup_clicked(ui->num9);
            break;
        case Qt::Key_0:
            numberGroup_clicked(ui->num0);
            break;
        //Operators
        case Qt::Key_Plus:
            actionGroup_clicked(ui->actionPlus);
            break;
        case Qt::Key_Minus:
            actionGroup_clicked(ui->actionMinus);
            break;
        case Qt::Key_Asterisk:
            actionGroup_clicked(ui->actionMul);
            break;
        case Qt::Key_Slash:
            actionGroup_clicked(ui->actionDiv);
            break;
        //Comma
        case Qt::Key_Period:
            on_comma_clicked();
            break;
        //Return (enter)
        case Qt::Key_Enter:
        case Qt::Key_Return:
            on_actionCalc_clicked();
            break;
        //Backspace and delete
        case Qt::Key_Backspace:
            on_actionDel_clicked();
            break;
        case Qt::Key_Delete:
            on_actionClear_clicked();
            break;
        //Percentage
        case Qt::Key_Percent:
            on_actionPercent_clicked();
            break;
    case Qt::Key_A:
        numberGroup_clicked(ui->numA);
        break;
    case Qt::Key_B:
        numberGroup_clicked(ui->numB);
        break;
    case Qt::Key_C:
        numberGroup_clicked(ui->numC);
        break;
    case Qt::Key_D:
        numberGroup_clicked(ui->numD);
        break;
    case Qt::Key_E:
        numberGroup_clicked(ui->numE);
        break;
    case Qt::Key_F:
        numberGroup_clicked(ui->numF);
        break;
    default:
        break;
    }
}

//Саши
double MainWindow::power(std::string a, int base_a, std::string b, int base_b) {
    double at = transf(a, base_a);
    double bt = transf(b, base_b);

    if (abs(at) < 0.00000001 && abs(bt) < 0.00000001)
        error();

    return std::pow(at,bt);
}
double MainWindow::del(std::string a, int base_a, std::string b, int base_b) {
    double at = transf(a, base_a);
    double bt = transf(b, base_b);

    if (abs(bt) < 0.000000001)
        error();

    return at / bt;
}
double MainWindow::mult(std::string a, int base_a, std::string b, int base_b) {
    double at = transf(a, base_a);
    double bt = transf(b, base_b);
    return at * bt;
}
void MainWindow::error() {
    std::cout << "ERROR ";
}

double MainWindow::transf(std::string x, int base) {

    auto sep_flag = x.find(sep);
    std::string xm = x.substr(0, sep_flag);
    std::string xs = x.substr(sep_flag+1,x.size()-sep_flag+1);
    std::reverse(xm.begin(), xm.end());

    double ans = 0;
    int base_power = 1;
    for (int i = 0; i < xm.size(); ++i) {
        if (pa[xm[i]] >= base)
            error();
        ans += pa[xm[i]] * base_power;
        base_power *= base;
    }
    double base_minor_power = 1.0 / base;
    for (int i = 0; i < xs.size(); ++i) {
        if (pa[xs[i]] >= base)
            error();
        ans += (pa[xs[i]]) * base_minor_power;
        base_minor_power /= base;
    }
    return ans;
}

double MainWindow::add(std::string a, int base_a, std::string b, int base_b) {
    double at = transf(a, base_a);
    double bt = transf(b, base_b);
    return at+bt;
}
double MainWindow::minus(std::string a, int base_a, std::string b, int base_b) {
    double at = transf(a, base_a);
    double bt = transf(b, base_b);
    return at - bt;
}
