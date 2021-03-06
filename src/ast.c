#define EXPR_KIND_START       0x100
#define STMT_KIND_START       0x200
#define DECL_KIND_START       0x300
#define AST_KIND_START        0x400

#define EXPR_KINDS                                    \
    FOR_EACH(Ident, "identifier")                     \
    FOR_EACH(Paren, "parenthesis")                    \
    FOR_EACH(Call, "call")                            \
    FOR_EACH(Selector, "selector")                    \
    FOR_EACH(Subscript, "subscript")                  \
    FOR_EACH(Slice, "slice")                          \
    FOR_EACH(Unary, "unary")                          \
    FOR_EACH(Binary, "binary")                        \
    FOR_EACH(Ternary, "ternary")                      \
    FOR_EACH(Cast, "cast")                            \
    FOR_EACH(Autocast, "autocast")                    \
    FOR_EACH(KeyValue, "key value")                   \
    FOR_EACH(LocationDirective, "location directive") \
    FOR_EACH(LitNil, "nil literal")                   \
    FOR_EACH(LitInt, "integer literal")               \
    FOR_EACH(LitFloat, "float literal")               \
    FOR_EACH(LitString, "string literal")             \
    FOR_EACH(LitCompound, "compound literal")       \
    FOR_EACH(LitFunction, "function literal")         \
    FOR_EACH(TypePointer, "pointer type")             \
    FOR_EACH(TypeArray, "array type")                 \
    FOR_EACH(TypeSlice, "slice type")                 \
    FOR_EACH(TypeStruct, "struct type")               \
    FOR_EACH(TypeEnum, "enum type")                   \
    FOR_EACH(TypeUnion, "union type")                 \
    FOR_EACH(TypePolymorphic, "polymorphic type")     \
    FOR_EACH(TypeVariadic, "variadic type")           \
    FOR_EACH(TypeFunction, "function type")

#define STMT_KINDS                 \
    FOR_EACH(Empty, "empty")       \
    FOR_EACH(Label, "label")       \
    FOR_EACH(Assign, "assignment") \
    FOR_EACH(Return, "return")     \
    FOR_EACH(Defer, "defer")       \
    FOR_EACH(Using, "using")       \
    FOR_EACH(Goto, "goto")         \
    FOR_EACH(Block, "block")       \
    FOR_EACH(If, "if")             \
    FOR_EACH(For, "for")           \
    FOR_EACH(ForIn, "for in")      \
    FOR_EACH(Switch, "switch")

// TODO: This needs to include some more directives (for example static asserts `#assert` should be supported at top level)
#define DECL_KINDS                 \
    FOR_EACH(Variable, "variable") \
    FOR_EACH(Constant, "constant") \
    FOR_EACH(Import, "import")

typedef enum ExprKind ExprKind;
enum ExprKind {
    ExprKind_Invalid = 0,

    _ExprKind_Start = EXPR_KIND_START,
#define FOR_EACH(kindName, ...) ExprKind_##kindName,
    EXPR_KINDS
#undef FOR_EACH
    _ExprKind_End,
};

typedef enum StmtKind StmtKind;
enum StmtKind {
    StmtKind_Invalid = 0,

    _StmtKind_Start = STMT_KIND_START,
#define FOR_EACH(kindName, ...) StmtKind_##kindName,
    STMT_KINDS
#undef FOR_EACH
    _StmtKind_End,

    _StmtExprKind_Start = EXPR_KIND_START,
#define FOR_EACH(kindName, ...) StmtExprKind_##kindName,
    EXPR_KINDS
#undef FOR_EACH
    _StmtExprKind_End,

    _StmtDeclKind_Start = DECL_KIND_START,
#define FOR_EACH(kindName, ...) StmtDeclKind_##kindName,
    DECL_KINDS
#undef FOR_EACH
    _StmtDeclKind_End
};

typedef enum DeclKind DeclKind;
enum DeclKind {
    DeclKind_Invalid = 0,

    _DeclKind_Start = DECL_KIND_START,
#define FOR_EACH(kindName, ...) DeclKind_##kindName,
    DECL_KINDS
#undef FOR_EACH
    _DeclKind_End,
};

