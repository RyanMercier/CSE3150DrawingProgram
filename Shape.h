#ifndef SHAPE_H
#define SHAPE_H

#include "ECGraphicViewImp.h"

enum primitive
{
    rectangle,
    ellipse
};

struct Shape
{
    Shape() {}
    
    Shape(int p_id, primitive p_type, int p_x1, int p_y1, int p_x2, int p_y2, bool p_filled)
    {
        id = p_id;
        type = p_type;
        x1 = p_x1;
        y1 = p_y1;
        x2 = p_x2;
        y2 = p_y2;
        filled = p_filled;
    }

    Shape(int p_id, primitive p_type, int p_centerX, int p_centerY, double p_radiusX, double p_radiusY, bool p_filled)
    {
        id = p_id;
        type = p_type;
        x1 = p_centerX;
        y1 = p_centerY;
        radiusX = p_radiusX;
        radiusY = p_radiusY;
        filled = p_filled;
    }

    ~Shape(){}

    int id;
    primitive type;
    int x1, y1 = 0;
    int x2, y2 = 0;
    int radiusX = 0;
    int radiusY = 0;
    bool visible = true;
    bool filled = true;
    ECGVColor color = ECGV_BLACK;
};

#endif // SHAPE_H
