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
    String::NewFromUtf8(isolate, "id").ToLocalChecked(),
      Number::New(isolate, dnsRes->head.id))
      .FromJust();


  //formating answer section
  Local<Array> answerArray = Array::New(isolate);

  int ansIndex = 0;
  for(RESOURCE_RECORD ansRR: dnsRes->answer){
    Local<Object> ansObj = Object::New(isolate);
    std::string name = convertSequenceLabelToHostName(ansRR.rrName);
    std::string data = convertUnsignedCharToIPAdress(ansRR.rdata);

    

    // std::cout << "testing: " << ansRR.ttl << std::endl;

    std::string strAnsIndex = std::to_string(ansIndex);
    
    //convert into char so it can be used
    const char * charName = name.c_str();
    const char * charStrAnsIndex = strAnsIndex.c_str();
    const char * charData = data.c_str();

    ansObj->Set(context,
    String::NewFromUtf8(isolate, "name").ToLocalChecked(), String::NewFromUtf8(isolate, charName).ToLocalChecked()
      ).FromJust();

    ansObj->Set(context,
    String::NewFromUtf8(isolate, "type").ToLocalChecked(), Number::New(isolate, ansRR.rrType)
      ).FromJust();

    ansObj->Set(context,
    String::NewFromUtf8(isolate, "class").ToLocalChecked(), Number::New(isolate, ansRR.rrClass)
      ).FromJust();

    ansObj->Set(context,
    String::NewFromUtf8(isolate, "ttl").ToLocalChecked(), Number::New(isolate, ansRR.ttl)
      ).FromJust();

    ansObj->Set(context,
    String::NewFromUtf8(isolate, "length").ToLocalChecked(), Number::New(isolate, ansRR.rdlength)
      ).FromJust();

    ansObj->Set(context,
    String::NewFromUtf8(isolate, "data").ToLocalChecked(), String::NewFromUtf8(isolate, charData).ToLocalChecked()
      ).FromJust();

    //appends object into the answeArray
    answerArray->Set(context,
    String::NewFromUtf8(isolate, charStrAnsIndex).ToLocalChecked(), ansObj)
      .FromJust();

    
      

    ansIndex++;
    std::string strAnsIndex2 = std::to_string(ansIndex);
    const char * charStrAnsIndex2 = strAnsIndex.c_str();

  Local<Object> ansObj2 = Object::New(isolate);

    // ansObj2->Set(context,
    // String::NewFromUtf8(isolate, "name").ToLocalChecked(),
    //   String::NewFromUtf8(isolate, charName).ToLocalChecked()
    //    )
    //   .FromJust();

    answerArray->Set(context,
    ansIndex,
      String::NewFromUtf8(isolate, "testingdudeomg").ToLocalChecked())
      .FromJust();


  }  


  //adding all sections into the return object
  obj->Set(context,
    String::NewFromUtf8(isolate, "head").ToLocalChecked(),
      head )
      .FromJust();
  
  obj->Set(context,
    String::NewFromUtf8(isolate, "answer").ToLocalChecked(),
      answerArray )
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