const char *AstDescriptions[] = {
#define FOR_EACH(kindName, s) "" s "",
    [EXPR_KIND_START] = "invalid",
    EXPR_KINDS
    "invalid",
    [STMT_KIND_START] = "invalid",
    STMT_KINDS
    "invalid",
    [DECL_KIND_START] = "invalid",
    DECL_KINDS
    "invalid",
#undef FOR_EACH
};


typedef struct Expr Expr;
typedef struct Stmt Stmt;
typedef struct Decl Decl;

// predefine all Ast Structs
#define FOR_EACH(kindName, s) typedef struct Expr_##kindName Expr_##kindName;
EXPR_KINDS
#undef FOR_EACH

#define FOR_EACH(kindName, s) typedef struct Stmt_##kindName Stmt_##kindName;
STMT_KINDS
#undef FOR_EACH

#define FOR_EACH(kindName, s) typedef struct Decl_##kindName Decl_##kindName;
DECL_KINDS
#undef FOR_EACH

typedef struct AstInvalid AstInvalid;
struct AstInvalid {
    Position start;
    Position end;
};

struct Expr_Ident {
    Position start;
    const char *name;
};

struct Expr_Paren {
    Position start;
    Expr *expr;
    Position end;
};

struct Expr_Call {
    Position start;
    Expr *expr;
    DynamicArray(Expr_KeyValue *) args;
    Position end;
};

struct Expr_Selector {
    Position start;
    Expr *expr;
    const char *name;
    Position end;
};

struct Expr_Subscript {
    Position start;
    Expr *expr;
    Expr *index;
    Position end;
};

struct Expr_Slice {
    Position start;
    Expr *expr;
    Expr *lo;
    Expr *hi;
    Position end;
};

struct Expr_Unary {
    Position start;
    TokenKind op;
    Expr *expr;
};

struct Expr_Binary {
    Position start;
    TokenKind op;
    Position pos;
    Expr *lhs;
    Expr *rhs;
};

struct Expr_Ternary {
    Position start;
    Expr *cond;
    Expr *pass;
    Expr *fail;
};

struct Expr_Cast {
    Position start;
    Expr *type;
    Expr *expr;
};

struct Expr_Autocast {
    Position start;
    Expr *expr;
};

typedef enum KeyValueFlag KeyValueFlag;
enum KeyValueFlags {
    KeyValueFlagIndex = 1,
};
struct Expr_KeyValue {
    Position start;
    // TODO: We should add support for C style {['0'] = 0; ['1'] = 1; } which may will require a different key type
    Expr *key; 
    Expr *value;
    u8 flags;
};

struct Expr_LocationDirective {
    Position start;
    const char *name;
};

struct Expr_LitNil {
    Position start;
};

struct Expr_LitInt {
    Position start;
    u64 val;
};

struct Expr_LitFloat {
    Position start;
    f64 val;
};

struct Expr_LitString {
    Position start;
    const char *val;
};

struct Expr_LitCompound {
    Position start;
    Expr *type;
    DynamicArray(Expr_KeyValue *) elements;
    Position end;
};

struct Expr_LitFunction {
    Position start;
    Expr *type;
    Stmt_Block *body;
    u8 flags;
};

struct Expr_TypePointer {
    Position start;
    Expr *type;
};

struct Expr_TypeArray {
    Position start;
    Expr *length;
    Expr *type;
};

struct Expr_TypeSlice {
    Position start;
    Expr *type;
};

typedef struct AggregateItem AggregateItem;
struct AggregateItem {
    Position start;
    DynamicArray(const char *) names;
    Expr *type;
};

struct Expr_TypeStruct {
    Position start;
    DynamicArray(AggregateItem) items;
};

struct Expr_TypeUnion {
    Position start;
    DynamicArray(AggregateItem) items;
};

typedef struct EnumItem EnumItem;
struct EnumItem {
    Position start;
    const char *name;
    Expr *init;
};

struct Expr_TypeEnum {
    Position start;
    Expr *explicitType;
    DynamicArray(EnumItem) items;
};

struct Expr_TypePolymorphic {
    Position start;
    const char *name;
};

typedef enum TypeVariadicFlag TypeVariadicFlag;
enum TypeVariadicFlags {
    TypeVariadicFlagCVargs = 1,
};
struct Expr_TypeVariadic {
    Position start;
    Expr *type;
    b8 flags;
};

