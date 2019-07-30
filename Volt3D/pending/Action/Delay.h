#ifndef VOLT3D_DELAY_H
#define VOLT3D_DELAY_H

#include "Action.h"

#include "Volt3D/Macro/Define.h"

namespace v3d
{
	/**
	*	@class Delay
	*	@brief Does nothing until times up.
	*
	*	Delay simply waits until action is finished.
	*	It doesn't perform any action.
	*/
	class VOLT3D_DLL Delay : public v3d::Action
	{
	private:
		// Default constructor
		Delay();

		// initialize
		bool init(const float duration);

	public:
		// Default destructor
		~Delay();

		/**
		*	Create delay action
		*	@param duration Duration of delay
		*/
		static Delay* create(const float duration);

		// override
		bool setTarget(v3d::TransformNode* target) override;

		// ovrrides
		void update(const float delta) override;
	};
}

#endif