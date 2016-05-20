
#include "sudoku.h"
#include "gtest/gtest.h"

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
TEST_F(SudokuTest, RangeValid)
{
    auto lineOfSudoku = std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9};
    auto result = validator->CheckRange(lineOfSudoku);
    EXPECT_TRUE(result);
}

TEST_F(SudokuTest, RangeInvalid)
{
    auto lineOfSudoku = std::vector<int>{1, 2, 12, 4, 5, 6, 7, 8, 9};
    auto result = validator->CheckRange(lineOfSudoku);
    EXPECT_FALSE(result);
    lineOfSudoku = std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 0};
    result = validator->CheckRange(lineOfSudoku);
    EXPECT_FALSE(result);
}

TEST_F(SudokuTest, VectorValid)
{
    auto lineOfSudoku = std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9};
    auto result = validator->CheckVector(lineOfSudoku);
    EXPECT_TRUE(result);
}

TEST_F(SudokuTest, VectorInvalid)
{
    auto lineOfSudoku = std::vector<int>{4, 5, 6, 1, 8, 9, 1, 2, 3};
    auto result = validator->CheckVector(lineOfSudoku);
    EXPECT_FALSE(result);
}

/*!
 * These tests extract each area (i.e. row, column and square) as a vector
 */
TEST_F(SudokuTest, GenerateFifthColumnAsVector)
{
    auto expectedVector = std::vector<int>{6, 9, 3, 7, 1, 4, 8, 2, 5};
    auto resultVector = validator->CreateSegment(sudoku::COLUMN, 5);
    ASSERT_EQ(expectedVector, resultVector);
}

TEST_F(SudokuTest, GenerateEigthLineAsVector)
{
    auto expectedVector = std::vector<int>{9, 1, 2, 3, 4, 5, 6, 7, 8};
    auto resultVector = validator->CreateSegment(sudoku::ROW, 8);
    ASSERT_EQ(expectedVector, resultVector);
}

/*!
 * These tests check the logical for parallel tests
 */
TEST_F(SudokuTest, ValidateTheSudokuTable)
{
    bool result = validator->CheckResult();
    ASSERT_TRUE(result);
}

TEST_F(SudokuTest, InvalidateTheSudokuTable)
{
    validator->SetMatrix({{1, 2, 3, 4, 5, 3, 7, 8, 9},
                          {4, 5, 6, 7, 8, 9, 1, 2, 3},
                          {7, 8, 9, 1, 2, 3, 4, 5, 6},
                          {2, 3, 4, 5, 6, 7, 8, 9, 1},
                          {5, 6, 7, 8, 9, 1, 2, 3, 4},
                          {8, 9, 1, 2, 3, 4, 5, 6, 7},
                          {3, 4, 5, 6, 7, 8, 9, 8, 2},
                          {6, 7, 8, 9, 5, 2, 3, 4, 5},
                          {9, 1, 2, 3, 4, 5, 6, 7, 8}});
    bool result = validator->CheckResult();
    ASSERT_FALSE(result);
}

/*!
 * Test to extract each square as vector of the sudoku table
 */
class SudokuSquareParserTest :
        public testing::TestWithParam<std::pair<int, std::vector<int>>>
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

TEST_P(SudokuSquareParserTest, GenerateAllSquareAsVector)
{
    int position;
    std::vector<int> expectedVector;
    std::tie(position, expectedVector) = GetParam();
    auto resultVector = validator->CreateSegment(sudoku::SQUARE, position);
    ASSERT_EQ(expectedVector, resultVector);
}

INSTANTIATE_TEST_CASE_P(
        Parse,
        SudokuSquareParserTest,
        testing::Values(
                std::make_pair(0, std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9}),
                std::make_pair(1, std::vector<int>{4, 5, 6, 7, 8, 9, 1, 2, 3}),
                std::make_pair(2, std::vector<int>{7, 8, 9, 1, 2, 3, 4, 5, 6}),
                std::make_pair(3, std::vector<int>{2, 3, 4, 5, 6, 7, 8, 9, 1}),
                std::make_pair(4, std::vector<int>{5, 6, 7, 8, 9, 1, 2, 3, 4}),
                std::make_pair(5, std::vector<int>{8, 9, 1, 2, 3, 4, 5, 6, 7}),
                std::make_pair(6, std::vector<int>{3, 4, 5, 6, 7, 8, 9, 1, 2}),
                std::make_pair(7, std::vector<int>{6, 7, 8, 9, 1, 2, 3, 4, 5}),
                std::make_pair(8, std::vector<int>{9, 1, 2, 3, 4, 5, 6, 7, 8})));