struct Expr_TypeFunction {
    Position start;
    DynamicArray(Expr *) result;
    DynamicArray(Expr_KeyValue *) params;
};

struct Stmt_Empty {
    Position start;
};

struct Stmt_Label {
    Position start;
    const char *name;
};

struct Stmt_Assign {
    Position start;
    DynamicArray(Expr *) lhs;
    DynamicArray(Expr *) rhs;
};

struct Stmt_Return {
    Position start;
    DynamicArray(Expr *) exprs;
};

struct Stmt_Defer {
    Position start;
    Stmt *stmt;
};

struct Stmt_Using {
    Position start;
    Expr *expr;
};

struct Stmt_Goto {
    Position start;
    const char *keyword;
    Expr_Ident *label;
};

struct Stmt_Block {
    Position start;
    DynamicArray(Stmt *) stmts;
    Position end;
};

struct Stmt_If {
    Position start;
    Expr *cond;
    Stmt *pass;
    Stmt *fail;
};

struct Stmt_For {
    Position start;
    Stmt *init;
    Expr *cond;
    Stmt *step;
    Stmt_Block *body;
};

struct Stmt_ForIn {
    Position start;
    Expr_Ident *valueName;
    Expr_Ident *indexName;
    Expr *aggregate;
    Stmt_Block *body;
};

typedef struct SwitchCase SwitchCase;
struct SwitchCase {
    Token token;
    DynamicArray(Expr *) matches;
    Stmt_Block *block;
};

struct Stmt_Switch {
    Position start;
    Expr *match;
    DynamicArray(SwitchCase *) cases;
};

struct Decl_Variable {
    Position start;
    DynamicArray(Expr_Ident *) names;
    Expr *type;
    DynamicArray(Expr *) values;
};

struct Decl_Constant {
    Position start;
    DynamicArray(Expr_Ident *) names;
    Expr *type;
    DynamicArray(Expr *) values;
};

struct Decl_Import {
    Position start;
    const char *path;
    const char *alias;
};

#define FOR_EACH(kindName, s) Expr_##kindName kindName;
typedef union ExprValue ExprValue;
union ExprValue {
    EXPR_KINDS
};
#undef FOR_EACH

#define FOR_EACH(kindName, s) Stmt_##kindName kindName;
typedef union StmtValue StmtValue;
union StmtValue {
    STMT_KINDS
};
#undef FOR_EACH

#define FOR_EACH(kindName, s) Decl_##kindName kindName;
typedef union DeclValue DeclValue;
union DeclValue {
    DECL_KINDS
};
#undef FOR_EACH

struct Stmt {
    StmtKind kind;
    union {
        Position start;
        StmtValue stmt;
        ExprValue expr;
        DeclValue decl;
        AstInvalid Invalid;

#define FOR_EACH(kindName, ...) Stmt_##kindName kindName;
        STMT_KINDS
#undef FOR_EACH

#define FOR_EACH(kindName, ...) Expr_##kindName kindName;
        EXPR_KINDS
#undef FOR_EACH

#define FOR_EACH(kindName, ...) Decl_##kindName kindName;
        DECL_KINDS
#undef FOR_EACH
    };
};

struct Expr {
    ExprKind kind;
    union {
        Position start;
        ExprValue expr;
        AstInvalid Invalid;

#define FOR_EACH(kindName, ...) Expr_##kindName kindName;
        EXPR_KINDS
#undef FOR_EACH
    };
};

struct Decl {
    DeclKind kind;
    union {
        Position start;
        DeclValue decl;
        AstInvalid Invalid;

#define FOR_EACH(kindName, ...) Decl_##kindName kindName;
        DECL_KINDS
#undef FOR_EACH
    };
};

b32 isExpr(Stmt *stmt) {
    return stmt->kind > _ExprKind_Start && stmt->kind < _ExprKind_End;
}

b32 isDecl(Stmt *stmt) {
    return stmt->kind > _DeclKind_Start && stmt->kind < _DeclKind_End;
}

void *AllocAst(Package *package, size_t size) {
    ASSERT(size != 0);
    void *mem = ArenaCalloc(&package->arena, size);
    return mem;
}

// TODO: We can implement these correctly to improve error quality at some point
// Alternatively we *may* be able to come up with something smarter
Position EndForStmt(Stmt *stmt) {
    return stmt->start;
}

