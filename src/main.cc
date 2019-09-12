#include <node.h>

using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::NewStringType;
using v8::Object;
using v8::String;
using v8::Value;

void method(const FunctionCallbackInfo<Value> &args) {
  Isolate *isolate = args.GetIsolate();
  args.GetReturnValue().Set(
      String::NewFromUtf8(isolate, "Hello World", NewStringType::kNormal)
          .ToLocalChecked());
}

void initialize(Local<Object> exports, Local<Object> module) {
  NODE_SET_METHOD(module, "exports", method);
}

NODE_MODULE(main, initialize);
