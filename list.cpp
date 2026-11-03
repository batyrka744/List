#include "list.h"



int main() {

    List list = {};
    ListInit(&list, 15);
    PrintfList(&list);

    for (size_t i = 1; i < 10; i++) {
        ListAdd(&list, list.head, i*10);
    }

    // for (size_t i = 0; i < list.capacity; i++) {
    //     printf("%lg ", list.data[i]);
    // }
    // printf("\n");
    // for (size_t i = 0; i < list.capacity; i++) {
    //     printf("%d ", list.next[i]);
    // }
    PrintfList(&list);

    ListAdd(&list, 3, 35);

    PrintfList(&list);

    ListDelete(&list, 6);
    PrintfList(&list);

    
    return 0;
}

errors PrintfList(List* list) {

    ASSERT_OK(list);

    int current = list->tail; 

    while (current > 0) {
        printf("%lg ", list->data[current]);
        current = list->next[current]; 
    }
    printf("\n");


    ASSERT_OK(list);
    return ERR_OK;
}



errors ListInit(List* list, size_t capacity ON_DBG(, const char* name, const char* file, size_t line)) {

    list->capacity = capacity;
    list->size = 0;
    list->tail = 0;
    list->head = 0;

    if (capacity <= 0) return ERR_WRONG_CAPACITY;

    my_assert(list != NULL, ERR_NULL_PTR);

    list->data = (ListElem_t*)calloc(capacity, sizeof(ListElem_t));
    list->next = (int*)calloc(capacity, sizeof(int));
    list->prev = (int*)calloc(capacity, sizeof(int));

    if (list->data == NULL || list->next == NULL || list->prev == NULL) 
        return ERR_NO_MEMORY;


    list->free_list = 1;


    for (size_t i = 1; i < capacity - 1; i++) {
        list->data[i] = POISON_DATA;
        list->next[i] = (int)(i + 1);
        list->prev[i] = FREE_CELL;
    }
    list->data[list->capacity - 1] = POISON_DATA;
    list->prev[capacity - 1] = FREE_CELL;

    // for (size_t i = 0; i < list->capacity; i++) {
    //     printf("%lg ", list->data[i]);
    // }
    // printf("\n");
    // for (size_t i = 0; i < list->capacity; i++) {
    //     printf("%d ", list->next[i]);
    // }
    // printf("\n");
    // for (size_t i = 0; i < list->capacity; i++) {
    //     printf("%d ", list->prev[i]);
    // }
    
        

    if (list->data == NULL) 
        return ERR_NO_MEMORY;
    
    //stk->real_data[0] = canary;

    //stk->data = stk->real_data + 1;
    list->next[0] = 0;
    list->prev[0] = 0;
    list->data[0] = POISON_DATA;

    //stk->real_data[capacity + 1] = canary;

    

    #ifdef STACK_DEBUG
    stk->file = file;
    stk->name = name;
    stk->line = line;
    #endif 

    ASSERT_OK(list);

    return ERR_OK;
}

errors ListAdd(List* list, int num_prev_elem, ListElem_t value) {

    ASSERT_OK(list);

    int free_index = list->free_list;
    list->free_list = list->next[list->free_list];

    // for (size_t i = 1; i < list->capacity; i++) {
    //     if (list->next[i] == FREE_CELL) {
    //         list->data[i] = value;
    //         free_index = i;
    //         break;
    //     }
    // }
    // my_assert(free_index != -1, ERR_OVERFLOW);

    list->next[free_index] = list->next[num_prev_elem];
    list->prev[free_index] = num_prev_elem;

    list->prev[list->next[free_index]] = free_index;
    list->next[num_prev_elem] = free_index;
    
    list->data[free_index] = value;
    

    if (num_prev_elem == list->head || list->size == 0)
        list->head = free_index;
    if (list->size == 0)
        list->tail = free_index;
    
    list->size++;

    ASSERT_OK(list);

    return ERR_OK;
}

