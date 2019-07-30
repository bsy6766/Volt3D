#ifndef VOLT3D_EASE_H
#define VOLT3D_EASE_H

#include "Volt3D/Macro/Define.h"

#include "Action.h"

namespace v3d
{
	class VOLT3D_DLL Ease : public v3d::Action
	{
	protected:
		// Constructor
		Ease();

		// prev t
		float prevT;

		// action
		v3d::Action* action;

		// init
		bool init(v3d::Action* action);

	public:
		// Destructor
		virtual ~Ease();

		// Check if repeat is done
		bool isDone() const override;

		// reset
		void reset() override;

		// override
		float getExceededTime() override;

		// Set target to both actions
		bool setTarget(v3d::TransformNode* target) override;

		// override
		void update(const float delta) override;

		// Get ease t
		virtual float getT() const = 0;
	};
}

#endif