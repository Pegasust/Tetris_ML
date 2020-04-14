"""
    Given an unsorted array of length N, this array contains duplicate integers.
    This array only contains element e such that 0 <= e <= L (L <= N-1)
    Also given an array length (L+1) that contains the counts of number of elements
    of each element e.
    Example:
        unsorted = [1, 1, 2, 1, 3, 0, 0, 3, 2], where l = 3
        counts =   [2, 3, 2, 2].
        counts can contain 0 too:
        unsorted = [1, 1, 2, 1, 3, 3, 1, 3, 2], where l = 3
        counts =   [0, 4, 2, 3].
    Create a sorting algorithm that sorts unsorted array in O(l*n) time.
"""

def swap(unsorted, idx0, idx1):
    (unsorted[idx0], unsorted[idx1]) = (unsorted[idx1], unsorted[idx0])
    
def bucket_swap_sort(unsorted, count):
    count = get_pos(count)
    # Stable sort
    output = [0] * len(unsorted)
    # Going with the decrement of count makes a stable sort.
    for i in range(len(unsorted)-1, 0, -1):
        output[count[unsorted[i]]-1] = unsorted[i]
        count[unsorted[i]] -= 1
    return output

def rearrange_counting_sort(unsorted, count):
    
        

##def order_plan(plan_array, initial_index, swap_index):
##    """
##        Represents the order of swapping
##    """
##    plan_array.append(initial_index)
##    plan_array.append(swap_index)
##    return plan_array
##
##def lookup_plan(plan_array, initial_index, swap_index):
##    plan_array[initial_index] = swap

def get_rearrange_plan(unsorted, count, debug = False):
    """
        The swapping order (output array of len(unsorted) * 2.
        (Returns (retval, lookup, n_swap))
    """
    n_swap = 0
    retval = []
    (min_pos, max_pos) = initialize(count)
    pos = min_pos[:]
    i_sorted = [False]*len(unsorted)
    lookup = list(range(0,len(unsorted)))
    for i in range(len(unsorted)):
        if(debug):
            print("i = {}".format(i))
            print("retval:\t{}".format(retval))
            print("i_sorted:\t{}".format(i_sorted))
            print("pos:\t\t{}".format(pos))
            print("min_pos:\t{}".format(min_pos))
            print("max_pos:\t{}".format(max_pos))
            print("retval:\t\t{}".format(retval))
            print("lookup:\t\t{}".format(lookup))
        while not(i_sorted[i]) and\
              not(i >= min_pos[unsorted[lookup[i]]] and\
                  i < max_pos[unsorted[lookup[i]]]):          
            # unsroted[swap_index] is in desirable position.
            swap_index = pos[unsorted[lookup[i]]]
            i_sorted[lookup[swap_index]] = True
            # The unsorted[swap_index] will be sorted in the correct position.
            # advance pos[unsorted[i]] so that it is in a potential unsorted position.
            pos[unsorted[lookup[i]]] += 1
            if unsorted[swap_index] != unsorted[lookup[i]]:
                if(debug):
                    print("Swapping {} with {}".format(i, swap_index))
                # Swap because it's not overlapping on position with swap_index
                n_swap += 1
                retval.append(i)
                retval.append(swap_index)
                (lookup[i], lookup[swap_index]) = (lookup[swap_index], lookup[i])
            # else: unsorted[swap_index] is in the sorted position, don't swap.
            if(debug):
                print("i_sorted:\t{}".format(i_sorted))
                print("pos:\t\t{}".format(pos))
                print("min_pos:\t{}".format(min_pos))
                print("max_pos:\t{}".format(max_pos))
                print("retval:\t\t{}".format(retval))
                print("lookup:\t\t{}".format(lookup))  
        # unsorted[i] should now be in its desirable position.
        i_sorted[i] = True
        pos[unsorted[lookup[i]]] += 1
    #print("Total swaps: {}".format(n_swap))
    return (retval, lookup, n_swap)
            
