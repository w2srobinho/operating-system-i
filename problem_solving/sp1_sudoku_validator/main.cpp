#include <iostream>
#include <vector>

#include "sudoku.h"

int main()
{
    /********************** Test if the correct table **********************/
    std::vector<std::vector<int>> sudokuMatrixToTest{{1, 2, 3, 4, 5, 6, 7, 8, 9},
                                                     {4, 5, 6, 7, 8, 9, 1, 2, 3},
                                                     {7, 8, 9, 1, 2, 3, 4, 5, 6},
                                                     {2, 3, 4, 5, 6, 7, 8, 9, 1},
                                                     {5, 6, 7, 8, 9, 1, 2, 3, 4},
                                                     {8, 9, 1, 2, 3, 4, 5, 6, 7},
                                                     {3, 4, 5, 6, 7, 8, 9, 1, 2},
                                                     {6, 7, 8, 9, 1, 2, 3, 4, 5},
                                                     {9, 1, 2, 3, 4, 5, 6, 7, 8}};

    sudoku::Validator validator{sudokuMatrixToTest};

    bool test_valid_table = validator.CheckResult();
    if (!test_valid_table) {
        std::cerr << "Error: The test_valid_table should have passed.\n";
        return -1;
    }
    std::cout << "test_valid_table sucessful\n";
    /***********************************************************************/

    /*************** Test invalid table with repeated values ***************/
    validator.SetMatrix({{1, 2, 3, 4, 5, 3, 7, 8, 9},
                         {4, 5, 6, 7, 8, 9, 1, 2, 3},
                         {7, 8, 9, 1, 2, 3, 4, 5, 6},
                         {2, 3, 4, 5, 6, 7, 8, 9, 1},
                         {5, 6, 7, 8, 9, 1, 2, 3, 4},
                         {8, 9, 1, 2, 3, 4, 5, 6, 7},
                         {3, 4, 5, 6, 7, 8, 9, 8, 2},
                         {6, 7, 8, 9, 5, 2, 3, 4, 5},
                         {9, 1, 2, 3, 4, 5, 6, 7, 8}});
    bool test_repeated_numbers_in_table = validator.CheckResult();
    if (test_repeated_numbers_in_table) {
        std::cerr << "Error: The test_repeated_numbers_in_table should have passed.\n";
        return -1;
    }
    std::cout << "test_repeated_numbers_in_table sucessful\n";
    /***********************************************************************/

    /********* Test invalid table with "x" elements x < 1 or x > 9 *********/
    validator.SetMatrix({{1, 2, 3, 4, 5, 3, 7, 8, 9},
                         {4, 5, 6, 7, 12, 9, 1, 2, 3},
                         {7, 8, 9, 1, 2, 3, 4, 5, 6},
                         {2, 3, 4, 5, 6, 7, 8, 9, 1},
                         {5, 6, 7, 8, 9, 1, 2, 3, 4},
                         {8, 9, 1, 2, 3, 4, 5, 6, 7},
                         {3, 4, 5, 6, 7, 8, 9, 8, 2},
                         {6, 7, 8, 9, 5, 2, 3, 4, 5},
                         {9, 0, 2, 3, 4, 5, 6, 7, 8}});
    bool test_range_out_element = validator.CheckResult();
    if (test_range_out_element) {
        std::cerr << "Error: The test_range_out_element should have passed.\n";
        return -1;
    }
    std::cout << "test_range_out_element sucessful\n";
    /***********************************************************************/

    return 0;
};
