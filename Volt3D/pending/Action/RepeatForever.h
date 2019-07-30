#ifndef VOLT3D_REPEATFOREVER_H
#define VOLT3D_REPEATFOREVER_H

#include "Action.h"

#include "Volt3D/Macro/Define.h"

namespace v3d
{
	// Foward declaration
	class TransformNode;

	/**
	*	@class RepeatForever
	*	@brief Repeats action forever
	*/
	class VOLT3D_DLL RepeatForever : public v3d::Action
	{
	private:
		// constructor
		RepeatForever();

		// Action to decorate
		v3d::Action* action;

		// init
		bool init(v3d::Action* action);

		// override
		void updateEase(const float delta) override;

	public:
		// Destructor
		~RepeatForever();

		/**
		*	Create an action that repeats forever
		*	@param action An action that needs to repeat forever.
		*	Action must not be instant action (0.0f seconds
		*	duration). Any instant action will be rejected.
		*/
		static v3d::RepeatForever* create(v3d::Action* action);

		// Check if repeat is done
		bool isDone() const override;

		// reset
		void reset() override;

		// override
		float getExceededTime() override;

		// Set target to both actions
		bool setTarget(v3d::TransformNode* target) override;

		// Update sequence
		void update(const float delta) override;
	};
}

#endif