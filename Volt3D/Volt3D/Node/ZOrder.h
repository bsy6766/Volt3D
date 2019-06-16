#ifndef V3D_Z_ORDER_H
#define V3D_Z_ORDER_H

#include "Utils/Macros.h"

namespace v3d
{
	// Foward declaration
	class TransformNode;

	/**
	*	@class ZOrder
	*	@brief Order for nodes.
	*
	*	ZOrder has two different z order.
	*	
	*	Global z order defines global order of rendering nodes.
	*	Nodes that have negative global z order will be rendered first
	*	regardless of type.
	*	i.e) Setting 2d sprite's global z order lower than 3d sprite will 
	*		render 2d sprite first even the 3d sprite is closer to camera
	*
	*	Local z order defines order between siblings in node.
	*	
	*	@see v3d::Renderer for detailed rendering order.
	*	
	*/
	class VOLT3D_DLL ZOrder
	{
		friend struct ZOrderComp;

	private:
		int global;
		int local;

	public:
		// Constructor
		ZOrder();

		/**
		*	Constructor with local z order. Global sets to 0.
		*	@param localZOrder Local z order
		*/
		ZOrder(const int localZOrder);

		/**
		*	Constructor with global and local z order
		*	@param globalZOrder Global z order
		*	@param localZOrder Local z order
		*/
		ZOrder(const int globalZOrder, const int localZOrder);

		// destructor
		~ZOrder();

		// Get global z order
		int getGlobalZOrder() const;

		// Get local z order
		int getLocalZOrder() const;

		/**
		*	Set global z order
		*	@param globalZOrder A new global z order to set
		*/
		void setGlobalZOrder(const int globalZOrder);

		/**
		*	Set local z order
		*	@param localZOrder A new global z order to set
		*/
		void setLocalZOrder(const int localZOrder);

		// equal operator
		bool operator==(const v3d::ZOrder& other);
				
		/**
		*	Checks if global z order is MAX_INT
		*	return True if global z order is MAX_INT. Else, false.
		*/
		bool isGlobalZOrderMaxInt() const;

		/**
		*	Checks if local z order is MAX_INT
		*	return True if global z order is MAX_INT. Else, false.
		*/
		bool isLocalZOrderMaxInt() const;

		// Get max value of zOrder
		static int getMaxZOrder();

		// Get min value of zOrder
		static int getMinZOrder();

		// Log zorder. for debug.
		void print() const;
	};

	// Comparator for ZORder
	struct ZOrderComp
	{
		bool operator()(const ZOrder& lhs, const ZOrder& rhs) const
		{
			// Check global order first
			if (lhs.global == rhs.global)
			{
				// global is same. Check local
				if (lhs.local == rhs.local)
				{
					// local is same. 
					return false;
				}
				else
				{
					// compare local
					return lhs.local < rhs.local;
				}
			}
			else
			{
				// compare global
				return lhs.global < rhs.global;
			}
		}
	};
}

#endif