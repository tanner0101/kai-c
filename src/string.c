
typedef struct Intern Intern;
struct Intern {
    Intern *next;
    size_t len;
    char data[];
};

Arena internArena;
Map interns;

const char *StrInternRange(const char *start, const char *end) {
    size_t len = end - start;
    u64 hash = HashBytes(start, len);
    u64 key = hash ? hash : 1; // 0 is a sentinal
    Intern *intern = MapGetU64(&interns, key);
    for (Intern *it = intern; it; it = it->next) {
        if (it->len == len && strncmp(it->data, start, len) == 0) {
            return it->data;
        }
    }
    Intern *newIntern = ArenaAlloc(&internArena, offsetof(Intern, data) + len + 1);
    newIntern->len = len;
    newIntern->next = intern;
    ASSERT(intern != newIntern);
    memcpy(newIntern->data, start, len);
    newIntern->data[len] = 0; // Nul terminate
    MapSet(&interns, (void*) key, (void*) newIntern);
    return newIntern->data;
}

const char *StrIntern(const char *str) {
    return StrInternRange(str, str + strlen(str));
}

size_t StrInternLen(const char *str) {
    if (!str) return 0;
    
    Intern *intern = (Intern*) (str - offsetof(Intern, data));
    return intern->len;
}

#if TEST
void test_stringInterning() {
    char *mem = (char*) Alloc(DefaultAllocator, MB(4));

#define ALPHA "alpha"
#define BETA "beta"
    strncpy(mem, ALPHA, sizeof(ALPHA));
    mem[sizeof(ALPHA)] = 0;

    ASSERT(strncmp(ALPHA, mem, sizeof(ALPHA)) == 0);

    const char *stored = StrIntern(ALPHA);
    ASSERT(strncmp(stored, mem, sizeof(ALPHA)) == 0);

    // Push ourselves over allocated space so we allocate a new block
    StrInternRange(mem + MB(1), mem + MB(2));
    ASSERT(ArrayLen(internArena.blocks) > 1);

    const char *retrieved = StrIntern(ALPHA);
    ASSERT(stored == retrieved);

    stored = StrIntern(BETA);
    retrieved = StrIntern(BETA);
    ASSERT(stored == retrieved);

    char stackAlpha[] = ALPHA;
    ASSERT(StrIntern(ALPHA) == StrIntern(stackAlpha));

    char stackBeta[] = BETA;
    ASSERT(StrIntern(BETA) == StrIntern(stackBeta));

#undef ALPHA
#undef BETA
}
#endif

enum StringEscapeError {
    SEE_Error,
    SEE_NoAllocations,
    SEE_AllocatedMem
};

/*
 // TODO(Brett): custom allocator
 StringEscapeError EscapeString(String *str) {
 String s = *str;

 if (s.len < 2) {
 return SEE_Error;
 }

 u8 quote = s[0];
 if (s[s.len-1] != quote && quote != '"') {
 return SEE_Error;
 }

 s = Slice(s, 1, s.len - 1);

 b32 requiresEscape = false;
 for (u32 i = 0; i < s.len; i += 1) {
 if (s[i] == '\\') {
 requiresEscape = true;
 break;
 }
 }

 if (!requiresEscape) {
 *str = s;
 return SEE_NoAllocations;
 }

 // TODO(Brett): actually escape string
 *str = s;
 return SEE_AllocatedMem;
 }
 */
