#ifndef QFD_H
#define QFD_H

#include "vpi_user.h"
#include "qfd_expr.h"

/* 
 * The QFD API (Questa Fast Logging) is meant to be used by Questa
 * waveform-logging clients. The API's assume that there is only one
 * client of the API in a given simulation. In case of multiple clients,
 * i.e. two clients try to set a fast callback on same Verilog object, or the same
 * client tries to set more than one fast callback on the same Verilog object,
 * the second callback request will fail. Setting multiple fast callbacks on the
 * same VHDL object is supported.
 *
 * The QFD API provides the following functionality:
 * 1. Help filter the signals/variables that should be logged.
 * 2. Setup fast value change callbacks.
 * 3. Provide reconstruction expressions for signals/variables
 *    that are not logged. These object values should be computed at
 *    visualization time based on the map expression provided by QFD.
 *
 * The QFD API does not provide any design iteration capability.
 * It is meant to be used in conjunction with standard VPI/FLI design iteration.
 * Design iteration returns VPI/FLI handles to signals and variables to be logged.
 * Use the QFD API to set up fast callbacks and perform value reconstruction
 * on objects that have been optimized out of the simulator's memory image.
 *
 * qfd_init() should be called once during the simulation before using any
 * other QFD APIs.
 *
 * qfd_cleanup() should be called once QFD setup is done.  This will destroy
 * iteration related infrastructure like the mapping between qfdHandle and
 * user data which can be significant in terms of memory.  Only the structures
 * that are required for giving callbacks during simulation are retained.
 * Once cleaned up, QFD APIs cannot be used.  Not calling this function
 * will mean that we will continue to maintain those structures during
 * simulation which in most cases shouldn't be required.
 * 
 * qfd_getValType() is the filtering routine that should be called on all 
 * signal/variable objects discovered by VPI/FLI iteration. The routine
 * helps the waveform logger determine if the objects need to be logged.
 *
 * qfd_setCbAndGetValPtr() should be called on all objects that need to be 
 * logged (QFD_VAL_PRIMARY). The function may return NULL, which means fast 
 * logging is not possible for the object. The client should log such objects 
 * using standard VPI/FLI callbacks instead. Some of the reasons that fast callbacks
 * can fail are:
 * 1. Unsupported object types like Verilog unpacked arrays, struct and class objects, etc
 * 2. The Verilog object already has a fast callback.
 * NOTE: These limitations don't exist when the object is VHDL.
 *
 * qfd_getMapExpr() should be called on all objects that need to be 
 * reconstructed (QFD_VAL_SECONDARY). The map expression tree for reconstruction
 * is returned in terms of the TQfdSecMapExpr structures defined in this API.
 *
 * A client can ask for QFD_VAL_SECONDARY objects to be logged using 
 * qfd_setCbAndGetValPtrForSecondary(). These objects will then be reconstructed
 * by the Questa simulator and the callback function will be called as if the
 * objects were primaries. This will adversely affect simulation performance
 * and memory compared to performing visualization-side reconstruction.
 */

/* 
 *******************************************************************************
 * Filter objects for logging
 *******************************************************************************
 */

/* 
 * Enum TQfdValType
 *
 * Value types returned by qfd_getValType()
 *
 * QFD_VAL_CANCELLED objects can not be logged or reconstructed.
 * The waveform for these objects will not be available.
 *
 * QFD_VAL_PRIMARY objects are loggable. 
 *
 * QFD_VAL_SECONDARY objects can be reconstructed.
 *
 * QFD_VAL_LITERAL objects do not change value during simulation,
 * as determined by post optimization analysis.
 */
typedef enum {
    QFD_VAL_NONE,        
    QFD_VAL_CANCELLED,   /* Object not available               */
    QFD_VAL_PRIMARY,     /* Loggable                           */
    QFD_VAL_SECONDARY,   /* Value can be reconstructed         */
    QFD_VAL_LITERAL      /* Constant value                     */
} TQfdValType;

typedef enum {
	QFD_VALKIND_NONE,
	QFD_VALKIND_2STATE,
	QFD_VALKIND_4STATE,
	QFD_VALKIND_VHDL,
} TQfdValDescKind;

/*
 * qfd_getValType()
 *
 * Get value type for an object handle.
 */
TQfdValType 
qfd_getValType(
    qfdHandle hndl);


/* 
 *******************************************************************************
 * Fast Value Change Callback
 *******************************************************************************
 */

typedef void (*qfd_func_p)(void* userData, uint32_t highTime, uint32_t lowTime);

/*
 * qfd_setCbAndGetValPtr()
 *
 * Should be called for the following value types.
 *   QFD_VAL_PRIMARY
 *   QFD_VAL_LITERAL
 *
 * For QFD_VAL_PRIMARY:
 *   - Sets a callback on an object handle.
 *   - A change in value of the object will result in 'qfd_func_p' getting called
 *     with the corresponding userData.
 *   - Sets userData for this var which can be retrieved using qfd_getUserData();
 *   - The return value is the value location as described by qfd_getValDescKind().
 *       - A null value means fast logging callbacks could not be set up.
 *         Use standard VPI/FLI callbacks for such objects.
 *
 * For QFD_VAL_LITERAL:
 *   - userData is ignored.
 *   - The return value will hold the actual value of this constant.
 *   - No callback happens for these since these objects do not change value.
 */
