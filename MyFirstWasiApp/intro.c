#include <assert.h>
#include <mono-wasi/driver.h>
#include <mono/metadata/object.h>
#include <mono/metadata/exception.h>
#include <mono/metadata/appdomain.h>
#include <stdio.h>
#include <stdlib.h>


#define __IMPORT(module, name) __attribute__((__import_module__(#module), __import_name__(#name)))
extern char* _http_post_request_runtime(char* p) __IMPORT(runtime, _http_post_request_runtime);



void attach_internal_calls()
{
  mono_add_internal_call("hello.Program::_http_post_request_runtime", _http_post_request_runtime);

}



static int runtime_initialized = 0;
extern void _start(void);











MonoObject* invoke_hello(MonoObject* target_instance, void* method_params[]) {
    static MonoMethod* method_hello = NULL;

    if (!method_hello) {
        method_hello = lookup_dotnet_method("MyFirstWasiApp.dll", "hello", "Program", "Hello", -1);
        assert(method_hello);
    }

    MonoObject* exception = NULL;
    MonoObject* res = mono_runtime_invoke(method_hello, target_instance, method_params, &exception);
    assert(!exception);

    return res;
}



__attribute__((export_name("run")))
char* hello(const char* input) {


    if (runtime_initialized == 0) {
        _start();
        attach_internal_calls();
        runtime_initialized = 1;
    }
    MonoString* inputArg =  mono_string_new ( mono_domain_get() , input);
    void *params[] = { inputArg };
    
    MonoObject* invokeResult =  invoke_hello(NULL, params);

    MonoString* stringResult =  mono_object_to_string (invokeResult, NULL);
    char* final =  mono_string_to_utf8_checked (stringResult, NULL);
    return final;
}

