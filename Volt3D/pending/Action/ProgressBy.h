#ifndef VOLT3D_PROGRESS_BY_H
#define VOLT3D_PROGRESS_BY_H

#include "Action.h"

#include "Volt3D/Macro/Define.h"

namespace v3d
{
	// forward declaration
	class ProgressTimer;

	/**
	*	@class ProgressBy
	*	@brief Progresses ProgressTimer by changing percentage with amount of give percentage in duration.
	*/
	class VOLT3D_DLL ProgressBy : public v3d::Action
	{
	private:
		// Constructor
		ProgressBy();

		// percentage
		float percentage;

		// target. Must be progress timer
		v3d::ProgressTimer* target;

		// init
		bool init(const float duration, const float percentage);

		// override
		void updateEase(const float delta) override;

	public:
		// Destructor
		~ProgressBy();

		/**
		*	Creates ProgressBy action
		*	@param duration Duration of action
		*	@param percentage Percentage to change
		*/
		static v3d::ProgressBy* create(const float duration, const float percentage);

		// override. Set target.
		bool setTarget(v3d::TransformNode* progressTimerTarget) override;

		// ovrrides. Update action
		void update(const float delta) override;
	};
}

#endif