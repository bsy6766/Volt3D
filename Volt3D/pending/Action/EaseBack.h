#ifndef VOLT3D_EASE_BACK_H
#define VOLT3D_EASE_BACK_H

#include "Volt3D/Macro/Define.h"

#include "Ease.h"

namespace v3d
{
	/**
	*	@class EaseBackIn
	*	@brief Updates action with circ in ease function.
	*/
	class VOLT3D_DLL EaseBackIn : public v3d::Ease
	{
	private:
		// Constructor
		EaseBackIn();

	public:
		// Destructor
		~EaseBackIn();

		// Create
		static v3d::EaseBackIn* create(v3d::Action* action);
		
		// override. Get ease time.
		float getT() const override;
	};



	/**
	*	@class EaseBackIn
	*	@brief Updates action with circ out ease function.
	*/
	class VOLT3D_DLL EaseBackOut : public v3d::Ease
	{
	private:
		// Constructor
		EaseBackOut();

	public:
		// Destructor
		~EaseBackOut();

		// Create
		static v3d::EaseBackOut* create(v3d::Action* action);

		// override. Get ease time.
		float getT() const override;
	};



	/**
	*	@class EaseBackIn
	*	@brief Updates action with circ in out ease function.
	*/
	class VOLT3D_DLL EaseBackInOut : public v3d::Ease
	{
	private:
		// Constructor
		EaseBackInOut();

	public:
		// Destructor
		~EaseBackInOut();

		// Create
		static v3d::EaseBackInOut* create(v3d::Action* action);

		// override. Get ease time.
		float getT() const override;
	};
}

#endif