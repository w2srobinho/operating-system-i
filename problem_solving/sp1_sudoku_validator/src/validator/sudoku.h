#ifndef VALIDATE_SUDOKU_H
#define VALIDATE_SUDOKU_H

#include <bitset>
#include <functional>
#include <vector>

namespace validate {
    /*!
     * \brief sudoku search type
     */

    /*!
     * Size of the Sudoku table LENGTH x LENGTH
     */
    constexpr int LENGTH = 9;

    /*!
     * vector with result of the three segment types
     */
    auto validations = std::vector<std::bitset<LENGTH>>(3);

    /*!
     * \brief enum segment to represent the search by locals validations
     */
    enum class segment {
        ROW,        ///< This is for search in the row
        COLUMN,     ///< This is for search in the column
        SQUARE      ///< This is for search in the square
    };

    /*!
     * \brief check if vector is valid for sudoku game
     *
     * \param const std::vector<int>: reference of the vector to validate,
     *                                this vector isn't changed
     * \return bool: true if vector is valid;
     *               false otherwise
     */
    bool CheckSudokuVector(const std::vector<int> &chunk);

    /*!
     * \brief create a vector with square elements of the passed position
     *
     * \param const std::vector<std::vector<int>>: matrix representing the sudoku table,
     *                                             this table isn't changed
     * \param int: square position between 0 and 8
     * \return std::vector<int>, the square in vector format
     */
    std::vector<int> CreateSquareSegment(
        const std::vector<std::vector<int>> &matrixSudoku,
        int position);

    /*!
     * \brief create a vector with column elements of the passed position
     *
     * \param const std::vector<std::vector<int>>: matrix representing the sudoku table,
     *                                             this table isn't changed
     * \param int: column position
     * \return std::vector<int>, the column in vector format
     */
    std::vector<int> CreateColumnSegment(
            const std::vector<std::vector<int>> &matrixSudoku,
            int position);

    /*!
     * \brief create a vector with line elements of the passed position
     *
     * \param const std::vector<std::vector<int>>: matrix representing the sudoku table,
     *                                             this table isn't changed
     * \param int: line position
     * \return std::vector<int>, the line in vector format
     */
    std::vector<int> CreateLineSegment(
            const std::vector<std::vector<int>> &matrixSudoku,
            int position);

    void *ThreadRun(void* threadParam) {
        auto validation = *reinterpret_cast<std::function<void ()>*>(threadParam);
        validation();
        return nullptr;
    }

    void parallelValidation(const std::vector<std::vector<int>> &matrixSudoku)
    {
        /* Array of threads */
        const int NUM_THREADS = 27;     ///< number of threads for check each segment
        pthread_t workers[NUM_THREADS];

        /*!
         * Create all threads
         * Using pthread create
         */
        int segmentIndex = 0;
        for (int i = 0; i < LENGTH; i = i + 3, ++segmentIndex) {
            /*!
             * Declare a lambda arguments to pass to the function that the thread executes
             */
            std::function<void ()> thRowArgs = [&segmentIndex, &matrixSudoku]() {
                auto row = CreateLineSegment(matrixSudoku, segmentIndex);
                if (CheckSudokuVector(matrixSudoku[segment::ROW)) {
                    
                }
            };

            std::function<void ()> thColumnArgs = [&segmentIndex, &matrixSudoku]() {
                auto row = CreateLineSegment(matrixSudoku, segmentIndex);
                // validate
            };

            std::function<void ()> thSquareArgs = [&segmentIndex, &matrixSudoku]() {
                auto row = CreateLineSegment(matrixSudoku, segmentIndex);
                // validate
            };

            /*!
             * create the threads for check segments
             */
            pthread_create(&workers[i], nullptr, ThreadRun, &thRowArgs);
            pthread_create(&workers[i+1], nullptr, ThreadRun, &thColumnArgs);
            pthread_create(&workers[i+2], nullptr, ThreadRun, &thSquareArgs);
        }

        for (int i = 0; i < LENGTH; i = i + 3) {
            pthread_join(workers[i], nullptr);
            pthread_join(workers[i+1], nullptr);
            pthread_join(workers[i+2], nullptr);
        }
    }

    bool sudokuValidator(const std::vector<std::vector<int>> &matrixSudoku)
    {

    }
}

#endif