#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <limits>
#include <memory>
#include <random>
#endif
#include "ColorPaletteSampler.h"

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    std::vector<winrt::float3> ColorPaletteSampler::KMeansCluster(std::span<winrt::float3> points, int k, std::vector<int>& counts)
    {
        // Track the assigned cluster of each point
        std::vector<int> clusterIds;
		clusterIds.resize(points.size());
        // Track the centroids of each cluster and its member count
        std::unique_ptr<winrt::float3[]> centroids = std::make_unique<winrt::float3[]>(k);
        counts.resize(k);

        // Split the points into arbitrary clusters
        Split(k, clusterIds);

        std::span<winrt::float3> centroidView{ centroids.get(), static_cast<size_t>(k) };
        bool converged = false;
        while (!converged)
        {
            // Assume we've converged. If we haven't, we'll assign converged
            // to false when adjust the clusters
            converged = true;

            // Calculate/Recalculate centroids
            CalculateCentroidsAndPrune(centroidView, counts, points, clusterIds);

            // Move each point's clusterId to the nearest cluster centroid
            for (size_t i = 0; i < points.size(); i++)
            {
                auto nearestIndex = FindNearestClusterIndex(points[i], centroidView);

                // The nearest cluster hasn't changed. Do nothing
                if (clusterIds[i] == nearestIndex)
                    continue;

                // Update the cluster id and note that we have not converged
                clusterIds[i] = nearestIndex;
                converged = false;
            }
        }

        return std::vector<winrt::float3>{ centroidView.begin(), centroidView.end() };
    }

    void ColorPaletteSampler::Split(int k, std::vector<int>& clusterIds)
    {
        static thread_local std::mt19937 rng{ std::random_device{}() };
        std::uniform_int_distribution<int> dist(0, k - 1);

        int offset = dist(rng);

        // Assign each clusters id
        for (size_t i = 0; i < clusterIds.size(); i++)
            clusterIds[i] = (i + offset) % k;
    }

    void ColorPaletteSampler::CalculateCentroidsAndPrune(std::span<winrt::float3>& centroids, std::vector<int>& counts, std::span<winrt::float3> points, const std::vector<int>& clusterIds)
    {
        // Clear centroids and counts before recalculation
        for (size_t i = 0; i < centroids.size(); i++)
        {
            centroids[i] = winrt::float3::zero();
            counts[i] = 0;
        }

        // Accumulate step in centroid calculation
        for (size_t i = 0; i < clusterIds.size(); i++)
        {
            int id = clusterIds[i];
            centroids[id] += points[i];
            counts[id]++;
        }

        // Prune empty clusters
        // All empty clusters are swapped to the end of the span
        // then a slice is taken with only the remaining populated clusters
        int pivot = static_cast<int>(counts.size());
        for (int i = 0; i < pivot;)
        {
            // Increment and continue if populated
            if (counts[i] != 0)
            {
                i++;
                continue;
            }

            // The item is not populated. Swap to end and move pivot
            // NOTE: This is a one-way "swap". We're discarding the 0s anyways.
            pivot--;
            centroids[i] = centroids[pivot];
            counts[i] = counts[pivot];
        }

        // Perform slice
        counts.resize(pivot);
        centroids = centroids.subspan(0, pivot);

        // Division step in centroid calculation
        for (size_t i = 0; i < centroids.size(); i++)
            centroids[i] /= static_cast<float>(counts[i]);
    }

    int ColorPaletteSampler::FindNearestClusterIndex(winrt::float3 point, std::span<winrt::float3> centroids)
    {
        // Track the nearest centroid's distance and the index of that centroid
        float nearestDistance = std::numeric_limits<float>::infinity();
        int nearestIndex = -1;

        for (size_t j = 0; j < centroids.size(); j++)
        {
            // Compare the point to the jth centroid
            float distance = distance_squared(point, centroids[j]);

            // Skip the cluster if further than the nearest seen cluster 
            if (nearestDistance < distance)
                continue;

            // This is the nearest cluster
            // Update the distance and index
            nearestDistance = distance;
            nearestIndex = static_cast<int>(j);
        }

        return nearestIndex;
    }
}
