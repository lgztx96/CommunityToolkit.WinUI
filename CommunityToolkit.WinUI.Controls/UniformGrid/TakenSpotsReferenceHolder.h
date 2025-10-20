#pragma once
#include <algorithm>
#include <vector>

namespace winrt::CommunityToolkit::WinUI::Controls
{
    /// </summary>
    class TakenSpotsReferenceHolder final
    {
    public:
        /// <summary>
        /// Initializes a new instance of the TakenSpotsReferenceHolder class.
        /// </summary>
        /// <param name="rows">The number of rows to track.</param>
        /// <param name="columns">The number of columns to track.</param>
        TakenSpotsReferenceHolder(int rows, int columns)
            : _height(rows),
            _width(columns),
            _spotsTaken(static_cast<size_t>(rows)* static_cast<size_t>(columns), false)
        {
        }

        /// <summary>
        /// Gets the height of the grid to monitor.
        /// </summary>
        [[nodiscard]] int Height() const noexcept { return _height; }

        /// <summary>
        /// Gets the width of the grid to monitor.
        /// </summary>
        [[nodiscard]] int Width() const noexcept { return _width; }

        /// <summary>
        /// Gets or sets the value of a specified grid cell.
        /// </summary>
        /// <param name="i">The vertical offset.</param>
        /// <param name="j">The horizontal offset.</param>
        bool Get(int i, int j) const noexcept
        {
            return _spotsTaken[static_cast<size_t>(i) * _width + static_cast<size_t>(j)];
        }

        void Set(int i, int j, bool value) noexcept
        {
            _spotsTaken[static_cast<size_t>(i) * _width + static_cast<size_t>(j)] = value;
        }

        /// <summary>
        /// Indexer-like operator overload for convenience.
        /// </summary>
        bool operator()(int i, int j) const noexcept { return Get(i, j); }
        const bool& operator()(int i, int j) noexcept { return _spotsTaken.at(static_cast<size_t>(i) * _width + static_cast<size_t>(j)); }

        /// <summary>
        /// Fills the specified area in the current grid with a given value.
        /// Invalid coordinates are safely ignored.
        /// </summary>
        void Fill(bool value, int row, int column, int width, int height) noexcept
        {
            // Compute clamped region
            int left = std::clamp(column, 0, _width);
            int top = std::clamp(row, 0, _height);
            int right = std::clamp(column + width, 0, _width);
            int bottom = std::clamp(row + height, 0, _height);

            for (int i = top; i < bottom; ++i)
            {
                for (int j = left; j < right; ++j)
                {
                    Set(i, j, value);
                }
            }
        }

        /// <summary>
        /// Resets the current reference holder.
        /// </summary>
        void Reset() noexcept
        {
            std::fill(_spotsTaken.begin(), _spotsTaken.end(), false);
        }

    private:
        int _height;
        int _width;
        std::vector<bool> _spotsTaken;
    };
}
