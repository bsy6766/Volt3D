#ifndef VOLT3D_EASE_SINE_H
#define VOLT3D_EASE_SINE_H

#include "Volt3D/Macro/Define.h"

#include "Ease.h"

namespace v3d
{
	/**
	*	@class EaseSineIn
	*	@brief Updates action with sine in ease function.
	*/
	class VOLT3D_DLL EaseSineIn : public v3d::Ease
	{
	private:
		// Constructor
		EaseSineIn();

	public:
		// Destructor
		~EaseSineIn();

		// Create
		static v3d::EaseSineIn* create(v3d::Action* action);
		
		// override. Get ease time.
		float getT() const override;
	};



	/**
	*	@class EaseSineIn
	*	@brief Updates action with sine out ease function.
	*/
	class VOLT3D_DLL EaseSineOut : public v3d::Ease
	{
	private:
		// Constructor
		EaseSineOut();

	public:
		// Destructor
		~EaseSineOut();

		// Create
		static v3d::EaseSineOut* create(v3d::Action* action);

		// override. Get ease time.
		float getT() const override;
	};



	/**
	*	@class EaseSineIn
	*	@brief Updates action with sine in out ease function.
	*/
	class VOLT3D_DLL EaseSineInOut : public v3d::Ease
	{
	private:
		// Constructor
		EaseSineInOut();

	public:
		// Destructor
		~EaseSineInOut();

		// Create
		static v3d::EaseSineInOut* create(v3d::Action* action);

		// override. Get ease time.
		float getT() const override;
	};
}

#endif