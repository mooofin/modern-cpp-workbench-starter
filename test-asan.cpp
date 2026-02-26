#include <iostream>

int main() {
    int* leak = new int(42);
    int* heap_overflow = new int[10];
    
    heap_overflow[10] = 999;
    
    delete[] heap_overflow;
    
    return 0;
}
