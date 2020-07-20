/*
Copyright (c) 2020 S.E. B.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once

#include <vector>
#include <stdexcept>

#define RV2D__VERSION_MAJOR 0
#define RV2D__VERSION_MINOR 1
#define RV2D__VERSION_PATCH 0

namespace rv2d
{
	static constexpr struct
	{
		uint8_t major, minor, patch;
	} version = {
		RV2D__VERSION_MAJOR,
		RV2D__VERSION_MINOR,
		RV2D__VERSION_PATCH};
} // namespace rv2d

namespace rv2d
{
	/**
	 * @brief A simple two-dimensional array using a vector
	 * 
	 * @tparam T 
	 */template <class T>
class RowVector2D
{
	// array of data
	std::vector<T> data_;
	// topology of the 2D array
	size_t cols_;
	size_t rows_;

public:
	template <class U>
	class Indexer
	{
		size_t offset_;
		std::vector<U> &data_;
		std::vector<std::pair<size_t, size_t>> row_col_idx_helper_;

		size_t row_;
		size_t col_;

	public:
		Indexer(size_t row, size_t cols_, std::vector<U> &data) : data_(data)
		{
			row_ = row;
			offset_ = (row_ - 1) * cols_;
			row_col_idx_helper_.resize(data_.size());
		}

		T &operator[](size_t col)
		{
			col_ = col;

			size_t idx = offset_ + (col - 1);
			if (idx >= data_.size())
				throw std::out_of_range("index out of range :(");

			// set the index helper to convert idx to (row,col)
			row_col_idx_helper_[idx] = (std::make_pair(row_, col_));
			return data_[idx];
		}
	};

	RowVector2D()
	{
	}
	RowVector2D(size_t rows, size_t cols) : data_(rows * cols), cols_(cols), rows_(rows)
	{
	}

	Indexer<T> operator[](size_t row)
	{
		return Indexer<T>(row, cols_, data_);
	}

	T &operator()(size_t row, size_t col)
	{
		if ((row > rows_) or (col > cols_))
			throw std::out_of_range(std::to_string(row) + " > " + std::to_string(rows_) + " or " +
									std::to_string(col) + " > " + std::to_string(cols_) + " index out of range :(");
		if ((row < 0) or (col < 0))
			throw std::out_of_range(std::to_string(row) + " or " + std::to_string(col) + " <= 0: index out of range :(");

		return this->operator[](row).operator[](col);
	}

	size_t rows()
	{
		return rows_;
	}

	size_t cols()
	{
		return cols_;
	}

	size_t size()
	{
		return data_.size();
	}

	void resize(size_t rows, size_t cols)
	{
		this->clear();
		cols_ = cols;
		rows_ = rows;
		data_.resize(rows_ * cols_);
	}

	void clear()
	{
		rows_ = 0;
		cols_ = 0;
		data_.clear();
		data_.shrink_to_fit();
	}

	bool empty()
	{
		return data_.empty();
	}
};
} // namespace rv2d