void* 
qfd_setCbAndGetValPtr(
    qfdHandle  hndl,
    qfd_func_p funcp,
    void*      userData);

/*
 * qfd_setStrengthCbAndGetValPtr
 *
 * Should only be called if vpi_get(vpiExpanded, vpiHndl) > 0.
 * The value/strength can be determined using qfd_getValAndStrength.
 */
void* 
qfd_setStrengthCbAndGetValPtr(
    qfdHandle  hndl,
    qfd_func_p funcp,
    void*      userData);

/*
 * Net and strength value for value pointer returned by qfd_setStrengthCbAndGetValPtr().
 * Return value :
 *     s_vpi_strengthval type as defined in vpi_user.h
 */
s_vpi_strengthval qfd_getValAndStrength(void* valPtr);

/*
 * qfd_getValDescKind()
 *
 * This function describes the value pointer returned by functions
 * qfd_setCbAndGetValPtr()
 * qfd_setCbAndGetValPtrForSecondary()
 *
 * For QFD_VALKIND_2STATE value representation, the value location has aval only
 * For QFD_VALKIND_4STATE value representation, the value location is in aval/bval format
 * For QFD_VALKIND_VHDL   value representation, the value location is a VHDL value buffer
 *
 */
TQfdValDescKind
qfd_getValDescKind(
	qfdHandle  hndl,
	int*       size);


/* 
 *******************************************************************************
 * Value Reconstruction Expressions and Secondary Objects
 *******************************************************************************
 */

/*
 * qfd_getMapExpr()
 *
 * Get a map expression for a QFD_VAL_SECONDARY variable in terms of qfdHandles
 * of the primaries.
 *
 * A terminal qfdHandle can be a handle to a secondary in case hndl is an alias 
 * of another secondary object. If multiple secondaries have the same map expr (e.g.
 * multiple port formals map to same apex actual expression), then qfd_getMapExpr() 
 * will return the map expr for only one of the secondaries and mark the other
 * secondaries as aliases of the first one.
 */
TQfdSecMapBaseExpr*
qfd_getMapExpr(
    qfdHandle hndl,
	int*      isAliasOfSecondaryP);


/*
 * qfd_setCbAndGetValPtrForSecondary()
 *
 * Set callback on secondary object.
 *  - To be used if client does not intend to use the map expression returned
 *    by qfd_GetMapExpr() to evaluate the value of a secondary.
 *   - A change in value of the variable will result in 'funcp' getting called with
 *     the corresponding userData.
 *   - Sets userData for this var which can be retrieved using qfd_getUserData();
 *   - The return value is the value location as described by qfd_getValDescKind().
 *       - A null value means fast logging callbacks could not be set up.
 *         Use standard VPI/FLI callbacks for such objects.
 *
 * Note: Setting a callback on a secondary will cause the simulator to reconstruct
 * the secondary.
 */
void*
qfd_setCbAndGetValPtrForSecondary(
    qfdHandle  hndl,
    qfd_func_p funcp,
    void*      userData);


/*
 * qfd_setUserData()
 *
 * Return value :
 *     0 - User data could not be set for this var.
 *     1 - Setting of user_data was successful.
 */
int
qfd_setUserData(
    qfdHandle hndl,
	void*     userData);


/*
 * qfd_getUserData()
 *
 * Returns the userData associated with an object which was used
 * when setting QFD callback on this object.
 */
void*
qfd_getUserData(
    qfdHandle hndl);


/*
 * qfd_removeCb()
 *
 * Remove QFD callback for this object.
 * Return value :
 *     0 - Callback could not be removed or does not exist.
 *     1 - Removal was successful.
 */
int
qfd_removeCb(
    qfdHandle hndl);


/* 
 *******************************************************************************
 * Miscellaneus Functions
 *******************************************************************************
 */

/*
 * qfd_init()
 *
 * Call this once before accessing any other QFD APIs.
 * It can be called only once during a simulation.
 * Return value :
 *     0 - QFD is not available.
 *     1 - QFD is available.
 */
int
qfd_init();


/*
 * qfd_start()
 *
 * qfd_start() pairs with qfd_finalize(). All QFD callbacks, namely
 *      qfd_setCbAndGetValPtr()
 *      qfd_setCbAndGetValPtrForSecondary()
 * should be set within start and finalize.
 *
 * The pair can be called multiple times during simulation. QFD callbacks
 * set outside qfd_start()/qfd_finalize() will not work.
 */
void
qfd_start();


/*
 * qfd_finalize()
 *
 * qfd_finalize() pairs with qfd_start(). All QFD callbacks should be set 
 * within start and finalize.
 * The pair can be called multiple times during simulation. QFD callbacks
 * set outside qfd_start()/qfd_finalize() will not work.
 *
 */
void
qfd_finalize();


/*
 * qfd_cleanup()
 *
 * The function frees QFD related memory.
 * Any subsequent QFD call will result in undefined behavior.
 * It can be called only once during a simulation.
 */
void
qfd_cleanup();


#endif
