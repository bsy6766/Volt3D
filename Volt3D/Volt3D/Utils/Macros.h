/**
*	@file Macros.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef MACROS_H
#define MACROS_H

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

#define CLASS_TO_VULKAN_HANDLE(CLASS, HANDLE)								\
inline explicit operator const CLASS& () const noexcept { return HANDLE; }	\
inline const CLASS& getHandle() const { return HANDLE; }


#endif