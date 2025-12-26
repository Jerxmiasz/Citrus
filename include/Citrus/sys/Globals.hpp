#ifndef CITRUS_SYS_SYSGLOBALS_HPP
#define CITRUS_SYS_SYSGLOBALS_HPP
namespace citrus {
  struct _Citrus_Sys_Context {
    _Citrus_Sys_Context();
    ~_Citrus_Sys_Context();
    bool monitor_callabck_set = false;
  };
}

#endif