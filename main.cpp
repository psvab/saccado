
#include <cstdlib>

#include "ComponentManager.h"
#include "Logger.h"


using namespace std;
using namespace SC;

/*
 * the main entry point into the application
 */
int main(int argc, char** argv) 
{
    TRY
    auto& mgr = ComponentManager::instance();    
    EXCEPTION_ASSERT(argc>=2, "Usage :<binary> <config_file>");
    LOG_INFO("Loading config from " << argv[1]);
    mgr.load_from_json(argv[1]);
    mgr.run();
    FATAL_CATCH              
    return 0;
}

