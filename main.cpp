#include <iostream>

struct IntArray {
  void add(int i);
  int get(size_t id) const noexcept;
  size_t getSize() const noexcept;
  int last() const noexcept;
  IntArray(int i);
  ~IntArray();
  int * a;
  size_t size;
  int at(size_t id()) const;
  IntArray(const IntArray & rhs);
  IntArray & operator=(const IntArray & rhs); 
  IntArray (IntArray && rhs);
  IntArray & operator=(IntArray && rhs);
};

struct IntMatrix {
  IntArray* rows;
  size_t rowCount;
  size_t colCount;

  IntMatrix(size_t rows, size_t cols) : rowCount(rows), colCount(cols) {
    this->rows = new IntArray[rowCount];
    for (size_t i = 0; i < rowCount; ++i) {
      this->rows[i] = IntArray(0);
      for (size_t j = 0; j < colCount; ++j) {
        this->rows[i].add(0);
      }
    }
  }

  ~IntMatrix() {
    delete[] rows;
  }

  void setValue(size_t row, size_t col, int value) {
    if (row < rowCount && col < colCount) {
      rows[row].a[col] = value;
    }
  }

  int getValue(size_t row, size_t col) const {
    if (row < rowCount && col < colCount) {
      return rows[row].get(col);
    }
    return 0;
  }

  void print() const {
    for (size_t i = 0; i < rowCount; ++i) {
      for (size_t j = 0; j < colCount; ++j) {
        std::cout << getValue(i, j) << " ";
      }
      std::cout << std::endl;
    }
  }

  void addRowAfter(size_t afterRow, int fillValue) {
    if (afterRow > rowCount) return;

    IntArray* newRows = new IntArray[rowCount + 1];
    size_t newIndex = 0;

    for (size_t i = 0; i < rowCount; ++i) {
      newRows[newIndex++] = rows[i];
      if (i == afterRow) {
        IntArray newRow(0);
        for (size_t j = 0; j < colCount; ++j) {
          newRow.add(fillValue);
        }
        newRows[newIndex++] = newRow;
      }
    }

    if (afterRow == 0) {
      IntArray newRow(0);
      for (size_t j = 0; j < colCount; ++j) {
        newRow.add(fillValue);
      }
      IntArray* temp = newRows;
      newRows = new IntArray[rowCount + 1];
      newRows[0] = newRow;
      for (size_t i = 0; i < rowCount; ++i) {
        newRows[i + 1] = temp[i];
      }
      delete[] temp;
    }

    delete[] rows;
    rows = newRows;
    rowCount++;
  }

  void addRowAndColumnAfter(size_t afterRow, size_t afterCol) {
    addRowAfter(afterRow, 0);

    for (size_t i = 0; i < rowCount; ++i) {
      IntArray newRow(0);
      for (size_t j = 0; j < colCount; ++j) {
        if (j == afterCol) {
          newRow.add(0);
        }
        newRow.add(rows[i].get(j));
      }
      if (afterCol == 0) {
        newRow.add(0);
        for (size_t j = 0; j < colCount; ++j) {
          newRow.add(rows[i].get(j));
        }
      }
      rows[i] = newRow;
    }
    colCount++;
  }
};

IntArray::~IntArray() {
  delete[] a;
}

IntArray::IntArray(int i) :
  a(new int[1]),
  size(1)
{
  * a = i;
}

int IntArray::get(size_t id) const noexcept
{
  return a[id];
}

size_t IntArray::getSize() const noexcept
{
  return size;
}

int IntArray::last() const noexcept
{
  return get(getSize() - 1);
}

void IntArray::add(int i)
{
  int * tmp = new int[getSize() + 1];
  for (size_t i = 0; i < getSize(); ++i)
  {
    tmp[i] = get(i);
  }
  delete[] a;
  a = tmp;
  ++size;
}

IntArray::IntArray (const IntArray & rhs) :
  a(new int [rhs.getSize()]),
  size(rhs.getSize())
{

  for (size_t i = 0; i < getSize(); ++i)
  {
    a[i] = rhs.get(i);
  }
}

IntArray & IntArray::operator=(const IntArray & rhs)
{
  int * tmp = new int [rhs.getSize()];
  for (size_t i = 0; i < rhs.getSize(); ++i)
  {
    delete[] a;
    a = tmp;
    size = rhs.getSize();
    return * this;
  }
}

IntArray::IntArray (IntArray && rhs):
a(rhs.a),
size(rhs.getSize())
{
  rhs.a = nullptr;
}

IntArray & IntArray::operator=(IntArray && rhs)
{
  delete[] a;
  a = rhs.a;
  size = rhs.size;
  rhs.a = nullptr;
  return * this;
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file) {
    return 1;
  }

  size_t rows, cols;
  if (!(file >> rows >> cols)) {
    return 1;
  }

  try {
    IntMatrix matrix(rows, cols);

    for (size_t i = 0; i < rows; ++i) {
      for (size_t j = 0; j < cols; ++j) {
        int value;
        if (!(file >> value)) {
          return 1;
        }
        matrix.setValue(i, j, value);
      }
    }

    int cmd, param1, param2;
    while (std::cin >> cmd >> param1 >> param2) {
      if (cmd == 1) {
        matrix.addRowAfter(param1, param2);
        matrix.print();
      }
      else if (cmd == 3) {
        matrix.addRowAndColumnAfter(param1, param2);
        matrix.print();
      }
      else {
        return 3;
      }
    }

    if (std::cin.fail() && !std::cin.eof()) {
      return 1;
    }
  }
  catch (...) {
    return 2;
  }

  return 0;
}
