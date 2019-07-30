#ifndef VOLT3D_EASE_CIRC_H
#define VOLT3D_EASE_CIRC_H

#include "Volt3D/Macro/Define.h"

#include "Ease.h"

namespace v3d
{
	/**
	*	@class EaseCircIn
	*	@brief Updates action with circ in ease function.
	*/
	class VOLT3D_DLL EaseCircIn : public v3d::Ease
	{
	private:
		// Constructor
		EaseCircIn();

	public:
		// Destructor
		~EaseCircIn();

		// Create
		static v3d::EaseCircIn* create(v3d::Action* action);
		
		// override. Get ease time.
		float getT() const override;
	};



	/**
	*	@class EaseCircIn
	*	@brief Updates action with circ out ease function.
	*/
	class VOLT3D_DLL EaseCircOut : public v3d::Ease
	{
	private:
		// Constructor
		EaseCircOut();

	public:
		// Destructor
		~EaseCircOut();

		// Create
		static v3d::EaseCircOut* create(v3d::Action* action);

		// override. Get ease time.
		float getT() const override;
	};



	/**
	*	@class EaseCircIn
	*	@brief Updates action with circ in out ease function.
	*/
	class VOLT3D_DLL EaseCircInOut : public v3d::Ease
	{
	private:
		// Constructor
		EaseCircInOut();

	public:
		// Destructor
		~EaseCircInOut();

		// Create
		static v3d::EaseCircInOut* create(v3d::Action* action);

		// override. Get ease time.
		float getT() const override;
	};
}

#endif