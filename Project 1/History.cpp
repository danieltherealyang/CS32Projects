#include "History.h"

History::History(int nRows, int nCols) {
    m_rows = nRows;
    m_cols = nCols;
    grid = new int*[nRows];
    for (int i = 0; i < nRows; i++) {
        grid[i] = new int[nCols];
        for (int j = 0; j < nCols; j++) {
            grid[i][j] = 0;
        }
    }
}

History::~History() {
    for (int i = 0; i < m_rows; i++) {
        delete [] grid[i];
    }

    delete [] grid;
}

bool History::record(int r, int c) {
    if (r < 1 || r > m_rows)
        return false;
    if (c < 1 || c > m_cols)
        return false;
    grid[r-1][c-1] += 1;
    return true;
}

void History::display() const {
    clearScreen();
    for (int i = 0; i < m_rows; i++) {
        for (int j = 0; j < m_cols; j++) {
            if (grid[i][j] == 0)
                cout << '.';
            else if (grid[i][j] > 26)
                cout << 'Z';
            else {
                char output = 'A' + grid[i][j] - 1;
                cout << output;
            }
        }
        cout << endl;
    }
    cout << endl;
}