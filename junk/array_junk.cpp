#include "../src/common.cpp"
#include "../src/lexer.cpp"

// forward declarations
#include "../src/parser.hpp"
#include "../src/checker.hpp"

#include "../src/parser.cpp"
#include "../src/checker.cpp"


typedef struct TestData {
    i32 num;
} TestData;


i32 main(void) {

    printf("sizeof(TestData) = %lu\n\n", sizeof(TestData));
    printf("sizeof(Array<TestData>) = %lu\n\n", sizeof(Array<TestData>));
    printf("sizeof(ArrayArena<TestData>) = %lu\n\n", sizeof(ArrayArena<TestData>));


    ArrayArena<TestData> aa;

    InitArrayArena(&aa, 10, 7);

    Array<TestData> ar;

    for (u32 i=0; i<14; ++i) {
        InitArray(&aa, &ar);


        DumpArray(ar);
    }

    printf("ar is a slice : %d\n", (bool) ARRAY_IS_SLICE((&ar)));

    ReallocArray(aa, &ar, 15);

    printf("ar is a slice : %d\n", (bool) ARRAY_IS_SLICE((&ar)));

    ReallocArray(aa, &ar, 20);

    Array<TestData> ar2;
    InitArray(MakeDefaultAllocator(), &ar2, 12);
    DumpArray(ar2);

    return 0;
}

