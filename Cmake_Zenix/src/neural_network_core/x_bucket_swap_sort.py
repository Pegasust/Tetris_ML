def get_pos(count):
    retval = count[:]
    for i in range(1,len(count)):
        retval[i] += retval[i-1]
    return retval

def swap(unsorted, idx0, idx1):
    (unsorted[idx0], unsorted[idx1]) = (unsorted[idx1], unsorted[idx0])
    
def count_inplace_stable(unsorted, count):
    pos = get_pos(count)
    i_sorted = [False] * len(unsorted)
    lookup = list(range(len(unsorted))) # Not needed for the sort.
    swaps_n = 0 # not needed
    for i in reversed(range(0, len(unsorted))):
##        print("i = {}".format(i))
##        print("pos = {}".format(pos))
        if i_sorted[i]:
            continue
        i_sorted[i] = True
        pos[unsorted[i]] -= 1
        while i > (pos[unsorted[i]]):
            # Not i is not in correct position,
            # swap until it retains the correct one
##            pos[unsorted[i]] -= 1
            swap_index = pos[unsorted[i]]
            i_sorted[swap_index] = True
            if unsorted[swap_index] != unsorted[i]:
                # This ensures stability
                (lookup[swap_index], lookup[i]) = (lookup[i], lookup[swap_index]) # not needed
                swap(unsorted, i, swap_index) # Inplace sort
                swaps_n += 1 # not needed
##                print("Swap index: {} to {}".format(i, swap_index))
            pos[unsorted[i]] -= 1
##            print("pos = {}".format(pos))
##            print("unsorted = {}".format(unsorted))
        # The position of unsorted[i] is actually sorted
##        pos[unsorted[i]] -= 1
    return (lookup, swaps_n)

def unlookup(arr_sorted, lookup):
    retval = arr_sorted[:]
    for i in range(len(retval)):
        retval[lookup[i]] = arr_sorted[i]
    return retval

def count_arr(array):
    type_count = []
    for i in range(len(array)):
        # Reserve enough capacity for size
        for _ in range(len(type_count), array[i]+1):
            type_count.append(0)
        type_count[array[i]] += 1
    return type_count

def arr_increment(arr, L):
    """
        Increases the left most value until it hits L.
        If it does flow pass L, increment the value to its right and
        set its value to 0.
        If this increment causes overflow, arr will be made to all
        zeroes and return false, otherwise, return true.
        
    """
    for i in range(len(arr)):
        if arr[i] >= L:
            arr[i] = 0
            if i == len(arr)-1:
                # this would mean increment would "cause overflow."
                # return false instead, cause we couldn't do it
                return False
            continue
        arr[i] += 1
        return True

if __name__ == "__main__":
    def do_test(test, count):
        print("====== TESTING: {} ======".format(test))
        unsorted = test[:]
        solution = sorted(unsorted)
        (lookup, num_swaps) = count_inplace_stable(unsorted, count)
        print("arranged:{}".format(unsorted))
        print("solution:{}".format(solution))
        print("lookup:  {}".format(lookup))
        print("swaps_m: {}".format(num_swaps))
        og_unsorted = unlookup(unsorted, lookup)
        print("unlookup:{}".format(og_unsorted))
        assert unsorted == solution
        assert og_unsorted == test
        
    tests = [[1, 1, 2, 1, 3, 0, 0, 3, 2],
             [0, 0, 1, 1, 1, 2, 2, 3, 3],
             [3, 3, 2, 2, 1, 1, 1, 0, 0],
             [3, 0, 0, 1, 1, 1, 2, 2, 3],
             [2, 2, 3, 3, 0, 0, 1, 1, 1],
             [1, 2, 2, 3, 3, 0, 0, 1, 1]]
    for test in tests:
        do_test(test, [2,3,2,2])
    do_test([2, 0, 2, 1, 1], [1,2,2])
    def brute_force_test(min_length, max_length):
        MAX_THRESHOLD = 15
        test_array = [0] * (min_length - 1 if min_length > 0 else 0)
        print_threshold = min_length + 1
        p_count = 0
        for length in range(min_length, max_length):
            print("\n\n\n\n\n======== BRUTE-FORCING LENGTH {} ========".format(length))
            if length > 0:
                test_array.append(0)
            L = length - 1
            dowhile = True
            while dowhile:
                count = count_arr(test_array)
                do_print = (p_count >> print_threshold) > 1
                if do_print:
                    print("test:     {} << {} >>".format(test_array, len(test_array)))
                    print("count:    {}".format(count))
                unsorted = test_array[:]
                try:
                    (lookup, swap_num) = count_inplace_stable(unsorted, count)
                except Exception as e:
##                    printb_now()
                    print("Exception occurred:")
##                    print(traceback.format_exc())
                    print(e)
                    print("Test case: {}, count: {}".format(test_array, count))
                    return
##                assert unsorted == test_array, "get_rearrange_plan should not rearrange unsorted"
                solution = sorted(test_array)
                og = unlookup(unsorted, lookup)
                same_solution = (unsorted == solution) and og == test_array
                if do_print:
                    print("solution: {}".format(solution))
                    print("rearrange:{}".format(unsorted))
                    print("og:       {}".format(og))
                    print("")
                if not same_solution:
                    print("test:     {} << {} >>".format(test_array, len(test_array)))
                    print("count:    {}".format(count))
                    print("solution: {}".format(solution))
                    print("rearrange:{}".format(unsorted))
                    print("og:       {}".format(og))
                    print("")
                assert same_solution, "rearrange spits different values"
                p_count = 0 if do_print else p_count + 1
                dowhile = arr_increment(test_array, L)
            print_threshold = max(MAX_THRESHOLD, print_threshold+1)
        print("Test done. All passed.")
                
##    brute_force_test(0, 90)
