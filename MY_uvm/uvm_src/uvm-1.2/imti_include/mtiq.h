/*****************************************************************************
 *
 * C Foreign Language Interface Definitions that work on new QIS database
 *
 * Copyright 1991-2016 Mentor Graphics Corporation
 *
 * All Rights Reserved.
 *
 * THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS THE PROPERTY OF 
 * MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS SUBJECT TO LICENSE TERMS.
 * $Revision: #1 $
 * $Id: //dvt/mti/rel/10.5b/src/vsim/mtiq.h#1 $
 *
 *****************************************************************************/

#ifndef MTIQ_H
#define MTIQ_H

#include "mti.h"

/*****************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/

#ifdef __cplusplus
//extern "C" {
#endif /* __cplusplus */

/***********
 * Regions *
 ***********/

//extern mtiRegionIdT        mtiq_CreateRegion         PROTO((mtiRegionIdT parent, char * name));
extern mtiRegionIdT        mtiq_FindRegion           PROTO((char * name));
//extern mtiRegionIdT        mtiq_FirstLowerRegion     PROTO((mtiRegionIdT reg));
//extern mtiRegionIdT        mtiq_GetCallingRegion     PROTO((void));
//extern mtiRegionIdT        mtiq_GetCurrentRegion     PROTO((void));
//extern mtiRegionIdT        mtiq_GetTopRegion         PROTO((void));
extern mtiRegionIdT        mtiq_HigherRegion         PROTO((mtiRegionIdT reg));
//extern mtiRegionIdT        mtiq_NextRegion           PROTO((mtiRegionIdT reg));

extern char              * mtiq_GetLibraryName       PROTO((mtiRegionIdT reg));
extern char              * mtiq_GetPrimaryName       PROTO((mtiRegionIdT reg));
extern char              * mtiq_GetRegionFullName    PROTO((mtiRegionIdT reg));
extern char              * mtiq_GetRegionName        PROTO((mtiRegionIdT reg));
extern char              * mtiq_GetRegionSourceName  PROTO((mtiRegionIdT reg));
extern char              * mtiq_GetSecondaryName     PROTO((mtiRegionIdT reg));

extern int                 mtiq_GetRegionKind        PROTO((mtiRegionIdT reg));

extern mtiInterfaceListT * mtiq_GetGenericList       PROTO((mtiRegionIdT reg));

/*************
 * Processes *
 *************/

//extern mtiProcessIdT  mtiq_CreateProcess      PROTO((char * name, mtiVoidFuncPtrT func, void * param));
//extern mtiProcessIdT  mtiq_CreateProcessWithPriority PROTO((char * name, mtiVoidFuncPtrT func, void * param, mtiProcessPriorityT priority));
extern mtiProcessIdT  mtiq_FirstProcess       PROTO((mtiRegionIdT reg));
extern mtiProcessIdT  mtiq_NextProcess        PROTO((void));

extern char         * mtiq_GetProcessName     PROTO((mtiProcessIdT proc));
//extern mtiRegionIdT   mtiq_GetProcessRegion   PROTO((mtiProcessIdT proc));

extern void           mtiq_Desensitize        PROTO((mtiProcessIdT proc));
extern void           mtiq_ScheduleWakeup     PROTO((mtiProcessIdT proc, mtiDelayT delay));
extern void           mtiq_ScheduleWakeup64   PROTO((mtiProcessIdT proc, mtiTime64T delay));
extern void           mtiq_Sensitize          PROTO((mtiProcessIdT proc, mtiSignalIdT sig, mtiProcessTriggerT when));

/***********
 * Signals *
 ***********/

//extern mtiSignalIdT   mtiq_CreateSignal         PROTO((char * name, mtiRegionIdT reg, mtiTypeIdT type));
//extern mtiSignalIdT   mtiq_FindPort             PROTO((mtiInterfaceListT * list, char * name));
extern mtiSignalIdT   mtiq_FindSignal           PROTO((char * name));
extern mtiSignalIdT   mtiq_FirstSignal          PROTO((mtiRegionIdT reg));
extern mtiSignalIdT   mtiq_NextSignal           PROTO((void));

