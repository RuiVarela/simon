#include "source/Project.h"

int main(void)
{

    re::Simon simon;

    simon.setup();
    simon.run();
    simon.shutdown();

    return 0;
}
