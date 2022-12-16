//
//  Created by Yufeng Wu on 3/2/22.
//

#include "Application.h"

// Test graphical view code
int real_main(int argc, char **argv)
{
    if (argc > 1)
    {
        Application app = Application(argv[1]);
    }

    else
    {
        Application app = Application("tmp.dat");
    }

    return 0;
}

int main(int argc, char **argv)
{
    return real_main(argc, argv);
}

