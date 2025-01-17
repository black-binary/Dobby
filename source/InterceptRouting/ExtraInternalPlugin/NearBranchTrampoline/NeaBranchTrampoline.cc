#include "InterceptRouting/ExtraInternalPlugin/NearBranchTrampoline/NearBranchTrampoline.h"

#include "dobby_internal.h"

#include "MemoryAllocator/NearMemoryArena.h"

#include "InterceptRouting/ExtraInternalPlugin/RegisterPlugin.h"

using namespace zz;

PUBLIC void dobby_enable_near_branch_trampoline() {
  RoutingPlugin *plugin = new NearBranchTrampolinePlugin;
  ExtraInternalPlugin::registerPlugin("near_branch_trampoline", plugin);
  ExtraInternalPlugin::near_branch_trampoline = plugin;
}

PUBLIC void dobby_disable_near_branch_trampoline() {
  NearBranchTrampolinePlugin *plugin = (NearBranchTrampolinePlugin *)ExtraInternalPlugin::near_branch_trampoline;
  delete plugin;
  ExtraInternalPlugin::near_branch_trampoline = NULL;
}

#if 0
int NearBranchTrampolinePlugin::PredefinedTrampolineSize() {
#if __arm64__
  return 4;
#elif __arm__
  return 4;
#endif
}
#endif

extern CodeBufferBase *GenerateNearTrampolineBuffer(InterceptRouting *routing, addr_t from, addr_t to);
bool NearBranchTrampolinePlugin::GenerateTrampolineBuffer(InterceptRouting *routing, void *src, void *dst) {
  CodeBufferBase *trampoline_buffer;
  trampoline_buffer = GenerateNearTrampolineBuffer(routing, (addr_t)src, (addr_t)dst);
  if (trampoline_buffer == NULL)
    return false;
  routing->SetTrampolineBuffer(trampoline_buffer);
  return true;
}

// generate trampoline, patch the original entry
bool NearBranchTrampolinePlugin::Active(InterceptRouting *routing) {
  addr_t src, dst;
  HookEntry *entry = routing->GetHookEntry();
  src = (addr_t)entry->target_address;
  dst = (addr_t)routing->GetTrampolineTarget();
  return true;
}
