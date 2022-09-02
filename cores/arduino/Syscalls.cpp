
extern "C" {

void _init(void) {}
void _fini(void) {}

int __errno;
void *__dso_handle = (void *)&__dso_handle;
}

namespace std
{
    
//These are error handlers. They could alternatively restart, or log an error
void __throw_bad_function_call() { while (1); }
void __cxa_pure_virtual() { while (1); }

} // namespace std
