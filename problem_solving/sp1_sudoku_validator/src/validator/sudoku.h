#ifndef VALIDATE_SUDOKU_H
#define VALIDATE_SUDOKU_H

#include <bitset>
#include <vector>

namespace sudoku {
/*!
 * \brief enum segment to represent the search by locals validations
 */
enum segment {
    ROW,        ///< This is for search in the row
    COLUMN,     ///< This is for search in the column
    SQUARE      ///< This is for search in the square
};

/*!
 * \brief Class to validate table of the sudoku game
 */
class Validator {
public:
    /*!
     * Constructor of the Validator
     *
     * \param std::vector<std::vector<int>> matrixSudokuTable: Matrix representing
     *                                                         the Sudoku Table
     */
    Validator(std::vector<std::vector<int>> matrixSudokuTable);

    /*!
     * \brief check if vector is valid for sudoku game
     *
     * \param const std::vector<int> &chunk: reference of the vector to validate,
     *                                       this vector isn't changed
     * \return bool: true if vector is valid;
     *               false otherwise
     */
    bool CheckVector(const std::vector<int> &chunk) const;

    /*!
    * \brief check if the all vectors resultant are true
    *
    * \return bool: true if all vectors has been set
    *               false otherwise
    */
    bool CheckResult();

    /*!
     * \brief check if the elements are between 1 and 9
     *
     * \param const std::vector<int> &vector: a vector constant to check
     * \return bool: true if all elements are between 1 and 9
     *               false otherwise
     */
    bool CheckRange(const std::vector<int> &vector) const;

    /*!
     * \brief Create a vector of the segment in the passed position
     *
     * \param segment: Is the segment type, this can assume
     *                 three values of the enum segment
     *                 ex. ROW, COLUMN or SQUARE
     * \param int index: The position of element extract the
     *                   segment in the sudoku matrix table
     * \return std::vector<int>: A copy of the segment
     *                           extracted as a std::vector<int>
     */
    std::vector<int> CreateSegment(segment segmentType, int index) const;

    /*!
     * \brief Redefine the matrix table
     *
     * \param std::vector<std::vector<int>> newSudokuTable: the new matrix table to redefine
     */
    void SetMatrix(std::vector<std::vector<int>> newSudokuTable);

    /*!
     * \brief Define segment passed as parameter in the position as valid
     *
     * \param segment segmentType: Segment to validate
     * \param int index: Index of segment to validate
     */
    void SetSegmentAsValid(segment segmentType, int index);

private:
    /*!
     * \brief take the vector square of the matrix passed in constructor
     *
     * \param int: square position between 0 and 8
     * \return std::vector<int>, the square in vector format
     */
    std::vector<int> CreateSquareSegment(int position) const;

    /*!
     * \brief take the vector column of the matrix passed in constructor
     *
     * \param int: column position
     * \return std::vector<int>, the column in vector format
     */
    std::vector<int> CreateColumnSegment(int position) const;

    /*!
     * \brief take the vector row of the matrix passed in constructor
     *
     * \param int: line position
     * \return std::vector<int>, the line in vector format
     */
    std::vector<int> CreateLineSegment(int position) const;

private:
    /*!
    * vector with result of the three segment types
    */
    std::vector<std::bitset<9>> validations;
    /*!
    * Matrix to validate
    */
    std::vector<std::vector<int>> matrixSudoku;
};

}
#endif