extern int            mtiq_ForceSignal          PROTO((mtiSignalIdT sigid, char        * value_string, mtiDelayT     delay, mtiForceTypeT force_type, mtiInt32T     cancel_period, mtiInt32T     repeat_period));
extern int            mtiq_ReleaseSignal        PROTO((mtiSignalIdT sigid));

extern void         * mtiq_GetArraySignalValue  PROTO((mtiSignalIdT sig, void * buf));
//extern mtiSignalIdT * mtiq_GetDrivingSignals    PROTO((char * name));
extern mtiSignalIdT   mtiq_GetParentSignal      PROTO((mtiSignalIdT sig));
//extern mtiSignalIdT   mtiq_GetResolvedSignalParent  PROTO((mtiSignalIdT sig));
//extern mtiSignalIdT   mtiq_GetEquivSignal       PROTO((mtiSignalIdT sig));
extern mtiDirectionT  mtiq_GetSignalMode        PROTO((mtiSignalIdT sig));
extern char         * mtiq_GetSignalName        PROTO((mtiSignalIdT sig));
extern char         * mtiq_GetSignalNameIndirect  PROTO((mtiSignalIdT sig, char * buf, int length));
extern mtiRegionIdT   mtiq_GetSignalRegion      PROTO((mtiSignalIdT sig));
extern mtiSignalIdT * mtiq_GetSignalSubelements PROTO((mtiSignalIdT sig, mtiSignalIdT * buf));
extern mtiTypeIdT     mtiq_GetSignalType        PROTO((mtiSignalIdT sig));
extern mtiInt32T      mtiq_GetSignalValue       PROTO((mtiSignalIdT sig));
extern void         * mtiq_GetSignalValueIndirect PROTO((mtiSignalIdT sig, void * buf));

//extern void           mtiq_SetSignalValue       PROTO((mtiSignalIdT sig, mtiLongT val));

//extern char         * mtiq_SignalImage          PROTO((mtiSignalIdT sig));
//extern int            mtiq_SignalIsResolved     PROTO((mtiSignalIdT sig));
//extern void           mtiq_SignalDump           PROTO((mtiSignalIdT sig));

/***********
 * Drivers *
 ***********/

extern mtiDriverIdT   mtiq_CreateDriver         PROTO((mtiSignalIdT sig));
extern mtiDriverIdT   mtiq_FindDriver           PROTO((mtiSignalIdT sig));
extern char        ** mtiq_GetDriverNames       PROTO((mtiSignalIdT sig, mtiInt32T * length));
extern char         * mtiq_GetDriverValues      PROTO((mtiSignalIdT sig, mtiInt32T * length));

#define mtiq_GetDriverSubelements   mti_GetDriverSubelements
#define mtiq_ScheduleDriver         mti_ScheduleDriver
#define mtiq_ScheduleDriver64       mti_ScheduleDriver64

extern void           mtiq_SetDriverOwner       PROTO((mtiDriverIdT drv, mtiProcessIdT proc));

/**************************************************
 * Variables/Generics/Constants/SystemC Variables *
 *************************************************/

extern mtiVariableIdT   mtiq_FindVar            PROTO((char * name));
extern mtiVariableIdT   mtiq_FirstVar           PROTO((mtiProcessIdT proc));
extern mtiVariableIdT   mtiq_FirstVarByRegion   PROTO((mtiRegionIdT reg));
extern mtiVariableIdT   mtiq_NextVar            PROTO((void));

