#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->loadMainWindowFeatures();

    this->counterLettersInLine = 0;
    this->cursorPosition = 0;
    this->indexOfCommandLines = -1;
    this->nextEmptyCommandLine = 0;
    this->commandLines = new QString[this->limitCommandLines]();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadMainWindowFeatures()
{
    QDesktopWidget *desktop = QApplication::desktop();

    int heightToSet = (desktop->height()-this->windowHeight)/2;
    int widthToSet = (desktop->width()-this->windowWidth)/2;

    this->setGeometry(widthToSet, heightToSet,this->windowWidth,this->windowHeight);
    this->setMinimumHeight(this->windowHeight);
    this->setMaximumHeight(this->windowHeight);
    this->setMinimumWidth(this->windowWidth);
    this->setMaximumWidth(this->windowWidth);
    setWindowFlags(Qt::FramelessWindowHint);

    QPixmap bkgnd(":/Multimedia/BackgroundApp.png");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);

    this->setWindowTitle("Boreal");
    this->setWindowIcon(QIcon(":/Multimedia/origame.png"));

    ui->textEdit->viewport()->setAutoFillBackground(false);
    ui->textEdit->setGeometry(520, 30, 451, 549);
    ui->textEdit->setFrameStyle(QFrame::NoFrame);
    ui->textEdit->setTextColor(QColor("white"));
    ui->textEdit->setFont(QFont("courier"));
    ui->textEdit->setFontWeight(12);
    ui->textEdit->insertPlainText("[User]: |");
    ui->textEdit->installEventFilter(this);
    ui->textEdit->setFocus();
}

void MainWindow::on_closeButton_clicked()
{
    this->close();
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == ui->textEdit)
    {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

            qDebug() << keyEvent->key();

            if(keyEvent->key() == this->enterKey){
                this->enterKeyPressed();
            }
            else if(keyEvent->key() == this->deleteKey){
                this->deleteKeyPressed();
            }
            else if(keyEvent->key() == this->leftKey){

                if(this->cursorPosition == this->currentCommandLine.size()-1){
                    QString newText = this->currentCommandLine.toStdString().substr(0, ui->textEdit->toPlainText().size()-2);
                    newText.append("|");
                    newText.append(this->currentCommandLine[this->currentCommandLine.size()-2]);

                }

                if(this->cursorPosition < this->currentCommandLine.size()-1){
                    QString* splittedText = this->currentCommandLine.split("|");
                    QString newText = splittedText[0].toStdString().substr(0, splittedText[0].size()-1);
                    newText.append("|");
                    newText.append(splittedText[0][splittedText[0].size()-1]);
                }

                return true;
            }
            else if(keyEvent->key() == this->rightKey){
                return true;
            }
            else if(keyEvent->key() == this->upKey){
                return true;
            }
            else if(keyEvent->key() == this->downKey){
                return true;
            }
            else{
                QChar pressedKey = QChar(keyEvent->key()).toLower();
                QString charactersAllowed = "-_/*+.:";

                if(pressedKey.isLetterOrNumber() || charactersAllowed.contains(pressedKey) || keyEvent->key()==this->spaceKey){

                    QString newText;
                    newText.append(this->allTextInserted);
                    newText.append("\n[User]: ");

                    if(this->currentCommandLine.isEmpty()){
                        this->currentCommandLine.append(pressedKey);
                        this->cursorPosition = 1;
                        newText.append(this->currentCommandLine);
                        newText.append("|");
                    }
                    else if(this->cursorPosition == 0){
                        this->currentCommandLine.insert(0, pressedKey);
                        this->cursorPosition = 1;
                        newText.append(pressedKey);
                        newText.append("|");
                        newText.append(this->currentCommandLine.mid(1, this->currentCommandLine-1));
                    }

                    this->currentCommandLine.append(pressedKey);
                    QString newText;
                    newText.append(this->allTextInserted);
                    newText.append("\n[User]: ");
                    newText.append(this->currentCommandLine);
                    newText.append("|");

                    ui->textEdit->clear();
                    ui->textEdit->insertPlainText(newText);
                    this->cursorPosition++;

                }
            }

            return true;
        }
    }
}

void MainWindow::enterKeyPressed()
{
    this->saveCommandLine(this->currentCommandLine);
    this->allTextInserted.append("\n[User]: ");
    this->allTextInserted.append(this->currentCommandLine);
    this->currentCommandLine.clear();
    this->cursorPosition = 0;
    this->indexOfCommandLines = this->nextEmptyCommandLine-1;

    ui->textEdit->clear();
    ui->textEdit->insertPlainText(this->allTextInserted);
    ui->textEdit->insertPlainText("\n[User]: |");
}

void MainWindow::deleteKeyPressed()
{
    if(this->cursorPosition != 0){
        QString newText;
        newText.append(this->allTextInserted);
        newText.append("\n[User]: ");

        if(this->currentCommandLine.size()==1){
            this->currentCommandLine = "";
            newText.append("|");
        }
        else if(this->cursorPosition==1){
            this->currentCommandLine = this->currentCommandLine.mid(1, this->currentCommandLine.size()-1);
            this->cursorPosition = 0;
            newText.append("|");
            newText.append(this->currentCommandLine);
        }
        else if(this->cursorPosition == this->currentCommandLine.size()){
            this->currentCommandLine = this->currentCommandLine.mid(0, this->currentCommandLine.size()-1);
            newText.append(this->currentCommandLine);
            newText.append("|");
        }
        else{
            QString newCommandLine = this->currentCommandLine.mid(0,this->cursorPosition-1);
            newCommandLine.append(this->currentCommandLine.mid(this->cursorPosition, this->currentCommandLine.size()-1));
            newText.append(this->currentCommandLine.mid(0,this->cursorPosition-1));
            newText.append("|");
            newText.append(this->currentCommandLine.mid(this->cursorPosition, this->currentCommandLine.size()-1));
            this->currentCommandLine = newCommandLine;
        }

        ui->textEdit->clear();
        ui->textEdit->insertPlainText(newText);
        this->currentCommandLine = this->currentCommandLine.mid(0,this->currentCommandLine.size()-1);
        this->counterLettersInLine--;
        this->cursorPosition;
        return true;
    }
}

void MainWindow::leftKeyPressed()
{}

void MainWindow::rightKeyPressed()
{}

void MainWindow::upKeyPressed()
{}

void MainWindow::downKeyPressed()
{}

void MainWindow::saveCommandLine(QString commandLine)
{
    if(this->nextEmptyCommandLine < this->limitCommandLines){
        this->commandLines[this->nextEmptyCommandLine] = commandLine;
        this->nextEmptyCommandLine++;
    }
    else{
        for(int i=0; i<(this->limitCommandLines-1); i++)
            this->commandLines[i] = this->commandLines[i+1];

        this->commandLines[this->limitCommandLines-1] = commandLine;
    }
}
