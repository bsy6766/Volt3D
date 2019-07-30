#ifndef VOLT3D_REPEAT_H
#define VOLT3D_REPEAT_H

#include "Action.h"

#include "Volt3D/Macro/Define.h"

namespace v3d
{
	// Foward declaration
	class TransformNode;

	/**
	*	@class Repeat
	*	@brief Repeats the action for given times.
	*/
	class VOLT3D_DLL Repeat : public v3d::Action
	{
	private:
		// constructor
		Repeat();

		// Action to decorate
		v3d::Action* action;

		// Number of repeat
		unsigned int repeat;
		unsigned int currentRepeat;

		// init
		bool init(v3d::Action* action, const unsigned int repeat);

		// override
		void updateEase(const float delta) override;

	public:
		// Destructor
		~Repeat();

		// Create
		static v3d::Repeat* create(v3d::Action* action, const unsigned int repeat);

		// get repeat count
		unsigned int getRepeat() const;

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