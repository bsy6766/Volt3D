/**
*	@file Utility.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_UTILITY_H
#define V3D_UTILITY_H

#include <random>
#include <string>
#include <iostream>

#include <glm/glm.hpp>

#include "Utils/Macros.h"

namespace v3d
{
	namespace Utility
	{
		// M_PI float
		static const float M_PIf = 3.14159265358979323846f;
		static const float M_PI_2f = 1.57079632679489661923f;
		static const float M_PI_4f = 0.785398163397448309616f;


		namespace Math
		{
			static float fastFloor(float value)
			{
				return static_cast<float>(value >= 0 ? static_cast<int>(value) : static_cast<int>(value) - 1);
			}

			static unsigned int findNearestPowTwo(unsigned int num)
			{
				num--;
				num |= num >> 1;
				num |= num >> 2;
				num |= num >> 4;
				num |= num >> 8;
				num |= num >> 16;
				num++;
				return num;
			}

			static float cross(const glm::vec2& a, const glm::vec2& b)
			{
				return (a.x * b.y) - (a.y * b.x);
			}
		}

		namespace Math3D
		{
			// Calculates surface normal from 3 points
			static inline glm::vec3	getNormal(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3)
			{
				glm::vec3 p2p1 = p2 - p1;
				glm::vec3 p3p1 = p3 - p1;

				glm::vec3 n;

				n.x = (p2p1.y * p3p1.z) - (p2p1.z * p3p1.y);
				n.y = (p2p1.z * p3p1.x) - (p2p1.x * p3p1.z);
				n.z = (p2p1.x * p3p1.y) - (p2p1.y * p3p1.x);

				return glm::normalize(n);
			}
		}

		namespace Polygon
		{
			static bool isPointInPolygon(const std::vector<glm::vec2>& polygon, const glm::vec2& point)
			{
				int nvert = static_cast<int>(polygon.size());

				int i, j, c = 0;

				for (i = 0, j = nvert - 1; i < nvert; j = i++)
				{
					if (((polygon.at(i).y > point.y) != (polygon.at(j).y > point.y)) && (point.x < (polygon.at(j).x - polygon.at(i).x) * (point.y - polygon.at(i).y) / (polygon.at(j).y - polygon.at(i).y) + polygon.at(i).x))
						c = !c;
				}

				if (c)
				{
					return true;
				}
				else
				{
					for (int i = 0; i < nvert - 1; i++)
					{
						if (glm::distance(polygon.at(i), point) + glm::distance(polygon.at(i + 1), point) == glm::distance(polygon.at(i), polygon.at(i + 1)))
						{
							return true;
						}
					}

					if (glm::distance(polygon.front(), point) + glm::distance(polygon.back(), point) == glm::distance(polygon.front(), polygon.back()))
					{
						return true;
					}

				}

				return false;
			}
		}

		namespace BitFlag
		{
			template <class T> inline T addFlag(T src, T flag)
			{ 
				return static_cast<T>(src | flag); 
			}

			template <class T> inline T removeFlag(T src, T flag) 
			{
				return static_cast<T>(src & ~flag);
			}
		}
	}
}

#endif