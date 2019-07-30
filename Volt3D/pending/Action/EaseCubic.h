#ifndef VOLT3D_EASE_CUBIC_H
#define VOLT3D_EASE_CUBIC_H

#include "Volt3D/Macro/Define.h"

#include "Ease.h"

namespace v3d
{
	/**
	*	@class EaseCubicIn
	*	@brief Updates action with cubic in ease function.
	*/
	class VOLT3D_DLL EaseCubicIn : public v3d::Ease
	{
	private:
		// Constructor
		EaseCubicIn();

	public:
		// Destructor
		~EaseCubicIn();

		// Create
		static v3d::EaseCubicIn* create(v3d::Action* action);
		
		// override. Get ease time.
		float getT() const override;
	};



	/**
	*	@class EaseCubicIn
	*	@brief Updates action with cubic out ease function.
	*/
	class VOLT3D_DLL EaseCubicOut : public v3d::Ease
	{
	private:
		// Constructor
		EaseCubicOut();

	public:
		// Destructor
		~EaseCubicOut();

		// Create
		static v3d::EaseCubicOut* create(v3d::Action* action);

		// override. Get ease time.
		float getT() const override;
	};



	/**
	*	@class EaseCubicIn
	*	@brief Updates action with cubic in out ease function.
	*/
	class VOLT3D_DLL EaseCubicInOut : public v3d::Ease
	{
	private:
		// Constructor
		EaseCubicInOut();

	public:
		// Destructor
		~EaseCubicInOut();

		// Create
		static v3d::EaseCubicInOut* create(v3d::Action* action);

		// override. Get ease time.
		float getT() const override;
	};
}

#endif