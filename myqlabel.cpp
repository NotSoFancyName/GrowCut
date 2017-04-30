#include "myqlabel.h"
#include "ui_myqlabel.h"
//#include <QDebug>
#include <QPainter>



myQlabel::myQlabel(QWidget* parent) :
    QLabel(parent),
    ui(new Ui::myQlabel)
{
    ui->setupUi(this);
}

myQlabel::~myQlabel()
{
    delete ui;
}

void myQlabel::mousePressEvent(QMouseEvent* event) {

    emit clicked(event);


}

void myQlabel::mouseReleaseEvent(QMouseEvent* event) {

   emit release(event);
   update();

}


