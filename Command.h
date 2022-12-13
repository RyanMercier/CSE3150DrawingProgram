#ifndef COMMAND_H
#define COMMAND_H

#include <vector>
#include "Shape.h"

enum operation
{
    draw,
    move,
    del
};

class Command
{
public:
    Command(operation p_op, std::vector<Shape> p_shapes)
    {
        op = p_op;
        shapes = p_shapes;
    }

    ~Command() {}

    operation op;
    std::vector<Shape> shapes;
    int deltaX;
    int deltaY;
};

#endif COMMAND_H
