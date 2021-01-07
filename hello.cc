// addon.cc
#include <node.h>
#include <sstream>
#include <node_api.h>
#include <iostream>
#include "dns.hpp"
#include "dns_struct.hpp"
#include "utility.hpp"

namespace cppdns {

using v8::Context;
using v8::Exception;
using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Number;
using v8::Object;
using v8::String;
using v8::Value;
using v8::Array;

// This is the implementation of the "add" method
// Input arguments are passed using the
// const FunctionCallbackInfo<Value>& args struct
void Add(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  Local<Context> context = isolate->GetCurrentContext();

  // Check the number of arguments passed.
  if (args.Length() < 2) {
    // Throw an Error that is passed back to JavaScript
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate,"Wrong number of arguments").ToLocalChecked()));
    return;
  }

  //TODO: implement here

  //get the the params and coonvert into std string
  v8::String::Utf8Value arg1(isolate, args[0]);
  std::string domainName(*arg1);

  v8::String::Utf8Value arg2(isolate, args[1]);
  std::string destAddress(*arg2);

  //running custom C++ DNS query
  DECODED_RESPONSE *dnsRes = dnsSendQuery(domainName, destAddress);

  //debugging here
  printDecodedResponse(*dnsRes);



  // Perform the operation
  double value = 5;

  Local<Object> obj = Object::New(isolate);
  Local<Object> head = Object::New(isolate);

  //formating header information
  //TODO: Fill in everything
  head->Set(context,
    String::NewFromUtf8(isolate, "msg").ToLocalChecked(),
      Number::New(isolate, dnsRes->head.id))
      .FromJust();


  //formating answer section
  Local<Array> answerArray = Array::New(isolate);

  for(RESOURCE_RECORD ansRR: dnsRes->answer){
    std::cout << "testing: " << ansRR.ttl << std::endl;
  }  


  //adding all sections into the return object
  obj->Set(context,
    String::NewFromUtf8(isolate, "head").ToLocalChecked(),
      head )
      .FromJust();

      

// obj->Set(String::NewFromUtf8(isolate, "hello"), String::NewFromUtf8(isolate, "hello there"));

  // Set the return value (using the passed in
  // FunctionCallbackInfo<Value>&)
  args.GetReturnValue().Set(obj);
}

void Init(Local<Object> exports) {
  NODE_SET_METHOD(exports, "add", Add);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Init)

}  // namespace demo