#include "growcut.h"

GrowCut::GrowCut()
{

}

GrowCut::GrowCut(QImage image, std::vector<QRect> Object, std::vector<QRect> Background){


    //this->cells = std::vector(Cell());

    for(int i = 0; i < image.width(); i++)
    {
        for(int j = 0; j < image.height(); j++)
        {
            cells[i][j].Label = label_of_the_Cell(Object,Background, QPoint(i,j));
            cells[i][j].Feature_vector =  image.pixel(QPoint(i,j));

            if(cells[i][j].Label != GrowCut::NONE)
                cells[i][j].Strenght = 1.0;
        }
    }



}

GrowCut::Cell::Cell(): Label(NONE), Strenght(0),Feature_vector(QRgb()){

    this->Label = NONE;
    this->Feature_vector = QRgb();
    this->Strenght = 0;
}

GrowCut::Cell::Cell(label l, double S, QRgb Fvector)
{
    this->Feature_vector = Fvector;
    this->Label = l;
    this->Strenght = S;
}

std::vector<class GrowCut::Cell> GrowCut::von_Neumann_Neighborhood(unsigned int x, unsigned int y)
{
    std::vector<class Cell> res;
    res.push_back(cells[x+1][y]);
    res.push_back(cells[x][y-1]);
    res.push_back(cells[x-1][y]);
    res.push_back(cells[x][y+1]);
    return res;
}

bool GrowCut::nextState()
{
    bool changed = false;
    double max_fv = find_MaxPNorm();
    for(unsigned int i = 0; i < cells.size(); i++)
    {
        for(unsigned int j = 0; j < cells[i].size(); j++)
        {
            std::vector<class Cell> neighborhood;
            neighborhood = von_Neumann_Neighborhood(i,j);
            for(unsigned int k = 0; k < neighborhood.size(); k++)
            {
                if(function_G( normSub(cells[i][j].Feature_vector,neighborhood[k].Feature_vector), max_fv) *
                        neighborhood[k].Strenght > cells[i][j].Strenght)
                {
                    changed = true;
                    cells[i][j].Label = neighborhood[k].Label;
                    cells[i][j].Strenght = function_G( normSub(cells[i][j].Feature_vector,neighborhood[k].Feature_vector), max_fv) *
                            neighborhood[k].Strenght;
                }
            }
        }
    }

    return changed;

}

double GrowCut::function_G(double x, double max)
{

    double res = 1.0 - ( x / max);

    return res;
}

double GrowCut::norm(QRgb pix)
{
    QColor c(pix);
    return qSqrt(c.red()*c.red() + c.blue()*c.blue() + c.green()*c.green());
}

double GrowCut::normSub(QRgb pix1,QRgb pix2)
{
    QColor c(pix1);
    QColor t(pix2);

    return qSqrt( (c.red()-t.red())*(c.red()-t.red()) +
              (c.blue()-t.blue())*(c.blue()-t.blue())+
            (c.green()-t.green())*(c.green()-t.green()));
}

double GrowCut::find_MaxPNorm()
{
    double max = 0.0;
    for(unsigned int i = 0; i < cells.size(); i++)
    {
        for(unsigned int j = 0; j < cells[i].size(); j++)
        {
            if(max < norm(cells[i][j].Feature_vector))
                max = norm(cells[i][j].Feature_vector);
        }
    }

    return max;
}


GrowCut::label GrowCut::label_of_the_Cell(std::vector<QRect> Object, std::vector<QRect> Background, QPoint p)
{
    for(unsigned int i = 0; i < Object.size(); i++)
    {
        if(Object[i].contains(p))
            return GrowCut::OBJECT;
    }

    for(unsigned int i = 0; i < Background.size(); i++)
    {
        if(Background[i].contains(p))
            return GrowCut::BACKGROUND;

    }

    return GrowCut::NONE;
}

unsigned int GrowCut::Split(){

    unsigned int counter = 0;
    while(nextState()){
        counter++;
    }

    return counter;
}
