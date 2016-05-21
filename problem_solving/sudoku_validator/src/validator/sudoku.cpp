#include <pthread.h>
#include <cassert>
#include <tuple>

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

/*!
 * Declare a struct as a tuple to pass params to be executed in the thread
 */
typedef struct {
    sudoku::segment segmentType;    ///< The segment type can be ROW, COLUMN or SQUARE
    int index;                      ///< The position in segment vector
    sudoku::Validator* p_validator; ///< the pointer to object that has the logical to validate
} m_tuple;

/*!
 * Method to run each thread and receive the logical
 * for validate the segment passed as parameter
 */
void* ThreadRun(void* threadParam)
{
    /*!
     * Get the parameters passed to function
     * does a cast from void* to m_tuple*
     * and after dereference to get m_tuple as value in thParams
     */
    auto thParams = *reinterpret_cast<m_tuple*>(threadParam);

    std::vector<int> segment = thParams.p_validator->CreateSegment(
            thParams.segmentType,
            thParams.index);

    if (thParams.p_validator->CheckVector(segment)) {
        thParams.p_validator->SetSegmentAsValid(thParams.segmentType, thParams.index);
    }

    pthread_exit(nullptr);
}

/*!
 * This function does the parallelization
 * for check if sudoku matrix table is valid
 */
void ParallelRun(sudoku::Validator* validator)
{
    /* Array of threads */
    const int NUM_THREADS = 27;     ///< number of threads for check each segment
    pthread_t workers[NUM_THREADS];

    /*!
     * Create all threads
     * Using pthread create
     */
    auto thArgs = std::vector<m_tuple>{};
    for (int j = 0; j < 3; ++j) {
        for (int i = 0; i < LENGTH; ++i) {
            /*!
             * Declare arguments to pass to the function that the thread executes
             */
            m_tuple param = {static_cast<sudoku::segment>(j), i, validator};
            thArgs.push_back(std::move(param));
            /*!
             * create the threads for check segments passing lambda
             * earlier created as parameter for thread
             */
            int thIndex = (j * LENGTH) + i;
            pthread_create(&workers[thIndex], nullptr, ThreadRun, &thArgs[thIndex]);
        }
    }
    /*!
     * Wait each thread finish
     */
    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(workers[i], nullptr);
    }
}

namespace sudoku {

Validator::~Validator() {
    pthread_mutex_destroy(&result_mtx);
}

Validator::Validator(std::vector<std::vector<int>> matrixSudokuTable) :
    validations{std::vector<std::bitset<9>>(3)},
    matrixSudoku{matrixSudokuTable}
{
    pthread_mutex_init(&result_mtx, nullptr);
}

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

std::vector<int> Validator::CreateSegment(segment segmentType, int index) const {
    if (segmentType == ROW) {
        return CreateLineSegment(index);
    }

    if (segmentType == COLUMN) {
        return CreateColumnSegment(index);
    }

    return CreateSquareSegment(index);
}

bool Validator::CheckResult() {
    ParallelRun(this);

    bool result = validations[ROW].all() &&
                  validations[COLUMN].all() &&
                  validations[SQUARE].all();
    return result;
}

void Validator::SetMatrix(std::vector<std::vector<int>> newSudokuTable) {
    for (auto &bset : validations) {
        bset.reset();
    }
    assert(validations[ROW].none() && "ROW segment should be all unset");
    assert(validations[COLUMN].none() && "COLUMN segment should be all unset");
    assert(validations[SQUARE].none() && "SQUARE segment should be all unset");
    matrixSudoku = newSudokuTable;
}

void Validator::SetSegmentAsValid(segment segmentType, int index)
{
    pthread_mutex_lock(&result_mtx);
    validations[segmentType].set(index);
    pthread_mutex_unlock(&result_mtx);
}

bool Validator::CheckRange(const std::vector<int> &vector) const {
    for (auto it: vector) {
        if (it > LENGTH || it < 1) {
            return false;
        }
    }
    return true;
}

}
