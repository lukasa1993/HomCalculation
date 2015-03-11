#include "poly_birdge.h"

#include <iostream>
#include <string>
#include <memory>

using namespace polymake;

Main* polymake_main = NULL;

int polymake_init() {
    try
    {
        if(polymake_main == NULL)
        {
            polymake_main = new Main();
        }
        polymake_main->set_application("polymake_bridge");
        polymake_main->set_custom("$Verbose::credits",2);
        return 1;
    }
    catch(const std::exception &ex)
    {
        std::cout << "ERROR: could not initialize polymake: " << ex.what() << std::endl;
        return 0;
    }
}