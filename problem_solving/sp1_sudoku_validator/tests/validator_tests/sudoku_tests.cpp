
#include "sudoku.h"
#include "gtest/gtest.h"


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