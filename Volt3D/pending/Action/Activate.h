#ifndef VOLT3D_ACTIVATE_H
#define VOLT3D_ACTIVATE_H

#include "Action.h"

#include "Volt3D/Macro/Define.h"

namespace v3d
{
	// forward declaration
	class TransformNode;

	/**
	*	@class Activate
	*	@brief Set object's active to true
	*/
	class VOLT3D_DLL Activate : public v3d::Action
	{
	private:
		// Default constructor
		Activate();

		// target. Need TransformNode at least.
		v3d::TransformNode* target;

		// active flag
		bool active;
		
	public:
		// Default destructor
		~Activate();

		/**
		*	Create Activate action.
		*	@param active true activates node. false deactivates node.
		*/
		static Activate* create(const bool active);

		// override
		bool setTarget(v3d::TransformNode* target) override;
		
		// ovrrides
		void update(const float delta) override;
	};
}

#endif