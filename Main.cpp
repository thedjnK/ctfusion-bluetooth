// ============================================================================
//
// This file are where the Conditions/Actions/Expressions are defined.
// You can manually enter these, or use CICK (recommended)
// See the Extension FAQ in this SDK for more info and where to download it
//
// ============================================================================

// Common Include
#include	"common.h"

// Quick memo: content of the eventInformations arrays
// ---------------------------------------------------
// Menu ID
// String ID
// Code
// Flags
// Number_of_parameters
// Parameter_type [Number_of_parameters]
// Parameter_TitleString [Number_of_parameters]

// Definitions of parameters for each condition
short conditionsInfos[]=
		{
		IDMN_CONDITION, M_CONDITION, CND_CONDITION, EVFLAGS_ALWAYS, 3, PARAM_EXPRESSION, PARAM_EXPRESSION, PARAM_EXPRESSION, M_CND_P1, M_CND_P2, M_CND_P3,
		};

// Definitions of parameters for each action
short actionsInfos[]=
		{
		IDMN_ACTION, M_ACTION,	ACT_ACTION,	0, 0,
		IDMN_OPEN, M_OPEN,	ACT_OPEN,	0, 0,
		IDMN_SCAN, M_SCAN,	ACT_SCAN,	0, 0,
		IDMN_GETINFO, M_GETINFO,	ACT_GETINFO,	0, 0,
		IDMN_CLOSE, M_CLOSE,	ACT_CLOSE,	0, 0,
		};

// Definitions of parameters for each expression
short expressionsInfos[]=
		{
		IDMN_ExpressionGetBTClass, M_ExpressionGetBTClass, EXP_ExpressionGetBTClass, 0, 0,
		IDMN_ExpressionGetBTLMPVersion, M_ExpressionGetBTLMPVersion, EXP_ExpressionGetBTLMPVersion, 0, 0,
		IDMN_ExpressionGetBTManufacturer , M_ExpressionGetBTManufacturer , EXP_ExpressionGetBTManufacturer , 0, 0,
		IDMN_ExpressionGetBTAddress, M_ExpressionGetBTAddress, EXP_ExpressionGetBTAddress, EXPFLAG_STRING, 0,
		IDMN_ExpressionGetBTName, M_ExpressionGetBTName, EXP_ExpressionGetBTName, EXPFLAG_STRING, 0,
		};



// ============================================================================
//
// CONDITION ROUTINES
// 
// ============================================================================

// -----------------
// Sample Condition
// -----------------
// Returns TRUE when the two values are equal!
// 

long WINAPI DLLExport Condition(LPRDATA rdPtr, long param1, long param2)
{

//  **** Still use this method for 1 or 2 parameters ****	
//	if (param1==param2)	
//		return TRUE;

	long p1 = CNC_GetParameter(rdPtr);
	long p2 = CNC_GetParameter(rdPtr);
	long p3 = CNC_GetParameter(rdPtr);

	if ((p1 + p2)==p3)
		return TRUE;
		 
	return FALSE;
}


// ============================================================================
//
// ACTIONS ROUTINES
// 
// ============================================================================

// -----------------
// Sample Action
// -----------------
// Does nothing!
// 
short WINAPI DLLExport Action(LPRDATA rdPtr, long param1, long param2)
{
	// Actions work just like Conditions

	// Use directly param1 and/or param2 if this action has 1 or 2 parameters

	// Use this if this action has 3 parameters or more
//	long p1 = CNC_GetParameter(rdPtr);
//	long p2 = CNC_GetParameter(rdPtr);
//	long p3 = CNC_GetParameter(rdPtr);
//	etc.

	return 0;
}

short WINAPI DLLExport ActOpen(LPRDATA rdPtr, long param1, long param2)
{
	//Open Bluetooth device
	rdPtr->LastBTError = 0;
	rdPtr->BTRadioFindParms = { sizeof(BLUETOOTH_FIND_RADIO_PARAMS) };

	HBLUETOOTH_RADIO_FIND BTRadionFindResult = NULL;
	BTRadionFindResult = BluetoothFindFirstRadio(
		&rdPtr->BTRadioFindParms,
		&rdPtr->BTRadioHandle
	);

	if (BTRadionFindResult == NULL)
	{
		//Failed
		int LastError = GetLastError();
		if (LastError == ERROR_NO_MORE_ITEMS)
		{
			//No BT radio found
			rdPtr->LastBTError = 1;
		}
		else if (LastError == ERROR_INVALID_PARAMETER)
		{
			//'BTRadioFindParms' is null
			rdPtr->LastBTError = 2;
		}
		else if (LastError == ERROR_REVISION_MISMATCH)
		{
			//'BTRadioFindParms' is not the correct size
			rdPtr->LastBTError = 2;
		}
		else if (LastError == ERROR_OUTOFMEMORY)
		{
			//Ran out of memory
			rdPtr->LastBTError = 3;
		}
	}
	return 0;
}

