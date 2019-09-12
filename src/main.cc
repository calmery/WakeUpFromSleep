#include <node.h>

using v8::Context;
using v8::Function;
using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::NewStringType;
using v8::Null;
using v8::Object;
using v8::String;
using v8::Value;

void method(const FunctionCallbackInfo<Value> &arguments) {
  Isolate *isolate = arguments.GetIsolate();
  Local<Context> context = isolate->GetCurrentContext();
  Local<Function> callback = Local<Function>::Cast(arguments[0]);
  const unsigned argumentCount = 1;
  Local<Value> argumentVector[argumentCount] = {
      String::NewFromUtf8(isolate, "Hello World", NewStringType::kNormal)
          .ToLocalChecked()};
  callback->Call(context, Null(isolate), argumentCount, argumentVector)
      .ToLocalChecked();
}

void initialize(Local<Object> exports, Local<Object> module) {
  NODE_SET_METHOD(module, "exports", method);
}

NODE_MODULE(main, initialize);
