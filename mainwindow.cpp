#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "myqlabel.h"





std::vector<QRect> object_parts;
std::vector<QRect> background_parts;
QPoint prev;
QImage myImage;
QImage resImage;
bool current_Object_Selection;
QString path = "c:\\!try\\test.jpg";
GrowCut * c;
//MyThread t;

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
    resImage = myImage;
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
        //resImage = myImage;

        try{
            c = new GrowCut();
            c->init(myImage,object_parts,background_parts);
            c->Split();
            QImage new_img = c->getObject();
            QPixmap p = QPixmap::fromImage(new_img);
            ui->label->setPixmap(p);
            //MyThread t(c,object_parts,background_parts,&resImage);
            //t.start();
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
    //p = QPoint(p.x()+1, p.y()+1);
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




void MyThread::run()
{
    c = new GrowCut();
    c->init(*i,o,b);
    c->Split();
    exit();
}

MyThread::MyThread(GrowCut* c ,std::vector<QRect> &object_parts, std::vector<QRect> &background_parts,QImage *i)
{
  this->o = object_parts;
  this->b = background_parts;
  this->i = i;
  this->c = c;
}


void MainWindow::updateCaption(){
    QPixmap p = QPixmap::fromImage(resImage);
    ui->label->setPixmap(p);
}