Position EndForExpr(Expr *expr) {
    return expr->start;
}

Position EndForDecl(Decl *decl) {
    return decl->start;
}

Expr *NewExpr(Package *package, ExprKind kind, Position start) {
    Expr *e = AllocAst(package, sizeof(Expr));
    e->kind = kind;
    e->start = start;
    return e;
}

Stmt *NewStmt(Package *package, StmtKind kind, Position start) {
    Stmt *s = AllocAst(package, sizeof(Stmt));
    s->kind = kind;
    s->start = start;
    return s;
}

Decl *NewDecl(Package *package, DeclKind kind, Position start) {
    Decl *d = AllocAst(package, sizeof(Decl));
    d->kind = kind;
    d->start = start;
    return d;
}

Expr *NewExprInvalid(Package *package, Position start, Position end) {
    Expr *e = NewExpr(package, ExprKind_Invalid, start);
    e->Invalid.end = end;
    return e;
}

Stmt *NewStmtInvalid(Package *package, Position start, Position end) {
    Stmt *s = NewStmt(package, StmtKind_Invalid, start);
    s->Invalid.end = end;
    return s;
}

Decl *NewDeclInvalid(Package *package, Position start, Position end) {
    Decl *d = NewDecl(package, DeclKind_Invalid, start);
    d->Invalid.end = end;
    return d;
}

Expr *NewExprIdent(Package *package, Position start, const char *name) {
    Expr *e = NewExpr(package, ExprKind_Ident, start);
    e->Ident.name = name;
    return e;
}

Expr *NewExprParen(Package *package, Expr *expr, Position start, Position end) {
    Expr *e = NewExpr(package, ExprKind_Paren, start);
    e->Paren.expr = expr;
    e->Paren.end = end;
    return e;
}

Expr *NewExprCall(Package *package, Expr *expr, DynamicArray(Expr_KeyValue *) args, Position end) {
    Expr *e = NewExpr(package, ExprKind_Call, expr->start);
    e->Call.expr = expr;
    e->Call.args = args;
    e->Call.end = end;
    return e;
}

Expr *NewExprSelector(Package *package, Expr *expr, const char *name, Position end) {
    Expr *e = NewExpr(package, ExprKind_Selector, expr->start);
    e->Selector.expr = expr;
    e->Selector.name = name;
    e->Selector.end = end;
    return e;
}

Expr *NewExprSubscript(Package *package, Expr *expr, Expr *index, Position end) {
    Expr *e = NewExpr(package, ExprKind_Subscript, expr->start);
    e->Subscript.expr = expr;
    e->Subscript.index = index;
    e->Subscript.end = end;
    return e;
}

Expr *NewExprSlice(Package *package, Expr *expr, Expr *lo, Expr *hi, Position end) {
    Expr *e = NewExpr(package, ExprKind_Slice, expr->start);
    e->Slice.expr = expr;
    e->Slice.lo = lo;
    e->Slice.hi = hi;
    e->Slice.end = end;
    return e;
}

Expr *NewExprUnary(Package *package, Position start, TokenKind op, Expr *expr) {
    Expr *e = NewExpr(package, ExprKind_Unary, start);
    e->Unary.op = op;
    e->Unary.expr = expr;
    return e;
}

Expr *NewExprBinary(Package *package, TokenKind op, Position pos, Expr *lhs, Expr *rhs) {
    Expr *e = NewExpr(package, ExprKind_Binary, lhs->start);
    e->Binary.op = op;
    e->Binary.pos = pos;
    e->Binary.lhs = lhs;
    e->Binary.rhs = rhs;
    return e;
}

Expr *NewExprTernary(Package *package, Expr *cond, Expr *pass, Expr *fail) {
    Expr *e = NewExpr(package, ExprKind_Ternary, cond->start);
    e->Ternary.cond = cond;
    e->Ternary.pass = pass;
    e->Ternary.fail = fail;
    return e;
}

Expr *NewExprCast(Package *package, Position start, Expr *type, Expr *expr) {
    Expr *e = NewExpr(package, ExprKind_Cast, start);
    e->Cast.type = type;
    e->Cast.expr = expr;
    return e;
}

