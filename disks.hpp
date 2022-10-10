///////////////////////////////////////////////////////////////////////////////
// disks.hpp
//
// Definitions for two algorithms that each solve the alternating disks problem.
//
// As provided, this header has four functions marked with TODO comments.
// You need to write in your own implementation of these functions.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <sstream>
#include <string>
#include <vector>
#include <functional>
#include <iostream>

enum disk_color { DISK_LIGHT, DISK_DARK};

class disk_state {
private:
  std::vector<disk_color> _colors;
public:
  disk_state(size_t light_count)
    : _colors(light_count * 2, DISK_LIGHT) {
    assert(light_count > 0);
    for (size_t i = 1; i < _colors.size(); i += 2) {
      _colors[i] = DISK_DARK;
    }
  }

  bool operator== (const disk_state& rhs) const {
    return std::equal(_colors.begin(), _colors.end(), rhs._colors.begin());
  }

  size_t total_count() const {
    return _colors.size();
  }

  size_t light_count() const {
    return total_count() / 2;
  }

  size_t dark_count() const {
    return light_count();
  }

  bool is_index(size_t i) const {
    return (i < total_count());
  }

  disk_color get(size_t index) const {
    assert(is_index(index));
    return _colors[index];
  }

  void swap(size_t left_index) {
    assert(is_index(left_index));
    auto right_index = left_index + 1;
    assert(is_index(right_index));
    std::swap(_colors[left_index], _colors[right_index]);
  }

  std::string to_string() const {
    std::stringstream ss;
    bool first = true;
    for (auto color : _colors) {
      if (!first) {
        ss << " ";
      }

      if (color == DISK_LIGHT) {
        ss << "L";
      } else {
        ss << "D";
      }

      first = false;
    }
    return ss.str();
  }

  // Return true when this disk_state is in alternating format. That means
  // that the first disk at index 0 is light, the second disk at index 1
  // is dark, and so on for the entire row of disks.
  bool is_initialized() const {
      for (size_t i = 0; i < total_count(); i++){     //check each position before function
        if (i % 2 == 0){                              //check even position --> should be light
            if (_colors[i] == DISK_DARK) {
              return false;
            }
        } else {                                      //check odd position --> should be dark
            if (_colors[i] == DISK_LIGHT) {
              return false;
            }
        }
      }

      return true;
  }

  // Return true when this disk_state is fully sorted, with all light disks on
  // the left (low indices) and all dark disks on the right (high indices).
  bool is_sorted() const {
      
      return true;
  }
};

// Data structure for the output of the alternating disks problem. That
// includes both the final disk_state, as well as a count of the number
// of swaps performed.
class sorted_disks {
private:
  disk_state _after;
  unsigned _swap_count;

public:
  sorted_disks(const disk_state& after, unsigned swap_count)
    : _after(after), _swap_count(swap_count) { }

  sorted_disks(disk_state&& after, unsigned swap_count)
    : _after(after), _swap_count(swap_count) { }

  const disk_state& after() const {
    return _after;
  }

  unsigned swap_count() const {
    return _swap_count;
  }
};

// Algorithm that sorts disks using the alternate algorithm.
sorted_disks sort_alternate(const disk_state& before) {
	int numOfSwap = 0;                                                                      //record # of step swap/ BEGIN TODO
	disk_state state = before;
	for(size_t i = 0; i < state.total_count() + 1; i++) {
	  if(i % 2 == 0) {
	    for(size_t j = 0; j < state.total_count() - 1; j=j+2) {
		if(state.get(j) > state.get(j+1)) {
		  state.swap(j);
		  numOfSwap++;
		}
	      }
	    }
	    else {
	      for(int j = 1; j < state.total_count() - 2; j=j+2) {
		if(state.get(j) > state.get(j+1)) {
		  state.swap(j);
		  numOfSwap++;
		}
	      }
	    }
	  }
  return sorted_disks(disk_state(state), numOfSwap);                                           /////////END OF TODO
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Algorithm that sorts disks using the lawnmower algorithm.                             /////////////BEGIN OF NEXT TODO
sorted_disks sort_lawnmower(const disk_state& before) {
	disk_state after=before;
	bool isGoingForward=true;
	size_t swapCount=0, left=1, right=before.total_count()-1, maxIterations=before.total_count()/2;
	for (size_t i=0; i<before.total_count()/2; i++) {
		if (isGoingForward) {						// !isGoingForward is moving from left to right
			for (size_t j=left; j<=right; j+=2) {
				disk_color currentColor=after.get(j);
				if (currentColor==DISK_DARK) {
					size_t swapIndex=j-1;
					after.swap(swapIndex);
					swapCount++;
				}
			}
			isGoingForward=false;
		}
		else {								// isGoingForward is moving right to left
			for (size_t j=right; j>=left; j-=2) {
				disk_color currentColor=after.get(j);
				if (currentColor==DISK_DARK) {
					size_t swapIndex=j-1;
					after.swap(swapIndex);
					swapCount++;
				}
			}
			isGoingForward = true;
		}
		left++;
		right--;
	}

	return sorted_disks(after, swapCount);
}
