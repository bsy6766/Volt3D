/**
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

#define SAFE_DELETE(_INSTANCE_)								\
if(_INSTANCE_) { delete _INSTANCE_; _INSTANCE_ = nullptr; }	

#define DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR(_CLASS_)	\
_CLASS_( const _CLASS_& ) = delete;							\
_CLASS_& operator=( const _CLASS_& ) = delete;

#define DEFAULT_MOVE_CONSTRUCTORS(_CLASS_)			\
_CLASS_( _CLASS_&& other ) = default;				\
_CLASS_& operator=( _CLASS_&& other ) = default;

#define CLASS_TO_VK_HANDLE(_CLASS_, _HANDLE_)									\
inline explicit operator const _CLASS_& () const noexcept { return _HANDLE_; }	\
inline const _CLASS_& getHandle() const { return _HANDLE_; }

#define UNIQUE_TO_CPP_VK_HANDLE(_UNIQUE_, _CPP_, _HANDLE_)						\
inline explicit operator const _UNIQUE_& () const noexcept { return _HANDLE_; }	\
inline const _UNIQUE_& getHandle() const { return _HANDLE_; }					\
inline const _CPP_& get() const { return _HANDLE_.get(); }

#define NO_THROW_NEW(_CLASS_)	\
new (std::nothrow) _CLASS_();

#define CHECK_INSTANCE_NULLPTR_AND_LOG(_CLASS_, _INSTANCE_)									\
if(_INSTANCE_ == nullptr) { v3d::Logger::getInstance().bad_alloc<_CLASS_>(); return false; }

#define GET_DEFULT_NODE_TYPE(_NODETYPE_)									\
v3d::NodeType getDefaultNodeType() const override { return _NODETYPE_; } 


#endif