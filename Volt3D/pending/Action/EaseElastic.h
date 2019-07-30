#ifndef VOLT3D_EASE_ELASTIC_H
#define VOLT3D_EASE_ELASTIC_H

#include "Volt3D/Macro/Define.h"

#include "Ease.h"

namespace v3d
{
	/**
	*	@class EaseElasticIn
	*	@brief Updates action with bounce in ease function.
	*/
	class VOLT3D_DLL EaseElasticIn : public v3d::Ease
	{
	private:
		// Constructor
		EaseElasticIn();

	public:
		// Destructor
		~EaseElasticIn();

		// Create
		static v3d::EaseElasticIn* create(v3d::Action* action);
		
		// override. Get ease time.
		float getT() const override;
	};



	/**
	*	@class EaseElasticIn
	*	@brief Updates action with bounce out ease function.
	*/
	class VOLT3D_DLL EaseElasticOut : public v3d::Ease
	{
	private:
		// Constructor
		EaseElasticOut();

	public:
		// Destructor
		~EaseElasticOut();

		// Create
		static v3d::EaseElasticOut* create(v3d::Action* action);

		// override. Get ease time.
		float getT() const override;
	};



	/**
	*	@class EaseElasticIn
	*	@brief Updates action with bounce in out ease function.
	*/
	class VOLT3D_DLL EaseElasticInOut : public v3d::Ease
	{
	private:
		// Constructor
		EaseElasticInOut();

	public:
		// Destructor
		~EaseElasticInOut();

		// Create
		static v3d::EaseElasticInOut* create(v3d::Action* action);

		// override. Get ease time.
		float getT() const override;
	};
}

#endif