Expr *NewExprAutocast(Package *package, Position start, Expr *expr) {
    Expr *e = NewExpr(package, ExprKind_Autocast, start);
    e->Autocast.expr = expr;
    return e;
}

Expr *NewExprKeyValue(Package *package, Expr *key, Expr *value) {
    Expr *e = NewExpr(package, ExprKind_KeyValue, key->start);
    e->KeyValue.key = key;
    e->KeyValue.value = value;
    return e;
}

Expr *NewExprLocationDirective(Package *package, Position start, const char *name) {
    Expr *e = NewExpr(package, ExprKind_LocationDirective, start);
    e->LocationDirective.name = name;
    return e;
}

Expr *NewExprLitNil(Package *package, Position start) {
    Expr *e = NewExpr(package, ExprKind_LitNil, start);
    return e;
}

Expr *NewExprLitInt(Package *package, Position start, u64 val) {
    Expr *e = NewExpr(package, ExprKind_LitInt, start);
    e->LitInt.val = val;
    return e;
}

Expr *NewExprLitFloat(Package *package, Position start, f64 val) {
    Expr *e = NewExpr(package, ExprKind_LitFloat, start);
    e->LitFloat.val = val;
    return e;
}

Expr *NewExprLitString(Package *package, Position start, const char *val) {
    Expr *e = NewExpr(package, ExprKind_LitString, start);
    e->LitString.val = val;
    return e;
}

Expr *NewExprLitCompound(Package *package, Position start, Expr *type, DynamicArray(Expr_KeyValue *) elements, Position end) {
    Expr *e = NewExpr(package, ExprKind_LitCompound, start);
    e->LitCompound.type = type;
    e->LitCompound.elements = elements;
    e->LitCompound.end = end;
    return e;
}

Expr *NewExprLitFunction(Package *package, Expr *type, Stmt_Block *body, u8 flags) {
    Expr *e = NewExpr(package, ExprKind_LitFunction, type->start);
    e->LitFunction.type = type;
    e->LitFunction.body = body;
    e->LitFunction.flags = flags;
    return e;
}

Expr *NewExprTypePointer(Package *package, Position start, Expr *type) {
    Expr *e = NewExpr(package, ExprKind_TypePointer, start);
    e->TypePointer.type = type;
    return e;
}

Expr *NewExprTypeArray(Package *package, Position start, Expr *length, Expr *type) {
    Expr *e = NewExpr(package, ExprKind_TypeArray, start);
    e->TypeArray.length = length;
    e->TypeArray.type = type;
    return e;
}

Expr *NewExprTypeSlice(Package *package, Position start, Expr *type) {
    Expr *e = NewExpr(package, ExprKind_TypeSlice, start);
    e->TypeSlice.type = type;
    return e;
}

Expr *NewExprTypeStruct(Package *package, Position start, DynamicArray(AggregateItem) items) {
    Expr *e = NewExpr(package, ExprKind_TypeStruct, start);
    e->TypeStruct.items = items;
    return e;
}

Expr *NewExprTypeEnum(Package *package, Position start, Expr *explicitType, DynamicArray(EnumItem) items) {
    Expr *e = NewExpr(package, ExprKind_TypeEnum, start);
    e->TypeEnum.explicitType = explicitType;
    e->TypeEnum.items = items;
    return e;
}

Expr *NewExprTypeUnion(Package *package);

Expr *NewExprTypePolymorphic(Package *package, Position start, const char *name) {
    Expr *e = NewExpr(package, ExprKind_TypePolymorphic, start);
    e->TypePolymorphic.name = name;
    return e;
}

Expr *NewExprTypeVariadic(Package *package, Position start, Expr *type, u8 flags) {
    Expr *e = NewExpr(package, ExprKind_TypeVariadic, start);
    e->TypeVariadic.type = type;
    e->TypeVariadic.flags = flags;
    return e;
}

Expr *NewExprTypeFunction(Package *package, Position start, DynamicArray(Expr_KeyValue *) params, DynamicArray(Expr *)result) {
    Expr *e = NewExpr(package, ExprKind_TypeFunction, start);
    e->TypeFunction.params = params;
    e->TypeFunction.result = result;
    return e;
}

Stmt *NewStmtEmpty(Package *package, Position start) {
    return NewStmt(package, StmtKind_Empty, start);
}

