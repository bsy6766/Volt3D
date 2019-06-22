/**
*	@file Action.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_ACTION_H
#define V3D_ACTION_H

#include <glm/glm.hpp>

#include "Utils/Macros.h"

namespace v3d
{
	class TransformNode;

	/**
	*	@class Action
	*	@brief A abstract base class for all actions.
	*
	*	@group action
	*
	*	@since 1.0
	*/
	class VOLT3D_DLL Action
	{
		friend class Ease;
		friend class Repeat;
		friend class RepeatForever;
		friend class Sequence;

	protected:
		/** Constructor */
		Action();
		
		float duration;
		float elapsedTime;

		/** Get linear ease time */
		float getLinearEaseTime();

		/** Set duration */
		bool setDuration(const float duration);

		/** Update action with ease.  */
		virtual void updateEase(const float delta);

	public:
		/** Destructor. */
		virtual ~Action();

		/** Check if action is done. Action is done if time is up (elapsedTime >= duration) */
		virtual bool isDone() const;

		/** Get exceeded time. */
		virtual float getExceededTime();
		
		/** Get duration */
		float getDuration() const;

		/** Get elapsedTime */
		float getElapsedTime() const;

		/** Check if istance (duration == 0 seconds) */
		bool isInstant() const;

		/** Reset action */
		virtual void reset();

		/** set target. Derived classes override this to set their own target. Some actions doesn't have target so it's up to derived class to update. Base class setTarget does nothing. */
		virtual bool setTarget(v3d::TransformNode* target) = 0;

		/** Update action. All derived actions must override */
		virtual void update(const float delta) = 0;
	};
}

#endif
