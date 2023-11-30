#ifndef QFD_EXPR_H
#define QFD_EXPR_H

/*
 * Expression types supported for reconstruction
 *
 * More types may be added in future Questa releases. The client code should be written
 * in a way that if client code sees a type not defined here, it should ask Questa to 
 * log the secondary (see qfd_setCbAndGetValPtrForSecondary)
 *
 * Example user code to print map expr
 * void print_map_expr(TQfdSecMapBaseExpr* mapExpr) {
 *     if (mapExpr == NULL) {
 *         printf("NULL");
 *         return;
 *     }
 *     switch(mapExpr->opType) {
 *         case QFD_SECMAP_VAR: 
 *         {
 *             TQfdSecMapVarExpr* expr = (TQfdSecMapVarExpr*)mapExpr);
 *             printf("%s", vpi_get_str(vpifullName, expr->handle));
 *             break;
 *         }
 *         case QFD_SECMAP_BITSEL:
 *         {
 *             TQfdSecMapBitSelectExpr* expr = (TQfdSecMapBitSelectExpr*)mapExpr;
 *             printf("%s[%d]", vpi_get_str(vpifullName, expr->handle), expr->idx).
 *             break;
 *         }
 *         case QFD_SECMAP_PARTSEL:
 *         {
 *             TQfdSecMapPartSelectExpr* expr = (TQfdSecMapPartSelectExpr*)mapExpr;
 *             printf("%s[%d:%d]", vpi_get_str(vpifullName, expr->handle), expr->lsb,
 *                 expr->msb);
 *             break;
 *         }
 *         case QFD_SECMAP_BITAND:
 *         case QFD_SECMAP_BITOR:
 *         case QFD_SECMAP_BITXOR:
 *         {
 *             TQfdSecMapInfixExpr* expr = (TQfdSecMapInfixExpr*)expr;
 *             print_map_expr(expr->lhs);
 *             print_operator(expr->opType);
 *             print_map_expr(expr->rhs);
 *             break;
 *         }
 *         .
 *         .
 *         .
 *         default:
 *             printf("Unexpected map expression\n");
 *     }
 * }
 *
 * void print_secondary_map_expr(qfdHandle handle) {
 *     print_map_expr(qfd_getMapExpr(handle));
 * }
 */

typedef void* qfdHandle;

typedef enum {
    QFD_SECMAP_VAR,              /* TQfdSecMapVarExpr        */
    QFD_SECMAP_BITSEL,           /* TQfdSecMapBitSelectExpr  */
    QFD_SECMAP_PARTSEL,          /* TQfdSecMapPartSelectExpr */
    QFD_SECMAP_CONCAT,           /* TQfdSecMapExpr           */
    QFD_SECMAP_REPLICATE,        /* TQfdSecMapReplicateExpr  */
    QFD_SECMAP_BITAND,           /* TQfdSecMapInfixExpr      */
    QFD_SECMAP_BITOR,            /* TQfdSecMapInfixExpr      */
    QFD_SECMAP_BITXOR,           /* TQfdSecMapInfixExpr      */
    QFD_SECMAP_BITNEG,           /* TQfdSecMapPrefixExpr     */
    QFD_SECMAP_BITBUF,           /* TQfdSecMapPrefixExpr     */
    QFD_SECMAP_TERNARY,          /* TQfdSecMapExpr           */
    QFD_SECMAP_LITERAL           /*                          */
} TQfdSecMapExprOpType;

#define QFD_MAPEXPR_BASE \
    TQfdSecMapExprOpType opType

/*
 * Base class for recon expression tree
 */
typedef struct {
    QFD_MAPEXPR_BASE;
} TQfdSecMapBaseExpr;

/*
 * Var alias expr
 * QFD_SECMAP_VAR
 *
 * The 'qfdHandle handle' value type (as return by qfd_getValType) can be 
 * 1. Primary. The client should add a fast callback on the handle.
 * 2. Secondary. This is a case where multiple secondaries have same map expr.
 *    In such case the map expr for one secondary is returned, and other
 *    secondaries are considered aliases of the first map expr.
 *    Clients can take advantage of this property by mapping
 *    multiple secondaries to the same map expr.
 */
typedef struct {
    QFD_MAPEXPR_BASE;
    qfdHandle handle;
} TQfdSecMapVarExpr;

/* 
 * Bit select expr
 * QFD_SECMAP_BITSEL
 *
 * The bit select idx is the bit offset index and not necessarily the user code index.
 * e.g. for a signal declared as 'wire [7:4] w', w[6] will have bit offset index 3.
 *
 * The array handle is always the qfdHandle of an object.
 * The array can not be another expression.
 */
typedef struct {
    QFD_MAPEXPR_BASE;
    int       idx;
    qfdHandle handle;
} TQfdSecMapBitSelectExpr;

/*
 * Part Select expr
 * QFD_SECMAP_PARTSEL
 *
 * The lsb and msb of part select expr are the part select offset range as commented 
 * for bit select. The msb is always greater than the lsb regardless of the direction
 * of the selected array.
 *
 * The array handle is always the qfdHandle of an object.
 * The array cannot be another expression.
 */
typedef struct {
    QFD_MAPEXPR_BASE;
    int       lsb;
    int       msb;
    qfdHandle handle;
} TQfdSecMapPartSelectExpr;

/*
 * Prefix expr
 * QFD_SECMAP_BITNEG    bitwise negation
 * QFD_SECMAP_BITBUF    equivalent to verilog buf primitive
 */
typedef struct {
    QFD_MAPEXPR_BASE;
    TQfdSecMapBaseExpr* operand;
} TQfdSecMapPrefixExpr;

/*
 * Infix expr
 * QFD_SECMAP_BITAND    bitwise AND
 * QFD_SECMAP_BITOR     bitwise OR
 * QFD_SECMAP_BITXOR    bitwise XOR
 */
typedef struct {
    QFD_MAPEXPR_BASE;
    TQfdSecMapBaseExpr* lhs;
    TQfdSecMapBaseExpr* rhs;
} TQfdSecMapInfixExpr;

/*
 * Replicate expr
 * QFD_SECMAP_REPLICATE {num{operand}}
 */
typedef struct {
    QFD_MAPEXPR_BASE;
    int                 num;
    TQfdSecMapBaseExpr* operand;
} TQfdSecMapReplicateExpr;

/*
 * Expressions that do not belong to above types
 * QFD_SECMAP_CONCAT numOperands can be any number. {operand[0], operand[1]....}
 * QFD_SECMAP_TERNARY numOperands is 3. operand[0] ? operand[1] : operand[2];
 */
typedef struct {
    QFD_MAPEXPR_BASE;
    int                  numOperands;
    TQfdSecMapBaseExpr** operands;
} TQfdSecMapExpr;

#endif

