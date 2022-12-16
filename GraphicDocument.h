#ifndef GRAPHIC_DOCUMENT_H
#define GRAPHIC_DOCUMENT_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
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

    void LoadFile(std::string filename)
    {
        // Creation of fstream class object
        std::fstream fio;

        // line buffer
        std::string line;

        fio.open(filename, std::ios::in | std::ios::app);
        
        // Execute a loop until EOF (End of File)
        // point read pointer at beginning of file
        fio.seekg(0, std::ios::beg);

        int id = 0;
        while (fio)
        {
            // Read a Line from File
            getline(fio, line);

            // Print line in Console
            std::cout << line << std::endl;

            if (line.size() > 1) // a shape is being declared
            {
                Shape toAdd;
                
                std::string buf;                    // Have a buffer string
                std::stringstream ss(line);         // Insert the line into a stream

                std::vector<std::string> params;    // Create vector to hold our shape settings

                while (ss >> buf)
                    params.push_back(buf);

                if (stoi(params[0]) == 0) // unfilled rectangle
                {
                    toAdd = Shape(id, primitive::rectangle, stoi(params[1]), stoi(params[2]), stoi(params[7]), stoi(params[8]), false);
                }

                else if (stoi(params[0]) == 2) // filled rectangle
                {
                    toAdd = Shape(id, primitive::rectangle, stoi(params[1]), stoi(params[2]), stoi(params[7]), stoi(params[8]), true);
                }

                else if (stoi(params[0]) == 1) // unfilled ellipse
                {
                    toAdd = Shape(id, primitive::ellipse, stoi(params[1]), stoi(params[2]), double(stoi(params[3])), double(stoi(params[4])), false);
                }

                else if (stoi(params[0]) == 3) // filled ellipse
                {
                    toAdd = Shape(id, primitive::ellipse, stoi(params[1]), stoi(params[2]), double(stoi(params[3])), double(stoi(params[4])), true);
                }

                shapes.push_back(toAdd);
                id++;
            }
        }

        // Close the file
        fio.close();
    }


    void SaveFile(std::string p_filename)
    {
        std::cout << "saving" << std::endl;
        
        // Creation of fstream class object
        std::fstream fio;

        // line buffer
        std::string line;

        // by default openmode = ios::in|ios::out mode
        // Automatically overwrites the content of file, To append
        // the content, open in ios:app
        // fio.open("sample.txt", ios::in|ios::out|ios::app)
        // ios::trunc mode delete all content before open
        fio.open(p_filename, std::ios::trunc | std::ios::out);
        if (fio)
        {
            fio << shapes.size() << std::endl;
            
            for (Shape s : shapes)
            {
                if (s.type == primitive::rectangle)
                {
                    if (s.filled)
                    {
                        fio << "2 ";
                    }

                    else
                    {
                        fio << "0 ";
                    }
                    
                    fio << s.x1 << " " << s.y1 << " " << s.x1 << " " << s.y2 << " " << s.x2 << " " << s.y1 << " " << s.x2 << " " << s.y2 << " " << s.color;
                }

                if (s.type == primitive::ellipse)
                {
                    if (s.filled)
                    {
                        fio << "3 ";
                    }

                    else
                    {
                        fio << "1 ";
                    }
                    
                    fio<< s.x1 << " " << s.y1 << " " << s.radiusX << " " << s.radiusY << " " << s.color;
                }

                fio << std::endl;
            }
        }

        else
        {
            std::cout << "File " << p_filename << " Could Not Open" << std::endl;
        }

        // Close the file
        fio.close();
    }

private:
    ECGraphicViewImp* view = nullptr;
    std::vector<Shape> shapes;
};

#endif