Stmt *NewStmtLabel(Package *package, Position start, const char *name) {
    Stmt *s = NewStmt(package, StmtKind_Label, start);
    s->Label.name = name;
    return s;
}

Stmt *NewStmtAssign(Package *package, Position start, DynamicArray(Expr *) lhs, DynamicArray(Expr*) rhs) {
    Stmt *s = NewStmt(package, StmtKind_Assign, start);
    s->Assign.lhs = lhs;
    s->Assign.rhs = rhs;
    return s;
}

Stmt *NewStmtReturn(Package *package, Position start, DynamicArray(Expr *) exprs) {
    Stmt *s = NewStmt(package, StmtKind_Return, start);
    s->Return.exprs = exprs;
    return s;
}

Stmt *NewStmtDefer(Package *package, Position start, Stmt *stmt) {
    Stmt *s = NewStmt(package, StmtKind_Defer, start);
    s->Defer.stmt = stmt;
    return s;
}

Stmt *NewStmtUsing(Package *package, Position start, Expr *expr) {
    Stmt *s = NewStmt(package, StmtKind_Using, start);
    s->Using.expr = expr;
    return s;
}

Stmt *NewStmtGoto(Package *package, Position start, const char *keyword, Expr_Ident *label) {
    Stmt *s = NewStmt(package, StmtKind_Goto, start);
    s->Goto.keyword = keyword;
    s->Goto.label = label;
    return s;
}

Stmt *NewStmtBlock(Package *package, Position start, DynamicArray(Stmt *) stmts, Position end) {
    Stmt *s = NewStmt(package, StmtKind_Block, start);
    s->Block.stmts = stmts;
    s->Block.end = end;
    return s;
}

Stmt *NewStmtIf(Package *package, Position start, Expr *cond, Stmt *pass, Stmt *fail) {
    Stmt *s = NewStmt(package, StmtKind_If, start);
    s->If.cond = cond;
    s->If.pass = pass;
    s->If.fail = fail;
    return s;
}

Stmt *NewStmtFor(Package *package, Position start, Stmt *init, Expr *cond, Stmt *step, Stmt_Block *body) {
    Stmt *s = NewStmt(package, StmtKind_For, start);
    s->For.init = init;
    s->For.cond = cond;
    s->For.step = step;
    s->For.body = body;
    return s;
}

Stmt *NewStmtForIn(Package *package, Position start, Expr_Ident *valueName, Expr_Ident *indexName, Expr *aggregate, Stmt_Block *body) {
    Stmt *s = NewStmt(package, StmtKind_ForIn, start);
    s->ForIn.valueName = valueName;
    s->ForIn.indexName = indexName;
    s->ForIn.aggregate = aggregate;
    s->ForIn.body = body;
    return s;
}

Stmt *NewStmtSwitch(Package *package, Position start, Expr *match, DynamicArray(SwitchCase *) cases) {
    Stmt *s = NewStmt(package, StmtKind_Switch, start);
    s->Switch.match = match;
    s->Switch.cases = cases;
    return s;
}

Decl *NewDeclVariable(Package *package, Position start, DynamicArray(Expr_Ident *) names, Expr *type, DynamicArray(Expr *) values) {
    Decl *d = NewDecl(package, DeclKind_Variable, start);
    d->Variable.names = names;
    d->Variable.type = type;
    d->Variable.values = values;
    return d;
}

Decl *NewDeclConstant(Package *package, Position start, DynamicArray(Expr_Ident *) names, Expr *type, DynamicArray(Expr *) values) {
    Decl *d = NewDecl(package, DeclKind_Constant, start);
    d->Constant.names = names;
    d->Constant.type = type;
    d->Constant.values = values;
    return d;
}

Decl *NewDeclImport(Package *package, Position start, const char *path, const char *alias) {
    Decl *d = NewDecl(package, DeclKind_Import, start);
    d->Import.path = path;
    d->Import.alias = alias;
    return d;
}

#if TEST
void test_isExpr_and_isDecl() {
    Package pkg = {0};
    Position pos = {0};
    Stmt *expr = (Stmt *) NewExprIdent(&pkg, pos, NULL);
    ASSERT(isExpr(expr));

    Stmt *decl = (Stmt *) NewDeclVariable(&pkg, pos, NULL, NULL, NULL);
    ASSERT(isDecl(decl));
}
#endif
