//Object identifier "H3K0"
#define IDENTIFIER	MAKEID(H,3,K,0)

//#include <Winsock2.h> //Will be needed for SPP data transfer in future
#include <Ws2bth.h>
#include <BluetoothAPIs.h>

// ------------------------------
// DEFINITION OF CONDITIONS CODES
// ------------------------------
#define	CND_CONDITION				0
#define	CND_LAST					1

// ---------------------------
// DEFINITION OF ACTIONS CODES
// ---------------------------
#define	ACT_ACTION					0
#define ACT_OPEN                    1
#define ACT_GETINFO                 2
#define ACT_SCAN					3
#define ACT_CLOSE					4
#define	ACT_LAST					5

// -------------------------------
// DEFINITION OF EXPRESSIONS CODES
// -------------------------------
/*#define	EXP_EXPRESSION				0
#define EXP_EXPRESSION2				1
#define EXP_EXPRESSION3				2*/
#define EXP_ExpressionGetBTAddress  0
#define EXP_ExpressionGetBTName 1
#define EXP_ExpressionGetBTClass 2
#define EXP_ExpressionGetBTLMPVersion 3
#define EXP_ExpressionGetBTManufacturer 4
#define	EXP_LAST                    5

// ---------------------
// OBJECT DATA STRUCTURE 
// ---------------------
// Used at edit time and saved in the MFA/CCN/EXE files

typedef struct tagEDATA_V1
{
	// Header - required
	extHeader		eHeader;

	// Object's data
//	short			swidth;
//	short			sheight;

} EDITDATA;
typedef EDITDATA *			LPEDATA;

// Object versions
#define	KCX_CURRENT_VERSION			1

// --------------------------------
// RUNNING OBJECT DATA STRUCTURE
// --------------------------------
// Used at runtime. Initialize it in the CreateRunObject function.
// Free any allocated memory or object in the DestroyRunObject function.
//
// Note: if you store C++ objects in this structure and do not store 
// them as pointers, you must call yourself their constructor in the
// CreateRunObject function and their destructor in the DestroyRunObject
// function. As the RUNDATA structure is a simple C structure and not a C++ object.

typedef struct tagRDATA
{
	// Main header - required
	headerObject	rHo;					// Header

	// Optional headers - depend on the OEFLAGS value, see documentation and examples for more info
//	rCom			rc;				// Common structure for movements & animations
//	rMvt			rm;				// Movements
//	rSpr			rs;				// Sprite (displayable objects)
//	rVal			rv;				// Alterable values

	// Object's runtime data
	BLUETOOTH_FIND_RADIO_PARAMS BTRadioFindParms = { sizeof(BLUETOOTH_FIND_RADIO_PARAMS) };
	HANDLE BTRadioHandle;
	int LastBTError = 0;
	byte RadioInfoAddress[7];
	WCHAR RadioInfoDeviceName[BLUETOOTH_MAX_NAME_SIZE];
	ULONG RadioInfoDeviceClass;
	USHORT RadioInfoLmpSubVersion;
	USHORT RadioInfoManufacturer;

} RUNDATA;
typedef	RUNDATA	*			LPRDATA;

// Size when editing the object under level editor
// -----------------------------------------------
#define	MAX_EDITSIZE			sizeof(EDITDATA)

// Default flags - see documentation for more info
// -------------
#define	OEFLAGS      			0
#define	OEPREFS      			0


// If to handle message, specify the priority of the handling procedure
// 0= low, 255= very high. You should use 100 as normal.                                                
// --------------------------------------------------------------------
#define	WINDOWPROC_PRIORITY		100
