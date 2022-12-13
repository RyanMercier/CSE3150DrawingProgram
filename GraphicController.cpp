#include <algorithm>
#include "GraphicController.h"

void GraphicController::Select(int p_x, int p_y)
{
    for (int i = 0; i < doc->GetShapes().size(); i++)
    {   
        if (doc->GetShapes()[i].type == primitive::rectangle)
        {
            if (p_x >= std::min(doc->GetShapes()[i].x1, doc->GetShapes()[i].x2) && p_x <= std::max(doc->GetShapes()[i].x1, doc->GetShapes()[i].x2) && p_y >= std::min(doc->GetShapes()[i].y1, doc->GetShapes()[i].y2) && p_y <= std::max(doc->GetShapes()[i].y1, doc->GetShapes()[i].y2)) // if mouse is in the rectangles bounds we should select it
            {
                int j;
                for (j = 0; j < selected.size(); j++)
                {
                    if (doc->GetShapes()[i].id == selected[j]->id)
                    {
                        break;
                    }
                }

                if (j != selected.size())
                {
                    selected.erase(selected.begin() + j);
                    doc->GetShapes()[i].color = ECGV_BLACK;
                }
                else
                {
                    std::cout << "Shape: " << doc->GetShapes()[i].id << " Selected" << std::endl;
                    selected.push_back(&doc->GetShapes()[i]);
                    doc->GetShapes()[i].color = ECGV_BLUE;
                }
            }
        }

        else if (doc->GetShapes()[i].type == primitive::ellipse)
        {
            // check the equation of ellipse with the given point
            double p = (double((p_x - doc->GetShapes()[i].x1) * (p_x - doc->GetShapes()[i].x1)) / double(doc->GetShapes()[i].radiusX * doc->GetShapes()[i].radiusX)) + (double((p_y - doc->GetShapes()[i].y1) * (p_y - doc->GetShapes()[i].y1)) / double(doc->GetShapes()[i].radiusY * doc->GetShapes()[i].radiusY)); // if mouse is in the ellipse bounds we should select it
            if (p <= 1)
            {
                int j;
                for (j = 0; j < selected.size(); j++)
                {
                    if (doc->GetShapes()[i].id == selected[j]->id)
                    {
                        break;
                    }
                }

                if (j != selected.size())
                {
                    selected.erase(selected.begin() + j);
                    doc->GetShapes()[i].color = ECGV_BLACK;
                }
                else
                {
                    std::cout << "Shape: " << doc->GetShapes()[i].id << " Selected" << std::endl;
                    selected.push_back(&doc->GetShapes()[i]);
                    doc->GetShapes()[i].color = ECGV_BLUE;
                }
            }
        }
    }
}

void GraphicController::ExecuteCommand(Command cmd)
{
    if (cmd.op == operation::draw)
    {
        for (int i = 0; i < cmd.shapes.size(); i++)
        {
            doc->AddShape(cmd.shapes[i]);
        }
    }

    if (cmd.op == operation::del)
    {
        for (int i = 0; i < cmd.shapes.size(); i++)
        {
            doc->RemoveShape(cmd.shapes[i]);
        }
    }

    if (cmd.op == operation::move)
    {
        for (int i = 0; i < cmd.shapes.size(); i++)
        {
            doc->MoveShape(cmd.shapes[i].id, cmd.shapes[i].x1 - cmd.deltaX, cmd.shapes[i].y1 - cmd.deltaY, cmd.shapes[i].x2 - cmd.deltaX, cmd.shapes[i].y2 - cmd.deltaY);
        }
    }
}

bool GraphicController::Undo()
{
    for (int i = 0; i < selected.size(); i++)
    {
        selected[i]->color = ECGV_BLACK;
    }
    selected.clear();
    
    if (index == 0)
    {
        return false;
    }

    index--;

    if (commandHistory[index].op == operation::draw)
    {
        for (Shape s : commandHistory[index].shapes)
        {
            doc->RemoveShape(s);
        }
    }

    if (commandHistory[index].op == operation::del)
    {
        for (Shape s : commandHistory[index].shapes)
        {
            doc->AddShape(s);
        }
    }

    if (commandHistory[index].op == operation::move)
    {
        for (Shape s : commandHistory[index].shapes)
        {
            doc->MoveShape(s.id, s.x1, s.y1, s.x2, s.y2);
        }
    }

    return true;
}

bool GraphicController::Redo()
{
    for (int i = 0; i < selected.size(); i++)
    {
        selected[i]->color = ECGV_BLACK;
    }
    selected.clear();
    
    if (index > commandHistory.size() - 1)
    {
        return false;
    }

    ExecuteCommand(commandHistory[index]);
    index++;

    return true;
}

void GraphicController::Delete()
{
    std::vector<Shape> shapes;
    for (int i = 0; i < selected.size(); i++)
    {
        selected[i]->color = ECGV_BLACK;
        shapes.push_back(*selected[i]);
    }

    Command cmd = Command(operation::del, shapes);

    selected.clear();

    if (commandHistory.size() > 0)
    {
        commandHistory.erase(commandHistory.begin() + index, commandHistory.end());
    }
    commandHistory.push_back(cmd);
    index++;

    // execute the saved command
    ExecuteCommand(cmd);
}

void GraphicController::InsertRec(int p_x1, int p_y1, int p_x2, int p_y2)
{
    // handle shape
    std::vector<Shape> shapes;
    Shape rec = Shape(shapeID, primitive::rectangle, p_x1, p_y1, p_x2, p_y2, filled);
    shapes.push_back(rec);
    shapeID++;

    // create command and clear any saved redo commands from history
    Command cmd = Command(operation::draw, shapes);

    if (commandHistory.size() > 0)
    {
        commandHistory.erase(commandHistory.begin() + index, commandHistory.end());
    }
    commandHistory.push_back(cmd);
    index++;

    // execute the saved command
    ExecuteCommand(cmd);
}

void GraphicController::InsertEllipse(int p_x, int p_y, double p_radiusX, double p_radiusY)
{
    // handle shape
    std::vector<Shape> shapes;
    Shape ellipse = Shape(shapeID, primitive::ellipse, p_x, p_y, p_radiusX, p_radiusY, filled);
    shapes.push_back(ellipse);
    shapeID++;

    // create command and clear any saved redo commands from history
    Command cmd = Command(operation::draw, shapes);

    if (commandHistory.size() > 0)
    {
        commandHistory.erase(commandHistory.begin() + index, commandHistory.end());
    }
    commandHistory.push_back(cmd);
    index++;

    // execute the saved command
    ExecuteCommand(cmd);
}

void GraphicController::Move(int p_deltaX, int p_deltaY)
{
    if (selected.size() > 0)
    {
        std::vector<Shape> shapes;
        for (Shape* s : selected)
        {
            shapes.push_back(*s);
        }
        Command cmd = Command(operation::move, shapes);
        cmd.deltaX = p_deltaX;
        cmd.deltaY = p_deltaY;

        if (commandHistory.size() > 0)
        {
            commandHistory.erase(commandHistory.begin() + index, commandHistory.end());
        }
        commandHistory.push_back(cmd);
        index++;

        // execute the saved command
        ExecuteCommand(cmd);
    }
}