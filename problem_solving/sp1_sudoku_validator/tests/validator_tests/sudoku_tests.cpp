
#include "sudoku.h"
#include "gtest/gtest.h"

#include <bitset>

/*!
 * These tests check the vector is valid in the sudoku rules
 */
TEST(SudokuTest, VectorValid)
{
    auto lineOfSudoku = std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9};
    auto result = validate::CheckSudokuVector(lineOfSudoku);
    EXPECT_TRUE(result);
}

TEST(SudokuTest, VectorUnvalid)
{
    auto lineOfSudoku = std::vector<int>{4, 5, 6, 1, 8, 9, 1, 2, 3};
    auto result = validate::CheckSudokuVector(lineOfSudoku);
    EXPECT_FALSE(result);
}

/*!
 * These tests extract each area (i.e. row, column and square) as a vector
 */
auto sudokuMatrixToTest = std::vector<std::vector<int>>{{1, 2, 3, 4, 5, 6, 7, 8, 9},
                                                        {4, 5, 6, 7, 8, 9, 1, 2, 3},
                                                        {7, 8, 9, 1, 2, 3, 4, 5, 6},
                                                        {2, 3, 4, 5, 6, 7, 8, 9, 1},
                                                        {5, 6, 7, 8, 9, 1, 2, 3, 4},
                                                        {8, 9, 1, 2, 3, 4, 5, 6, 7},
                                                        {3, 4, 5, 6, 7, 8, 9, 1, 2},
                                                        {6, 7, 8, 9, 1, 2, 3, 4, 5},
                                                        {9, 1, 2, 3, 4, 5, 6, 7, 8}};
TEST(SudokuTest, GenerateFirstSquareAsVector)
{
    auto expectedVector = std::vector<int>{4, 5, 6, 7, 8, 9, 1, 2, 3};
    auto resultVector = validate::CreateSquareSegment(sudokuMatrixToTest, 1);
    ASSERT_EQ(expectedVector, resultVector);
}

TEST(SudokuTest, GenerateFifthColumnAsVector)
{
    auto expectedVector = std::vector<int>{6, 9, 3, 7, 1, 4, 8, 2, 5};
    auto resultVector = validate::CreateColumnSegment(sudokuMatrixToTest, 5);
    ASSERT_EQ(expectedVector, resultVector);
}

TEST(SudokuTest, GenerateEigthLineAsVector)
{
    auto expectedVector = std::vector<int>{9, 1, 2, 3, 4, 5, 6, 7, 8};
    auto resultVector = validate::CreateLineSegment(sudokuMatrixToTest, 8);
    ASSERT_EQ(expectedVector, resultVector);
}

/*!
 * These tests check the logical for parallel tests
 */
TEST(SudokuTest, ValidateTheSudokuTable)
{
    bool result = validate::ParallelRun(sudokuMatrixToTest);
    ASSERT_TRUE(result);
}

TEST(SudokuTest, DISABLED_BitsetTest)
{
    auto validations = std::vector<std::bitset<9>>(3);
    ASSERT_EQ(3u, validations.size());
    ASSERT_EQ(9u, validations[0].size());
    ASSERT_EQ(9u, validations[1].size());
    ASSERT_EQ(9u, validations[2].size());
    for(auto i = 0u; i < validations[0].size(); ++i) {
        validations[0].set(i);
    }
    ASSERT_TRUE(validations[0].all());
    ASSERT_TRUE(validations[1].none());
    ASSERT_FALSE(validations[2].any());
}