#ifndef VOLT3D_PROGRESS_TO_H
#define VOLT3D_PROGRESS_TO_H

#include "Action.h"

#include "Volt3D/Macro/Define.h"

namespace v3d
{
	// forward declaration
	class ProgressTimer;

	/**
	*	@class ProgressTo
	*	@brief Progresses ProgressTimer by changing percentage to given percentage in duration.
	*/
	class VOLT3D_DLL ProgressTo : public v3d::Action
	{
	private:
		// Constructor
		ProgressTo();

		// percentage
		float percentage;
		float percentageDelta;

		// target. Must be progress timer
		v3d::ProgressTimer* target;

		// init
		bool init(const float duration, const float percentage);

		// override
		void updateEase(const float delta) override;

	public:
		// Destructor
		~ProgressTo();

		/**
		*	Creates ProgressTo action
		*	@param duration Duration of action
		*	@param percentage Percentage to change
		*/
		static v3d::ProgressTo* create(const float duration, const float percentage);

		// override. Set target.
		bool setTarget(v3d::TransformNode* progressTimerTarget) override;

		// ovrrides. Update action
		void update(const float delta) override;
	};
}

#endif