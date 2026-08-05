#pragma once
#include "pch.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.Numerics.h>
#include <queue>
#include <span>
#include <vector>
#endif

namespace winrt
{
	using namespace Windows::Foundation;
	using namespace Windows::Foundation::Numerics;
}

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    struct DBScan
    {
        static constexpr int Unclassified = -1;

        static std::vector<float3> Cluster(
            std::span<float3> points,
            float epsilon,
            int minPoints,
            std::vector<float>& weights)
        {
            std::vector<float3> centroids;
            std::vector<float> newWeights;

            // Create context
            DBScan context(points, weights, epsilon, minPoints);

            // Attempt to create a cluster around each point,
            // skipping that point if already classified
            for (size_t i = 0; i < points.size(); i++)
            {
                // Already classified, skip
                if (context.PointClusterIds[i] != Unclassified)
                    continue;

                winrt::float3 centroid{};
                float weight = 0.0f;
                // Attempt to create cluster
                if (context.CreateCluster(static_cast<int>(i), centroid, weight))
                {
                    centroids.push_back(centroid);
                    newWeights.push_back(weight);
                }
            }

            weights = std::move(newWeights);
            return centroids;
        }

    private:
        bool CreateCluster(int originIndex, float3& centroid, float& weight)
        {
            weight = 0.0f;
            centroid = float3::zero();

            bool isCore = false;
            auto seeds = GetSeeds(originIndex, isCore);

            // Not enough seeds to be a core point.
            // Cannot create a cluster around it
            if (!isCore)
                return false;

            ExpandCluster(seeds, centroid, weight);
            ClusterId++;

            return true;
        }

        void ExpandCluster(std::queue<int>& seeds, float3& centroid, float& weight)
        {
            weight = 0.0f;
            centroid = float3::zero();

            while (!seeds.empty())
            {
                int seedIndex = seeds.front();
                seeds.pop();

                // Skip duplicate seed entries
                if (PointClusterIds[seedIndex] != Unclassified)
                    continue;

                // Assign this seed's id to the cluster
                PointClusterIds[seedIndex] = ClusterId;

                float w = Weights[seedIndex];
                centroid += Points[seedIndex] * w;
                weight += w;

                // Check if this seed is a core point
                bool seedIsCore = false;
                auto grandSeeds = GetSeeds(seedIndex, seedIsCore);
                if (!seedIsCore)
                    continue;

                // This seed is a core point. Enqueue all its seeds
                while (!grandSeeds.empty())
                {
                    int idx = grandSeeds.front();
                    grandSeeds.pop();

                    if (PointClusterIds[idx] == Unclassified)
                        seeds.push(idx);
                }
            }

            centroid /= weight;
        }

        std::queue<int> GetSeeds(int originIndex, bool& isCore)
        {
            const auto& origin = Points[originIndex];

            // NOTE: Seeding could be done using a spatial data structure to improve traversal
            // speeds. However currently DBSCAN is run after KMeans with a maximum of 8 points.
            // There is no need.

            std::queue<int> seeds;
            for (size_t i = 0; i < Points.size(); i++)
            {
                if (distance_squared(origin, Points[i]) <= Epsilon2)
                    seeds.push(static_cast<int>(i));
            }

            // Count includes self, so compare without checking equals
            isCore = static_cast<int>(seeds.size()) > MinPoints;
            return seeds;
        }

        DBScan(
            std::span<float3> points,
            std::span<float> weights,
            float epsilon,
            int minPoints)
            : Points(points),
            Weights(weights),
            Epsilon2(epsilon* epsilon),
            MinPoints(minPoints),
            ClusterId(0),
            PointClusterIds(points.size(), Unclassified)
        {
        }

    private:
        std::span<float3> Points;
        std::span<float> Weights;
        int ClusterId;
        std::vector<int> PointClusterIds;
        float Epsilon2;
        int MinPoints;
    };
}
