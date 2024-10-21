/**
 * @file PhysicsUtils.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.0
 * @date 2024-02-11
 * @note Code from the ArcGameEngine - https://github.com/MohitSethi99/ArcGameEngine
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include <box2d/box2d.h>

namespace SW
{

	class PhysicsUtils final
	{
	public:
		static bool Inside(b2Vec2 cp1, b2Vec2 cp2, b2Vec2 p)
		{
			return (cp2.x - cp1.x) * (p.y - cp1.y) > (cp2.y - cp1.y) * (p.x - cp1.x);
		}

		static b2Vec2 Intersection(b2Vec2 cp1, b2Vec2 cp2, b2Vec2 s, b2Vec2 e)
		{
			const b2Vec2 dc(cp1.x - cp2.x, cp1.y - cp2.y);
			const b2Vec2 dp(s.x - e.x, s.y - e.y);
			const f32 n1 = cp1.x * cp2.y - cp1.y * cp2.x;
			const f32 n2 = s.x * e.y - s.y * e.x;
			const f32 n3 = 1.0f / (dc.x * dp.y - dc.y * dp.x);

			return {(n1 * dp.x - n2 * dc.x) * n3, (n1 * dp.y - n2 * dc.y) * n3};
		}

		static bool VerticesFromCircle(b2Fixture* fixture, std::vector<b2Vec2>& vertices, f32 resolution = 16.0f)
		{
			if (fixture->GetShape()->GetType() != b2Shape::e_circle)
				return false;

			const b2Shape* circle = fixture->GetShape();
			const b2Vec2 position = fixture->GetBody()->GetPosition();
			const f32 radius      = circle->m_radius;

			const f32 polyCountFloat = resolution * radius;
			const int polyCount      = static_cast<int>(glm::ceil(polyCountFloat));

			constexpr f32 twoPi = 6.28318530718f;

			const f32 deltaRadians = twoPi / static_cast<f32>(polyCount);

			vertices.reserve((size_t)polyCount);

			for (int i = 0; i < polyCount; ++i)
			{
				const f32 radians = deltaRadians * static_cast<f32>(i);

				b2Vec2 point = {glm::cos(radians), glm::sin(radians)};

				vertices.emplace_back(position + (radius * point));
			}

			return true;
		}

		// http://rosettacode.org/wiki/Sutherland-Hodgman_polygon_clipping#JavaScript
		// Note that this only works when fB is a convex polygon, but we know all
		// fixtures in Box2D are convex, so that will not be a problem
		static bool FindIntersectionOfFixtures(b2Fixture* fA, b2Fixture* fB, std::vector<b2Vec2>& outputVertices)
		{
			std::vector<b2Vec2> clipPolygon;

			const b2PolygonShape* polyA = nullptr;

			switch (fA->GetShape()->GetType())
			{
			case b2Shape::e_polygon:
				polyA = static_cast<b2PolygonShape*>(fA->GetShape());
				// fill subject polygon from fixtureA polygon
				for (int i = 0; i < polyA->m_count; i++)
					outputVertices.emplace_back(fA->GetBody()->GetWorldPoint(polyA->m_vertices[i]));
				break;
			case b2Shape::e_circle:
				VerticesFromCircle(fA, outputVertices);
				break;
			default:
				return false;
			}

			const b2PolygonShape* polyB = nullptr;
			switch (fB->GetShape()->GetType())
			{
			case b2Shape::e_polygon:
				polyB = static_cast<b2PolygonShape*>(fB->GetShape());
				// fill clip polygon from fixtureB polygon
				for (int i = 0; i < polyB->m_count; i++)
					clipPolygon.emplace_back(fB->GetBody()->GetWorldPoint(polyB->m_vertices[i]));
				break;
			case b2Shape::e_circle:
				VerticesFromCircle(fB, clipPolygon);
				break;
			default:
				return false;
			}

			if (clipPolygon.empty())
				return false;

			b2Vec2 cp1 = clipPolygon[clipPolygon.size() - 1];
			for (const b2Vec2 cp2 : clipPolygon)
			{
				if (outputVertices.empty())
					return false;

				std::vector<b2Vec2> inputList = outputVertices;
				outputVertices.clear();
				b2Vec2 s = inputList[inputList.size() - 1]; // last on the input list
				for (const b2Vec2 e : inputList)
				{
					if (Inside(cp1, cp2, e))
					{
						if (!Inside(cp1, cp2, s))
							outputVertices.emplace_back(Intersection(cp1, cp2, s, e));

						outputVertices.emplace_back(e);
					}
					else if (Inside(cp1, cp2, s))
					{
						outputVertices.emplace_back(Intersection(cp1, cp2, s, e));
					}
					s = e;
				}
				cp1 = cp2;
			}

			return !outputVertices.empty();
		}

		static b2Vec2 ComputeCentroid(std::vector<b2Vec2> vs, f32& area)
		{
			const size_t count = vs.size();
			b2Assert(count >= 3);

			b2Vec2 c = {};
			c.Set(0.0f, 0.0f);
			area = 0.0f;

			// pRef is the reference point for forming triangles.
			// Its location doesn't change the result (except for rounding error).
			const b2Vec2 pRef(0.0f, 0.0f);

			constexpr f32 inv3 = 1.0f / 3.0f;

			for (size_t i = 0; i < count; ++i)
			{
				// Triangle vertices.
				b2Vec2 p1 = pRef;
				b2Vec2 p2 = vs[i];
				b2Vec2 p3 = i + 1 < count ? vs[i + 1] : vs[0];

				b2Vec2 e1 = p2 - p1;
				b2Vec2 e2 = p3 - p1;

				f32 D = b2Cross(e1, e2);

				const f32 triangleArea = 0.5f * D;
				area += triangleArea;

				// Area weighted centroid
				c += triangleArea * inv3 * (p1 + p2 + p3);
			}

			// Centroid
			if (area > b2_epsilon)
				c *= 1.0f / area;
			else
				area = 0;
			return c;
		}

		static void HandleBuoyancy(b2Fixture* fluid, b2Fixture* fixture, b2Vec2 gravity, bool flipGravity, f32 density,
		                           f32 dragMultiplier, f32 flowMagnitude, f32 flowAngleInRadians)
		{
			std::vector<b2Vec2> intersectionPoints;
			if (FindIntersectionOfFixtures(fluid, fixture, intersectionPoints))
			{
				f32 area                    = 0;
				const b2Vec2 centroid       = ComputeCentroid(intersectionPoints, area);
				const f32 gravityMultiplier = flipGravity ? -1.0f : 1.0f;

				const f32 displacedMass    = density * area;
				const b2Vec2 buoyancyForce = displacedMass * gravityMultiplier * -gravity;
				fixture->GetBody()->ApplyForce(buoyancyForce, centroid, true);

				const b2Vec2 flowForce =
				    flowMagnitude * b2Vec2(glm::cos(flowAngleInRadians), glm::sin(flowAngleInRadians));
				fixture->GetBody()->ApplyForceToCenter(flowForce, true);

				// apply drag separately for each polygon edge
				for (size_t i = 0; i < intersectionPoints.size(); i++)
				{
					// the end points and mid-point of this edge
					b2Vec2 v0       = intersectionPoints[i];
					b2Vec2 v1       = intersectionPoints[(i + 1) % intersectionPoints.size()];
					b2Vec2 midPoint = 0.5f * (v0 + v1);

					// find relative velocity between object and fluid at edge midpoint
					b2Vec2 velDir = fixture->GetBody()->GetLinearVelocityFromWorldPoint(midPoint) -
					                fluid->GetBody()->GetLinearVelocityFromWorldPoint(midPoint);

					b2Vec2 edge   = v1 - v0;
					b2Vec2 normal = b2Cross(-gravityMultiplier, edge); // gets perpendicular vector

					const f32 dragDot = b2Dot(normal, velDir);
					if (dragDot < 0)
						continue; // normal points backwards - this is not a leading edge

					const f32 vel        = velDir.Normalize();
					const f32 edgeLength = edge.Normalize();

					const f32 dragMag      = dragDot * edgeLength * density * vel * vel;
					const b2Vec2 dragForce = dragMag * dragMultiplier * -velDir;
					fixture->GetBody()->ApplyForce(dragForce, midPoint, true);

					// apply lift
					const f32 liftMag      = b2Dot(edge, velDir) * dragMag;
					const b2Vec2 liftDir   = b2Cross(gravityMultiplier, velDir); // gets perpendicular vector
					const b2Vec2 liftForce = liftMag * liftDir;
					fixture->GetBody()->ApplyForce(liftForce, midPoint, true);
				}
			}
		}
	};

} // namespace SW
