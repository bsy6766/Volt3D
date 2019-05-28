﻿/**
*	@file Macros.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_MACROS_H
#define V3D_MACROS_H

#ifdef  VOLT3D_EXPORTS 
#define VOLT3D_DLL __declspec(dllexport)  
#else
#define VOLT3D_DLL __declspec(dllimport)  
#endif

#define DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR(_CLASS)	\
_CLASS( const _CLASS& ) = delete;							\
_CLASS& operator=( const _CLASS& ) = delete;

#define DEFAULT_MOVE_CONSTRUCTORS(_CLASS)		\
_CLASS( _CLASS&& other ) = default;				\
_CLASS& operator=( _CLASS&& other ) = default;

#define CLASS_TO_VK_HANDLE(_CLASS, _HANDLE)										\
inline explicit operator const _CLASS& () const noexcept { return _HANDLE; }	\
inline const _CLASS& getHandle() const { return _HANDLE; }

#define UNIQUE_TO_CPP_VK_HANDLE(_UNIQUE_, _CPP_, _HANDLE_)						\
inline explicit operator const _UNIQUE_& () const noexcept { return _HANDLE_; }	\
inline const _UNIQUE_& getHandle() const { return _HANDLE_; }					\
inline const _CPP_& get() const { return _HANDLE_.get(); }

#endif