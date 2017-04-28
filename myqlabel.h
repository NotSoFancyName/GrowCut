#ifndef MYQLABEL_H
#define MYQLABEL_H

#include <QWidget>
#include <QLabel>
#include <QMouseEvent>

namespace Ui {
class myQlabel;
}

class myQlabel : public QLabel
{
    Q_OBJECT

public:
      explicit myQlabel(QWidget* parent=0 );
    ~myQlabel();

private:
    Ui::myQlabel *ui;

signals:
   void clicked(QMouseEvent* event);
   void release(QMouseEvent* event);
public:
   void mousePressEvent(QMouseEvent* event) ;
   void mouseReleaseEvent(QMouseEvent* event);
};

#endif // MYQLABEL_H
