#include <cassert>
#include <tuple>
#include <iostream>

#include "sudoku.h"

namespace validate {

/*!
 * Size of the Sudoku table LENGTH x LENGTH
 */
const int LENGTH = 9;

/*!
 * vector with result of the three segment types
 */
auto validations = std::vector<std::bitset<LENGTH>>(3);

/*!
 * \brief enum segment to represent the search by locals validations
 */
enum segment {
    ROW,        ///< This is for search in the row
    COLUMN,     ///< This is for search in the column
    SQUARE      ///< This is for search in the square
};

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

bool CheckSudokuVector(const std::vector<int> &chunk) {
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

std::vector<int> CreateSquareSegment(
        const std::vector<std::vector<int>> &matrixSudoku,
        int position) {
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

std::vector<int> CreateColumnSegment(
        const std::vector<std::vector<int>> &matrixSudoku,
        int position) {
    assert(position >= 0 && position < LENGTH && "position has been between 0 and 8");

    auto columnVector = std::vector<int>{};
    for (std::size_t i = 0; i < matrixSudoku[position].size(); ++i) {
        columnVector.push_back(matrixSudoku[i][position]);
    }

    return columnVector;
}

std::vector<int> CreateLineSegment(
        const std::vector<std::vector<int>> &matrixSudoku,
        int position) {
    assert(position >= 0 && position < LENGTH && "position has been between 0 and 8 inclusive");

    return matrixSudoku[position];
}

bool CheckResult() {
    bool result = validations.at(segment::SQUARE).all();
    return result;/*validations.at(segment::ROW).all() &&
           validations.at(segment::COLUMN).all() &&
           validations.at(segment::SQUARE).all();*/
}

void* ThreadRun(void* threadParam) {
    auto validation = *reinterpret_cast<std::function<void ()>*>(threadParam);
    validation();
    return nullptr;
}

bool ParallelRun(const std::vector<std::vector<int>> &matrixSudoku)
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
        std::function<void ()> thRowArgs = [segmentIndex, &matrixSudoku]() {
            auto row = CreateLineSegment(matrixSudoku, segmentIndex);
            if (CheckSudokuVector(row)) {
                validations[segment::ROW].set(segmentIndex);
            }
        };

        std::function<void ()> thColumnArgs = [segmentIndex, &matrixSudoku]() {
            auto column = CreateLineSegment(matrixSudoku, segmentIndex);
            if (CheckSudokuVector(column)) {
                validations[segment::COLUMN].set(segmentIndex);
            }
        };

        std::function<void ()> thSquareArgs = [segmentIndex, &matrixSudoku]() {
            auto square = CreateLineSegment(matrixSudoku, segmentIndex);
            bool mycheck = CheckSudokuVector(square);
            if (mycheck) {
                std::cout << "Set 'square' vector on " << segmentIndex << " position\n";
                validations[segment::SQUARE].set(segmentIndex);
                std::cout << "Test 'square' vector on " << segmentIndex
                          << " position is test: " << validations[segment::SQUARE].test(segmentIndex)
                          << "\n";
            }
        };

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

    return CheckResult();
}

}
