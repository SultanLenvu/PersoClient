#include "async_wrapper.h"
#include "global_environment.h"

AsyncWrapper::AsyncWrapper(const QString& name)
    : Basis(GlobalEnvironment::instance()->getObject<NamedObject>(name)) {}