errors ListDelete(List* list, int num_of_delete) {

    ASSERT_OK(list);
    my_assert(num_of_delete <= list->capacity, ERR_ARR_CANARY);
    my_assert(num_of_delete != 0, ERR_ARR_CANARY);


    list->next[list->prev[num_of_delete]] = list->next[num_of_delete];

    list->prev[list->next[num_of_delete]] = list->prev[num_of_delete];


    if (num_of_delete == list->head) 
        list->head = list->prev[num_of_delete];

    if (num_of_delete == list->tail) 
        list->tail = list->next[num_of_delete];

    list->next[num_of_delete] = list->free_list;
    list->prev[num_of_delete] = FREE_CELL;
    list->data[num_of_delete] = POISON_DATA;

    
    list->size--;

    ASSERT_OK(list);
    return ERR_OK;
}



errors ListError(List* list) {

    if (list == NULL) 
        return ERR_NULL_PTR;
    
    if (list->data == NULL) 
        return ERR_NULL_DATA;
    
    if (list->next == NULL) 
        return ERR_NULL_NEXT;
    
    if (list->prev == NULL) 
        return ERR_NULL_PREV;
    
    if (list->capacity < 0) 
        return ERR_WRONG_CAPACITY;
    
    if (list->size < 0) 
        return ERR_WRONG_SIZE;
    
    if (list->size > list->capacity) 
        return ERR_OVERFLOW;
    

    return ERR_OK;
}

void ListDump(const List* list,  errors err_code, const char* file, int line, const char* func) {

    if (list == NULL) {
        printf("list_t [NULL pointer]\n");
        assert(0);
    }

    PrintTop(list, err_code, file, line, func);

    PrintBottom(list);

    //StackDump_file(list, err_code, file, line, func);

}

void PrintTop(const List* list, errors err_code, const char* file, int line, const char* func) {

    for (size_t i = 0; i < 19; i++) printf("=");
    printf(" LIST DUMP ");
    for (size_t i = 0; i < 19; i++) printf("=");


    printf("\nError code: %d (%s)\n", err_code, ErrCode(err_code));

    printf("Called from: %s() at %s:%d\n", func, file, line);

    #ifdef STACK_DEBUG
        printf("\nstack_t \"%s\"[%p] Created at %s:%d\n", stk->name, stk, stk->file, stk->line);
    #endif

    printf("capacity = %d\n", list->capacity);
    printf("size = %d\n", list->size);         
    printf("data [%p]\n{\n", list->data);
    printf("next [%p]\n{\n", list->next);
    printf("prev [%p]\n{\n", list->prev);
}

void PrintBottom(const List* list) {
    if (list->data == NULL || list->capacity <= 0) {
        printf("[data unavailable]\n}\n");
        return;
    }
    for (size_t i = 0; i < list->capacity; i++) {
        printf("   ");

        if (i < list->size) {
            printf("*");
        } else {
            printf(" ");
        }

        printf("[%zu] = %lg", i, list->data[i]);
        if (isnan(list->data[i])) {
            printf(" (POISON)");
        }
        printf("%d %d", list->next[i], list->prev[i]);
        
        printf("\n");
    }

    printf("}\n");

    for (size_t i = 0; i < 50; i++) printf("=");
    printf("\n");
}

const char* ErrCode(errors err) {

    switch(err) {
        case ERR_NULL_PTR:
            return "ERR_NULL_PTR";
        case ERR_NULL_DATA:
            return "ERR_NULL_DATA";
        case ERR_WRONG_CAPACITY:
            return "ERR_WRONG_CAPACITY";
        case ERR_WRONG_SIZE:
            return "ERR_WRONG_SIZE";
        case ERR_OVERFLOW:
            return "ERR_OVER_FLOW";
        case ERR_NO_MEMORY:
            return "ERR_NO_MEMORY";
        case ERR_ARR_CANARY:
            return "ERR_ARR_CANARY";
        case ERR_LIST_CANARY:
            return "ERR_LIST_CANARY";
        case ERR_NULL_NEXT:
            return "ERR_NULL_NEXT";
        case ERR_NULL_PREV:
            return "ERR_NULL_PREV";
        default:
            printf("No such error in switch-case\n");
    }

    return "ERR_OK";
}

void FreeMemory(List* list) {
    free(list->data);
    free(list->next);
    free(list->prev);
}