def initialize(counts):
    """
        Returns (min_pos, max_pos)
    """
    min_pos = [0]*len(counts)
    max_pos = counts[:]
    for i in range(len(counts)):
        max_pos[i] += min_pos[i]
        if i+1 in range(len(counts)):
            min_pos[i+1] = max_pos[i]
    return (min_pos, max_pos)
def get_pos(counts):
    retval = counts[:]
    for i in range(1,len(counts)):
        retval[i] += retval[i-1]
    return retval

def rearrange(unsorted, plan):
    for i in range(0,len(plan),2):
        swap(unsorted, plan[i], plan[i+1])
    return unsorted
##def bucket_swap_sort(unsorted, min_num, max_num):
##    bucket_length = 1+(max_num - min_num)
##    index_bucket = [-1]*bucket_length
##    swapped = 0
##    for i in range(1,len(unsorted)):
##        print(">>i = {}".format(i))
##        print(">>unsorted: {}".format(unsorted))
##        print(">>pos: {}".format(index_bucket))
##        if i == 1:
##            index_bucket[unsorted[0]-min_num] = 0
##        
##        
##    print("Done sorting: {}".format(unsorted))
##    print("Total swap: {}".format(swapped))
        
if __name__ == "__main__":
##    total_rearrange_swaps = 0
##    tests = [[1, 1, 2, 1, 3, 0, 0, 3, 2],
##             [0, 0, 1, 1, 1, 2, 2, 3, 3],
##             [3, 3, 2, 2, 1, 1, 1, 0, 0],
##             [3, 0, 0, 1, 1, 1, 2, 2, 3],
##             [2, 2, 3, 3, 0, 0, 1, 1, 1],
##             [1, 2, 2, 3, 3, 0, 0, 1, 1]]
##    for test in tests:
##        print("====== TESTING: {} ======".format(test))
##        unsorted = test
##        solution = sorted(unsorted)
####        bucket_swap_sort(unsorted, [0,2,5,7])
##        (rearrange_plan, num_swaps) = get_rearrange_plan(unsorted, [2,3,2,2])
##        assert unsorted == test, "rearrange_plan should not change list's composition."
##        print("re-plan: {}".format(rearrange_plan))
##        print("swaps_m: {}".format(num_swaps))
##        unarranged = test
##        rearrange(unarranged, rearrange_plan)
##        print("arranged:{}".format(unarranged))
##        assert unarranged == solution
##        bucket_swap_sort(unsorted, [2,3,2,2])      
##        print("result:  {}".format(unsorted))
##        print("solution:{}".format(solution))
##        assert unsorted == solution
##        
##    tests = [[1, 1, 2, 1, 3, 3, 1, 3, 2],
##             [1, 2, 1, 3, 1, 3, 1, 3, 2],
##             [2, 1, 3, 1, 2, 3, 1, 3, 1],
##             [1, 1, 1, 1, 2, 2, 3, 3, 3],
##             [3, 3, 3, 2, 2, 1, 1, 1, 1],
##             ]
##    count = [0,4,2,3]
##    for test in tests:
##        print("====== TESTING: {} ======".format(test))
##        unsorted = test
##        solution = sorted(unsorted)
####        bucket_swap_sort(unsorted, [0,2,5,7])
##        (rearrange_plan, num_swaps) = get_rearrange_plan(unsorted, count)
##        assert unsorted == test, "rearrange_plan should not change list's composition."
##        print("re-plan: {}".format(rearrange_plan))
##        print("swaps_m: {}".format(num_swaps))
##        unarranged = test
##        rearrange(unarranged, rearrange_plan)
##        print("arranged:{}".format(unarranged))
##        assert unarranged == solution
##        bucket_swap_sort(unsorted, count)      
##        print("result:  {}".format(unsorted))
##        print("solution:{}".format(solution))
##        assert unsorted == solution
    def rearrange_test(unsorted, count):
        print("====== TESTING: {} ======".format(unsorted))
        unsorted_cpy = unsorted[:]
        solution = sorted(unsorted)
        (plan, lookup, num_swaps) = get_rearrange_plan(unsorted_cpy, count, True)
        assert unsorted_cpy == unsorted, "rearrange_plan should not change list's composition."
        print("plan:       {}".format(plan))
        print("num_swaps:  {}".format(num_swaps))
        print("lookup:     {}".format(lookup))
        rearrange(unsorted_cpy, plan)
        print("arranged:   {}".format(unsorted_cpy))
        assert unsorted_cpy == solution, "Rearrangement does not yield solution"

    rearrange_test([2, 0, 2, 1, 1], [1, 2, 2])
    def count_arr(array):
        type_count = []
        for i in range(len(array)):
            # Reserve enough capacity for size
            for _ in range(len(type_count), array[i]+1):
                type_count.append(0)
            type_count[array[i]] += 1
        return type_count
