#ifndef GRAPHIC_CONTROLLER_H
#define GRAPHIC_CONTROLLER_H

#include <vector>
#include "Shape.h"
#include "Command.h"
#include "GraphicDocument.h"

class GraphicController
{
public:
    GraphicController(GraphicDocument* p_doc) { doc = p_doc; }
    ~GraphicController(){}

    bool SwitchMode()
    {
        insert = !insert;
        for (int i = 0; i < selected.size(); i++)
        {
            selected[i]->color = ECGV_BLACK;
        }
        selected.clear();
        return insert;
    }

    bool GetMode() { return insert; }

    bool SwitchShape()
    {
        rec = !rec;
        return rec;
    }

    bool GetShapeMode() { return rec; }

    bool FillShape()
    {
        filled = !filled;
        return filled;
    }

    bool GetFillMode() { return filled; }

    bool ToggleSelecting()
    {
        selecting = !selecting;
        return selecting;
    }

    bool GetSelecting() { return selecting; }

    void Select(int p_x, int p_y);

    std::vector<Shape*> &GetSelected()
    {
        return selected;
    }

    void ExecuteCommand(Command cmd);

    bool Undo();

    bool Redo();

    void Delete();

    void InsertRec(int p_x1, int p_y1, int p_x2, int p_y2);

    void InsertEllipse(int p_x, int p_y, double p_radiusX, double p_radiusY);

    void Move(int p_deltaX, int p_deltaY);

private:
    GraphicDocument* doc;
    bool insert = true;
    bool rec = true;
    bool filled = true;
    bool selecting = false;

    int index = 0;
    std::vector<Command> commandHistory;

    int shapeID = 0;
    std::vector<Shape*> selected;
};

#endif // !GRAPHIC_CONTROLLER_H
