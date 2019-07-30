#ifndef VOLT3D_EASE_QUART_H
#define VOLT3D_EASE_QUART_H

#include "Volt3D/Macro/Define.h"

#include "Ease.h"

namespace v3d
{
	/**
	*	@class EaseQuartIn
	*	@brief Updates action with quart in ease function.
	*/
	class VOLT3D_DLL EaseQuartIn : public v3d::Ease
	{
	private:
		// Constructor
		EaseQuartIn();

	public:
		// Destructor
		~EaseQuartIn();

		// Create
		static v3d::EaseQuartIn* create(v3d::Action* action);
		
		// override. Get ease time.
		float getT() const override;
	};



	/**
	*	@class EaseQuartIn
	*	@brief Updates action with quart out ease function.
	*/
	class VOLT3D_DLL EaseQuartOut : public v3d::Ease
	{
	private:
		// Constructor
		EaseQuartOut();

	public:
		// Destructor
		~EaseQuartOut();

		// Create
		static v3d::EaseQuartOut* create(v3d::Action* action);

		// override. Get ease time.
		float getT() const override;
	};



	/**
	*	@class EaseQuartIn
	*	@brief Updates action with quart in out ease function.
	*/
	class VOLT3D_DLL EaseQuartInOut : public v3d::Ease
	{
	private:
		// Constructor
		EaseQuartInOut();

	public:
		// Destructor
		~EaseQuartInOut();

		// Create
		static v3d::EaseQuartInOut* create(v3d::Action* action);

		// override. Get ease time.
		float getT() const override;
	};
}

#endif