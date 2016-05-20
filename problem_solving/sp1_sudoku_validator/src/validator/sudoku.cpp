#include <cassert>
#include <tuple>
#include <iostream>

#include "sudoku.h"

/******************** Constant ********************/
/*!
 * Size of the Sudoku table LENGTH x LENGTH
 */
const int LENGTH = 9;

/*!
 * \brief vector with position (x,y) of the first element foreach square
 */
const auto squareLimits = std::vector<std::pair<int, int>> {
        std::make_pair(0, 0), /**< first square */
        std::make_pair(0, 3), /**< second square */
        std::make_pair(0, 6), /**< third square */
        std::make_pair(3, 0), /**< fourth square */
        std::make_pair(3, 3), /**< fifth square */
        std::make_pair(3, 6), /**< sixth square */
        std::make_pair(6, 0), /**< seventh square */
        std::make_pair(6, 3), /**< eighth square */
        std::make_pair(6, 6), /**< ninth square */
};
/***************************************************/

static void* ThreadRun(void* threadParam)
{
    auto thParams =
            *reinterpret_cast<std::tuple<sudoku::segment, int, sudoku::Validator*>*>(threadParam);

    sudoku::segment segmentType;
    int index;
    sudoku::Validator *validator;
    std::tie(segmentType, index, validator) = thParams;

    std::vector<int> segment;
    switch (segmentType) {
        case sudoku::ROW:
            segment = validator->CreateLineSegment(index);
            break;
        case sudoku::COLUMN:
            segment = validator->CreateColumnSegment(index);
            break;
        case sudoku::SQUARE:
            segment = validator->CreateLineSegment(index);
    }

    if (validator->CheckVector(segment)) {
        validator->SetSegmentAsValid(segmentType, index);
    }

    return nullptr;
}

void ParallelRun(sudoku::Validator* validator)
{
    /* Array of threads */
    const int NUM_THREADS = 27;     ///< number of threads for check each segment
    pthread_t workers[NUM_THREADS];

    /*!
     * Create all threads
     * Using pthread create
     */
    int segmentIndex = 0;
    std::cout << "\n\n";
    for (int i = 0; i < NUM_THREADS; i += 3, ++segmentIndex) {

        /*!
         * Declare a lambda arguments to pass to the function that the thread executes
         * This lambda has the logical to validate the segment in position
         * passed by "segmentIndex" variable
         */
        auto thRowArgs = std::make_tuple(sudoku::ROW, segmentIndex, validator);
        auto thColumnArgs = std::make_tuple(sudoku::COLUMN, segmentIndex, validator);
        auto thSquareArgs = std::make_tuple(sudoku::SQUARE, segmentIndex, validator);

        /*!
         * create the threads for check segments passing lambda
         * earlier created as parameter for thread
         */
        pthread_create(&workers[i], nullptr, ThreadRun, &thRowArgs);
        pthread_create(&workers[i+1], nullptr, ThreadRun, &thColumnArgs);
        pthread_create(&workers[i+2], nullptr, ThreadRun, &thSquareArgs);
    }

    /*!
     * Wait each thread finish
     */
    for (int i = 0; i < NUM_THREADS; i += 3) {
        pthread_join(workers[i], nullptr);
        pthread_join(workers[i+1], nullptr);
        pthread_join(workers[i+2], nullptr);
    }
}

namespace sudoku {

Validator::Validator(std::vector<std::vector<int>> matrixSudokuTable) :
    validations{std::vector<std::bitset<9>>(3)},
    matrixSudoku{matrixSudokuTable}
{ }

bool Validator::CheckVector(const std::vector<int> &chunk) const
{
    for (std::size_t i = 0; i < (chunk.size() - 1); ++i) {
        int current = chunk[i];
        for (std::size_t j = i + 1; j < chunk.size(); ++j) {
            if (current == chunk[j]) {
                return false;
            }
        }
    }
    return true;
}

std::vector<int> Validator::CreateSquareSegment(int position) const
{
    assert(position >= 0 && position < LENGTH && "position has been between 1 and 9 inclusive");

    int x, y;
    std::tie(x, y) = squareLimits[position];
    auto squareVector = std::vector<int>{};
    for (int i = x; i < (x + 3); ++i) {
        for (int j = y; j < (y + 3); ++j) {
            squareVector.push_back(matrixSudoku[i][j]);
        }
    }

    return squareVector;
}

std::vector<int> Validator::CreateColumnSegment(int position) const
{
    assert(position >= 0 && position < LENGTH && "position has been between 0 and 8");

    auto columnVector = std::vector<int>{};
    for (std::size_t i = 0; i < matrixSudoku[position].size(); ++i) {
        columnVector.push_back(matrixSudoku[i][position]);
    }

    return columnVector;
}

std::vector<int> Validator::CreateLineSegment(int position) const {
    assert(position >= 0 && position < LENGTH && "position has been between 0 and 8 inclusive");

    return matrixSudoku[position];
}

bool Validator::CheckResult() {
    ParallelRun(this);

    bool result = validations[segment::SQUARE].all();
    return result;/*validations[segment::ROW].all() &&
           validations[segment::COLUMN].all() &&
           validations[segment::SQUARE].all();*/
}

void Validator::SetSegmentAsValid(segment segmentType, int index)
{
    validations[segmentType].set(index);
}

}
