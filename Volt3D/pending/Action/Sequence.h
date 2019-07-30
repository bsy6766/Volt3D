#ifndef VOLT3D_SEQUENCE_H
#define VOLT3D_SEQUENCE_H

#include <vector>
#include <initializer_list>

#include "Action.h"

#include "Volt3D/Macro/Define.h"

namespace v3d
{
	// Foward declaration
	class TransformNode;

	/**
	*	@class Sequence
	*	@Brief A sequence of action
	*
	*	Sequence is sequence of actions that executes actions in order.
	*	Sequence with 1 action will still be sequence, but will act same as just action itself. There is no point of making sequence with 1 action.
	*	Sequence with 2 actions will first excute first action and then second action.
	*	Sequence with mutliple actions will converted to nested two actions sequence.
	*	For example, actions [1,2,3,4] will be [[1, 2], 3] and 4.
	*/
	class VOLT3D_DLL Sequence : public v3d::Action
	{
	private:
		// Constructor
		Sequence();

		// First action
		v3d::Action* first;
		// Second action
		v3d::Action* second;

		// Initialize with 1 action
		bool init(v3d::Action* action);

		// Initialize with 2 actions
		bool init(v3d::Action* first, v3d::Action* second);

		// Initialize with multiple actions
		bool init(const std::initializer_list<v3d::Action*>& actions);

		// override
		void updateEase(const float delta) override;

	public:
		// Destructor
		~Sequence();

		// Create with 1 action
		static v3d::Sequence* create(v3d::Action* action);

		// Create with 2 actions
		static v3d::Sequence* create(v3d::Action* first, v3d::Action* second);

		// Create with multiple actions
		static v3d::Sequence* create(const std::initializer_list<Action*>& actions);

		// Check if sequence is done
		bool isDone() const override;

		// Get exceeded time
		float getExceededTime() override;

		// reset
		void reset() override;

		// Set target to both actions
		bool setTarget(v3d::TransformNode* target) override;

		// Update sequence
		void update(const float delta) override;
	};
}

#endif