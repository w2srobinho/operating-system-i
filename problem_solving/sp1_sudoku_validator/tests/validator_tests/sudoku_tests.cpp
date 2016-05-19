
#include "sudoku.h"
#include "gtest/gtest.h"

#include <bitset>
#include <memory>

class SudokuTest : public testing::Test
{
protected:
    std::vector<std::vector<int>> sudokuMatrixToTest;
    std::unique_ptr<sudoku::Validator> validator;

public:
    void SetUp() override
    {
        sudokuMatrixToTest = std::vector<std::vector<int>>{{1, 2, 3, 4, 5, 6, 7, 8, 9},
                                                           {4, 5, 6, 7, 8, 9, 1, 2, 3},
                                                           {7, 8, 9, 1, 2, 3, 4, 5, 6},
                                                           {2, 3, 4, 5, 6, 7, 8, 9, 1},
                                                           {5, 6, 7, 8, 9, 1, 2, 3, 4},
                                                           {8, 9, 1, 2, 3, 4, 5, 6, 7},
                                                           {3, 4, 5, 6, 7, 8, 9, 1, 2},
                                                           {6, 7, 8, 9, 1, 2, 3, 4, 5},
                                                           {9, 1, 2, 3, 4, 5, 6, 7, 8}};
        validator = std::unique_ptr<sudoku::Validator>{new sudoku::Validator(sudokuMatrixToTest)};
    }
};

/*!
 * These tests check the vector is valid in the sudoku rules
 */
TEST_F(SudokuTest, VectorValid)
{
    auto lineOfSudoku = std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9};
    auto result = validator->CheckVector(lineOfSudoku);
    EXPECT_TRUE(result);
}

TEST_F(SudokuTest, VectorUnvalid)
{
    auto lineOfSudoku = std::vector<int>{4, 5, 6, 1, 8, 9, 1, 2, 3};
    auto result = validator->CheckVector(lineOfSudoku);
    EXPECT_FALSE(result);
}

/*!
 * These tests extract each area (i.e. row, column and square) as a vector
 */
TEST_F(SudokuTest, GenerateAllSquareAsVector)
{
    auto expectedVectors = std::vector<std::vector<int>>{{1, 2, 3, 4, 5, 6, 7, 8, 9},
                                                         {4, 5, 6, 7, 8, 9, 1, 2, 3},
                                                         {7, 8, 9, 1, 2, 3, 4, 5, 6},
                                                         {2, 3, 4, 5, 6, 7, 8, 9, 1},
                                                         {5, 6, 7, 8, 9, 1, 2, 3, 4},
                                                         {8, 9, 1, 2, 3, 4, 5, 6, 7},
                                                         {3, 4, 5, 6, 7, 8, 9, 1, 2},
                                                         {6, 7, 8, 9, 1, 2, 3, 4, 5},
                                                         {9, 1, 2, 3, 4, 5, 6, 7, 8}};

    for (auto i = 0u; i < expectedVectors.size(); ++i) {
        auto resultVector = validator->CreateSquareSegment(i);
        ASSERT_EQ(expectedVectors[i], resultVector);
    }
}

TEST_F(SudokuTest, GenerateFifthColumnAsVector)
{
    auto expectedVector = std::vector<int>{6, 9, 3, 7, 1, 4, 8, 2, 5};
    auto resultVector = validator->CreateColumnSegment(5);
    ASSERT_EQ(expectedVector, resultVector);
}

TEST_F(SudokuTest, GenerateEigthLineAsVector)
{
    auto expectedVector = std::vector<int>{9, 1, 2, 3, 4, 5, 6, 7, 8};
    auto resultVector = validator->CreateLineSegment(8);
    ASSERT_EQ(expectedVector, resultVector);
}

/*!
 * These tests check the logical for parallel tests
 */
TEST_F(SudokuTest, ValidateTheSudokuTable)
{
    bool result = validator->ParallelRun(sudokuMatrixToTest);
    ASSERT_TRUE(result);
}
