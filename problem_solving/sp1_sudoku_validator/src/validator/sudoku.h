#ifndef VALIDATE_SUDOKU_H
#define VALIDATE_SUDOKU_H

#include <vector>

namespace validate {
    /*!
     * \brief sudoku search type
     */
    enum segment {
        row,
        column,
        square
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
}

#endif