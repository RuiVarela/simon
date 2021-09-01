#include "source/Project.h"

static re::Simon simon;

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
    void WebUpdate(void) 
    {
        simon.step();
    }
#endif


int main(void)
{
    simon.setup();
    
#if defined(PLATFORM_WEB)

    emscripten_set_main_loop(WebUpdate, 0, 1);
#else
    simon.run();
#endif

    simon.shutdown();

    return 0;
}
