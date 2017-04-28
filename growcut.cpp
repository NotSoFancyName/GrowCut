#include "growcut.h"

GrowCut::GrowCut()
{

}

//initialize initial conditions
bool GrowCut::init(QImage &image, std::vector<QRect> &Object, std::vector<QRect> &Background)
{

    cells = std::vector<std::vector<Cell> >((unsigned int)image.width(),std::vector<Cell>( (unsigned int)image.height() ));
    this->proccessingImg = image;

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

    return true;

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

// finding neighbohrs of the current cell
std::vector<class GrowCut::Cell> GrowCut::von_Neumann_Neighborhood(unsigned int x, unsigned int y)
{
    std::vector<class Cell> res;
    if(x+1 < proccessingImg.width())
        res.push_back(cells[x+1][y]);
    if( (y-1) != -1)
        res.push_back(cells[x][y-1]);
    if( (x-1) != -1)
        res.push_back(cells[x-1][y]);
    if(x+1 < proccessingImg.height())
        res.push_back(cells[x][y+1]);

    return res;
}

// next state of cellular automaton
bool GrowCut::nextState(double max_norm)
{
    bool changed = false;
    for(unsigned int i = 0; i < cells.size(); i++)
    {
        for(unsigned int j = 0; j < cells[i].size(); j++)
        {
            std::vector<Cell> neighborhood = von_Neumann_Neighborhood(i,j);

            for(unsigned int k = 0; k < neighborhood.size(); k++)
            {
               if(neighborhood[k].Label != cells[i][j].Label) {

                   double cell_assault_power =
                        function_G( normSub(cells[i][j].Feature_vector,
                          neighborhood[k].Feature_vector), max_norm) *
                            neighborhood[k].Strenght;

                   if( cell_assault_power > cells[i][j].Strenght )
                   {
                        changed = true;
                        cells[i][j].Label = neighborhood[k].Label;
                        cells[i][j].Strenght = cell_assault_power;
                   }
               }
            }
        }
    }

    return changed;

}

// defines the "difference" between two cells
double GrowCut::function_G(double x, double max)
{

    double res = 1.0 - ( x / max);

    return res;
}

// get norm of feature vector
double GrowCut::norm(QRgb pix)
{
    QColor c(pix);
    return qSqrt(c.red()*c.red() + c.blue()*c.blue() + c.green()*c.green());
}

// get norm of substitution of two feature vectors
double GrowCut::normSub(QRgb pix1,QRgb pix2)
{
    QColor c(pix1);
    QColor t(pix2);

    return qSqrt( (c.red()-t.red())*(c.red()-t.red()) +
              (c.blue()-t.blue())*(c.blue()-t.blue())+
            (c.green()-t.green())*(c.green()-t.green()));
}

// find feature vector with maximum norm
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

//check if the cell in the rectangular and if so get its label
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

// split object and background
unsigned int GrowCut::Split(){

    unsigned int counter = 0;
    double max_fv = find_MaxPNorm();

    while(nextState(max_fv)){
        counter++;

        if (counter > 1000)
            break;
    }

    return counter;
}

//get image without background
QImage GrowCut::getObject()
{
    for(int i = 0; i < proccessingImg.width(); i++)
    {
        for(int j = 0; j < proccessingImg.height(); j++)
        {
            if(cells[i][j].Label == BACKGROUND || cells[i][j].Label == NONE)
            {
                proccessingImg.setPixel(QPoint(i,j),QRgb() );
            }
        }
    }

    return proccessingImg;
}