short WINAPI DLLExport ActGetInfo(LPRDATA rdPtr, long param1, long param2)
{
	//Get radio info
	BLUETOOTH_RADIO_INFO BTRadioInfo = { sizeof(BLUETOOTH_RADIO_INFO) };
	DWORD SuccessCode = BluetoothGetRadioInfo(
		rdPtr->BTRadioHandle,
		&BTRadioInfo
	);

	if (SuccessCode == ERROR_SUCCESS)
	{
		//Success
		rdPtr->LastBTError = 0;

		memcpy(&rdPtr->RadioInfoAddress, &BTRadioInfo.address.rgBytes, 6); //todo: convert into hex
		rdPtr->RadioInfoAddress[6] = 0;
		wcscpy(rdPtr->RadioInfoDeviceName, BTRadioInfo.szName);
		rdPtr->RadioInfoDeviceClass = BTRadioInfo.ulClassofDevice;
		rdPtr->RadioInfoLmpSubVersion = BTRadioInfo.lmpSubversion;
		rdPtr->RadioInfoManufacturer = BTRadioInfo.manufacturer;
	}
	else
	{
		//Failed
		if (SuccessCode == ERROR_INVALID_PARAMETER)
		{
			//'BTRadioHandle or 'BTRadioInfo' is null
			rdPtr->LastBTError = 4;
		}
		else if (SuccessCode == ERROR_REVISION_MISMATCH)
		{
			//'BTRadioInfo' is not the correct size
			rdPtr->LastBTError = 5;
		}
	}
	return 0;
}

short WINAPI DLLExport ActScan(LPRDATA rdPtr, long param1, long param2)
{
	//Scan for Bluetooth devices
	return 0;
}

short WINAPI DLLExport ActClose(LPRDATA rdPtr, long param1, long param2)
{
	//Close Bluetooth device
	CloseHandle(
		rdPtr->BTRadioHandle
	);
	return 0;
}


// ============================================================================
//
// EXPRESSIONS ROUTINES
// 
// ============================================================================

// -----------------
// Sample expression
// -----------------
// Add three values
// 
/*
long WINAPI DLLExport Expression(LPRDATA rdPtr,long param1)
{

	long p1 = CNC_GetFirstExpressionParameter(rdPtr, param1, TYPE_INT);
	long p2 = CNC_GetNextExpressionParameter(rdPtr, param1, TYPE_INT);
	long p3 = CNC_GetNextExpressionParameter(rdPtr, param1, TYPE_INT);

	// Performs the wonderfull calculation
	return p1+p2+p3;
}
*/

long WINAPI DLLExport ExpressionGetBTAddress(LPRDATA rdPtr, long param1)
{
	//String
	rdPtr->rHo.hoFlags |= HOF_STRING;
	return (long)&rdPtr->RadioInfoAddress;
}

long WINAPI DLLExport ExpressionGetBTName(LPRDATA rdPtr, long param1)
{
	//String
	rdPtr->rHo.hoFlags |= HOF_STRING;
	return (long)&rdPtr->RadioInfoDeviceName;
}

long WINAPI DLLExport ExpressionGetBTClass(LPRDATA rdPtr, long param1)
{
	return rdPtr->RadioInfoDeviceClass;
}

long WINAPI DLLExport ExpressionGetBTLMPVersion(LPRDATA rdPtr, long param1)
{
	return rdPtr->RadioInfoLmpSubVersion;
}

long WINAPI DLLExport ExpressionGetBTManufacturer(LPRDATA rdPtr, long param1)
{
	return rdPtr->RadioInfoManufacturer;
}

//Todo: return error code function
//return rdPtr->LastBTError;

//Todo: return error string function (and lookup)


// ----------------------------------------------------------
// Condition / Action / Expression jump table
// ----------------------------------------------------------
// Contains the address inside the extension of the different
// routines that handle the action, conditions and expressions.
// Located at the end of the source for convinience
// Must finish with a 0
//
long (WINAPI * ConditionJumps[])(LPRDATA rdPtr, long param1, long param2) = 
			{ 
			Condition, //Todo: remove
			0
			};
	
short (WINAPI * ActionJumps[])(LPRDATA rdPtr, long param1, long param2) =
			{
			Action, //Todo: remove
			ActOpen,
			ActGetInfo,
			ActScan,
			ActClose,
			0
			};

long (WINAPI * ExpressionJumps[])(LPRDATA rdPtr, long param) = 
			{
			ExpressionGetBTAddress,
			ExpressionGetBTName,
			ExpressionGetBTClass,
			ExpressionGetBTLMPVersion,
			ExpressionGetBTManufacturer,
			0
			};