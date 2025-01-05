#include <so/so_null.h>
#include <so/work/so_general_work_abstract.h>

soGeneralWorkAbstract g_soGeneralWorkAbstract;

soGeneralWorkAbstract::soGeneralWorkAbstract()
    : soNullable(true) { }
