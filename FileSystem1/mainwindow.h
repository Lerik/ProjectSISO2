#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDesktopWidget>
#include <QDebug>
#include <QKeyEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_closeButton_clicked();

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private:
    Ui::MainWindow *ui;


    /* Constants needed for mainwindow size        */
    static const int windowHeight = 600;
    static const int windowWidth = 1000;
    /* ------------------------------------------- */



    /* Constants needed for console widget         */
    static const int leftKey = 16777234;
    static const int rightKey = 16777236;
    static const int upKey = 16777235;
    static const int downKey = 16777237;
    static const int deleteKey = 16777219;
    static const int enterKey = 16777220;
    static const int spaceKey = 32;
    static const int limitCommandLines = 10;
    int counterLettersInLine;
    int cursorPosition;
    int indexOfCommandLines;
    int nextEmptyCommandLine;
    QString* commandLines;
    QString currentCommandLine;
    QString allTextInserted;
    /* ------------------------------------------- */

    int counterOfLetters;

    void loadMainWindowFeatures();
    void saveCommandLine(QString commandLine);

};

#endif // MAINWINDOW_H
