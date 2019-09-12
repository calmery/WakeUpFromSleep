#include <IOKit/IOMessage.h>
#include <IOKit/pwr_mgt/IOPMLib.h>
#include <node.h>

using v8::Context;
using v8::Exception;
using v8::Function;
using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::NewStringType;
using v8::Object;
using v8::String;
using v8::Value;

Isolate *isolate;
Local<Function> callbackFunction;
IONotificationPortRef thePortRef;
io_connect_t root_port;
io_object_t notifier;

// Reference:
// https://developer.apple.com/documentation/iokit/ioserviceinterestcallback
void callback(void *refcon, io_service_t service, uint32_t messageType,
              void *messageArgument) {

  if (messageType == kIOMessageSystemHasPoweredOn) {
    CFRunLoopRemoveSource(CFRunLoopGetCurrent(),
                          IONotificationPortGetRunLoopSource(thePortRef),
                          kCFRunLoopCommonModes);
    IODeregisterForSystemPower(&notifier);
    IOServiceClose(root_port);
    IONotificationPortDestroy(thePortRef);
    CFRunLoopStop(CFRunLoopGetMain());

    callbackFunction->Call(isolate->GetCurrentContext(), Null(isolate), 0, NULL)
        .ToLocalChecked();
  }
}

void method(const FunctionCallbackInfo<Value> &arguments) {
  isolate = arguments.GetIsolate();

  if (arguments.Length() == 0) {
    isolate->ThrowException(Exception::Error(
        String::NewFromUtf8(isolate, "Wrong number of arguments",
                            NewStringType::kNormal)
            .ToLocalChecked()));
    return;
  }

  callbackFunction = Local<Function>::Cast(arguments[0]);

  // Reference:
  // https://developer.apple.com/documentation/iokit/1557114-ioregisterforsystempower
  root_port = IORegisterForSystemPower(NULL, &thePortRef, callback, &notifier);

  if (!root_port) {
    isolate->ThrowException(Exception::Error(
        String::NewFromUtf8(isolate,
                            "IORegisterForSystemPower execution Failed",
                            NewStringType::kNormal)
            .ToLocalChecked()));
    return;
  }

  CFRunLoopAddSource(CFRunLoopGetCurrent(),
                     IONotificationPortGetRunLoopSource(thePortRef),
                     kCFRunLoopCommonModes);
  CFRunLoopRun();
}

void initialize(Local<Object> exports, Local<Object> module) {
  NODE_SET_METHOD(module, "exports", method);
}

NODE_MODULE(main, initialize);