extern void           * mtiq_GetArrayVarValue   PROTO((mtiVariableIdT var, void * buf));
extern void           * mtiq_GetVarAddr         PROTO((char * name));
//extern char           * mtiq_GetVarImage        PROTO((char * name));
//extern char           * mtiq_GetVarImageById    PROTO((mtiVariableIdT var));
extern char           * mtiq_GetVarName         PROTO((mtiVariableIdT var));
extern mtiVariableIdT * mtiq_GetVarSubelements  PROTO((mtiVariableIdT var, mtiVariableIdT * buf));
extern mtiTypeIdT       mtiq_GetVarType         PROTO((mtiVariableIdT var));
extern mtiInt32T        mtiq_GetVarValue        PROTO((mtiVariableIdT var));
extern void           * mtiq_GetVarValueIndirect PROTO((mtiVariableIdT var, void * buf));
extern int              mtiq_GetVarKind         PROTO((mtiVariableIdT var));

//extern void             mtiq_SetVarValue        PROTO((mtiVariableIdT var, mtiLongT val));

/*********
 * Types *
 *********/

#define mtiq_CreateArrayType        mti_CreateArrayType
#define mtiq_CreateEnumType         mti_CreateEnumType
#define mtiq_CreateRealType         mti_CreateRealType
#define mtiq_CreateScalarType       mti_CreateScalarType
#define mtiq_CreateTimeType         mti_CreateTimeType
#define mtiq_GetArrayElementType    mti_GetArrayElementType
#define mtiq_GetEnumValues          mti_GetEnumValues
#define mtiq_GetPhysicalData        mti_GetPhysicalData
#define mtiq_GetTypeKind            mti_GetTypeKind
#define mtiq_IsSystemcType          mti_IsSystemcType
#define mtiq_IsSystemcSignedType    mti_IsSystemcSignedType
#define mtiq_Image                  mti_Image
#define mtiq_TickDir                mti_TickDir
#define mtiq_TickHigh               mti_TickHigh
#define mtiq_TickLeft               mti_TickLeft
#define mtiq_TickLength             mti_TickLength
#define mtiq_TickLow                mti_TickLow
#define mtiq_TickRight              mti_TickRight
#define mtiq_GetNumRecordElements   mti_GetNumRecordElements

/*************
 * Callbacks *
 *************/

#define mtiq_AddInputReadyCB        mtiq_AddInputReadyCB
#define mtiq_AddOutputReadyCB       mtiq_AddOutputReadyCB
#define mtiq_AddSocketInputReadyCB  mtiq_AddSocketInputReadyCB
#define mtiq_AddSocketOutputReadyCB mtiq_AddSocketOutputReadyCB
#define mtiq_AddEnvCB               mtiq_AddEnvCB
#define mtiq_AddLoadDoneCB          mtiq_AddLoadDoneCB
#define mtiq_AddQuitCB              mtiq_AddQuitCB
#define mtiq_AddRestartCB           mtiq_AddRestartCB
#define mtiq_AddRestoreCB           mtiq_AddRestoreCB
#define mtiq_AddRestoreDoneCB       mtiq_AddRestoreDoneCB
#define mtiq_AddSaveCB              mtiq_AddSaveCB
#define mtiq_AddSimStatusCB         mtiq_AddSimStatusCB
#define mtiq_AddUCDBSaveCB          mtiq_AddUCDBSaveCB
#define mtiq_RemoveEnvCB            mtiq_RemoveEnvCB
#define mtiq_RemoveLoadDoneCB       mtiq_RemoveLoadDoneCB
#define mtiq_RemoveQuitCB           mtiq_RemoveQuitCB
#define mtiq_RemoveRestartCB        mtiq_RemoveRestartCB
#define mtiq_RemoveRestoreCB        mtiq_RemoveRestoreCB
#define mtiq_RemoveRestoreDoneCB    mtiq_RemoveRestoreDoneCB
#define mtiq_RemoveSaveCB           mtiq_RemoveSaveCB
#define mtiq_RemoveSimStatusCB      mtiq_RemoveSimStatusCB
#define mtiq_AddDPISaveRestoreCB    mtiq_AddDPISaveRestoreCB


