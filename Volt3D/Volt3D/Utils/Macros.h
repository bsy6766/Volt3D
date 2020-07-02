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

#define V3D_NS_BEGIN	\
namespace v3d {

#define V3D_NS_END	\
}	// namesapce v3d

#define GLFW_NS_BEGIN	\
namespace glfw {

#define GLFW_NS_END	\
}	// namespace glfw

#define VK_NS_BEGIN	\
namespace vulkan {

#define VK_NS_END	\
}	// namespace vulkan

//#define V3D_ASSERT_WARN(cond, msg) if(!(cond)) { v3d::Logger::getInstance().warn("Assertion failed: {}", msg);}
//#define V3D_ASSERT_ERROR(cond, msg) if(!(cond)) { v3d::Logger::getInstance().error("Assertion failed: {}", msg); return false; }

#define CLASS_NAME(_CLASS_) typeid(_CLASS_).name()

#define CHECKERROR(_COND_, _MSG_) if(!(_COND_)) { v3d::Logger::getInstance().error(_MSG_); }
#define CHECKERROR_CLS(_COND_, _MSG_, _CLASS_) if(!(_COND_)) { v3d::Logger::getInstance().error("{} {}", CLASS_NAME(_CLASS_), _MSG_); }

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

#define CREATE_SCENE(_CLASS_)	\
static _CLASS_* create() { _CLASS_* newScene = new (std::nothrow) _CLASS_(); if(newScene && newScene->onInit()) return newScene; else return nullptr;}

#define GET_DEFULT_NODE_TYPE(_NODETYPE_)									\
v3d::NodeType getDefaultNodeType() const override { return _NODETYPE_; } 


#endif