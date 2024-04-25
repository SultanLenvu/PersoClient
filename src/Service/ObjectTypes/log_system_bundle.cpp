#include "log_system_bundle.h"
#include "global_environment.h"
#include "log_system.h"

LogSystemBundle::LogSystemBundle() : QObject{nullptr} {
  LogSystem* ls =
      GlobalEnvironment::instance()->getObject<LogSystem>("LogSystem");
  assert(ls);

  connect(this, &LogSystemBundle::logging, ls, &LogSystem::generate);
}