/*********************
 * Memory Management *
 *********************/

#define mtiq_Malloc                 mti_Malloc
#define mtiq_Realloc                mti_Realloc
#define mtiq_Free                   mti_Free
#define mtiq_VsimFree               mti_VsimFree

/******************
 * Save & Restore *
 ******************/

#define mtiq_GetCheckpointFilename  mti_GetCheckpointFilename
#define mtiq_IsRestore              mti_IsRestore
#define mtiq_IsColdRestore          mti_IsColdRestore
#define mtiq_SaveBlock              mti_SaveBlock
#define mtiq_SaveChar               mti_SaveChar
#define mtiq_SaveLong               mti_SaveLong
#define mtiq_SaveShort              mti_SaveShort
#define mtiq_SaveString             mti_SaveString
#define mtiq_RestoreBlock           mti_RestoreBlock
#define mtiq_RestoreChar            mti_RestoreChar
#define mtiq_RestoreLong            mti_RestoreLong
#define mtiq_RestoreShort           mti_RestoreShort
#define mtiq_RestoreString          mti_RestoreString
//extern void     mtiq_RestoreProcess        PROTO((mtiProcessIdT   proc, char *          name, mtiVoidFuncPtrT func, void *          param));

/*****************
 * Time & Events *
 *****************/

#define mtiq_Delta                  mti_Delta
#define mtiq_Now                    mti_Now
#define mtiq_NowIndirect            mti_NowIndirect
#define mtiq_NowUpper               mti_NowUpper
#define mtiq_NowFormatted           mti_NowFormatted
#define mtiq_TimeToString           mti_TimeToString
#define mtiq_GetNextEventTime       mti_GetNextEventTime
#define mtiq_GetNextNextEventTime   mti_GetNextNextEventTime
#define mtiq_GetResolutionLimit     mti_GetResolutionLimit
#define mtiq_GetRunStopTime         mti_GetRunStopTime

/****************************
 * Communication & Commands *
 ****************************/

#define mtiq_AddCommand             mti_AddCommand

#ifdef _TCL
#define mtiq_AddTclCommand          mti_AddTclCommand
#endif /* _TCL */

#define mtiq_Command                mti_Command
#define mtiq_Cmd                    mti_Cmd
#define mtiq_Interp                 mti_Interp

#define mtiq_AskStdin               mti_AskStdin
#define mtiq_PrintMessage           mti_PrintMessage
#define mtiq_PrintFormatted         mti_PrintFormatted

#define mtiq_Break                  mti_Break
#define mtiq_FatalError             mti_FatalError
#define mtiq_Exit                   mti_Exit
#define mtiq_Quit                   mti_Quit
#define mtiq_QuitWithErrorCode      mti_QuitWithErrorCode

/*****************
 * Miscellaneous *
 *****************/

#define mtiq_GetProductVersion      mti_GetProductVersion
#define mtiq_GetWlfFilename         mti_GetWlfFilename
#define mtiq_FindProjectEntry       mti_FindProjectEntry
#define mtiq_WriteProjectEntry      mti_WriteProjectEntry
#define mtiq_IsFirstInit            mti_IsFirstInit
#define mtiq_KeepLoaded             mti_KeepLoaded

#define mtiq_AddAttrToVsimTestrecord        mti_AddAttrToVsimTestrecord
#define mtiq_GetAttrFromVsimTestrecord      mti_GetAttrFromVsimTestrecord
#define mtiq_GetSimulationStatus            mti_GetSimulationStatus
#define mtiq_RemoveAttrFromVsimTestrecord   mti_RemoveAttrFromVsimTestrecord

#define mtiq_CallStack              mti_CallStack
#define mtiq_IsVoptMode             mti_IsVoptMode

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* mtiq_H */

/* ***************************** End of Header ***************************** */
