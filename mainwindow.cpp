#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "growcut.h"
#include "myqlabel.h"
#include <QDesktopWidget>
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QException>
#include <QFileDialog>
#include <QPainter>


std::vector<QRect> object_parts;
std::vector<QRect> background_parts;
QPoint prev;
QImage myImage;
bool current_Object_Selection;
QString path = "c:\\!try\\test.jpg";


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    connect(ui->label, &myQlabel::clicked,this, &MainWindow::onClick );
    connect(ui->label, &myQlabel::release,this, &MainWindow::onRelease );

    current_Object_Selection = true;
    ui->label->setAlignment( Qt::AlignLeft | Qt::AlignTop );

    myImage = QImage(path);
    QPixmap p = QPixmap::fromImage(myImage);
    ui->label->setPixmap(p);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked(bool checked)
{

    if( object_parts.size() != 0 && background_parts.size() != 0 ){
        QImage i = myImage;
        std::vector<QRect> o = object_parts;
        std::vector<QRect> b = background_parts;

        try{
            GrowCut *c = new GrowCut();
            c->init(i,o,b);
            c->Split();
            QImage new_img = c->getObject();
            QPixmap p = QPixmap::fromImage(new_img);
            ui->label->setPixmap(p);
        }
        catch(QException exp){

        }
    }
}

// change selection to object
void MainWindow::on_radioButton_clicked()
{
    current_Object_Selection = true;
}

// change selection to background
void MainWindow::on_radioButton_2_clicked()
{
    current_Object_Selection = false;
}

// adding first diaganal point of the rect
void MainWindow::onClick(QMouseEvent *event)
{
    prev = event->pos();
    return;
}

// adding a rectangular
void MainWindow::onRelease(QMouseEvent *event)
{
    QPoint p = event->pos();
    if(current_Object_Selection){
        object_parts.push_back(QRect(prev,p));
    }
    else{
        background_parts.push_back(QRect(prev,p));
    }


    return;
}

// image browser
void MainWindow::on_pushButton_2_clicked()
{
     path =
        QFileDialog::getOpenFileName(this, "Open a file", "directoryToOpen",
            "Images (*.png *.xpm *.jpg);;Text files (*.txt);;XML files (*.xml)");

    myImage = QImage(path);
    QPixmap p = QPixmap::fromImage(myImage);
    ui->label->setPixmap(p);

    object_parts.erase(object_parts.begin(), object_parts.end());
    background_parts.erase(background_parts.begin(), background_parts.end());

}

//reset button
void MainWindow::on_pushButton_3_clicked()
{
    object_parts.erase(object_parts.begin(), object_parts.end());
    background_parts.erase(background_parts.begin(), background_parts.end());
}
