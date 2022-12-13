#ifndef GRAPHIC_DOCUMENT_H
#define GRAPHIC_DOCUMENT_H

#include <vector>
#include "ECGraphicViewImp.h"
#include "Shape.h"

class GraphicDocument
{
public:
    GraphicDocument(ECGraphicViewImp* p_view)
    {
        view = p_view;
    }

    ~GraphicDocument()
    {
    }

    std::vector<Shape>& GetShapes()
    {
        return shapes;
    }

    void AddShape(Shape shape)
    {
        shapes.push_back(shape);
    }

    void RemoveShape(Shape shape)
    {
        int i;
        for (i = 0; i < shapes.size(); i++)
        {
            if (shapes[i].id == shape.id)
            {
                break;
            }
        }

        shapes.erase(shapes.begin() + i);
    }

    void MoveShape(int id, int x1, int y1, int x2, int y2)
    {
        for (int i = 0; i < shapes.size(); i++)
        {
            if (shapes[i].id == id)
            {
                shapes[i].x1 = x1;
                shapes[i].y1 = y1;
                shapes[i].x2 = x2;
                shapes[i].y2 = y2;
            }
        }
    }

    void Draw()
    {
        for (Shape shape : shapes)
        {
            if (shape.type == primitive::rectangle && shape.filled == false && shape.visible)
            {
                view->DrawRectangle(shape.x1, shape.y1, shape.x2, shape.y2, 3, shape.color);
            }

            if (shape.type == primitive::ellipse && shape.filled == false && shape.visible)
            {
                view->DrawEllipse(shape.x1, shape.y1, shape.radiusX, shape.radiusY, 3, shape.color);
            }

            if (shape.type == primitive::rectangle && shape.filled == true && shape.visible)
            {
                view->DrawFilledRectangle(shape.x1, shape.y1, shape.x2, shape.y2, shape.color);
            }

            if (shape.type == primitive::ellipse && shape.filled == true && shape.visible)
            {
                view->DrawFilledEllipse(shape.x1, shape.y1, shape.radiusX, shape.radiusY, shape.color);
            }
        }
    }

private:
    ECGraphicViewImp* view = nullptr;
    std::vector<Shape> shapes;
};

#endif