

#pragma once

#include <cppurses/widget/layouts/vertical.hpp>
#include <cppurses/widget/layouts/horizontal.hpp>

class Table: public cppurses::layout::Vertical {
public:
	Table(int columns)
		: _num_columns(columns)
		, _num_cells(0) {
		border.enable();
		border.segments.south.disable();
		border.segments.west.disable();
		border.segments.east.disable();
		border.segments.south_west.disable();
		border.segments.south_east.disable();
	}

	template <typename Widg_t, typename... Args>
	Widg_t& make_cell(Args&&... args) {
		auto& row = get_next_free_row();
		auto& cell = row.make_child<Widg_t>(std::forward<Args>(args)...);
		cell.border.enable();
		cell.border.segments.west.disable();
		cell.border.segments.north.disable();
		cell.border.segments.north_west.disable();
		cell.border.segments.north_east.disable();
		cell.border.segments.south_west.disable();
		_num_cells++;
		return cell;
	}

	bool resize_event(cppurses::Area new_size, cppurses::Area old_size) override {
		// prevent rows with empty cells to use all the horizontal space by
		// forcing the width on each cell.
		bool res = cppurses::layout::Vertical::resize_event(new_size, old_size);
		for(auto& row : _rows) {
			for(auto& cell : row.get().children.get()) {
				cell.get()->width_policy.fixed(std::max(0, (int)(width() / _num_columns)));
			}
		}
		return res;
	}

	template <typename Widg_t>
	Widg_t& get_cell_at(size_t col, size_t row) {
		return static_cast<Widg_t&>(*_rows[row].get().children.get().at(col).get());
	}

	template <typename Widg_t>
	Widg_t& get_cell_at(size_t index) {
		return get_cell_at<Widg_t>(index % _num_columns, index / _num_columns);
	}

	size_t num_rows() const {
		return _rows.size();
	}

private:
	const size_t _num_columns;
	size_t _num_cells;
	std::vector<std::reference_wrapper<cppurses::layout::Horizontal>> _rows;

	cppurses::layout::Horizontal& get_next_free_row() {
		if(_num_cells % _num_columns == 0) {
			return make_empty_row();
		} else {
			return _rows.at(_rows.size()-1).get();
		}
	}

	cppurses::layout::Horizontal& make_empty_row() {
		auto& row = make_child<cppurses::layout::Horizontal>();
		_rows.push_back(row);
		return row;
	}
};
