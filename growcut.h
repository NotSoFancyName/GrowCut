#ifndef GROWCUT_H
#define GROWCUT_H

#include <vector>
#include <QImage>
#include <QColor>
#include <QtCore/qmath.h>


// Algorithm from http://www.graphicon.ru/oldgr/en/publications/text/gc2005vk.pdf

class GrowCut
{
public:

    class Cell;

    enum label
    {
        NONE = 0,
        OBJECT = 1,
        BACKGROUND = 2
    };

    //explict

    GrowCut();

    bool init(QImage &image, std::vector<QRect> &Object, std::vector<QRect> &Background);

    QImage getObject();

    unsigned int Split();

    std::vector<class Cell>  von_Neumann_Neighborhood(unsigned int x, unsigned int y);

    bool nextState(double max_norm);

    class Cell getCell(int x, int y);

    class Cell
    {
       public:

        Cell();
        Cell(label l, double Strenght, QRgb Feature_vector);

        label Label;
        double Strenght;
        QRgb Feature_vector;
    };

private:

    std::vector< std::vector<Cell> > cells;

    label label_of_the_Cell(std::vector<QRect> Object, std::vector<QRect> Background, QPoint p);

    double find_MaxPNorm();

    double norm(QRgb pix);

    double normSub(QRgb pix1,QRgb pix2);

    double function_G(double x, double max);

    QImage proccessingImg;
};

#endif // GROWCUT_H