##    for test in tests:
##        assert count_arr(test) == count

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
##    assert arr_increment(test,4-1) == False
##    assert test == [0]*4
    def print_now_buf(buffer):
        BUFFER = 0
        COUNT = 1
        print(buffer[BUFFER][:-1])
        buffer[BUFFER] = ""
        buffer[COUNT] = 0
    def print_buf(content, buffer, print_threshold = 10):
        BUFFER = 0
        COUNT = 1
        CONTENT_HALF = 2

        buffer[BUFFER] += content
        buffer[BUFFER] += "\n"
        buffer[COUNT] += 1
        if buffer[COUNT] == print_threshold//2:
            print(buffer[CONTENT_HALF][:-1])
        if buffer[COUNT] >= print_threshold:
            print(buffer[CONTENT_HALF][:-1])
            print_now_buf(buffer)
            return True
        return False
    def flush_content_half(buffer):
        buffer[2] = buffer[0]
        buffer[0] = ""
        
##    import traceback
    def brute_force_test(max_length):
        MAX_THRESHOLD = 15
        test_array = []
##        print_buffer = ["", 0, ""]
        print_threshold = 0
##        printb = lambda content: print_buf(content, print_buffer, print_threshold)
##        printb_now = lambda: print_now_buf(print_buffer)
        p_count = 0
##        flush_status = lambda: flush_content_half(print_buffer)
        for length in range(max_length+1):
            print("\n\n\n\n\n======== BRUTE-FORCING LENGTH {} ========".format(length))
##            printb_now()
            if length > 0:
                test_array.append(0)
            L = length-1
            # brute force test while the the test_array can be incremented.
            dowhile = True
            while dowhile:
##                flush_status()
                count = count_arr(test_array)
                do_print = (p_count >> print_threshold) > 1
                if do_print:
                    print("test:     {} << {} >>".format(test_array, len(test_array)))
                    print("count:    {}".format(count))
                unsorted = test_array[:]
                try:
                    (plan, lookup, swap_num) = get_rearrange_plan(unsorted, count)
                except Exception as e:
                    printb_now()
                    print("Exception occurred:")
##                    print(traceback.format_exc())
                    print(e)
                    print("Test case: {}, count: {}".format(test_array, count))
                    return
##                assert unsorted == test_array, "get_rearrange_plan should not rearrange unsorted"
                solution = sorted(test_array)
                same_solution = (rearrange(unsorted, plan) == solution)
                if do_print:
                    print("solution: {}".format(solution))
                    print("rearrange:{}".format(unsorted))
                    print("")
                if not same_solution:
                    print("test:     {} << {} >>".format(test_array, len(test_array)))
                    print("count:    {}".format(count))
                    print("solution: {}".format(solution))
                    print("rearrange:{}".format(unsorted))
                    print("")
                assert same_solution, "rearrange spits different values"
                p_count = 0 if do_print else p_count + 1
                dowhile = arr_increment(test_array, L)
            if print_threshold < MAX_THRESHOLD:
                print_threshold = max(MAX_THRESHOLD, print_threshold+1)
    brute_force_test(90)
                
