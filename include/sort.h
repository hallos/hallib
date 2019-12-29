#include <algorithm>
#include <vector>

namespace hallos
{

template <class It>
void mergesort(It begin, It end)
{
    using difference_type = typename std::iterator_traits<It>::difference_type;
    difference_type range_length = std::distance(begin, end);

    if (range_length == 2)
    {
        if (*(begin+1) < *begin)
        {
            std::iter_swap(begin, begin + 1);
        }
    }
    else if (range_length > 2)
    {
        auto middle_of_range = begin + (range_length / 2);
        mergesort(begin, middle_of_range);
        mergesort(middle_of_range, end);

        std::inplace_merge(begin, middle_of_range, end);
    }
    return;
}

}; //namespace hallos