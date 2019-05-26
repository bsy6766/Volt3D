#pragma

#ifdef  VOLT3D_EXPORTS 
#define VOLT3D_DLL __declspec(dllexport)  
#else
#define VOLT3D_DLL __declspec(dllimport)  
#endif

#define DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR(CLASS)	\
CLASS( const CLASS& ) = delete;							\
CLASS& operator=( const CLASS& ) = delete;

#define DEFAULT_MOVE_CONSTRUCTORS(CLASS)		\
CLASS( CLASS&& other ) = default;				\
CLASS& operator=( CLASS&& other ) = default;