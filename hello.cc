// addon.cc
#include <node.h>
#include <sstream>
#include <node_api.h>
#include <iostream>

namespace cppdns {

using v8::Exception;
using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Number;
using v8::Object;
using v8::String;
using v8::Value;

// This is the implementation of the "add" method
// Input arguments are passed using the
// const FunctionCallbackInfo<Value>& args struct
void Add(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  // Check the number of arguments passed.
  if (args.Length() < 2) {
    // Throw an Error that is passed back to JavaScript
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate,"Wrong number of arguments").ToLocalChecked()));
    return;
  }

  //TODO: implement here
  v8::Local<v8::String> argsINPUT;
  v8::String::Utf8Value strTest(isolate, args[0]);
  std::string cppStr(*strTest);

  std::stringstream str;
  std::cout << "hello, " << cppStr << std::endl;

  // Perform the operation
  double value = 5;

  // Set the return value (using the passed in
  // FunctionCallbackInfo<Value>&)
  args.GetReturnValue().Set(value);
}

void Init(Local<Object> exports) {
  NODE_SET_METHOD(exports, "add", Add);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Init)

}  // namespace demo