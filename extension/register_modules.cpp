#include "register_modules.h"
#include "example_module.h"

#include <soft/classdb.h>

void initialize_python_module()
{
    soft::ClassDB::register_class<ExampleModule>();
}

void uninitialize_python_module()
{
    
}