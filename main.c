#include <ApplicationServices/ApplicationServices.h>
#include <stdint.h>

#include "deps/parson/parson.h"
#include "types.h"

struct keyblock blockedKeys[32];
size_t numBlockedKeys;

CGEventRef filterKeysCallback(CGEventTapProxy proxy, CGEventType type,
                              CGEventRef event, void *refcon) {
  CGKeyCode keyCode =
      (CGKeyCode)CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);

  size_t i;
  double timeElapsed;

  for (i = 0; i < numBlockedKeys; i += 1) {
    if (keyCode == (CGKeyCode)blockedKeys[i].keyCode) {
      gettimeofday(&blockedKeys[i].start, NULL);

      timeElapsed =
          (blockedKeys[i].start.tv_sec - blockedKeys[i].oldStart.tv_sec) *
          1000.0;
      timeElapsed +=
          (blockedKeys[i].start.tv_usec - blockedKeys[i].oldStart.tv_usec) /
          1000.0;

      gettimeofday(&blockedKeys[i].oldStart, NULL);

      if (timeElapsed < blockedKeys[i].delay) {
        return NULL;
      } else {
        return event;
      }
    }
  }

  return event;
}

bool load_settings() {
  JSON_Value *root_value = json_parse_file("/usr/local/etc/keyblock/settings.json");

  if (json_value_get_type(root_value) != JSONObject) {
    return false;
  }

  JSON_Object *root_obj = json_value_get_object(root_value);
  JSON_Array *keys = json_object_get_array(root_obj, "keys");

  size_t i;
  JSON_Object *key;
  numBlockedKeys = json_array_get_count(keys);

  for (i = 0; i < numBlockedKeys; i += 1) {
    key = json_array_get_object(keys, i);
    blockedKeys[i].keyCode = (int)json_object_dotget_number(key, "keyCode");
    blockedKeys[i].delay = (int)json_object_dotget_number(key, "delay");

    gettimeofday(&blockedKeys[i].start, NULL);
    gettimeofday(&blockedKeys[i].oldStart, NULL);
  }

  json_value_free(root_value);

  return true;
}

int main(void) {
  if (load_settings() == false) {
    printf("Cannot find/parse settings.json\n");

    return EXIT_FAILURE;
  }

  // Create the event tap before any pre-existing event taps
  CFMachPortRef eventTap = CGEventTapCreate(
      kCGSessionEventTap, kCGHeadInsertEventTap, kCGEventTapOptionDefault,
      CGEventMaskBit(kCGEventKeyDown), filterKeysCallback, NULL);

  if (!eventTap) {
    printf("Failed to create event tap. Must run with root privileges.\n");
    return EXIT_FAILURE;
  }

  printf("Event tap created. Intercepting events...\n");

  CFRunLoopSourceRef runLoopSource =
      CFMachPortCreateRunLoopSource(kCFAllocatorDefault, eventTap, 0);
  CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource,
                     kCFRunLoopCommonModes);
  CGEventTapEnable(eventTap, true);
  CFRunLoopRun();

  return EXIT_SUCCESS;
}
