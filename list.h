#include <stdio.h>
#include <math.h>
#include <assert.h>

typedef double ListElem_t;

const ListElem_t canary = 0xEBADA1;

struct List {
    ListElem_t* data;
    int* next;
    int* prev;

    int head;
    int tail;
    size_t size, capacity;

    int free_list;
};

#ifdef STACK_DEBUG
#define ON_DBG(...)  __VA_ARGS__
#else
#define ON_DBG(...)
#endif

#define BLACK "\x1b[30m"
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN "\x1b[36m"
#define WHITE "\x1b[37m"
#define RESET "\x1b[0m"

#define my_assert(exp, ret) if(!(exp)) {\
    \
    printf(YELLOW "%s:%d: [Assertation failed: %s, file %s, line %d]\n" RESET, __FILE__, __LINE__, #exp,  __FILE__, __LINE__);\
    return ret;} 

#define POISON_DATA NAN
#define POISON_SIZE -1488
#define FREE_CELL -1

#define LIST_INIT(list, capacity) ListInit(&(list), (capacity) ON_DBG(, #list, __FILE__, __LINE__))
#define LIST_DESTROY(stk) ListDestroy(&(list))
#define ASSERT_OK(list) do {\
    errors err = ListError(list);\
    if (err != ERR_OK) {\
        ListDump(list, err, __FILE__, __LINE__, __func__);\
        free(list->next);\
        free(list->data);\
        free(list->prev);\
        assert(0);}\
    } while(0);

#define FREE_CELL -1


enum errors {
    ERR_OK = 0, 
    ERR_NULL_PTR = 1, 
    ERR_NULL_DATA = 2,
    ERR_WRONG_CAPACITY = 3, 
    ERR_WRONG_SIZE = 4,
    ERR_OVERFLOW = 5, 
    ERR_NO_MEMORY = 6, 
    ERR_ARR_CANARY = 7, 
    ERR_LIST_CANARY = 8, 
    ERR_NULL_NEXT = 9,
    ERR_NULL_PREV = 10
};

errors ListAdd(List* list, int num_prev_elem, ListElem_t value);

errors PrintfList(List* list);

errors ListInit(List* list, size_t capacity ON_DBG(, const char* name, const char* file, size_t line));

void ListDump(const List* list,  errors err_code, const char* file, int line, const char* func);

void PrintTop(const List* list, errors err_code, const char* file, int line, const char*func);

void PrintBottom(const List* list);

errors ListError(List* list);

const char* ErrCode(errors err);

errors ListDelete(List* list, int num_of_delete);

void FreeMemory(List* list);