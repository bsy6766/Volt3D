#ifndef VOLT3D_EASE_QUINT_H
#define VOLT3D_EASE_QUINT_H

#include "Volt3D/Macro/Define.h"

#include "Ease.h"

namespace v3d
{
	/**
	*	@class EaseQuintIn
	*	@brief Updates action with quint in ease function.
	*/
	class VOLT3D_DLL EaseQuintIn : public v3d::Ease
	{
	private:
		// Constructor
		EaseQuintIn();

	public:
		// Destructor
		~EaseQuintIn();

		// Create
		static v3d::EaseQuintIn* create(v3d::Action* action);
		
		// override. Get ease time.
		float getT() const override;
	};



	/**
	*	@class EaseQuintIn
	*	@brief Updates action with quint out ease function.
	*/
	class VOLT3D_DLL EaseQuintOut : public v3d::Ease
	{
	private:
		// Constructor
		EaseQuintOut();

	public:
		// Destructor
		~EaseQuintOut();

		// Create
		static v3d::EaseQuintOut* create(v3d::Action* action);

		// override. Get ease time.
		float getT() const override;
	};



	/**
	*	@class EaseQuintIn
	*	@brief Updates action with quint in out ease function.
	*/
	class VOLT3D_DLL EaseQuintInOut : public v3d::Ease
	{
	private:
		// Constructor
		EaseQuintInOut();

	public:
		// Destructor
		~EaseQuintInOut();

		// Create
		static v3d::EaseQuintInOut* create(v3d::Action* action);

		// override. Get ease time.
		float getT() const override;
	};
}

#endif