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
void query(const FunctionCallbackInfo<Value>& args) {
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


  // Perform the operation
  double value = 5;

  Local<Object> obj = Object::New(isolate);

  Local<Object> head = Object::New(isolate);

  //formating header information
  head->Set(context, String::NewFromUtf8(isolate, "id").ToLocalChecked(),Number::New(isolate, dnsRes->head.id))
      .FromJust();

  head->Set(context, String::NewFromUtf8(isolate, "qr").ToLocalChecked(),Number::New(isolate, dnsRes->head.qr))
      .FromJust();
  
  head->Set(context, String::NewFromUtf8(isolate, "opcode").ToLocalChecked(),Number::New(isolate, dnsRes->head.opcode))
      .FromJust();
  
  head->Set(context, String::NewFromUtf8(isolate, "aa").ToLocalChecked(),Number::New(isolate, dnsRes->head.aa))
      .FromJust();

  head->Set(context, String::NewFromUtf8(isolate, "tc").ToLocalChecked(),Number::New(isolate, dnsRes->head.tc))
      .FromJust();
  
  head->Set(context, String::NewFromUtf8(isolate, "rd").ToLocalChecked(),Number::New(isolate, dnsRes->head.rd))
      .FromJust();
  
  head->Set(context, String::NewFromUtf8(isolate, "ra").ToLocalChecked(),Number::New(isolate, dnsRes->head.ra))
      .FromJust();
  
  head->Set(context, String::NewFromUtf8(isolate, "z").ToLocalChecked(),Number::New(isolate, dnsRes->head.z))
      .FromJust();

  head->Set(context, String::NewFromUtf8(isolate, "ad").ToLocalChecked(),Number::New(isolate, dnsRes->head.ad))
      .FromJust();

  head->Set(context, String::NewFromUtf8(isolate, "cd").ToLocalChecked(),Number::New(isolate, dnsRes->head.cd))
      .FromJust();

  head->Set(context, String::NewFromUtf8(isolate, "rcode").ToLocalChecked(),Number::New(isolate, dnsRes->head.rcode))
      .FromJust();

  head->Set(context, String::NewFromUtf8(isolate, "questionCount").ToLocalChecked(),Number::New(isolate, dnsRes->head.qdcount))
      .FromJust();

  head->Set(context, String::NewFromUtf8(isolate, "answerCount").ToLocalChecked(),Number::New(isolate, dnsRes->head.ancount))
      .FromJust();

  head->Set(context, String::NewFromUtf8(isolate, "authorityCount").ToLocalChecked(),Number::New(isolate, dnsRes->head.nscount))
      .FromJust();

  head->Set(context, String::NewFromUtf8(isolate, "additionalCount").ToLocalChecked(),Number::New(isolate, dnsRes->head.arcount))
      .FromJust();

  //formating question section
  Local<Object> question = Object::New(isolate);

  std::string qname = convertSequenceLabelToHostName(dnsRes->question.qname);
  const char * charQName = qname.c_str();

  question->Set(context, String::NewFromUtf8(isolate, "name").ToLocalChecked(),String::NewFromUtf8(isolate, charQName).ToLocalChecked() )
      .FromJust();
  
  question->Set(context,
    String::NewFromUtf8(isolate, "type").ToLocalChecked(), Number::New(isolate, dnsRes->question.qtype)
      ).FromJust();
  
  question->Set(context,
    String::NewFromUtf8(isolate, "class").ToLocalChecked(), Number::New(isolate, dnsRes->question.qclass)
      ).FromJust();

  


  //formating answer section
  Local<Array> answerArray = Array::New(isolate);

  int ansIndex = 0;
  for(RESOURCE_RECORD ansRR: dnsRes->answer){
    Local<Object> ansObj = Object::New(isolate);
    std::string name = convertSequenceLabelToHostName(ansRR.rrName);
    std::string data = convertUnsignedCharToIPAdress(ansRR.rdata);
    
    //convert into char so it can be used
    const char * charName = name.c_str();
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
    ansIndex, ansObj)
      .FromJust();
    ansIndex++;
  }  

  ansIndex = 0;

  //auth section
  Local<Array> authArray = Array::New(isolate);

  for(RESOURCE_RECORD authRR: dnsRes->authNameServer){
    Local<Object> authObj = Object::New(isolate);
    std::string name = convertSequenceLabelToHostName(authRR.rrName);
    std::string data = convertSequenceLabelToHostName(authRR.rdata);
    
    //convert into char so it can be used
    const char * charName = name.c_str();
    const char * charData = data.c_str();

    authObj->Set(context,
    String::NewFromUtf8(isolate, "name").ToLocalChecked(), String::NewFromUtf8(isolate, charName).ToLocalChecked()
      ).FromJust();

    authObj->Set(context,
    String::NewFromUtf8(isolate, "type").ToLocalChecked(), Number::New(isolate, authRR.rrType)
      ).FromJust();

    authObj->Set(context,
    String::NewFromUtf8(isolate, "class").ToLocalChecked(), Number::New(isolate, authRR.rrClass)
      ).FromJust();

    authObj->Set(context,
    String::NewFromUtf8(isolate, "ttl").ToLocalChecked(), Number::New(isolate, authRR.ttl)
      ).FromJust();

    authObj->Set(context,
    String::NewFromUtf8(isolate, "length").ToLocalChecked(), Number::New(isolate, authRR.rdlength)
      ).FromJust();

    authObj->Set(context,
    String::NewFromUtf8(isolate, "data").ToLocalChecked(), String::NewFromUtf8(isolate, charData).ToLocalChecked()
      ).FromJust();

    //appends object into the answeArray
    authArray->Set(context,
    ansIndex, authObj)
      .FromJust();
    ansIndex++;
  }  
  
  ansIndex = 0;
  
  //additional section
  Local<Array> additionalArray = Array::New(isolate);

  for(RESOURCE_RECORD additionalRR: dnsRes->additional){
    Local<Object> additionalObj = Object::New(isolate);
    std::string name = convertSequenceLabelToHostName(additionalRR.rrName);
    std::string data = convertUnsignedCharToIPAdress(additionalRR.rdata);
    
    //convert into char so it can be used
    const char * charName = name.c_str();
    const char * charData = data.c_str();

    additionalObj->Set(context,
    String::NewFromUtf8(isolate, "name").ToLocalChecked(), String::NewFromUtf8(isolate, charName).ToLocalChecked()
      ).FromJust();

    additionalObj->Set(context,
    String::NewFromUtf8(isolate, "type").ToLocalChecked(), Number::New(isolate, additionalRR.rrType)
      ).FromJust();

    additionalObj->Set(context,
    String::NewFromUtf8(isolate, "class").ToLocalChecked(), Number::New(isolate, additionalRR.rrClass)
      ).FromJust();

    additionalObj->Set(context,
    String::NewFromUtf8(isolate, "ttl").ToLocalChecked(), Number::New(isolate, additionalRR.ttl)
      ).FromJust();

    additionalObj->Set(context,
    String::NewFromUtf8(isolate, "length").ToLocalChecked(), Number::New(isolate, additionalRR.rdlength)
      ).FromJust();

    additionalObj->Set(context,
    String::NewFromUtf8(isolate, "data").ToLocalChecked(), String::NewFromUtf8(isolate, charData).ToLocalChecked()
      ).FromJust();

    //appends object into the answeArray
    additionalArray->Set(context,
    ansIndex, additionalObj)
      .FromJust();
    ansIndex++;
  }  

  //adding all sections into the return object
  obj->Set(context,
    String::NewFromUtf8(isolate, "head").ToLocalChecked(),
      head )
      .FromJust();
  
  obj->Set(context,
    String::NewFromUtf8(isolate, "question").ToLocalChecked(),
      question )
      .FromJust();

  obj->Set(context,
    String::NewFromUtf8(isolate, "answer").ToLocalChecked(),
      answerArray )
      .FromJust();

  obj->Set(context,
    String::NewFromUtf8(isolate, "authority").ToLocalChecked(),
      authArray )
      .FromJust();

  obj->Set(context,
    String::NewFromUtf8(isolate, "additional").ToLocalChecked(),
      additionalArray )
      .FromJust();    

// obj->Set(String::NewFromUtf8(isolate, "hello"), String::NewFromUtf8(isolate, "hello there"));

  // Set the return value (using the passed in
  // FunctionCallbackInfo<Value>&)
  args.GetReturnValue().Set(obj);
}

void Init(Local<Object> exports) {
  NODE_SET_METHOD(exports, "query", query);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Init)

}  // namespace demo